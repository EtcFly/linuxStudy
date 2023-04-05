/**
 * @file pthread.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread线程属性接口测试
 * @version 1.0.0
 * @date 2022-11-15
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
 * @brief 线程属性相关设置接口
 * pthread_attr_init: 线程属性设为默认值
 * pthread_attr_destroy： 线程属性销毁
 * pthread_attr_getdetachstate： 获取线程分离状态
 * pthread_attr_setdetachstate: 设置线程分离状态
 * pthread_attr_getstack： 获取线程堆栈(pthread_attr_getstackaddr已经废弃)
 * pthread_attr_setstack：设置线程堆栈(pthread_attr_setstackaddr已经废弃)
 * pthread_attr_getstacksize: 获取线程堆栈大小
 * pthread_attr_setstacksize: 设置线程堆栈大小
 * pthread_attr_getguardsize: 获取警戒位置大小
 * pthread_attr_setguardsize: 设置警戒位置大小(堆栈溢出警戒)
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
void *new_thread(void *arg)
{
    printf("new thread start!\r\n");
    pthread_cleanup_push(show_clean_cb, "new thread clean 1"); //安装清理函数
    pthread_cleanup_push(show_clean_cb, "new thread clean 2");
    printf("new thread complete!\r\n");

    if ((int long)arg)
    {
        pthread_exit((void *)1);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    pthread_exit((void *)199);
    // return (void *)199; //使用return 清理函数不会被调用
}

void show_detach_state(const pthread_attr_t *stat)
{
    int detachState;
    EXEC_RETURN_ZERO(0 == pthread_attr_getdetachstate(stat, &detachState)); //获取默认状态
    if (detachState == PTHREAD_CREATE_DETACHED)
    {
        printf("get detach state!\r\n");
    }
    else
    {
        printf("get joinable state!\r\n");
    }
}

void show_stack_size(pthread_attr_t *attr)
{
    size_t stacksize;
    EXEC_RETURN_ZERO(0 == pthread_attr_getstacksize(attr, &stacksize));
    printf("get stack size:%ld\r\n", stacksize);
}

void show_stack_addr(const pthread_attr_t *attr)
{
    void *addr;
    size_t size;
    EXEC_RETURN_ZERO(0 == pthread_attr_getstack(attr, &addr, &size));
    printf("get stack address:%p size:%ld\r\n", addr, size);
}

void show_guard_size(const pthread_attr_t *attr)
{
    size_t size;
    EXEC_RETURN_ZERO(0 == pthread_attr_getguardsize(attr, &size));
    printf("get guard size:%ld\r\n", size);
}

static char buffer[800 * 1024] __attribute__((aligned(32)));
int main(int argc, char *argv[])
{

    pthread_t tid;
    pthread_attr_t init_attr;

    EXEC_RETURN_ZERO(0 == pthread_attr_init(&init_attr));

    show_detach_state(&init_attr);                                    //显示默认值
    pthread_attr_setdetachstate(&init_attr, PTHREAD_CREATE_DETACHED); //如果系统不设置detach, 然后有没有join等待, 线程会存在栈泄漏的风险
    show_detach_state(&init_attr);                                    //显示设置值

    show_stack_size(&init_attr); //显示默认堆栈大小
    pthread_attr_setstacksize(&init_attr, 8000 * 1024ul);
    show_stack_size(&init_attr);

    show_guard_size(&init_attr);
    pthread_attr_setguardsize(&init_attr, 1 * 1024); //默认会页对齐
    show_guard_size(&init_attr);

    show_stack_addr(&init_attr); //接口被弃用
    pthread_attr_setstack(&init_attr, &buffer[0], sizeof(buffer));
    printf("stack buffer address:%p\r\n", buffer);
    show_stack_addr(&init_attr);

    if (0 != pthread_create(&tid, &init_attr, new_thread, (void *)1))
    {
        pthread_attr_destroy(&init_attr);
        perror("can't creat thread!");
        exit(0);
    }

    pthread_attr_destroy(&init_attr); // init attr是按值传递, 这里可以销毁

    sleep(2);
    show_pthread_info(__FUNCTION__);
    .printf("Down!\r\n");
    pthread_exit(0);
    printf("test run here!\r\n");
    // return 0;
}
