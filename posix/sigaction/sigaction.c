/**
 * @file sigaction.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  sigaction设置信号阻塞
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

#if 0
void show_signal_set(const sigset_t *set)
{
    bool show_flag = false;
    for (int i = 1; i < _NSIG; i++)
    {
        int status;
        status = sigismember(set, i);
        if (1 == status)
        {
            show_flag = true;
            printf("index[%d] -- > status[%d]\r\n", i, status);
        }
    }

    if (show_flag)
    {
        printf("----------------------------------------------\r\n");
    }
    else
    {
        printf("---------------- nothing to show -------------\r\n");
    }
}
void signal_cb_ext(int igno, siginfo_t *info, void *context)
{
    printf("enter ext signal callback %d\r\n", igno);
    printf("code:%d sino:%d\r\n", info->si_code, info->si_signo);
}
void signal_cb(int igno)
{
    static int index = 0;

    index++;
    if (index == 3)
    {
        exit(0);
    }

    struct sigaction old_mask;
    sigemptyset(&old_mask.sa_mask);
    sigaction(0, NULL, &old_mask);
    show_signal_set(&old_mask.sa_mask);

    sigset_t set;
    if (sigpending(&set) == 0)
    {
        printf("-------- show pending signal ---------\r\n");
        show_signal_set(&set);
    }
    else
    {
        printf("get sigpending err!\r\n");
    }
    printf("----  end recv signal :%d ----- \r\n", igno);
}

void sig_cb_old(int signo)
{
}
// sigprocmask
int main(int argc, char *argv[])
{
    struct sigaction act;
    struct sigaction oact;

    sigset_t mask, old_mask;
    printf("sig_old:%p sig_new:%p\r\n", sig_cb_old, signal_cb);

    signal(SIGINT, sig_cb_old);
    signal(SIGUSR1, signal_cb);
    sigemptyset(&mask);
    // sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGSEGV);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTSTP);

    act.sa_flags = SA_SIGINFO;
    act.sa_handler = signal_cb;
    act.sa_sigaction = signal_cb_ext;
    if (sigaction(SIGINT, &act, &oact) != 0)
    {
        perror("set sigaction err!");
        exit(-1);
    }

    // sigprocmask(0, NULL, &old_mask);
    show_signal_set(&oact.sa_mask);
    printf("old_cb:%p\r\n", oact.sa_handler);

    for (;;)
    {
        pause();
        sigemptyset(&oact.sa_mask);
        sigaction(SIGINT, NULL, &oact);
        show_signal_set(&oact.sa_mask); // oact表示之前的进程掩码位
    }

    printf("Done\r\n");
    return 0;
}
#endif

#if 0
void show_signal_set(const sigset_t *set)
{
    bool show_flag = false;
    for (int i = 1; i < _NSIG; i++)
    {
        int status;
        status = sigismember(set, i);
        if (1 == status)
        {
            show_flag = true;
            printf("index[%d] -- > status[%d]\r\n", i, status);
        }
    }

    if (show_flag)
    {
        printf("---------  signal show end  ----------\r\n");
    }
    else
    {
        printf("---------- nothing to show -----------\r\n");
    }
}

void signal_cb(int igno)
{
    printf("----  end recv signal :%d ----- \r\n", igno);
}

void sig_cb_old(int signo)
{
}
// sigprocmask
int main(int argc, char *argv[])
{
    struct sigaction act;
    struct sigaction oact;

    sigset_t mask, old_mask;
    printf("sig_old:%p sig_new:%p\r\n", sig_cb_old, signal_cb);

    signal(SIGINT, sig_cb_old);
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGSEGV);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTSTP);
    act.sa_handler = signal_cb;
    act.sa_flags = 0;
    if (sigaction(SIGINT, &act, &oact) != 0)
    {
        perror("set sigaction err!");
        exit(-1);
    }

    show_signal_set(&oact.sa_mask);
    printf("old_cb:%p\r\n", oact.sa_handler);

    printf("Done\r\n");
    return 0;
}
#endif

void signal_cb(int signo)
{
    printf("recv signal:%d\r\n", signo);
}

int main(int agrc, char *argv[])
{
    struct sigaction act;

    // signal(SIGINT, signal_cb);
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = 0;
    act.sa_handler = signal_cb;
    if (sigaction(SIGINT, &act, NULL) < 0)
    {
        perror("err!");
    }

    for (;;)
    {
        pause();
        printf("get finish!!\r\n");
    }
    printf("Down!\r\n");
    return 0;
}
