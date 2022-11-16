/**
 * @file pthread_rwlock.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread_rwlock读写锁接口测试
 * @version 1.0.0
 * @date 2022-11-4
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define _GNU_SOURCE

/**
 * @brief 读写锁常用接口如下:
 * pthread_rwlockattr_init: 读写锁初始化
 * pthread_rwlockattr_destroy: 读写锁销毁
 * pthread_rwlockattr_getpshared: 读锁加锁
 * pthread_rwlockattr_setpshared: 尝试读锁加锁
 */
#define EXEC_RETURN_ZERO(x)              \
    do                                   \
    {                                    \
        if (!(x))                        \
        {                                \
            printf("%s [FAIL]\r\n", #x); \
        }                                \
    } while (0)

void show_pthread_info(const char *str)
{
    printf("%s:pid:%llu tid:%llX\r\n", str, (unsigned long long)getpid(), (unsigned long long)pthread_self());
}

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *wr_exec_task0(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        if (pthread_rwlock_wrlock(&lock) == 0)
        {
            printf("this is [wr --> 0] thread!!!!!\r\n");
            sleep(1);
            pthread_rwlock_unlock(&lock);
            sleep(7);
        }
        else
        {
            usleep(1000*1510);
            printf("thread0 wr lock get [fail]\r\n");
        }
    }
    pthread_exit((void *)199);
}

void *wr_exec_task1(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        if (pthread_rwlock_trywrlock(&lock) == 0)
        {
            printf("this is [wr --> 1] thread!!!!!\r\n");
            sleep(1);
            pthread_rwlock_unlock(&lock);
            sleep(1);
        }
        else
        {
            usleep(1000*1000);
            printf("thread1 wr try lock get [fail]\r\n");
        }
    }
    pthread_exit((void *)199);
}

void *wr_exec_task2(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        struct timespec spec;
        
        clock_gettime(CLOCK_REALTIME, &spec);
        spec.tv_sec += 2;

        if (pthread_rwlock_timedwrlock(&lock, &spec) == 0)
        {
            printf("this is [wr ---> 2] thread!!!!!\r\n");
            sleep(3);
            pthread_rwlock_unlock(&lock);
            sleep(5);
        }
        else
        {
            sleep(4);
            printf("thread2 wr timeout get [fail]\r\n");
        }
    }
    pthread_exit((void *)199);
}

void *thread_exec_task0(void *arg)
{
    int len = 0;
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        if (0 == pthread_rwlock_rdlock(&lock))
        {
            printf("this is task[0] thread!\r\n");
            usleep(100*1000);
            pthread_rwlock_unlock(&lock);
            usleep(323*1000);
        }
        else
        {
            sleep(1);
            printf("task0 get rwlock [fail]\r\n");
        }
    }
    return (void *)0;
}

void *thread_exec_task1(void *arg)
{
    int len = 0;
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        if (0 == pthread_rwlock_rdlock(&lock))
        {
            printf("this is task[1] thread!\r\n");
            usleep(150*1000);
            pthread_rwlock_unlock(&lock);
            usleep(721*1000);
        }
        else
        {
            sleep(1);
            printf("task1 get rwlock [fail]\r\n");
        }
    }

    return (void *)0;
}

void *thread_exec_task2(void *arg)
{
    int len = 0;
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        if (0 == pthread_rwlock_tryrdlock(&lock))
        {
            printf("this is task[2] thread!\r\n");
            usleep(170*1000);
            pthread_rwlock_unlock(&lock);
            usleep(520*1000);
        }
        else
        {   
            sleep(1);
            printf("try get rdlock [fail]!\r\n");
        }
    }

    return (void *)0;
}

void *thread_exec_task3(void *arg)
{
    int len = 0;
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        struct timespec spec;
        
        clock_gettime(CLOCK_REALTIME, &spec);
        spec.tv_sec += 2;

        if (0 == pthread_rwlock_timedrdlock(&lock, &spec))
        {
            printf("this is task[3] thread!\r\n");
            usleep(230*1000);
            pthread_rwlock_unlock(&lock);
            usleep(470*1000);
        }
        else
        {
            printf("rdlock timeout [fail]\r\n");
            sleep(3);
        }
    }

    return (void *)0;
}

typedef void *(*THREAD_EXEC_CB)(void *);
int main(int argc, char *argv[])
{
    pthread_t tid;

    if (pthread_rwlock_init(&lock, NULL) != 0)
    {
        perror("rwlock init fail!");
        exit(-1);
    }

    #define MAX_THREAD_NUM 4 //最大读线程
    #define MAX_WR_THREAD_NUM 3 //最大写线程

    pthread_t rd_tid[MAX_THREAD_NUM];
    THREAD_EXEC_CB rd_exec_cb[MAX_THREAD_NUM] = {
        thread_exec_task0,
        thread_exec_task1,
        thread_exec_task2,
        thread_exec_task3,
    };

    pthread_t wr_tid[MAX_WR_THREAD_NUM];
    THREAD_EXEC_CB wr_exec_cb[MAX_WR_THREAD_NUM] = {
        wr_exec_task0,
        wr_exec_task1,
        wr_exec_task2,
    };

    for (int i=0; i < MAX_WR_THREAD_NUM; i++)
    {
        if (0 != pthread_create(&wr_tid[i], NULL, wr_exec_cb[i], NULL))
        {
            pthread_rwlock_destroy(&lock);
            perror("can't creat thread!");
            exit(0);
        }
        EXEC_RETURN_ZERO(0 == pthread_detach(wr_tid[i]));
    }

    for (int i = 0; i < MAX_THREAD_NUM; i++)
    {
        if (0 != pthread_create(&rd_tid[i], NULL, rd_exec_cb[i], NULL))
        {
            pthread_rwlock_destroy(&lock);
            perror("can't creat thread!");
            exit(-1);
        }

        EXEC_RETURN_ZERO(0 == pthread_detach(rd_tid[i]));
    }

    printf("Down!\r\n");
    pthread_exit((void *)0);
}