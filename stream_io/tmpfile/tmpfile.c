/**
 * @file tmpfile.c
 * @author your name (you@domain.com)
 * @brief 测试tmp临时文件
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    //1、mkdtemp, 该概述必须以XXXXXX为结尾, 多线程容易生成重复字符串名
    char tmp_buff[] = "/tmp/tmpfileXXXXXX";
    char *name = mkdtemp(tmp_buff);
    printf("name:%s\r\n", name);
    unlink(name);

    //2、mkstemp
    char mktmp_buff[] = "/tmp/mktmpXXXXXX";
    int fd = mkstemp(mktmp_buff);
    if (fd < 0) 
    {
        printf("line:%d err:%d,%s\r\n", __LINE__, errno, strerror(errno));
        return -1;
    }
    FILE *file = fdopen(fd, "rw+");
    if (NULL == file)
    {
        printf("line:%d err:%d,%s\r\n", __LINE__, errno, strerror(errno));
        return -1;
    }
    int len = fputs("this is  a test\r\n", file);
    if (len <= 0)
    {
        printf("line:%d err:%d,%s\r\n", __LINE__, errno, strerror(errno));
        return -1;
    }
    rewind(file);
    char lc_tmp[100];
    char* get_str = fgets(lc_tmp, sizeof(lc_tmp), file);
    if (NULL == get_str)
    {
        fclose(file);
        return -1;
    }
    unlink(mktmp_buff); //文件不会主动删除, 需要手动删除
    printf("len:%d %s\r\n", len, get_str);

    //3、mkstemps 添加尾部, 尾部长度test需要指定, 否则报错
    char memtmp_buff[] = "/tmp/memXXXXXXtest";
    int fd2 = mkstemps(memtmp_buff, 4);
    if (fd2 < 0)
    {
        printf("line:%d err:%d,%s\r\n", __LINE__, errno, strerror(errno));
        return -1;
    }
    unlink(memtmp_buff); //文件不会主动删除, 需要手动删除
    printf("fd2:%d\r\n", fd2);
    printf("Done\r\n");

    sleep(10);
    return 0;
}