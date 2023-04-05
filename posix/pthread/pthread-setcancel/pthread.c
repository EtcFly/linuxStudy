/**
 * @file pthread.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread 取消选项
 * @version 1.0.0
 * @date 2022-11-21
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
 * @brief 线程cancel的取消选项
 * pthread_setcancelstate: 设置取消状态属性
 * pthread_setcanceltype： 设置取消状态类型属性
 * pthread_testcancel : 测试取消状态点
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

#define EXEC_RETURN_ZERO(x)              \
    do                                   \
    {                                    \
        if (!(x))                        \
        {                                \
            printf("%s [FAIL]\r\n", #x); \
        }                                \
    } while (0)

#if 0
/**
 * @brief 在取消点退出测试
 */
void *new_thread(void *arg)
{
    printf("new thread start!\r\n");
    EXEC_RETURN_ZERO(0 == pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL));
    while (true)
    {
        count_index++;
        if (count_index > 10000000000)
        {
            count_index += 100;
            if (count_index >= 20000000000)
            {
                EXEC_RETURN_ZERO(0 == pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL));
            }
        }
        pthread_testcancel();
    }
}
#endif

#if 1
/**
 * @brief 在取消点退出测试
 */
void *new_thread(void *arg)
{
    printf("new thread start!\r\n");
    // EXEC_RETURN_ZERO(0 == pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL));     //取消点取消(需要检测取消点)
    EXEC_RETURN_ZERO(0 == pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)); //任一点取消(异步取消)
    while (true)
    {
        count_index++;
        if (count_index > 10000000000)
        {
            count_index += 100;
        }
    }
}
#endif

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (0 != pthread_create(&tid, &attr, new_thread, (void *)NULL))
    {
        perror("can't creat threda!");
        exit(0);
    }
    EXEC_RETURN_ZERO(0 == pthread_cancel(tid));

    pthread_attr_destroy(&attr);
    show_pthread_info(__FUNCTION__);
    printf("Down!\r\n");
    pthread_exit(0);
}