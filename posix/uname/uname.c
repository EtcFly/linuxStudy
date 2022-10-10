/**
 * @file uname.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 系统调用:uname相关用法
 * @version 1.0.0
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>

int main(void)
{
    struct utsname name;
    if (uname(&name) < 0)
    {
        perror(strerror(errno));
        return -1;
    }

    printf("%s/%s/%s/%s/%s\r\n", name.machine, name.nodename, name.release, name.sysname, name.version);

    char hostname[100];
    gethostname(hostname, sizeof(hostname));
    printf("%s\r\n", hostname);
    printf("Done\r\n");
    return 0;
}