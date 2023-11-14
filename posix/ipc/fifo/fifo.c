/**
 * @file pipe.c
 * @author your name (you@domain.com)
 * @brief 管道通信
 * @version 0.1
 * @date 2023-10-21
 *
 * @copyright Copyright (c) 2023
 *
 */
 #include <asm-generic/errno-base.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_PIPE_SIZE 1000000000
#define PIPT_BUF_SIZE 4097

#define FIFIO_NAME "test-fifo"
int main(int argc, char *argv[])
{
    // int ret = mkfifo(FIFIO_NAME, 0644);
    // if (ret < 0 && errno != EEXIST) {
    //     perror("mkfifo fail");
    //     return -1;
    // }
    int dfd = open("../fifo", O_DIRECTORY);
    if (dfd < 0) {
        perror("open fd fail");
        return -1;
    }

    int ret = mkfifoat(dfd, "./fifoat", 0644);
    if (ret < 0 && errno != EEXIST) {
        perror("mkfifoat fail");
        return -1;
    }

    int fd = open(FIFIO_NAME, O_WRONLY);
    printf("open fifo\r\n");
    if (fd < 0) {
        perror("open fifo fail");
        return -1;
    }

    char *s = "hello fifo";
    ret = write(fd, s, strlen(s));
    printf("ret:%d\r\n", ret);
    sleep(2);
    // unlink(FIFIO_NAME);
    return 0;
}

