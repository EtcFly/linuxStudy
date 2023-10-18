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
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

void set_fl(int fd, int flag)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        goto err;
    }

    val |= flag;
    if ((val = fcntl(fd, F_SETFL, val)) < 0) {
        goto err;
    }
    return ;

err:
    printf("errno:%d %s\n", errno, strerror(errno));
}

void clr_fl(int fd, int flag)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        goto err;
    }

    val &= ~flag;
    if ((val = fcntl(fd, F_SETFL, val)) < 0) {
        goto err;
    }
    return ;

err:
    printf("errno:%d %s\n", errno, strerror(errno));
}

int main(int argc, char *argv[])
{
    int fd = open("./test.file", O_CREAT | O_RDWR | O_TRUNC , 0777);
    if (fd < 0) {
        printf("open file fail %d %s\n", errno, strerror(errno));
        return fd;
    }
    struct flock lock;
    pid_t pid = fork();
    if (pid == 0) {
        lock.l_len = 0;
        lock.l_start = 0;
        lock.l_whence = SEEK_END;
        lock.l_type = F_WRLCK;
        errno = 0;
        int ret = fcntl(fd, F_SETLK, &lock); //子进程设置写锁
        printf("child lock ret:%d %s\n", errno, strerror(errno));

        const char *s = "child write";
        int len = write(fd, s, strlen(s));

        sleep(5); //延迟5s释放写锁
        lock.l_len = -strlen(s);
        lock.l_type = F_UNLCK;
        errno = 0;
        ret = fcntl(fd, F_SETLK, &lock); //释放写锁
        printf("child unlock ret:%d %s\n", errno, strerror(errno));
    } else {
        sleep(1); //等待子进程锁住资源
        printf("parent sleep end %d\n", pid);
        lock.l_len = 12; //子进程锁未释放, SEEK_SET一个超过文件当前长度的位置导致文件尾偏移, 等下子进程通过-strlen(len)释放文件锁实际不成功(返回却成功, 疑惑!)
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_type = F_RDLCK;
        errno = 0;
        int ret = fcntl(fd, F_SETLKW, &lock); //设置读锁
        printf("parent lock ret:%d %s\n", errno, strerror(errno));
        if (ret != 0) {
            ret = fcntl(fd, F_GETLK, &lock);
            printf("parent get lock ret:%d %s %d, type:%d\n", errno, strerror(errno), lock.l_pid, lock.l_type);
        }

        sleep(1); //延迟释放读锁
        lock.l_type = F_UNLCK;
        errno = 0;
        ret = fcntl(fd, F_SETLKW, &lock); //释放锁
        printf("parent unlock ret:%d %s\n", errno, strerror(errno));

        int status;
        errno = 0;
        ret = waitpid(pid, &status, 0); //等待子进程结束
        printf("waipid ret:%d %s\n", errno, strerror(errno));
    }
    sleep(5);
    printf("sleep end pid:%d\n", pid);
    close(fd);
    printf("pid:%d open ok!\r\n", pid);
    return 0;
}
