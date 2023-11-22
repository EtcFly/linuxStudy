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
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    sem_t *sem = (sem_t *)mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (MAP_FAILED == sem) {
        perror("map fail");
        return -1;
    }

    int ret = sem_init(sem, 1, 0);
    if (ret < 0) {
        perror("sem init fail");
        return -1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        int count = 0;
        while (1) {
            ret = sem_wait(sem);
            if (ret != 0) {
                perror("wait sem fail");
                break;
            }
            printf("recv sem ok %d\r\n", count++);
        }
        exit(-1);
    } else {
        int count = 0;
        int max_count = 10;
        while (1) {
            int ret = sem_post(sem);
            if (ret != 0) {
                perror("post fail");
                break;
            }

            // sem_getvalue(sem, &count);
            printf("post sem ok %d\r\n", count++);
            usleep(100);
            if (count >= max_count) {
                break;
            }
        }
    }
    
    int status;
    waitpid(pid, &status, 0);
    sem_destroy(sem);
    return 0;
}
