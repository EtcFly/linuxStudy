/**
 * @file fopen.c
 * @author your name (you@domain.com)
 * @brief 测试ferror和feof函数
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fd = fopen("./test.txt", "w");
    if (NULL == fd)
    {
        printf("open file err:%d:%s\r\n", errno, strerror(errno));
        perror(strerror(errno));
        return -1;
    }

    int ch;

    while (EOF != (ch = fgetc(fd)))
    {
        printf("%c", ch);
    }
    printf("\r\n");
    if (ferror(fd))
    {
        printf("文件操作出错\r\n");
    }
    else if (feof(fd))
    {
        printf("到达文件结尾\r\n");
    }

    int lc_fd = fileno(fd);
    printf("fd:%d\r\n", lc_fd);

    fclose(fd);
    printf("Done\r\n");
    return 0;
}