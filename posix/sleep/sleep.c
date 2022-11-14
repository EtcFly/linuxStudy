/**
 * @file sleep.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  sleep测试系统执行
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

void signal_cb(int signo)
{
    printf("recv signal %d\r\n", signo);
}

int main(int argc, char *argv[])
{
    time_t now_time, prev_time;
    struct timeval val, nowval;
    struct timezone zone;

    signal(SIGQUIT, signal_cb);
    gettimeofday(&val, NULL);
    prev_time = time(NULL);
    sleep(5);
    now_time = time(NULL);
    gettimeofday(&nowval, NULL);

    printf("Down %ld:%ld\r\n", nowval.tv_sec - val.tv_sec,
           nowval.tv_usec - nowval.tv_usec);

    struct timespec spec, remain;

    spec.tv_sec = 1;
    spec.tv_nsec = 0;
    nanosleep(&spec, &remain); // remain表示被信号终止还剩多长时间延时   spec表示要求的延时

    system("date");
    clock_nanosleep(CLOCK_REALTIME, 0, &spec, NULL);
    system("date");
    return 0;
}