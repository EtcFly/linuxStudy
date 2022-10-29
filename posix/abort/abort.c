/**
 * @file abort.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  abort进程忽略
 * @version 1.0.0
 * @date 2022-10-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#if 1
// abort
int main(int argc, char *argv[])
{
    if (signal(SIGABRT, SIG_IGN) == SIG_ERR)
    {
        perror("signal set err!");
        exit(-1);
    }

    printf("abort signal set ok\r\n");
    abort();
    printf("Done\r\n"); //无法执行
    fflush(NULL);
    return 0;
}
#endif

#if 0
// abort
void signal_cb(int igno)
{
    printf("recv signal:%d\r\n", igno); //依旧执行
}

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

int main(int argc, char *argv[])
{
    sigset_t igno;

    sigemptyset(&igno);
    sigaddset(&igno, SIGABRT);
    sigprocmask(SIG_SETMASK, &igno, NULL);

    if (signal(SIGABRT, signal_cb) == SIG_ERR)
    {
        printf("err set!\rr\n");
        exit(-1);
    }

    printf("abort signal set ok\r\n");
    // kill(getpid(), SIGABRT);
    abort();
    printf("Done\r\n"); //无法执行
    fflush(NULL);
    return 0;
}
#endif
