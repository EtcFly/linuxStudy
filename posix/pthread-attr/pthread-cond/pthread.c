/**
 * @file pthread_cond.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread_cond线程属性接口测试
 * @version 1.0.0
 * @date 2022-11-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define _GNU_SOURCE

/**
 * @brief 条件变量相关API操作
 * pthread_condattr_init: 条件变量初始化
 * pthread_condattr_destroy: 条件变量销毁
 * pthread_condattr_getpshared: 发送信号
 * pthread_condattr_setpshared: 广播信号
 * pthread_condattr_getclock: 等待信号
 * pthread_condattr_setclock: 带超时的等待信号
 */

#define EXEC_RETURN_ZERO(x)              \
    do                                   \
    {                                    \
        if (!(x))                        \
        {                                \
            printf("%s [FAIL]\r\n", #x); \
        }                                \
    } while (0)

unsigned long long count_index = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef void *(*THREAD_EXEC_CB)(void *);

void *signal_send_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        pthread_mutex_lock(&lock);
        /*在这里进行保护数据的操作*/
        usleep(100 * 1000);
        pthread_mutex_unlock(&lock);

        usleep(300 * 1000);                  //等待线程切换, 其他线程进入wait状态, 避免丢信号
        if (0 == pthread_cond_signal(&cond)) //发送信号, 正常不需要加锁
        {
            printf("[%s] send ok!\r\n", __FUNCTION__);
            sleep(2);
        }
        else
        {
            sleep(5);
            printf("[%s] send fail!\r\n", __FUNCTION__);
        }
    }
    pthread_exit((void *)199);
}

void *signal_broadcase_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        pthread_mutex_lock(&lock);
        /*在这里进行数据的操作*/
        usleep(200 * 1000);
        pthread_mutex_unlock(&lock);

        usleep(200 * 1000); //等待线程切换, 其他线程进入wait状态, 避免丢信号
        if (0 == pthread_cond_broadcast(&cond))
        {
            printf("[%s] send ok!\r\n", __FUNCTION__);
            sleep(7);
        }
        else
        {
            sleep(3);
            printf("[%s] send fail!\r\n", __FUNCTION__);
        }
    }
    pthread_exit((void *)199);
}

void *signal_recv_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        pthread_mutex_lock(&lock);
        /*在这里进行数据的操作*/
        if (0 == pthread_cond_wait(&cond, &lock)) //进入cond后互斥锁解锁然后条件锁加锁, 返回后条件锁解锁, 互斥锁加锁
        {
            printf("[%s] wait signal success!\r\n", __FUNCTION__);
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            printf("[%s] wait signal fail!\r\n", __FUNCTION__);
            sleep(1);
        }
    }
    pthread_exit((void *)199);
}

void *signal_timeRecv_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        sleep(2);
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        now.tv_sec += 2;

        pthread_mutex_lock(&lock);
        /*在这里进行数据的操作*/
        if (0 == pthread_cond_timedwait(&cond, &lock, &now)) //会自动去unlock 然后wait
        {
            printf("[%s] timeout signal success!\r\n", __FUNCTION__);
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            sleep(1);
            printf("[%s] timeout signal fail!\r\n", __FUNCTION__);
        }
    }

    return (void *)0;
}

#include <time.h>
/* in /usr/include/linux/time.h
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID 3
#define CLOCK_MONOTONIC_RAW 4
#define CLOCK_REALTIME_COARSE 5
#define CLOCK_MONOTONIC_COARSE 6
#define CLOCK_BOOTTIME 7
#define CLOCK_REALTIME_ALARM 8
#define CLOCK_BOOTTIME_ALARM 9
*/
void show_clock_info(pthread_condattr_t *attr)
{
    clockid_t id;
    EXEC_RETURN_ZERO(0 == pthread_condattr_getclock(attr, &id));

    switch (id)
    {
    case CLOCK_REALTIME:
        printf("get clock CLOCK_REALTIME\r\n");
        break;
    case CLOCK_PROCESS_CPUTIME_ID:
        printf("get clock CLOCK_PROCESS_CPUTIME_ID\r\n");
        break;
    case CLOCK_THREAD_CPUTIME_ID:
        printf("get clock CLOCK_THREAD_CPUTIME_ID\r\n");
        break;
    case CLOCK_MONOTONIC:
        printf("get clock CLOCK_MONOTONIC\r\n");
        break;
    default:
        printf("Unknow Clock Source!\r\n");
        break;
    }
}

#define CONOF(a) (sizeof(a) / sizeof(a[0]))
int main(int argc, char *argv[])
{
    pthread_condattr_t cond_attr;

    EXEC_RETURN_ZERO(0 == pthread_condattr_init(&cond_attr));
    EXEC_RETURN_ZERO(0 == pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_PRIVATE));

    show_clock_info(&cond_attr);
    EXEC_RETURN_ZERO(0 == pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC));
    show_clock_info(&cond_attr);

    pthread_t tid;
    THREAD_EXEC_CB exec_cb[] = {
        signal_send_thread,
        signal_broadcase_thread,
        signal_recv_thread,
        signal_timeRecv_thread,
    };

    // initnal condition and mutex
    if (0 != pthread_mutex_init(&lock, NULL))
    {
        perror("mutex init error!");
        exit(-1);
    }

    if (0 != pthread_cond_init(&cond, &cond_attr))
    {
        pthread_mutex_destroy(&lock);
        perror("mutex init error!");
        exit(-1);
    }

    for (int i = 0; i < CONOF(exec_cb); i++)
    {
        if (0 != pthread_create(&tid, NULL, exec_cb[i], NULL))
        {
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&cond);
            perror("can't creat thread!");
            exit(0);
        }
        EXEC_RETURN_ZERO(0 == pthread_detach(tid));
    }

    printf("Down!\r\n");
    pthread_exit((void *)0);
    // return 0;
}