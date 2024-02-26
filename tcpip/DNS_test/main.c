
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        perror("sock fail");
        return 0;
    }

    int flag = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(inet_network("192.168.125.24"));
    server_addr.sin_port = htons(10234);
    server_addr.sin_family = AF_INET;
    int ret = bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("bind fail");
        return -1;
    }

    ret = listen(fd, 5);
    if (ret == -1) {
        perror("listen fail");
        return -1;
    }

    int rfd = accept(fd, NULL, NULL);
    if (rfd == -1) {
        perror("accept fail");
        return -1;
    }
    char recvbuf[100];
    ret = recv(rfd, recvbuf, sizeof(recvbuf), 0);

    if (ret < 0) {
        perror("recv fail");
    } else {
        printf("recvbuf:%s ret:%d\r\n", recvbuf, ret);
    }
    close(fd);
    return 0;
}
