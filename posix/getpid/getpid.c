/**
 * @file getpid.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  获取进程pid信息
 * @version 1.0.0
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid, ppid;
    uid_t uid, euid;
    gid_t gid, egid;

    pid = getpid();
    ppid = getppid();
    printf("pid:%d ppid:%d\r\n", pid, ppid);

    uid = getuid();
    euid = geteuid();
    printf("uid:%d guid:%d\r\n", uid, euid);

    gid = getgid();
    egid = getegid();
    printf("gid:%d egid:%d\r\n", gid, egid);

    printf("Done\r\n");
    return 0;
}
