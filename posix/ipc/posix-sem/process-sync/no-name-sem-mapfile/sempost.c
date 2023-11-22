/**
 * @file sempost.c
 * @author etcfly (etcflywy@126.com)
 * @brief 
 * @version 1.0.0
 * @date 2023-11-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
// #define __USE_GNU
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

#define SEM_NAME "/helloworld"
#define SHARE_FILE_NAME "./test"
#define SHARE_MEM_SIZE sizeof(sem_t)

int main(int argc, char *argv[])
{
    int fd = open(SHARE_FILE_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("open test fail");
        return -1;
    }
    lseek(fd, sizeof(sem_t)+1, SEEK_SET);
    ftruncate(fd, sizeof(sem_t)+1);

    void *addr = mmap(0, SHARE_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr) {
        perror("map fail");
        close(fd);
        return -1;
    }

    sem_t *sem = (sem_t *)addr;
    printf("semfd %p\r\n", sem);
    int ret = sem_init(sem, 1, 0);
    if (ret != 0) {
        perror("sem init fail");
        munmap(addr, SHARE_MEM_SIZE);
        close(fd);
        return -1;
    }

    int count = 0;
    int max_count = 10;
    while (1) {
        int ret = sem_post(sem);
        if (ret != 0) {
            perror("post fail");
            break;
        }
        printf("post sem ok %d\r\n", count++);
        usleep(100);
        if (count >= max_count) {
            break;
        }
    }
    pause();
    sem_close(sem);
    return 0;
}
