/**
 * @file select.c
 * @author your name (you@domain.com)
 * @brief io多路转接
 * @version 0.1
 * @date 2023-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <poll.h>

void show_time(const char *str)
{
    struct timespec now_time;
    clock_gettime(CLOCK_REALTIME, &now_time);
    printf("%s:%lu %lu\n", str, now_time.tv_sec, now_time.tv_nsec/1000);
}

int main(int argc, char *argv[])
{
    struct timeval val = {
        .tv_sec = 10,
        .tv_usec = 1000,
    };

    struct timespec valspec = {
        .tv_sec = 0,
        .tv_nsec = 2000,
    };
    show_time("start");

    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(STDIN_FILENO, &readfd);

    int ret = select(FD_SETSIZE, &readfd, NULL,NULL,&val);
    printf("select ret:%d\n", ret);
    if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfd)) {
        uint8_t readBuf[512];
        read(STDIN_FILENO, readBuf, sizeof(readBuf));
        printf("stdin data:%s\n", readBuf);
    }
    show_time("end");

    show_time("spec start");
    pselect(FD_SETSIZE, NULL, NULL, NULL, &valspec, NULL);
    show_time("spec end");

    struct pollfd fd[10];
    poll(fd, sizeof(fd)/sizeof(fd[0]), 10); 
    return 0;
}
