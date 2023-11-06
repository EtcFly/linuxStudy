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


int main(int argc, char *argv[])
{
    FILE *pipe = popen("./sub", "r");
    if (pipe == NULL) {
        perror("pipe fail");
        return -1;
    }

    char buf[512];
    while (fgets(buf, sizeof(buf), pipe) != NULL) {
        printf("recv: %s", buf);
    }

    pclose(pipe);

    return 0;
}

