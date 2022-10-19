/**
 * @file script.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 系统调用:脚本调用接口测试
 * @version 1.0.0
 * @date 2022-10-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <sys/acct.h>
int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork err!");
        return -1;
    }

    if (pid == 0)
    {
#define ARG_LIST "show", "no", "yes-baby", "this is a test", NULL
        char *argv[] = {ARG_LIST};
        char *env[] = {
            "USER=unknow",
            "PATH=/tmp",
            NULL,
        };
        // int status = execv("./show", argv);
        // int status = execve("./show", argv, env);
        int status = execve("./show.shell", argv, env);
        if (status < 0)
        {
            printf("exec err:%d, %s\r\n", errno, strerror(errno));
        }
        // int status = execvp("./show", argv);
        // int status = execl("./show", ARG_LIST);
        // int status = execle("./show", "this", "this is ates", NULL, env);
        // int status = execlp("./show", ARG_LIST);
        // int fd = open("./show", O_RDONLY);
        // if (fd < 0)
        // {
        //     perror("fd open err!");
        //     exit(-1);

        // int status = fexecve(fd, argv, env);
        printf("pid: %d has down! status:%d\r\n", pid, status);
    }
    else
    {
        int status;
        wait(&status);
        printf("pid:%d child_pid:%d status:%d\r\n", getpid(), pid, status);
    }

    printf("value:%d\r\n", sysconf(_SC_SAVED_IDS)); //测试是否支持保存ID
    printf("Done\r\n");
    return 0;
}
