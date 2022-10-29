/**
 * @file signal.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 进程signal信号测试
 * @version 1.0.0
 * @date 2022-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

void signal_callback(int signal_code)
{
    if (signal_code == SIGUSR1)
    {
        printf("recv signal SIGUSR1\r\n");
    }
    else if (SIGUSR2 == signal_code)
    {
        printf("recv signal SIGUSR2\r\n");
    }
    else if (SIGCHLD == signal_code)
    {
        printf("recv signal SIGCHLD\r\n");
    }
    else
    {
        printf("recv signal code:%d\r\n", signal_code);
    }
}

// 1、测试主进程的信号处理
#if 0
int main(void)
{
    __sighandler_t ret;
    ret = signal(SIGUSR1, signal_callback);
    printf("ret:%p cb:%p\r\n", ret, signal_callback);
    if (ret == SIG_ERR)
    {
        printf("can't catch SIGUSR1");
    }

    ret = signal(SIGUSR2, signal_callback); // signal每次返回设置前的值
    printf("ret:%p cb:%p\r\n", ret, signal_callback);
    if (ret == SIG_ERR)
    {
        printf("can't catch SIGUSR2");
    }

    if (signal(SIGTERM, signal_callback) == SIG_ERR)
    {
        printf("can't catch SIGTERM");
    }

    int index = 0;
    for (;;)
    {
        pause(); //每次收到监听信号才会唤醒一次, 默认信号不会
        index++;
        printf("wakup timr %d!\r\n", index);
    }
    printf("Done\r\n");
    return 0;
}
#endif

#if 0
int main(void)
{
    if (signal(SIGUSR1, signal_callback) == SIG_ERR)
    {
        printf("can't catch SIGUSR1");
    }

    if (signal(SIGUSR2, signal_callback) == SIG_ERR)
    {
        printf("can't catch SIGUSR2");
    }

    if (signal(SIGTERM, signal_callback) == SIG_ERR)
    {
        printf("can't catch SIGTERM");
    }

    if (signal(SIGCHLD, signal_callback) == SIG_ERR)
    {
        printf("can't catch SIGCHLD");
    }

    pid_t pid;

    if ((pid = fork()) < 0) //两次fork将孙子进程过继给init进程, 避免僵尸进程的出现
    {
        printf("fork err!\r\n");
    }
    else if (pid == 0)
    {
        char *argv[] = {NULL};
        char *env[] = {NULL};
        if ((pid = fork()) < 0)
        {
            printf("fork err!\r\n");
            exit(-1);
        }
        else if (pid == 0)
        {
            execve("./child_signal", argv, env);
            printf("exec err! %d,%s\r\n", errno, strerror(errno));
            exit(-1);
        }

        exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("child pid:%d !\r\n", pid);
    }

    int index = 0;
    for (;;)
    {
        pause(); //每次收到监听信号才会唤醒一次, 默认信号不会
        index++;
        printf("pid:%d wakup timr %d!\r\n", pid, index);
    }
    printf("Done\r\n");
    return 0;
}

#endif

#include <signal.h>

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

//测试进程信号屏蔽字API
#if 0
int main(int argc, char *argv[])
{
    sigset_t set;
    sigset_t old;
    int val;

    // val = sigemptyset(&set); //清空所有信号
    // val = sigfillset(&set); //置位所有信号
    // val = sigaddset(&set, 5); //设置某一位信号
    // val = sigdelset(&set, 5); //清除某一位信号

    //获取系统默认信号屏蔽字
    val = sigprocmask(0, NULL, &set);
    printf("emptry set:%d\r\n", val);
    show_signal_set(&set);

    //设置特定信号屏蔽字
    printf("set mask!\r\n");
    val = sigemptyset(&set);
    sigaddset(&set, 5);
    val = sigprocmask(SIG_BLOCK, &set, &old);
    show_signal_set(&set);
    sigaddset(&set, 6);
    val = sigprocmask(SIG_BLOCK, &set, &old);
    show_signal_set(&old);

    //设置特定信号屏蔽字
    printf("clear mask!\r\n");
    val = sigemptyset(&set);
    sigaddset(&set, 5);
    val = sigprocmask(SIG_UNBLOCK, &set, &old);
    show_signal_set(&old);
    val = sigprocmask(0, NULL, &old); //获取设置后的屏蔽位
    show_signal_set(&old);

    //设置固定信号屏蔽位
    printf("set fix mask!\r\n");
    val = sigemptyset(&set);
    sigaddset(&set, 9);
    sigaddset(&set, 8);
    sigaddset(&set, 7);
    val = sigprocmask(SIG_SETMASK, &set, &old);
    show_signal_set(&old);
    val = sigprocmask(0, NULL, &old); //获取设置后的屏蔽位
    show_signal_set(&old);

    printf("Done!\r\n");
    return 0;
}
#endif

void signal_cb(int ingo)
{
    printf("recv signal %d\r\n", ingo);
    if (ingo == SIGALRM)
    {
        printf("alarm signal wakup !\r\n");
    }
}
//测试信号阻塞时到解除的信号处理调用
int main(int argc, char *argv[])
{
    sigset_t set;
    sigset_t old;
    int val;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    show_signal_set(&set);

    if (signal(SIGALRM, signal_cb) == SIG_ERR)
    {
        printf("catch SIGALRM signal err!");
    }
    alarm(30);

    if (signal(SIGUSR1, signal_cb) == SIG_ERR)
    {
        printf("catch SIGUSR1 signal err!");
    }

    sleep(20);
    sigset_t pend_mask;
    sigpending(&pend_mask);
    printf("show pending mask!\r\n");
    show_signal_set(&pend_mask);
    printf("-------------------------------------------------\r\n");

    int index = 0;
    while (true)
    {
        pause();
        index++;
        printf("wakup signal %d\r\n", index);
        if (index == 1)
        {
            sigset_t set;
            sigemptyset(&set);
            sigaddset(&set, SIGUSR1);
            int val = sigprocmask(SIG_UNBLOCK, &set, NULL); //该函数属于不可重入函数, 放在信号处理程序里会出现问题。
            printf("val:%d,%s\r\n", errno, strerror(errno));
        }
    }

    printf("Done!\r\n");
    return 0;
}
