/**
 * @file pthread.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread线程私有数据接口测试
 * @version 1.0.0
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define _GNU_SOURCE

/**
 * @brief 线程创建和销毁相关接口
 * pthread_key_create: 线程创建
 * pthread_key_delete:
 * pthread_once:
 * pthread_getspecific:
 * pthread_setspecific:
 */

#define EXEC_RETURN_ZERO(x)              \
    do                                   \
    {                                    \
        if (!(x))                        \
        {                                \
            printf("%s [FAIL]\r\n", #x); \
        }                                \
    } while (0)

void show_pthread_info(const char *str)
{
    printf("%s:pid:%llu tid:%llX\r\n", str, (unsigned long long)getpid(), (unsigned long long)pthread_self());
}

void show_clean_cb(void *arg)
{
    printf("clean cb: %s\r\n", (char *)arg);
}

unsigned long long count_index = 0;
pthread_key_t key_v;
pthread_once_t once_v = PTHREAD_ONCE_INIT;

void ley_v_free(void *arg)
{
    if (NULL != arg)
    {
        printf("free mem: %p\r\n", arg);
        free(arg);
    }

    printf("%llX free!\r\n", (unsigned long long)pthread_self());
}

void key_v_init(void)
{
    EXEC_RETURN_ZERO(0 == pthread_key_create(&key_v, ley_v_free));
    if (NULL == pthread_getspecific(key_v))
    {
        void *data = malloc(sizeof(4096));
        if (NULL != data)
        {
            printf("malloc: %p\r\n", data);
            EXEC_RETURN_ZERO(0 == pthread_setspecific(key_v, data));
        }
        else
        {
            printf("malloc mem err!\r\n");
        }
    }
    printf("%llX init!\r\n", (unsigned long long)pthread_self());
}

void *new_thread(void *arg)
{
    EXEC_RETURN_ZERO(0 == pthread_once(&once_v, key_v_init));

    printf("new thread start!\r\n");
    pthread_cleanup_push(show_clean_cb, "new thread clean 1"); //安装清理函数
    pthread_cleanup_push(show_clean_cb, "new thread clean 2");
    printf("new thread complete!\r\n");

    EXEC_RETURN_ZERO(0 == pthread_key_delete(key_v));

    if ((int long)arg)
    {
        pthread_exit((void *)1);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    pthread_exit((void *)199);
    // return (void *)199; //使用return 清理函数不会被调用
}

void *old_thread(void *arg)
{
    pthread_t *tid = (pthread_t *)arg;
    // pthread_once(&once_v, key_v_init);
    printf("old thread start! tid:%lX\r\n", *tid);
    pthread_cleanup_push(show_clean_cb, "old thread clean 1");
    pthread_cleanup_push(show_clean_cb, "old thread clean 2");
    printf("old thread complete!\r\n");

    if (pthread_equal(*tid, pthread_self()) != 0)
    {
        printf("this is thread self!\r\n");
    }
    else
    {
        printf("this diff thread is %lX\r\n", pthread_self());
    }

    while (1)
    {
        sleep(1);
        printf("wait sleep to cancel!\r\n");
    }

    pthread_cleanup_pop((int long)arg);
    pthread_cleanup_pop((int long)arg);

    printf("thread to cancel finish!\r\n");
    return (void *)455;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, new_thread, (void *)1))
    {
        perror("can't creat threda!");
        exit(0);
    }

    static pthread_t tid1;
    if (0 != pthread_create(&tid1, NULL, old_thread, (void *)&tid1))
    {
        perror("can't creat threda!");
        exit(0);
    }

    sleep(2);
    show_pthread_info(__FUNCTION__);
    // if (pthread_cancel(tid) != 0)
    // {
    //     perror("thread cancel!");
    // }

    void *ret;
    if (0 != pthread_detach(tid))
    {
        perror("pthread detach fail!");
        exit(-1);
    }

    sleep(2);

    int v = 0;
    if ((v = pthread_cancel(tid1)) != 0) //被控线程必须内部存在系统调用点
    {
        printf("thread tid to cancel fail! %s\r\n", strerror(v));
        exit(-1);
    }

    if (0 != pthread_join(tid1, &ret))
    {
        printf("thread join err!\r\n");
        exit(-1);
    }
    printf("ret value:%ld %ld\r\n", (long int)ret, (long int)PTHREAD_CANCELED);

    printf("Down!\r\n");
    pthread_exit(0);
}