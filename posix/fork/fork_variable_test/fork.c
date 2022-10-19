/**
 * @file fork.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 进程fork局部变量测试
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

static int globalval = 10;
int main(void)
{
    pid_t pid;
    int localvar;
    const char *pstr = "this is a write test!\r\n";

    printf("this is father!\r\n");
    write(STDOUT_FILENO, pstr, strlen(pstr));

    localvar = 80;
    if ((pid = fork()) == 0)
    {
        localvar += 1;
        globalval += 1;
    }
    else
    {
        sleep(4);
    }

    printf("pid:%d global:%d %p local:%d %p\r\n", pid, globalval, &globalval, localvar, &localvar);

    printf("Done\r\n");
    return 0;
}