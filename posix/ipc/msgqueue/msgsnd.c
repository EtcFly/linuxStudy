/**
 * @file pipe.c
 * @author your name (you@domain.com)
 * @brief 管道通信
 * @version 0.1
 * @date 2023-10-21
 *
 * @copyright Copyright (c) 2023
 *
 */
 #include <asm-generic/errno-base.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


#define MSG_BASE_PATH "./msgrcv.c"
#define MSG_BASE_ID (12345678)
int main(int argc, char *argv[])
{
    key_t key = ftok(MSG_BASE_PATH, MSG_BASE_ID);
    if (key == -1) {
        perror("ftok fail");
        return -1;
    }

    int msgid = msgget(key, IPC_CREAT | IPC_EXCL | S_IRWXU);
    if (msgid < 0) {
        if (errno == EEXIST) {
            msgid = msgget(key, S_IRWXU);
        }
        if (msgid < 0) {
            perror("msgid get fail");
            return -1;
        }
    }
    printf("msgid:% d errno:%d\r\n", msgid, errno);
    struct msqid_ds s_ds;
    int ret = msgctl(msgid, IPC_STAT, &s_ds);
    if (ret != 0) {
        perror("msgctl fail");
        return -1;
    }
    printf("cgid:%d cuid:%d mod:%d, gid:%d uid:%d\r\n", s_ds.msg_perm.cgid, s_ds.msg_perm.cuid, s_ds.msg_perm.mode, s_ds.msg_perm.gid, s_ds.msg_perm.uid);

    s_ds.msg_qbytes = 16*1024;
    ret = msgctl(msgid, IPC_SET, &s_ds);
    if (ret < 0) {
        perror("set ipc_ds");
        return -1;
    }
    printf("qnum:%ld qbyte:%ld lrpid:%d lspid:%d\r\n", s_ds.msg_qnum, s_ds.msg_qbytes, s_ds.msg_lrpid, s_ds.msg_lspid);
    printf("ctime:%ld rtime:%ld stime:%ld\r\n", s_ds.msg_ctime, s_ds.msg_rtime, s_ds.msg_stime);
    printf("cgid:%d cuid:%d mod:%d, gid:%d uid:%d\r\n", s_ds.msg_perm.cgid, s_ds.msg_perm.cuid, s_ds.msg_perm.mode, s_ds.msg_perm.gid, s_ds.msg_perm.uid);
    struct msg {
        long type;
        char buf[512];
    };
    struct msg smsg;
    for (int i = 0; i < 100; i++) {
        smsg.type = i+1;
        for (int j = 0; j < sizeof(smsg.buf); j++) {
            smsg.buf[j] = j+i;
        }
        int ret = msgsnd(msgid, &smsg, sizeof(smsg.buf), 0);
        if (ret < 0) {
            printf("msgsnd fail %d %s\r\n", i, strerror(errno));
            break;
        }
        // sleep(1);
        struct msqid_ds s_ds;
        ret = msgctl(msgid, IPC_STAT, &s_ds);
        if (ret != 0) {
            perror("msgctl fail");
            return -1;
        }
        printf("qnum:%ld qbyte:%ld lrpid:%d lspid:%d\r\n", s_ds.msg_qnum, s_ds.msg_qbytes, s_ds.msg_lrpid, s_ds.msg_lspid);
        printf("ctime:%ld rtime:%ld stime:%ld\r\n", s_ds.msg_ctime, s_ds.msg_rtime, s_ds.msg_stime);
    }
    ret = msgctl(msgid, IPC_RMID, NULL);
    printf("msgctl rm ret %d errno:%d\r\n", ret, errno);

    return 0;
}

