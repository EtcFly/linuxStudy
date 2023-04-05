
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <error.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // struct hostent *host;
    // host = gethostbyname("www.7k7k.com");

    // printf("name:%s\r\n", host->h_name); //输出规范名

    // for (int i = 0; host->h_aliases[i] != NULL; i++)
    // {
    //     printf("addr:%s\r\n", host->h_aliases[i]);
    // }

    // char str[INET_ADDRSTRLEN];
    // for (int i = 0; host->h_addr_list[i] != NULL; i++)
    // {
    //     printf("addr:%s\r\n", inet_ntop(host->h_addrtype, host->h_addr_list[i], str, sizeof(str)));
    // }
    printf("%s\r\n", strerror(115));
    return 0;
}
