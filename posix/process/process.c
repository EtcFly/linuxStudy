/**
 * @file process.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 进程环境测试
 * @version 1.0.0
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>

void my_exit1(void)
{
    printf("my exit1\r\n");
}

void my_exit2(void)
{
    printf("my exit2\r\n");
}

extern char **environ;

void show_process_para(const char *name, int resc)
{
    struct rlimit limit;
    int ret = getrlimit(resc, &limit);
    if (ret < 0)
    {
        printf("%s: err %d,%s\r\n", name, errno, strerror(errno));
        return;
    }

    int v = 0;
    if (limit.rlim_cur == RLIM_INFINITY)
    {
        v |= 0x01;
    }
    if (limit.rlim_max == RLIM_INFINITY)
    {
        v |= 0x02;
    }

    switch (v)
    {
    case 0:
        printf("%s: curr[%ld] max[%ld]\r\n", name, limit.rlim_cur, limit.rlim_max);
        break;
    case 1:
        printf("%s: curr[INF] max[%ld]\r\n", name, limit.rlim_max);
        break;
    case 2:
        printf("%s: curr[%ld] max[INF]\r\n", name, limit.rlim_cur);
        break;
    case 3:
        printf("%s: curr[%ld] max[%ld]\r\n", name, limit.rlim_cur, limit.rlim_max);
        break;
    default:
        printf("error limit value:%d\r\n", v);
        break;
    }
}
int main(void)
{
    long max_atexit = sysconf(_SC_ATEXIT_MAX);
    printf("最大exit:%ld\r\n", max_atexit);

    //注册exec退出回调
    atexit(my_exit1);
    atexit(my_exit1);
    atexit(my_exit2);
    printf("Done\r\n");

    //打印系统环境变量
    for (int i = 0; environ[i] != NULL; i++)
    {
        printf("%s\r\n", environ[i]);
    }

    printf("\r\n\r\n");
#define SHOW_LIMIT_PARA(name) show_process_para(#name, name)
    //获取系统限制
    SHOW_LIMIT_PARA(RLIMIT_AS);
    SHOW_LIMIT_PARA(RLIMIT_CORE);
    SHOW_LIMIT_PARA(RLIMIT_CPU);
    SHOW_LIMIT_PARA(RLIMIT_DATA);
    SHOW_LIMIT_PARA(RLIMIT_FSIZE);
    SHOW_LIMIT_PARA(RLIMIT_MEMLOCK);
    SHOW_LIMIT_PARA(RLIMIT_MSGQUEUE);
    SHOW_LIMIT_PARA(RLIMIT_NICE);
    SHOW_LIMIT_PARA(RLIMIT_NOFILE);
    SHOW_LIMIT_PARA(RLIMIT_NPROC);
    SHOW_LIMIT_PARA(RLIMIT_RSS);
    SHOW_LIMIT_PARA(RLIMIT_SIGPENDING);
    SHOW_LIMIT_PARA(RLIMIT_STACK);

    printf("\r\n\r\n");

    //获取系统环境变量
    printf("%s\r\n", getenv("HOME"));
    printf("%s\r\n", getenv("LINES"));
    printf("%s\r\n", getenv("PATH"));
    printf("%s\r\n", getenv("PWD"));

    printf("\r\n\r\n");
    // return 0;
    // exit(0);
}
