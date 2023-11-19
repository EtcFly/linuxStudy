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
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/mman.h>

static int __sem_init(int recv_flag)
{
#define SEM_BASE_PATH "../../sem/semrcv.c"
#define SEM_BASE_ID (87654321)
    int semid;
    key_t semkey = ftok(SEM_BASE_PATH, SEM_BASE_ID);
    if (semkey == -1) {
        perror("semkey get fail");
        return -1;
    }

    if (!recv_flag) {
        semid = semget(semkey, 1, IPC_CREAT | 0666);
        if (semid < 0) {
            perror("semget fail");
            return -1;
        }

        union semun
        {
            int val;
            struct semid_ds *buf;
            unsigned short int *array;
            struct seminfo *__buf;
        } set;
        set.val = 0;
        int ret = semctl(semid, 0, SETVAL, set);
        if (ret < 0) {
            perror("semctl fail");
            return -1;
        }
    } else {
        semid = semget(semkey, 1, 0666);
        if (semid < 0) {
            perror("semget fail");
            return -1;
        }
    }

    return semid;
}

static int __sem_wait(int semid)
{
    struct sembuf buf = {
        .sem_flg = 0,
        .sem_num = 0,
        .sem_op = -1,
    };
    int ret = semop(semid, &buf, 1);
    if (ret < 0) {
        perror("semop wait fail");
        return -1;
    }
    return 0;
}

static int __sem_post(int semid)
{
    struct sembuf buf = {
        .sem_flg = 0,
        .sem_num = 0,
        .sem_op = 1,
    };
    int ret = semop(semid, &buf, 1);
    if (ret < 0) {
        perror("semop wait fail");
        return -1;
    }
    return 0;
}

static void __sem_get_value(int semid)
{
    int semvalue = semctl(semid, 0, GETVAL);
    if (semvalue < 0) {
        perror("semvalue get fail");
        return ;
    }
    printf("semvalue is %d\r\n", semvalue);
}

#define MAX_SHM_SIZE (4)

int main(int argc, char *argv[])
{
    int fd = open("/dev/zero", O_RDONLY);
    if (fd < 0) {
        perror("open fail");
        return -1;
    }

    int *addr = mmap(0, MAX_SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0); ///dev/zero同样是匿名存储映射
    if (MAP_FAILED == addr) {
        perror("mmap fail");
        return -1;
    }
    close(fd);

    int semid = __sem_init(1);
    if (semid < 0) {
        perror("sem init fail");
        return -1;
    }

    while (1) {
        if (__sem_wait(semid) < 0) {
            break;
        }

        __sem_get_value(semid);
        for (int i = 0; i < MAX_SHM_SIZE; i+=4) {
            printf("i:%d addr:%d ", i, addr[i/4]);
        }
        // sleep(4);
        printf("\r\n");
    }

    int ret = munmap(addr, MAX_SHM_SIZE);
    if (ret < 0) {
        perror("del munmap fail");
        return -1;
    }

    return 0;
}
