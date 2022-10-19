/**
 * @file wait.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  wait进程状态获取
 * @version 1.0.0
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// NOTE: 1、测试父进程终止后子进程由init进程接管
#if 0
int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }

    if (pid == 0) // for child
    {
        pid_t child_pid;
        if ((child_pid = fork()) < 0)

        {
            perror("fork err");
            exit(0);
        }

        if (child_pid != 0)
        {
            exit(0);
        }

        sleep(1);
        printf("ppid:%d\r\n", getppid());
        exit(0);
    }

    printf("wait pid:%d\r\n", waitpid(pid, NULL, 0));
    sleep(3);
    printf("Done\r\n");
    return 0;
}
#endif

// 2、测试wait/waitpid等待不同线程
#if 0
#include <time.h>
int main(int argc, char *argv[])
{
    pid_t pid1, pid2;
    pid_t ret_pid;
    int status;

    if ((pid1 = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }
    else
    {
        if (pid1 == 0)
        {
            sleep(2);
            printf("this is test1\r\n");
            exit(0);
        }
    }

    if ((pid2 = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }
    else
    {
        if (pid2 == 0)
        {
            sleep(4);
            printf("this is test2\r\n");
            exit(0);
        }
    }

    // ret_pid = wait(&status);
    ret_pid = waitpid(pid2, &status, 0); // wait processs 2

    printf("wait2\r\n");
    ret_pid = wait(&status);
    printf("Done\r\n");
    return 0;
}
#endif

// 3、测试wait获取进程不同类型终止状态码
#if 0
void show_process_exit_code(int status)
{
    if (WIFEXITED(status)) // normal exit
    {
        int exit_code = WEXITSTATUS(status);
        printf("normal exit code:%d\r\n", exit_code);
    }
    else if (WIFSIGNALED(status))
    {
        int exit_code = WTERMSIG(status);
        printf("abort exit code:%d coredump file generated\r\n", exit_code);
    }
    else if (WIFSTOPPED(status))
    {
        int exit_code = WSTOPSIG(status);
        printf("stop exit code:%d\r\n", exit_code);
    }
}
int main(int argc, char *argv[])
{
    pid_t pid;
    pid_t ret_pid;
    int status;
    if ((pid = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }

    if (pid == 0)
    {
        abort();
    }
    else
    {
        ret_pid = wait(&status);
        show_process_exit_code(status);
    }

    if ((pid = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }

    if (pid == 0)
    {
        exit(0);
    }
    else
    {
        ret_pid = wait(&status);
        show_process_exit_code(status);
    }

    if ((pid = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }

    if (pid == 0)
    {
        int value = 100;
        value /= 0;
    }
    else
    {
        ret_pid = wait(&status);
        show_process_exit_code(status);
    }
    printf("Done\r\n");
    return 0;
}
#endif

// 4、测试waitpid的用法
/*
 pid == -1: 等待任意子进程
 pid == 0: 等待组ID等于调用进程组ID的任一子进程
 pid > 0: 等待特定ID的子进程
 pid < -1: 等待组ID等于pid绝对值的子进程
*/
#if 0
#include <time.h>
int main(int argc, char *argv[])
{
    pid_t pid1, pid2;
    pid_t ret_pid;
    int status;

    if ((pid1 = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }
    else
    {
        if (pid1 == 0)
        {
            sleep(2);
            printf("this is test1\r\n");
            exit(0);
        }
    }

    if ((pid2 = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }
    else
    {
        if (pid2 == 0)
        {
            sleep(4);
            printf("this is test2\r\n");
            exit(0);
        }
    }

    // ret_pid = wait(&status);
    printf("process 1 pid:%d\r\n", pid1);
    printf("process 2 pid:%d\r\n", pid2);
    ret_pid = waitpid(-1, &status, WNOHANG); // wait processs 2

    printf("wait2 %d\r\n", ret_pid);
    ret_pid = wait(&status);
    printf("Done\r\n");
    return 0;
}
#endif

// 5、测试waittid用法
#include <time.h>
int main(int argc, char *argv[])
{
    pid_t pid1, pid2;
    pid_t ret_pid;
    int status;

    if ((pid1 = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }
    else
    {
        if (pid1 == 0)
        {
            sleep(2);
            printf("this is test1\r\n");
            exit(0);
        }
    }

    if ((pid2 = fork()) < 0)
    {
        perror("fork err");
        return -1;
    }
    else
    {
        if (pid2 == 0)
        {
            sleep(4);
            printf("this is test2\r\n");
            exit(0);
        }
    }

    // ret_pid = wait(&status);
    printf("process 1 pid:%d\r\n", pid1);
    printf("process 2 pid:%d\r\n", pid2);
    // ret_pid = waitid(-1, &status, WNOHANG); // wait processs 2
    // ret_pid = waitid(P_PID, pid2, NULL, WEXITED); //等待特定进程
    ret_pid = waitid(P_PGID, getegid(), NULL, WEXITED); //等待特定进程组任意子进程
                                                        // ret_pid = waitid(P_ALL, 0, NULL, WEXITED);     //等待任意子进程

    printf("wait2 %d group:%d\r\n", ret_pid, getgid());
    ret_pid = wait(&status);
    printf("Done\r\n");
    return 0;
}