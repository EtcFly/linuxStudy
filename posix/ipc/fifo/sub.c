#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFIO_NAME "test-fifo"
int main(int argc, char *argv[])
{
    printf("this is a test sub\r\n");
    fflush(stdout);
    int fd = open(FIFIO_NAME, O_RDONLY);
    if (fd < 0) {
        perror("open fail");
        return -1;
    }
    printf("read open\r\n");
    char buf[512];
    int len = read(fd, buf, sizeof(buf));
    buf[len] = '\0';
    printf("read :%s len:%d\r\n", buf, len);
    fflush(stdout);
    return 0;
}