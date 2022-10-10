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

int main(int argc, char *argv[])
{
    printf("%%");
    printf("Done");

    char buff[1024];
    char *str = fgets(buff, sizeof(buff), stdin); //使用fgets会刷新stdout标准输出流
    sleep(10);
    printf("str:%s\r\n", str);
    return 0;
}
