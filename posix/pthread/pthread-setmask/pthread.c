/**
 * @file pthread.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread线程信号处理接口测试
 * @version 1.0.0
 * @date 2022-10-30
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
#include <signal.h>
#include <sys/types.h>

#define _GNU_SOURCE

/**
 * @brief 线程创建和销毁相关接口
 * pthread_sigmask: 设置多线程信号掩码
 * sigwait: 设置信号等待
 * pthread_kill: 发送特定信号
 */

#define EXEC_RETURN_ZERO(x)              \
    do                                   \
    {                                    \
        if (!(x))                        \
        {                                \
            printf("%s [FAIL]\r\n", #x); \
        }                                \
    } while (0)

void *new_thread(void *arg)
{
    sigset_t *set = (sigset_t *)arg;
    printf("new thread start!\r\n");

    while (true)
    {
        int signo;
        int ret = sigwait(set, &signo);
        if (0 == ret)
        {
            switch (signo)
            {
            case SIGINT:
                printf("SIGINT signo!\r\n");
                break;
            case SIGQUIT:
                printf("SIGQUIT signo!\r\n");
                break;
            default:
                printf("Unknow signo!\r\n");
                break;
            }
        }
        else
        {
            printf("other signo has recv!\r\n");
        }
    }
    return (void *)199; //使用return 清理函数不会被调用
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t init_attr;

    EXEC_RETURN_ZERO(0 == pthread_attr_init(&init_attr));
    EXEC_RETURN_ZERO(0 == pthread_attr_setdetachstate(&init_attr, PTHREAD_CREATE_DETACHED));

    //设置信号掩码
    sigset_t setMask;
    sigset_t oldMask;

    sigemptyset(&setMask);
    sigaddset(&setMask, SIGINT);
    sigaddset(&setMask, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &setMask, &oldMask);

    if (0 != pthread_create(&tid, &init_attr, new_thread, (void *)&setMask))
    {
        perror("can't creat threda!");
        exit(0);
    }

    EXEC_RETURN_ZERO(0 == pthread_attr_destroy(&init_attr));

    pthread_sigmask(SIG_SETMASK, &oldMask, NULL); //恢复原先的信号掩码

    pthread_exit(0);
}