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
#include <sys/ipc.h>
#include <sys/shm.h>

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

#define MSG_BASE_PATH "./shareMem_send.c"
#define MSG_BASE_ID (12345678)
#define MAX_SHM_SIZE (4096)

int main(int argc, char *argv[])
{
    key_t key = ftok(MSG_BASE_PATH, MSG_BASE_ID);
    if (key == -1) {
        perror("ftok fail");
        return -1;
    }

    int shmid = shmget(key, MAX_SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmid get fail");
        return -1;
    }

    get_shm_stat(shmid);

    int *addr = shmat(shmid, 0, 0);
    if (NULL == addr) {
        perror("addr NULL");
        return -1;
    }
    
    for (int i = 0; i < MAX_SHM_SIZE; i+=4) {
        addr[i/4] = i;
    }
    printf("get shmid: %d\r\n", shmid);

    return 0;
}

