/**
 * @file fork.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 进程fork用法
 * @version 1.0.0
 * @date 2022-10-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
//#include <sys/types.h>

int main(void)
{
    printf("this is father!\r\n");
    int pid;
    
    for (int i=0; i<2; i++)
    {
        if ((pid = fork()) == 0)
        {
            printf("child!\r\n");
            int index = 0;
            while (1)
            {
                printf("child %d\r\n", index++);
                sleep(2);
            }
        }
        else {
            printf("create child %d\r\n", pid);
            //int status;
            //int ret_pid = wait(&status);
            //printf("ret pid:%d\r\n", ret_pid);
        }
    }
    printf("Done\r\n");
    return 0;
}