/**
 * @file fgets.c
 * @author your name (you@domain.com)
 * @brief 标准输入输出缓存刷新测试
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void set_fl(int fd, int flag)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        goto err;
    }

    val |= flag;
    if ((val = fcntl(fd, F_SETFL, val)) < 0) {
        goto err;
    }
    return ;

err:
    printf("errno:%d %s\n", errno, strerror(errno));
}

void clr_fl(int fd, int flag)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        goto err;
    }

    val &= ~flag;
    if ((val = fcntl(fd, F_SETFL, val)) < 0) {
        goto err;
    }
    return ;

err:
    printf("errno:%d %s\n", errno, strerror(errno));
}

int main(int argc, char *argv[])
{
    // stderr
    #define MAX_SIZE 1024*1204*2
    char *buf = malloc(MAX_SIZE);
    int fileSize = read(STDIN_FILENO, buf, MAX_SIZE);
    printf("fileSize:%d\r\n", fileSize);
    fprintf(stderr, "read size %d bytes\n", fileSize);

    set_fl(STDOUT_FILENO, O_NONBLOCK);

    int size = 0;
    while (size < fileSize) {
        errno = 0;
        int ret = write(STDOUT_FILENO, &buf[size], fileSize-size);
        fprintf(stderr, "write size %d bytes error:%d\n", ret, errno);
        if (ret > 0) {
            size += ret;
        }
    }
    clr_fl(STDOUT_FILENO, O_NONBLOCK);
    free(buf);
    return 0;
}
