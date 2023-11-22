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
#include <sys/shm.h>
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

int main(int argc, char *argv[])
{
    void *addr = get_shm_mem(SHARE_FILE_NAME);
    if (addr == NULL) {
        return -1;
    }

    sem_t *sem = (sem_t *)addr;
    printf("semfd %p\r\n", sem);
    int ret = sem_init(sem, 1, 0);
    if (ret != 0) {
        perror("sem init fail");
        shmdt(addr);
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
    shm_unlink(SHARE_FILE_NAME);
    sem_close(sem);
    return 0;
}
