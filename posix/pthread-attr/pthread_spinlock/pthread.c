/**
 * @file pthread_spilock.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread_spilock自旋锁
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
#include <stdbool.h>

#define _GNU_SOURCE

/**
 * @brief 自旋锁本身自旋, 自旋期间现场不会发生切换, 一般用于高并发场景减少线程切换耗时。
 * 主要接口如下:
 * pthread_spin_init: 自旋锁初始化
 * pthread_spin_destory: 自旋锁销毁
 * pthread_spin_lock: 自旋锁锁住
 * pthread_spin_unlock: 自旋锁解锁
 * pthread_spin_trylock: 自旋锁尝试枷锁
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


pthread_spinlock_t lock;

void *new_thread(void *arg)
{
    bool flag = true;
    printf("new thread start!\r\n");
    while (1)
    {
        //pthread_spin_lock(&lock);
        if (0 == pthread_spin_trylock(&lock))
        {
            flag = false;
            printf("this is [new] thread!!!!!!!!!!\r\n");
            sleep(1);
            pthread_spin_unlock(&lock);
            usleep(20);
        } else {
            if (!flag)
            {
                flag = true;
                printf("try get lock faiL!\r\n");
            }
            usleep(10);
        }
    }
    pthread_exit((void *)199);
}

void *old_thread(void *arg)
{
    int len = 0;
    printf("old thread start!\r\n");
    while (1)
    {
        pthread_spin_lock(&lock);
        printf("this is [old] thread!!!!!!!!!!\r\n");
        //usleep(1000*100);
        pthread_spin_unlock(&lock);
        usleep(12);
    }

    return (void *)0;
}

#define MAX_THREAD_NUM 2
int main(int argc, char *argv[])
{
    int ret;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_t tid1[MAX_THREAD_NUM];

    memset(tid1, 0, sizeof(tid1));
    ret = pthread_spin_init(&lock, 0); //自旋锁初始化, 会Malloc空间
    if (0 != ret)
    {
        perror("pthread spin init fail!");
        exit(-1);
    }

    if (0 != pthread_attr_init(&attr))
    {
        perror("pthread attr init err");
        pthread_spin_destroy(&lock);
        exit(-1);
    }

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //设置线程分离
    pthread_attr_setguardsize(&attr, 1024); //设置危险内存空间
    pthread_attr_setstacksize(&attr, 1024); //设置栈大小
    if (0 != pthread_create(&tid, &attr, new_thread, (void *)1))
    {
        pthread_attr_destroy(&attr);
        pthread_spin_destroy(&lock);
        perror("can't creat threda!");
        exit(-1);
    }

    for (int i = 0; i < MAX_THREAD_NUM; i++)
    {
        if (0 != pthread_create(&tid1[i], &attr, old_thread, (void *)1))
        {
            perror("can't creat threda!");
            goto _thread_err;
        }
    }

    pthread_attr_destroy(&attr);

    printf("thread max: %ld\r\n", sysconf(_SC_THREAD_THREADS_MAX));

    printf("Down!\r\n");
    pthread_exit((void *)0);
    // return 0;
_thread_err:
    for (int i=0; i < MAX_THREAD_NUM; i++)
    {
        if (tid1[i] != 0)
        {
            pthread_cancel(tid1[i]);
        }
    }
    pthread_cancel(tid);
    return 0;
}