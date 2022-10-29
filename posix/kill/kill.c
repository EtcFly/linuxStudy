/**
 * @file kill.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  kill杀死进程
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

#if 1
void signal_cb(int igno)
{
    printf("recv signal :%d\r\n", igno);
}
// kill
int main(int argc, char *argv[])
{
    if (signal(SIGINT, signal_cb) == SIG_ERR)
    {
        printf("signal stop set err!");
        exit(-1);
    }

    if (signal(SIGQUIT, signal_cb) == SIG_ERR)
    {
        printf("signal stop set err!");
        exit(-1);
    }

    raise(SIGQUIT);

    printf("Done\r\n"); //无法执行
    return 0;
}
#endif
