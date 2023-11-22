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
#include <sys/mman.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>

#define SEM_NAME "/helloworld"
#define SHARE_FILE_NAME "./test"
#define SHARE_MEM_SIZE sizeof(sem_t)

//! 必须sempost先启动初始化信号量, 然后再semrcv中不允许重复初始化信号量
int main(int argc, char *argv[])
{
    int fd = open(SHARE_FILE_NAME, O_RDWR, 0666);
    if (fd < 0) {
        perror("open test fail");
        return -1;
    }

    void *addr = mmap(0, SHARE_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr) {
        perror("map fail");
        close(fd);
        return -1;
    }

    sem_t *sem = (sem_t *)addr;
    printf("semfd %p\r\n", sem);
    int count = 0;
    while (1) {
        int ret = sem_wait(sem);
        if (ret != 0) {
            perror("wait sem fail");
            break;
        }
        printf("recv sem ok %d\r\n", count++);
    }

    return 0;
}

