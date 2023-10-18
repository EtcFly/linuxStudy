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
#include <asm-generic/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <aio.h>
#include <stdint.h>

#define MAX_AIO_SIZE 10
#define AIO_OP_NOUSED 0
#define AIO_OP_USED   1

struct aio_buff {
    uint8_t buf[512];
    int used;
};

#define AIO_OP_WRITE 0
#define AIO_OP_CHECK 1

struct aio_op {
    struct aiocb    aio_cb[MAX_AIO_SIZE];
    struct aio_buff buff[MAX_AIO_SIZE];
};

#define MAX_SIZE_FILE (512*10000)
int main(int argc, char *argv[])
{
    struct aiocb *cb[MAX_AIO_SIZE];
    struct aio_op op;
    int fd = open("./aio.test", O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0744);
    if (fd < 0) {
        perror("fd open fail\n");
        return -1;
    }

    int off = MAX_SIZE_FILE;
    memset(&op, 0, sizeof(op));
    while (1) {
        for (int i = 0; i < MAX_AIO_SIZE; i++) {
            if (op.buff[i].used == AIO_OP_NOUSED) {
                if (off < 0) {
                    continue;
                }
                printf("set used i:%d off:%d\r\n", i, off);
                op.buff[i].used = AIO_OP_USED;
                op.aio_cb[i].aio_buf =  op.buff[i].buf;
                op.aio_cb[i].aio_offset = off;
                op.aio_cb[i].aio_nbytes = sizeof(op.buff[i].buf);
                op.aio_cb[i].aio_fildes = fd;
                op.aio_cb[i].aio_sigevent.sigev_notify = SIGEV_NONE;
                cb[i] = &op.aio_cb[i];

                int ret = snprintf((void*)op.buff[i].buf, sizeof(op.buff[i].buf), "write %d off:%d ", i, (int)op.aio_cb[i].aio_offset);
                memset((void*)&op.buff[i].buf[ret], 'A', sizeof(op.buff[i].buf)-ret);
                op.buff[i].buf[sizeof(op.buff[i].buf)-2] = '\r';
                op.buff[i].buf[sizeof(op.buff[i].buf)-1] = '\n';
                // if (aio_write(&op.aio_cb[i]) < 0) {
                //     printf("aio %d write err %s(%d)\n", off, strerror(errno), errno);
                //     return -1;
                // }
                cb[i]->aio_lio_opcode = LIO_WRITE;
                if (lio_listio(LIO_NOWAIT, (void*)&cb[i], 1, NULL) < 0) {
                    printf("aio %d write err %s(%d)\n", off, strerror(errno), errno);
                    return -1;
                }

                off -= sizeof(op.buff[i].buf);
            } else {
                int err;
                if (cb[i] != NULL) {
                    printf("cb[%d]: off:%ld\r\n", i, cb[i]->aio_offset);
                    if ((err=aio_error(&op.aio_cb[i])) == EINPROGRESS) {
                        continue;
                    }

                    if (err == -1) {
                        perror("err -1\n");
                        return -1;
                    }

                    err = aio_return(&op.aio_cb[i]);
                    if (err < 0) {
                        printf("write err %d\n", err);
                        return -1;
                    }

                    printf("write ok %d off:%ld\n", err, op.aio_cb[i].aio_offset);
                    op.buff[i].used = AIO_OP_NOUSED;
                    cb[i] = NULL;
                }
            }
        }

        if (off < 0) {
            int count =  0;
            for (int i = 0; i < MAX_AIO_SIZE; i++) {
                if (cb[i] == NULL) {
                    count++;
                }
            }
            if (count == MAX_AIO_SIZE) {
                break;
            }
        }
    }

    int ret;
    if ((ret=aio_suspend((void*)cb, MAX_AIO_SIZE, NULL)) < 0) {
        printf("suspend fail ret:%d\n", ret);
        return -1;
    }

    if (aio_fsync(O_SYNC, &op.aio_cb[0]) < 0) {
        printf("fsync fail\n");
        return -1;
    }
    close(fd);
    printf("open ok %d\n", ret);
    
    return 0;
}
