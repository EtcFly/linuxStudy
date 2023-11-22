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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>

#define SEM_NAME "/helloworld"
int main(int argc, char *argv[])
{
    sem_t *sem = sem_open(SEM_NAME, 0);
    if (SEM_FAILED == sem) {
        perror("sem open fail");
        return -1;
    }

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

    sem_close(sem);
    return 0;
}

