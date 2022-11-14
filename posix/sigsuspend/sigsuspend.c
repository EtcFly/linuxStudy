/**
 * @file sigsuspend.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  sigsuspend测试信号挂起和返回
 * @version 1.0.0
 * @date 2022-10-30
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
#include <stdbool.h>
#include <setjmp.h>

void show_mask(void)
{
    sigset_t now_setmask;
    if (sigprocmask(0, NULL, &now_setmask) < 0)
    {
        perror("signal promask err!");
    }

    printf("INT SIGNAL: ");
    if (sigismember(&now_setmask, SIGINT))
    {
        printf("SIGINT ");
    }

    if (sigismember(&now_setmask, SIGUSR1))
    {
        printf("SIGUSR1 ");
    }

    if (sigismember(&now_setmask, SIGUSR2))
    {
        printf("SIGUSR2 ");
    }
    printf("\r\n");
}
static void sig_cb(int signo)
{
    printf("recv signal:%d\r\n", signo);
    show_mask();
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_cb);

    sigset_t now, old, wait;
    sigemptyset(&wait);
    sigaddset(&wait, SIGUSR1);

    sigemptyset(&now);
    sigaddset(&now, SIGINT);
    sigprocmask(SIG_BLOCK, &now, &old);

    for (;;)
    {
        show_mask();
        sigsuspend(&wait);
        printf("get finish!\r\n");
    }

    printf("Down\r\n");
    return 0;
}