/**
 * @file sigsetjmp.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  sigsetjmp设置信号阻塞
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
#include <stdbool.h>
#include <setjmp.h>

static sigjmp_buf jmpbuf;

void show_mask(void)
{
    sigset_t now_setmask;
    if (sigprocmask(0, NULL, &now_setmask) < 0)
    {
        perror("signal promask err!");
    }

    if (sigismember(&now_setmask, SIGINT))
    {
        printf("INT SIGNAL SETMASK!\r\n");
    }
}
static void sig_cb(int signo)
{
    printf("recv signal:%d\r\n", signo);
    sigset_t now_setmask;

    if (sigprocmask(0, NULL, &now_setmask) < 0)
    {
        perror("signal promask err!");
    }

    if (sigismember(&now_setmask, SIGINT))
    {
        printf("INT SIGNAL SETMASK!\r\n");
        siglongjmp(jmpbuf, 1);
        // longjmp(jmpbuf, 1);
    }
}

typedef int a_int[1];
int main(int argc, char *argv[])
{
    // if (setjmp(jmpbuf) != 0)
    // {
    //     show_mask();
    //     perror("setjump signal err!");
    //     exit(0);
    // }

    if (sigsetjmp(jmpbuf, 0) != 0)
    {
        show_mask();
        perror("setjump signal err!");
        exit(0);
    }
    signal(SIGINT, sig_cb);

    for (;;)
    {
        sigset_t set;
        sigemptyset(&set);
        // sigaddset(&set, SIGINT);
        sigsuspend(&set);
        printf("get finish!\r\n");
    }
    // sigsetjmp(&jmpbuf, sig_jmp);
    printf("Down\r\n");
    return 0;
}