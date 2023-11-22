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
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHARE_FILE_NAME "/test"
#define SHARE_MEM_SIZE sizeof(sem_t)

static void *get_shm_mem(const char *name)
{
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("fail open shm");
        return NULL;
    }
    ftruncate(fd, sizeof(sem_t));
    
    void *addr = mmap(0, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap fail");
        close(fd);
        return NULL;
    }
    return addr;
}

//! 需要semPost进程先起来初始化sem_t
int main(int argc, char *argv[])
{
    void *addr = get_shm_mem(SHARE_FILE_NAME);
    if (addr == NULL) {
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

    sem_close(sem);
    return 0;
}

