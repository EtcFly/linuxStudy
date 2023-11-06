/**
 * @file readV_writeV.c
 * @author your name (you@domain.com)
 * @brief read多缓冲区读写
 * @version 0.1
 * @date 2023-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <bits/types/struct_iovec.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


uint8_t buff1[512];
uint8_t buff2[1024];
uint8_t buff3[763];

uint8_t buff4[128];
uint8_t buff5[512];
uint8_t buff6[2048];
int main(int argc, char *argv[])
{
    int fd = open("./test.file", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd < 0) {
        perror("fd open fail\n");
        return -1;
    }

    memset(buff1, 'O', sizeof(buff1));
    buff1[sizeof(buff1)-1]='\n';
    memset(buff2, 'p', sizeof(buff2));
    buff2[sizeof(buff2)-1]='\n';
    memset(buff3, 'e', sizeof(buff3));
    buff3[sizeof(buff3)-1]='\n';

    struct iovec ec[3] = {
        [0].iov_base = buff1,
        [0].iov_len = sizeof(buff1),
        [1].iov_base = buff2,
        [1].iov_len = sizeof(buff2),
        [2].iov_base = buff3,
        [2].iov_len = sizeof(buff3),
    };
    //写多个缓冲区数据, 一次系统调用, 内核中直接将缓冲区复制到内核缓冲中
    int ret = writev(fd, ec, sizeof(ec)/sizeof(ec[0]));
    if (ret < 0) {
        perror("writeV fail\n");
        return -1;
    }
    printf("writen ok %d\n", ret);
    
    
    struct iovec rec[3] = {
        [0].iov_base = buff4,
        [0].iov_len = sizeof(buff4),
        [1].iov_base = buff5,
        [1].iov_len = sizeof(buff5),
        [2].iov_base = buff6,
        [2].iov_len = sizeof(buff6),
    };

    lseek(fd, 0, SEEK_SET);
    //读取多个缓冲区数据, 一次系统调用, 内核中直接将结果复制到缓冲区, 先填满一个缓冲区才填充接下来缓冲区
    ret = readv(fd, rec, sizeof(rec)/sizeof(rec[0]));
    if (ret < 0) {
        perror("readv fail\n");
        return -1;
    }

    printf("readv %d\n", ret);
    printf("buff4:%s\n", buff4);
    printf("buff5:%s\n", buff5);
    printf("buff6:%s\n", buff6);
    
    close(fd);
    return 0;
}
