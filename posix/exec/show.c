/**
 * @file exec.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 系统调用:exec相关接口测试
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

extern char **environ;
int main(int argc, char *argv[])
{
    if (0 == argc)
    {
        printf("argc == 0\r\n");
        return -1;
    }

    printf("argc:%d\r\n", argc);
    for (int i = 0; argv[i] != NULL; i++)
    {
        printf("argv[%d]:%s\r\n", i, argv[i]);
    }

    for (int i = 0; environ[i] != NULL; i++)
    {
        printf("environ[%d]:%s\r\n", i, environ[i]);
    }
    printf("Done pid:%d ppid:%d\r\n", getpid(), getppid());
    return 0;
}
