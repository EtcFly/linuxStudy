/**
 * @file shareMem_send.c
 * @author etcfly (etcflywy@126.com)
 * @brief 
 * @version 1.0.0
 * @date 2023-11-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
// #define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <fcntl.h>
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
        perror("semop post fail");
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

static void get_shm_stat(int shmid)
{
    struct shmid_ds stat;
    int ret = shmctl(shmid, IPC_STAT, &stat);
    if (ret < 0) {
        perror("get stat fail");
        return ;
    }

    printf("atime:%ld cpid:%u    ctime:%ld  dtime:%ld\r\n", stat.shm_atime, stat.shm_cpid, stat.shm_ctime, stat.shm_dtime);
    printf("lpid:%u  nattch:%ld  segsz:%ld\r\n", stat.shm_lpid, stat.shm_nattch, stat.shm_segsz);
    printf("cgid:%d  cuid:%d  gid:%d  mode:%X  uid:%d\r\n", stat.shm_perm.cgid, stat.shm_perm.cuid, stat.shm_perm.gid, stat.shm_perm.mode, stat.shm_perm.uid);
}

#define MAX_SHM_SIZE (4)

int main(int argc, char *argv[])
{
    int fd = open("/dev/zero", 0666);
    if (fd < 0) {
        perror("open fail");
        return -1;
    }
    printf("open fd:%d\r\n", fd);
    int *addr = mmap(0, MAX_SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr) {
        perror("mmap fail");
        return -1;
    }
    close(fd);

    int semid = __sem_init(0);
    if (semid < 0) {
        return -1;
    }

    printf("mmap addr:%p\r\n", addr);
    int count = 0;
    int max_count = 150000;
    while (1) {
        for (int i = 0; i < MAX_SHM_SIZE; i+=4) {
            addr[i/4] = count;
            count++;
        }
        printf("write ok %d\r\n", count);
        if (__sem_post(semid) < 0) {
            break;
        }

        __sem_get_value(semid);
        usleep(100);

        if (count > max_count) {
            break;
        }
    }

    int ret = munmap(addr, MAX_SHM_SIZE);
    if (ret < 0) {
        perror("del shareMem fail");
        return -1;
    }
    printf("total send count: %d\r\n", count);

    return 0;
}

