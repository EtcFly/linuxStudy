/**
 * @file mmap.c
 * @author your name (you@domain.com)
 * @brief mmap文件映射
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>


#define MAX_IN_FILE_SIZE (1024*1024*1U)
const char tab[] = "abcdefghijklmnopqrstuvwsyz!@#$/^&*()_+1234567890-=[],./";
int main(int argc, char *argv[])
{
    int in_fd = open("in.file", O_CREAT | O_TRUNC | O_RDWR, 0744);
    if (in_fd < 0) {
        return -1;
    }

    int out_fd = open("out.file", O_CREAT | O_TRUNC | O_RDWR, 0744);
    if (out_fd < 0) {
        close(in_fd);
        return -1;
    }

    char ch;
    for (int i = 0; i < MAX_IN_FILE_SIZE; i++) {
        ch = tab[i%sizeof(tab)];
        int ret = write(in_fd, &ch, 1);
        if (ret <= 0) {
            printf("write %d %c\r\n", i, ch);
            break;
        }
    }
    fsync(in_fd);
    lseek(in_fd, 0, SEEK_SET);

    struct stat f_stat;
    fstat(in_fd, &f_stat);
    ftruncate(out_fd, f_stat.st_size);
    printf("file size:%ld\r\n", f_stat.st_size);

    int off = 0;
    #define COPY_SIZE (1024*512U)
    while (off < MAX_IN_FILE_SIZE) {
        int size = (MAX_IN_FILE_SIZE-off) >= COPY_SIZE ? COPY_SIZE : (MAX_IN_FILE_SIZE-off);
        uint8_t *psrc = mmap(0, size, PROT_READ, MAP_SHARED, in_fd, off);
        if (psrc == MAP_FAILED) {
            perror("src map err\n");
            break;
        }

        uint8_t *pdst = mmap(0, size, PROT_WRITE, MAP_PRIVATE, out_fd, off);
        if (pdst == MAP_FAILED) {
            perror("pdst map err\n");
            break;
        }
        printf("map copy ok\n");
        memcpy(pdst, psrc, size);
        msync(pdst, size, MS_SYNC);
        mprotect(pdst, size, PROT_EXEC);
        munmap(pdst, size);
        munmap(psrc, size);
        off += size;
    }
    
    close(in_fd);
    close(out_fd);
    return 0;
}
