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
 #define _GNU_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_PIPE_SIZE 1000000000
#define PIPT_BUF_SIZE 4097
void child_exec(int fd[2])
{
    close(fd[0]);
    int max_write_len = MAX_PIPE_SIZE;
    int curr_write_len = 0;
    while (max_write_len > curr_write_len) {
        char buf[PIPT_BUF_SIZE];
        for (int i = 0; i < sizeof(buf); i++) {
            buf[i] = 0;
        }

        int l = write(fd[1], buf, sizeof(buf));
        printf("pid %d wirte l:%d\r\n", getpid(), l);
        curr_write_len += sizeof(buf);
        usleep(10);
    }
    close(fd[1]); //end of fd[1]
}

void child_exec2(int fd[2])
{
    close(fd[0]);
    int max_write_len = MAX_PIPE_SIZE;
    int curr_write_len = 0;
    while (max_write_len > curr_write_len) {
        char buf[PIPT_BUF_SIZE];
        for (int i = 0; i < sizeof(buf); i++) {
            buf[i] = 1;
        }

        int l = write(fd[1], buf, sizeof(buf));
        printf("pid %d wirte l:%d\r\n", getpid(), l);
        curr_write_len += sizeof(buf);
        usleep(10);
    }
    close(fd[1]); //end of fd[1]
}

int main(int argc, char *argv[])
{
    int fd[2] = {0x00, 0x00};
    int ret = pipe(fd);
    if (ret < 0) {
        perror("pipe fail");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork fail");
        return -1;
    }

    if (pid > 0) {
        int pid2 = fork();
        if (pid2 < 0) {
            perror("fork2 fail");
            return -1;
        }
        if (pid2 == 0) {
            child_exec2(fd);
            exit(-1); // end of child
        }

        close(fd[1]);

        int curr_read_size = 0;
        while (1) {
            char buf[PIPT_BUF_SIZE];
            int l = read(fd[0], buf, sizeof(buf));
            if (l == 0) {
                break;
            }
            printf("read file len %d dump:", l);
            for (int i = 0; i < l; i++) {
                if (buf[0] != buf[i]) {
                    printf("buf get fail\r\n");
                    break;
                }
            }
            printf("\r\n");
            curr_read_size += l;
        }
        printf("curr_read_size %d\r\n", curr_read_size);

        int status;
        waitpid(pid, &status, 0);
        waitpid(pid2, &status, 0);
    } else {
        child_exec(fd);
        exit(-1);
    }

    printf("pipe end\r\n");
    return 0;
}

