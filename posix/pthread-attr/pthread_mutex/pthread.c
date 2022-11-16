/**
 * @file pthread_mutex.c
 * @author mingyang (mingyang.wang@tuya.com)
 * @brief  pthread_mutex互斥锁属性接口测试
 * @version 1.0.0
 * @date 2022-11-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define _GNU_SOURCE

/**
 * @brief 互斥锁相关接口
 * pthread_mutexattr_init: 互斥量属性初始化默认值
 * pthread_mutexattr_destroy: 互斥量属性销毁
 * pthread_mutexattr_getpshared: 互斥量属性获取共享状态
 * pthread_mutexattr_setpshared: 互斥量属性设置共享状态
 * pthread_mutexattr_gettype: 互斥量获取类型
 * pthread_mutexattr_settype: 互斥量设置类型
 * pthread_mutexattr_getrobust: 获取互斥量健壮状态
 * pthread_mutexattr_getrobust_np
 * pthread_mutexattr_setrobust : 设置互斥量健壮状态
 * pthread_mutexattr_setrobust_np:
 * pthread_mutex_consistent: 恢复互斥量状态
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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *mutex_wait_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        pthread_mutex_lock(&lock);
        printf("this is [%s] thread!！！！！\r\n", __FUNCTION__);
        sleep(2);
        pthread_mutex_unlock(&lock);
        usleep(1000);
    }
    pthread_exit((void *)199);
}

void show_mutex_attr_sharedStatus(pthread_mutexattr_t *initMutexAttr)
{
    int share;
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_getpshared(initMutexAttr, &share));
    switch (share)
    {
    case PTHREAD_PROCESS_PRIVATE:
        printf("get share PTHREAD_PROCESS_PRIVATE\r\n");
        break;
    case PTHREAD_PROCESS_SHARED:
        printf("get share PTHREAD_PROCESS_SHARED\r\n");
        break;
    }
}

void show_mutex_attr_getrobust(pthread_mutexattr_t *initMutexAttr)
{
    int robust;
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_getrobust(initMutexAttr, &robust));
    switch (robust)
    {
    case PTHREAD_MUTEX_STALLED:
        printf("show robust: PTHREAD_MUTEX_STALLED\r\n");
        break;
    case PTHREAD_MUTEX_ROBUST:
        printf("show robust: PTHREAD_MUTEX_ROBUST\r\n");
        break;
    }
}

void show_mutex_attr_type(pthread_mutexattr_t *initMutexAttr)
{
    int type;
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_gettype(initMutexAttr, &type));
    switch (type)
    {
    case PTHREAD_MUTEX_NORMAL:
        printf("show type PTHREAD_MUTEX_NORMAL\r\n");
        break;
    case PTHREAD_MUTEX_RECURSIVE:
        printf("show type PTHREAD_MUTEX_RECURSIVE\r\n");
        break;
    case PTHREAD_MUTEX_ERRORCHECK:
        printf("show type PTHREAD_MUTEX_ERRORCHECK\r\n");
        break;
    default:
        printf("show type PTHREAD_MUTEX_DEFAULT\r\n");
        break;
    }
}

/**
 * @brief 测试互斥属性类型NORMAL
 *
 * @param argc
 * @param argv
 * @return int
 */
#if 0
int main(int argc, char *argv[])
{
#define MAX_THREAD_NUM 5

    int ret;
    pthread_t tid;
    pthread_mutexattr_t mutex_attr;

    EXEC_RETURN_ZERO(0 == pthread_mutexattr_init(&mutex_attr));

    show_mutex_attr_type(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL));
    show_mutex_attr_type(&mutex_attr);

    show_mutex_attr_getrobust(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_STALLED));
    show_mutex_attr_getrobust(&mutex_attr);

    show_mutex_attr_sharedStatus(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_PRIVATE));
    show_mutex_attr_sharedStatus(&mutex_attr);

    ret = pthread_mutex_init(&lock, &mutex_attr);
    if (0 != ret)
    {
        pthread_mutexattr_destroy(&mutex_attr);
        perror("mutex init err!");
        exit(-1);
    }

    if (0 != pthread_create(&tid, NULL, mutex_wait_thread, (void *)1))
    {
        pthread_mutexattr_destroy(&mutex_attr);
        pthread_mutex_destroy(&lock);
        perror("can't creat thread!");
        exit(-1);
    }

    pthread_mutexattr_destroy(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_detach(tid));

    printf("Down!\r\n");
    pthread_exit((void *)0);
}
#endif

#if 0
/**
 * @brief 测试互斥锁错误检查类型
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
#define MAX_THREAD_NUM 5

    int ret;
    pthread_t tid;
    pthread_mutexattr_t mutex_attr;

    EXEC_RETURN_ZERO(0 == pthread_mutexattr_init(&mutex_attr));

    show_mutex_attr_type(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK));
    show_mutex_attr_type(&mutex_attr);

    show_mutex_attr_getrobust(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_STALLED));
    show_mutex_attr_getrobust(&mutex_attr);

    show_mutex_attr_sharedStatus(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_PRIVATE));
    show_mutex_attr_sharedStatus(&mutex_attr);

    ret = pthread_mutex_init(&lock, &mutex_attr);
    if (0 != ret)
    {
        pthread_mutexattr_destroy(&mutex_attr);
        perror("mutex init err!");
        exit(-1);
    }

    EXEC_RETURN_ZERO(0 == pthread_mutex_lock(&lock));
    EXEC_RETURN_ZERO(0 == pthread_mutex_lock(&lock)); //尝试重复加锁错误
    EXEC_RETURN_ZERO(0 == pthread_mutex_unlock(&lock));

    pthread_mutexattr_destroy(&mutex_attr);

    printf("Down!\r\n");
    pthread_exit((void *)0);
}
#endif

#if 0
/**
 * @brief 测试互斥锁递归锁
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
#define MAX_THREAD_NUM 5

    int ret;
    pthread_t tid;
    pthread_mutexattr_t mutex_attr;

    EXEC_RETURN_ZERO(0 == pthread_mutexattr_init(&mutex_attr));

    show_mutex_attr_type(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE));
    show_mutex_attr_type(&mutex_attr);

    show_mutex_attr_getrobust(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_STALLED));
    show_mutex_attr_getrobust(&mutex_attr);

    show_mutex_attr_sharedStatus(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_PRIVATE));
    show_mutex_attr_sharedStatus(&mutex_attr);

    ret = pthread_mutex_init(&lock, &mutex_attr);
    if (0 != ret)
    {
        pthread_mutexattr_destroy(&mutex_attr);
        perror("mutex init err!");
        exit(-1);
    }

    pthread_attr_t thread_attr;
    EXEC_RETURN_ZERO(0 == pthread_attr_init(&thread_attr));
    EXEC_RETURN_ZERO(0 == pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED));
    if (0 != pthread_create(&tid, &thread_attr, mutex_wait_thread, (void *)1))
    {
        pthread_mutexattr_destroy(&mutex_attr);
        pthread_mutex_destroy(&lock);
        perror("can't creat thread!");
        exit(-1);
    }
    EXEC_RETURN_ZERO(0 == pthread_attr_destroy(&thread_attr));

    EXEC_RETURN_ZERO(0 == pthread_mutex_lock(&lock));
    EXEC_RETURN_ZERO(0 == pthread_mutex_lock(&lock)); //递归锁反复加锁
    EXEC_RETURN_ZERO(0 == pthread_mutex_lock(&lock)); //递归锁反复加锁
    EXEC_RETURN_ZERO(0 == pthread_mutex_unlock(&lock));

    pthread_mutexattr_destroy(&mutex_attr);

    printf("Down!\r\n");
    pthread_exit((void *)0);
}
#endif

#if 1
#include <errno.h>
void *mutex_robust_thread(void *arg)
{
    printf("%s thread start!\r\n", __FUNCTION__);
    while (1)
    {
        int ret;
        ret = pthread_mutex_lock(&lock);
        if (0 != ret)
        {
            if (EOWNERDEAD == ret)
            {
                printf("pthread_mutex_consistent solve!\r\n");
                EXEC_RETURN_ZERO(pthread_mutex_consistent(&lock) == 0);
                EXEC_RETURN_ZERO(pthread_mutex_unlock(&lock) == 0);
                EXEC_RETURN_ZERO(pthread_mutex_lock(&lock) == 0);
            }
            else
            {
                printf("ower lock fail!\r\n");
                sleep(1);
                continue;
            }
        }

        printf("this is [%s] thread!！！！！\r\n", __FUNCTION__);
        sleep(2);
        EXEC_RETURN_ZERO(pthread_mutex_unlock(&lock));
        usleep(1000);
    }
    pthread_exit((void *)199);
}

/**
 * @brief 测试互斥锁健壮性
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
#define MAX_THREAD_NUM 5

    int ret;
    pthread_t tid;
    pthread_mutexattr_t mutex_attr;

    EXEC_RETURN_ZERO(0 == pthread_mutexattr_init(&mutex_attr));

    show_mutex_attr_type(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_DEFAULT));
    show_mutex_attr_type(&mutex_attr);

    show_mutex_attr_getrobust(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_ROBUST));
    show_mutex_attr_getrobust(&mutex_attr);

    show_mutex_attr_sharedStatus(&mutex_attr);
    EXEC_RETURN_ZERO(0 == pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_PRIVATE));
    show_mutex_attr_sharedStatus(&mutex_attr);

    ret = pthread_mutex_init(&lock, &mutex_attr);
    if (0 != ret)
    {
        pthread_mutexattr_destroy(&mutex_attr);
        perror("mutex init err!");
        exit(-1);
    }

    pthread_attr_t thread_attr;
    EXEC_RETURN_ZERO(0 == pthread_attr_init(&thread_attr));
    EXEC_RETURN_ZERO(0 == pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED));
    if (0 != pthread_create(&tid, &thread_attr, mutex_robust_thread, (void *)1))
    {
        pthread_mutexattr_destroy(&mutex_attr);
        pthread_mutex_destroy(&lock);
        perror("can't creat thread!");
        exit(-1);
    }
    EXEC_RETURN_ZERO(0 == pthread_attr_destroy(&thread_attr));

    EXEC_RETURN_ZERO(0 == pthread_mutex_lock(&lock));

    pthread_mutexattr_destroy(&mutex_attr);

    printf("Down!\r\n");
    pthread_exit((void *)0);
}
#endif