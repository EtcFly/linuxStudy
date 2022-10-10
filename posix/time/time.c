/**
 * @file time.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief 系统调用:time相关用法
 * @version 1.0.0
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main(void)
{
    time_t now_time, pre_time;
    pre_time = time(&now_time); //获取当前时间

    struct tm *now_time_fmt;

    now_time_fmt = gmtime(&pre_time); //不考虑时区, 直接转为utc时间
    printf("%ld %ld\r\n", now_time, pre_time);
    printf("%d-%d-%d\r\n", 1900 + now_time_fmt->tm_year, now_time_fmt->tm_mon + 1, now_time_fmt->tm_mday);
    printf("yday:%d wday:%d mday:%d\r\n", now_time_fmt->tm_yday, now_time_fmt->tm_wday, now_time_fmt->tm_mday);
    printf("%d:%d:%d\r\n", now_time_fmt->tm_hour + 8, now_time_fmt->tm_min, now_time_fmt->tm_sec);
    printf("%d, %s\r\n", now_time_fmt->tm_isdst, now_time_fmt->tm_zone);

    // localtime
    struct tm *local_tm_date;
    local_tm_date = localtime(&now_time); //和gmtime的区别是其考虑了本地时区, 夏令时等。
    printf("%d:%d:%d\r\n", local_tm_date->tm_hour, local_tm_date->tm_min, local_tm_date->tm_sec);

    // mktime
    time_t mk_time = mktime(local_tm_date);
    printf("mktime:%ld\r\n", mk_time);

    //  gettimeofday
    struct timeval timeval_v;
    struct timezone timezone_v;
    if (gettimeofday(&timeval_v, &timezone_v) < 0)
    {
        perror("gettimeofday err!");
        return -1;
    }

    printf("%ld %ld\r\n", timeval_v.tv_sec, timeval_v.tv_usec);                 // utc秒和毫秒
    printf("zone:%d:%d\r\n", timezone_v.tz_minuteswest, timezone_v.tz_dsttime); //时区信息 tz_dsttime(夏令时)  tz_minuteswest(格林尼治分)

    // clock_gettime
    struct timespec lc_time;
    clock_gettime(CLOCK_REALTIME, &lc_time);
    printf("%ld %ld\r\n", lc_time.tv_sec, lc_time.tv_nsec);

    printf("Done\r\n");
    return 0;
}