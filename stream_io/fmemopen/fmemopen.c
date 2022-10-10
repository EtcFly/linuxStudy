/**
 * @file fmemopen.c
 * @author your name (you@domain.com)
 * @brief 内存流测试
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

int main(int argc, char *argv[])
{
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    FILE *file = fmemopen(buffer, sizeof(buffer), "r+"); //这里注意模式, 否则容易出错, 比如不支持rw模式
    if (NULL == file)
    {
        printf("err:%d %s\r\n", errno, strerror(errno));
        return -1;
    }

    fputs("this is a test\r\n", file);
    printf("%ld\r\n", ftell(file));
    fseek(file, 0, SEEK_SET);

    // fflush(file);

    char lc_buff[1024];
    printf("out:%s\r\n", fgets(lc_buff, sizeof(lc_buff), file));

    printf("%s\r\n", buffer);
    fclose(file);
    printf("Done\r\n");
    return 0;
}
