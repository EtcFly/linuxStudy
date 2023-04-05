/**
 * @file pthread.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread 文件锁相关接口
 * @version 1.0.0
 * @date 2022-11-20
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
#include <errno.h>

#define _GNU_SOURCE

/**
 * @brief 线程创建和销毁相关接口
 * pthread_create: 线程创建
 * pthread_exit： 线程退出
 * pthread_join： 线程同步等待子线程状态
 * pthread_detach: 线程分离, 子线程资源由操作系统回收
 * pthread_self： 获取当前线程自身
 * pthread_equal：线程比对
 * pthread_cancel: 线程退出信号
 * pthread_cleanup_push: 线程清理push
 * pthread_cleanup_pop: 线程清理弹出
 */

void show_pthread_info(const char *str)
{
    printf("%s:pid:%llu tid:%llX\r\n", str, (unsigned long long)getpid(), (unsigned long long)pthread_self());
}

void show_clean_cb(void *arg)
{
    printf("clean cb: %s\r\n", (char *)arg);
}

unsigned long long count_index = 0;
void *new_thread(void *arg)
{
    FILE *lc_file = (FILE *)arg;
    printf("new thread start!\r\n");
    while (true)
    {
        flockfile(lc_file);
        printf("[%s] get file lock!\r\n", __FUNCTION__);
        sleep(2);
        printf("[%s] release file lock!\r\n", __FUNCTION__);
        for (int i = 0; i < 26; i++)
        {
            putc_unlocked((char)(i + 'A'), lc_file);
        }
        funlockfile(lc_file);
        fflush(lc_file);
        usleep(1000 * 157);
    }
}

void *old_thread(void *arg)
{
    FILE *lc_file = (FILE *)arg;
    printf("old thread start!\r\n");
    while (true)
    {
        flockfile(lc_file);
        printf("[%s] get file lock!\r\n", __FUNCTION__);
        sleep(1);
        printf("[%s] release file lock!\r\n", __FUNCTION__);
        for (int i = 0; i < 26; i++)
        {
            putc_unlocked((char)(i + 'a'), lc_file);
        }
        funlockfile(lc_file);
        fflush(lc_file);
        usleep(1000 * 733);
    }
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    FILE *file;
    file = fopen("./test.txt", "w+");
    if (NULL == file)
    {
        printf("err:%d, %s\r\n", errno, strerror(errno));
        exit(-1);
    }

    if (0 != pthread_create(&tid, &attr, new_thread, (void *)file))
    {
        perror("can't creat threda!");
        exit(0);
    }

    static pthread_t tid1;
    if (0 != pthread_create(&tid1, &attr, old_thread, (void *)file))
    {
        perror("can't creat threda!");
        exit(0);
    }

    pthread_attr_destroy(&attr);
    show_pthread_info(__FUNCTION__);
    printf("Down!\r\n");
    pthread_exit(0);
}