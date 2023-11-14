/**
 * @file msgrcv.c
 * @author etcfly (etcflywy@126.com)
 * @brief 
 * @version 1.0.0
 * @date 2023-11-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#define __USE_GNU
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#define MSG_BASE_PATH "./msgrcv.c"
#define MSG_BASE_ID (12345678)
void hexdump(void *buf, int len)
{
    printf("recv len:%d ", len);
    for (int i = 0; i < len; i++) {
        printf("%d ", ((char *)buf)[i]);
    }
    printf("\r\n");
}
int main(int argc, char *agrv[])
{
    key_t key = ftok(MSG_BASE_PATH, MSG_BASE_ID);
    if (key == -1) {
        perror("ftok fail");
        return -1;
    }

    int msgid = msgget(key, S_IRUSR);
    if (msgid < 0) {
        perror("get msgid fail");
        return -1;
    }

    struct msg {
        long type;
        char buf[512];
    };

    struct msg rmsg;
    int ret = 0;
    do {
        ret = msgrcv(msgid, &rmsg, sizeof(rmsg.buf), 0, MSG_EXCEPT);
        if (ret < 0) {
            printf("msgrcv fail ret:%d\r\n", ret);
            break;
        }
        hexdump(rmsg.buf, ret); //dump 接受的数据
    } while (ret >= 0);
    return 0;
}