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

#define MSG_BASE_PATH "./semrcv.c"
#define MSG_BASE_ID (12345678)
int main(int argc, char *argv[])
{
    key_t key;
    key = ftok(MSG_BASE_PATH, MSG_BASE_ID);
    if (key < 0) {
        perror("ftok fail");
        return -1;
    }

    int ret, cnt;
    int semfd = semget(key, 2, IPC_CREAT | IPC_EXCL | S_IRWXU);
    if (semfd < 0) {
        if (errno != EEXIST) {
            perror("semfd get fail");
            return -1;
        }
        semfd = semget(key, 0, IPC_CREAT | S_IRWXU);
    }
    printf("semfd %d\r\n", semfd);

    struct semid_ds ds;
    ret = semctl(semfd, 10, IPC_STAT, &ds);
    if (ret < 0) {
        perror("semctl fail");
        return -1;
    }

    printf("ctime:%ld nsems:%ld otime:%ld\r\n", ds.sem_ctime, ds.sem_nsems, ds.sem_otime);
    printf("cgid:%d cuid:%d gid:%d mode:%X, uid:%d\r\n", ds.sem_perm.cgid, ds.sem_perm.cuid, ds.sem_perm.gid, ds.sem_perm.mode, ds.sem_perm.uid);
    cnt = semctl(semfd, 1, GETVAL);
    printf("get semcnt:%d\r\n", cnt);

    struct sembuf sembuf = {
        .sem_op = 1,
		.sem_num = 1,
		.sem_flg = SEM_UNDO,
    };
    ret = semop(semfd, &sembuf, 1);
    printf("get semop %d\r\n", ret);

	printf("get semcnt:%d\r\n", semctl(semfd, 1, GETVAL));
    return 0;
}
