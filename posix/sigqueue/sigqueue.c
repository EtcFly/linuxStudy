/**
 * @file sigqueue.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  sigqueue信号排队
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
#include <time.h>
#include <sys/time.h>

void signal_cb(int signo)
{
    printf("recv signal %d\r\n", signo);
}

void signal_cb_ext(int signo, siginfo_t *info, void *ext)
{
    union sigval *val = ext;
    printf("recv ext signal %d %d %d\r\n", signo, info->si_value.sival_int, info->si_int);

    sleep(5);
}

int main(int argc, char *argv[])
{
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = signal_cb_ext;
    sigaction(SIGINT, &act, NULL);

    for (int i = 0; i < 3; i++)
    {
        union sigval valsig;

        valsig.sival_ptr = NULL;
        valsig.sival_int = i;
        sigqueue(getpid(), SIGINT, valsig);
    }
    printf("Down!\r\n");
    return 0;
}