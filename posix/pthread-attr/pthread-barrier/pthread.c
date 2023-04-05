/**
 * @file barrier.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread屏障接口
 * @version 1.0.0
 * @date 2022-11-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define _GNU_SOURCE

/**
 * @brief 屏障相关接口API
 * pthread_barrierattr_init 屏障初始化
 * pthread_barrierattr_destroy: 屏障销毁
 * pthread_barrierattr_getpshared: 屏障等待
 * pthread_barrierattr_setpshared
 */

unsigned long long count_index = 0;
typedef void *(*THREAD_EXEC_CB)(void *);

#define EXEC_RETURN_ZERO(x)              \
    do                                   \
    {                                    \
        if (!(x))                        \
        {                                \
            printf("%s [FAIL]\r\n", #x); \
        }                                \
    } while (0)

pthread_barrier_t bat;
void *exec_thread(void *arg)
{
    long int value = (long int)(void *)arg;
    usleep(value);

    system("date");
    printf("%s [%ld] ready!!!\r\n", __FUNCTION__, value / 1000);

    int ret = pthread_barrier_wait(&bat);
    if (PTHREAD_BARRIER_SERIAL_THREAD == ret) //最后一个到达线程兼第一个唤醒线程
    {
        printf("%s [%ld] wakeup running\r\n", __FUNCTION__, value / 1000);
        system("date");
    }
    else if (0 == ret) //其他之前等待的线程
    {
        printf("%s [%ld] running\r\n", __FUNCTION__, value / 1000);
        system("date");
    }
    else
    {
        printf("wait pthread fail: [%ld]\r\n", value);
        exit(-1);
    }

    pthread_exit((void *)199);
}

#define MAX_THREAD_NUM 10
int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_barrierattr_t barrier_attr;

    EXEC_RETURN_ZERO(0 == pthread_barrierattr_init(&barrier_attr));
    EXEC_RETURN_ZERO(0 == pthread_barrierattr_setpshared(&barrier_attr, PTHREAD_PROCESS_PRIVATE));

    if (pthread_barrier_init(&bat, &barrier_attr, MAX_THREAD_NUM) != 0)
    {
        perror("barrier err!");
        exit(-1);
    }
    pthread_barrierattr_destroy(&barrier_attr);

    for (int i = 0; i < MAX_THREAD_NUM; i++)
    {
        if (0 != pthread_create(&tid, NULL, exec_thread, (void *)(long int)(1000 * 1000 * i)))
        {
            pthread_barrier_destroy(&bat);
            perror("can't creat threda!");
            exit(-1);
        }

        pthread_detach(tid);
    }

    printf("Down!\r\n");
    pthread_exit(0);
    printf("test run here!\r\n");
    // return 0;
}