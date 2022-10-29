/**
 * @file sigprocmask.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  sigprocmask设置信号阻塞
 * @version 1.0.0
 * @date 2022-10-27
 *
 * @copyright Copyright (c) 2022
 *
 */
#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

#if 1

void show_signal_set(const sigset_t *set)
{
    for (int i = 1; i < _NSIG; i++)
    {
        int status;
        status = sigismember(set, i);
        printf("index[%d] -- > status[%d]\r\n", i, status);
    }
    printf("----------------------------------------------\r\n");
}

void signal_cb(int igno)
{
    sigset_t set;
    if (sigpending(&set) == 0)
    {
        show_signal_set(&set);
    }
    else
    {
        printf("get sigpending err!\r\n");
    }
    printf("recv signal :%d\r\n", igno);
}
// sigprocmask
int main(int argc, char *argv[])
{
    if (signal(SIGINT, signal_cb) == SIG_ERR)
    {
        printf("signal stop set err!");
        exit(-1);
    }

    if (signal(SIGQUIT, signal_cb) == SIG_ERR)
    {
        printf("signal stop set err!");
        exit(-1);
    }

    sigset_t mask, old_mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_SETMASK, &mask, &old_mask) != 0)
    {
        printf("set mask err!\r\n");
    }

    pause();
    //首先执行ctrl+c, SIGINT信号被阻塞挂起, 在执行ctrl+\, 查看所有未决信号
    printf("Done\r\n");
    return 0;
}
#endif
