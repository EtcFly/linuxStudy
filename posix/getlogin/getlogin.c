/**
 * @file getlogin.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 测试getlogin函数接口
 * @version 1.0.0
 * @date 2022-10-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pwd.h>

int main(void)
{
    struct passwd *pw;

    printf("login:%s\r\n", getlogin());

    pw = getpwuid(getuid());
    printf("name:%s\r\n", pw->pw_name);
    printf("dir:%s\r\n", pw->pw_dir);
    printf("gecos:%s\r\n", pw->pw_gecos);
    printf("passwd:%s\r\n", pw->pw_passwd);
    printf("pw_shell:%s\r\n", pw->pw_shell);
    printf("gid:%d\r\n", pw->pw_gid);
    printf("uid:%d\r\n", pw->pw_uid);

    printf("%ld\r\n", sysconf(_SC_NZERO));
    printf("%ld\r\n", sysconf(_SC_CLK_TCK)); // 10ms一次时钟滴答

    printf("Done\r\n");
    return 0;
}