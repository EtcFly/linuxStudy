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
    else
    {
        printf("recv signal code:%d\r\n", signal_code);
    }
}

int main(void)
{
    int index = 0;
    for (;;)
    {
        pause(); //每次收到监听信号才会唤醒一次, 默认信号不会
        index++;
        printf("pid:%d wakup timr %d!\r\n", getpid(), index);
    }
    printf("Done\r\n");
    return 0;
}