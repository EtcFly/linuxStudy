/**
 * @file fork.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 父子进程vfork对比测试
 * @version 1.0.0
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

static int global_var = 10;
int main(void)
{
    pid_t pid;
    int localvar;

    localvar = 80;

    if ((pid = vfork()) == 0)
    {
        localvar += 1;
        global_var += 1;

        printf("[child] => uid:%d euid:%d gid:%d egid:%d\r\n", getuid(), geteuid(), getgid(), getegid());
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        printf("[father] => uid:%d euid:%d gid:%d egid:%d\r\n", getuid(), geteuid(), getgid(), getegid());
    }

    printf("pid:%d gloab:%d local:%d\r\n", pid, global_var, localvar);
    printf("Done\r\n");

    _exit(0);
    return 0;
}