/**
 * @file pthread_mutex.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread_mutex互斥锁接口测试
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
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define _GNU_SOURCE

/**
 * @brief 互斥锁相关接口
 * pthread_mutex_init: 互斥量初始化
 * pthread_mutex_destroy: 互斥量销毁
 * pthread_mutex_trylock: 互斥量尝试加锁
 * pthread_mutex_lock: 互斥量加锁
 * pthread_mutex_timedlock: 带超时的互斥量加锁
 * pthread_mutex_unlock: 互斥量解锁
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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *mutex_timeout_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        struct timeval tv;
        struct timespec ts;
        gettimeofday(&tv, NULL);

        ts.tv_sec = tv.tv_sec + 1;
        ts.tv_nsec = tv.tv_usec;

        if (0 == pthread_mutex_timedlock(&lock, &ts))
        {
            printf("this is [%s] thread!！！！！\r\n", __FUNCTION__);
            sleep(1);
            pthread_mutex_unlock(&lock);
            usleep(1000);
        } else {
            printf("this is [%s] timeout------\r\n", __FUNCTION__);
            sleep(1);
        }

    }
    pthread_exit((void *)199);
}

void *mutex_wait_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        pthread_mutex_lock(&lock);
        printf("this is [%s] thread!！！！！\r\n", __FUNCTION__);
        sleep(2);
        pthread_mutex_unlock(&lock);
        usleep(1000);
    }
    pthread_exit((void *)199);
}

void *mutex_try_wait_thread(void *arg)
{
    int len = 0;
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        if (0 == pthread_mutex_trylock(&lock))
        {
            printf("this is [%s] thread!！！！！\r\n", __FUNCTION__);
            sleep(3);
            pthread_mutex_unlock(&lock);
            usleep(1000);
        } else {
            printf("this is [%s] try fail!！！！！\r\n", __FUNCTION__);
            sleep(1);
        }

    }

    return (void *)0;
}

int main(int argc, char *argv[])
{
    #define MAX_THREAD_NUM 5

    int ret;
    pthread_t tid;
    pthread_t tid1;
    pthread_t tid2;

    ret = pthread_mutex_init(&lock, NULL);
    if (0 != ret)
    {
        perror("mutex init err!");
        exit(-1);
    }

    if (0 != pthread_create(&tid, NULL, mutex_try_wait_thread, (void *)1))
    {
        pthread_mutex_destroy(&lock);
        perror("can't creat threda!");
        exit(-1);
    }

    if (0 != pthread_create(&tid1, NULL, mutex_wait_thread, (void *)1))
    {
        pthread_mutex_destroy(&lock);
        perror("can't creat thread!");
        exit(-1);
    }

    if (0 != pthread_create(&tid2, NULL, mutex_timeout_thread, (void *)1))
    {
        pthread_mutex_destroy(&lock);
        perror("can't creat thread!");
        exit(-1);
    }

    EXEC_RETURN_ZERO(0 == pthread_detach(tid2));
    EXEC_RETURN_ZERO(0 == pthread_detach(tid1));
    EXEC_RETURN_ZERO(0 == pthread_detach(tid));

    printf("Down!\r\n");
    pthread_exit((void *)0);
}