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
 #define _GNU_SOURCE
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    key_t key = ftok("./ftok.c", 12345678);
    if (key == -1) {
        perror("ftok fail");
        return -1;
    }
    printf("ftok: %d\r\n", key);
    return 0;
}

