/**
 * @file system.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  system测试系统执行
 * @version 1.0.0
 * @date 2022-10-30
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
#include <stdbool.h>
#include <setjmp.h>

int main(int argc, char *argv[])
{
    int v = system("./test");
    printf("Down:%d\r\n", v);
    return 0;
}