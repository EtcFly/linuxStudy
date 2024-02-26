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

#define SEM_NAME "/helloworld"
int main(int argc, char *argv[])
{
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 0);
    if (SEM_FAILED == sem) {
        if (errno == EEXIST) {
            sem = sem_open(SEM_NAME, 0);
        }

        if (SEM_FAILED == sem) {
            perror("sem open fail");
            return -1;
        }
    }

    printf("semfd %p\r\n", sem);

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

    sem_close(sem);
    return 0;
}
