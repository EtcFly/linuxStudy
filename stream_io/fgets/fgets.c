/**
 * @file fgets.c
 * @author your name (you@domain.com)
 * @brief 标准输入输出缓存刷新测试
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    // printf("%%");
    // printf("Done");

    // char buff[1024];
    // char *str = fgets(buff, sizeof(buff), stdin); //使用fgets会刷新stdout标准输出流
    // sleep(10);
    // printf("str:%s\r\n", str);
    char buff[500000];
    printf("\r\n");
    int readSize = read(STDIN_FILENO, buff, sizeof(buff));
    printf("readSize:%d\r\n", readSize);

    int cmd = fcntl(STDOUT_FILENO, O_NONBLOCK);
    int writeSize = 0;
    while (readSize) {
        int size = write(STDOUT_FILENO, &buff[writeSize], readSize);
        if (size < 0) {
            printf("write fail:%d \r\n", writeSize);
            return -1;
        }
        writeSize += size;
        readSize -= size;
    }
    fcntl(STDOUT_FILENO, cmd);
    return 0;
}
