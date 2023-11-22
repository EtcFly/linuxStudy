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
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem;
static void * thread(void *arg)
{
    int count = 0;
    while (1) {
        int ret = sem_wait(&sem);
        if (ret < 0) {
            perror("recv fail");
            break;
        }
        printf("sem recv ok %d\r\n", count++);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t pthread;

    int ret = sem_init(&sem, 0, 0);
    if (ret < 0) {
        perror("sem init fail");
        return -1;
    }

    if (0 != pthread_create(&pthread, NULL, thread, NULL)) {
        perror("thread create fail");
        sem_destroy(&sem);
        return -1;
    }

    int count = 0;
    int max_count = 10;
    while (1) {
        ret = sem_post(&sem);
        if (ret < 0) {
            perror("post fail");
            break;
        }
        printf("sem post ok %d\r\n", count++);
        if (count > max_count) {
            break;
        }
    }
    pthread_join(pthread, NULL);
    sem_destroy(&sem);
    return 0;
}
