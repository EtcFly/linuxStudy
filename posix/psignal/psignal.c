/**
 * @file psignal.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  psignal
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
#include <time.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char *argv[])
{
    psignal(SIGINT, "signal init");
    printf("str:%s\r\n", strsignal(SIGSTOP));

    char str[20];
    sig2str(SIGINT, &str);
    printf("sig2str:%s\r\n", str);
    int v = 0;
    str2sig("Interrupt", &v);
    printf("v:%d\r\n", v);
    printf("Down!\r\n");
    return 0;
}