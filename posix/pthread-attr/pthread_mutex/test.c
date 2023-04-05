/**
 * @file test.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-11-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define EXEC_RETURN_ZERO(x)              \
    do                                   \
    {                                    \
        if (!(x))                        \
        {                                \
            printf("%s [FAIL]\r\n", #x); \
        }                                \
    } while (0)

int main(int argc, char *argv[])
{
    printf("argc:%d\r\n", argc);
    pthread_mutex_t *plock;
    while (true)
    {
        EXEC_RETURN_ZERO(pthread_mutex_lock(plock));

        printf("get lock !\r\n");
        sleep(1);

        EXEC_RETURN_ZERO(pthread_mutex_unlock(plock));
    }
    return 0;
}
