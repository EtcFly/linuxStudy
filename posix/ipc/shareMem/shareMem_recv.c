/**
 * @file shareMem_recv.c
 * @author etcfly (etcflywy@126.com)
 * @brief 
 * @version 1.0.0
 * @date 2023-11-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
// #define __USE_GNU
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define MSG_BASE_PATH "./shareMem_send.c"
#define MSG_BASE_ID (12345678)
#define MAX_SHM_SIZE (4096)

int main(int argc, char *argv[])
{
    key_t key;
    key = ftok(MSG_BASE_PATH, MSG_BASE_ID);
    if (key < 0) {
        perror("ftok fail");
        return -1;
    }

    int shmid = shmget(key, 0, 0666);
    if (shmid < 0) {
        perror("shmid get fail");
        return -1;
    }

    int *addr = shmat(shmid, 0, SHM_RDONLY);
    if (NULL == addr) {
        perror("NULL addr");
        return -1;
    }

    for (int i = 0; i < MAX_SHM_SIZE; i+=4) {
        printf("i:%d addr: %d\r\n", i, addr[i/4]);
    }

    return 0;
}
