/**
 * @file setuid.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 系统调用:测更改用户ID和更改组ID
 * @version 1.0.0
 * @date 2022-10-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <stdlib.h>

int main(void)
{
    printf("enter setuid process! uid:%d gid:%d euid:%d egid:%d\r\n", getuid(), getgid(), geteuid(), getegid());
    seteuid(126);
    printf("seteuid->126! uid:%d gid:%d euid:%d egid:%d\r\n", getuid(), getgid(), geteuid(), getegid());
    seteuid(0);
    printf("seteuid->0! uid:%d gid:%d euid:%d egid:%d\r\n", getuid(), getgid(), geteuid(), getegid());
    setuid(getuid());
    printf("seteuid->getpid! uid:%d gid:%d euid:%d egid:%d\r\n", getuid(), getgid(), geteuid(), getegid());
    printf("Done\r\n");
    return 0;
}
