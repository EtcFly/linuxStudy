#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

void tcp_server(void)
{
#define MAX_SOCKET_LISTEN 5
	int ret;
	struct sockaddr_in server;

	printf("start tcp server ... \r\n");

	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket create fail");
		return ;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(9000); //server port
	server.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr *)&server, sizeof(server));
	if (ret == -1) {
		perror("bind fail");
		close(sockfd);
		return ;
	}

	ret = listen(sockfd, MAX_SOCKET_LISTEN);
	if (ret == -1) {
		perror("listen fail");
		close(sockfd);
		return ;
	}

	while (1) {
		struct sockaddr_in client;
		socklen_t client_len = sizeof(client);
		memset(&client, 0, sizeof(client));

		int client_sockfd = accept(sockfd, (struct sockaddr *)&client, &client_len);
		if (client_sockfd == -1) {
			perror("accept fail");
			sleep(2);
			continue;
		}

		printf("recv new connect, client %s port %d\r\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		char recv_buff[512];
		//recv client message and send it back
		while (1) {
			int len = recv(client_sockfd, recv_buff, sizeof(recv_buff), 0);
			if (len < 0) {
				perror("recv message err");
				break;
			} else if (len == 0) { //remote client disconnect
				printf("client %s port:%d disconnect\r\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
				break;
			}

			recv_buff[len] = '\0';
			printf("recv len: %d data: %s", len, recv_buff);
		}
		close(client_sockfd);
	}
	close(sockfd);
}

void tcp_client(void)
{
	int ret;
	struct sockaddr_in server;

	printf("start tcp client ... \r\n");

	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket create fail");
		return ;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(9000); //server port
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	//for Reliable connection
	int retryCnt = 0;
	while (retryCnt < 5) {
		ret = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
		if (ret == -1) {
			perror("connect dst server fail");
			sleep(retryCnt<<1);
			continue;
		} else {
			break;
		}
	}
	if (retryCnt >= 5) {
		printf("connect fail too many, client to close\r\n");
		close(sockfd);
		return;
	}

	char buff[512];
	int len, send_len;
	for (int i = 0; i < 100; i++) {
		len = snprintf(buff, sizeof(buff), "this is a test, and client send message to server, index:%d\r\n", i);
		buff[len] = '\0';
		send_len = 0;

		while (send_len < len) {
			int tmp = send(sockfd, &buff[send_len], len-send_len, 0);
			if (tmp < 0) {
				perror("send message fail");
				sleep(2);
				continue;
			}

			send_len += tmp;
		}
	}

	close(sockfd);
}

void udp_server(void)
{
	int ret;
	struct sockaddr_in server;

	printf("start udp server ... \r\n");

	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0) {
		perror("socket create fail");
		return ;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(9000); //server port
	server.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr *)&server, sizeof(server));
	if (ret == -1) {
		perror("bind fail");
		close(sockfd);
		return;
	}

	char recv_buff[512];
	int recv_len = 0;

	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	memset(&client, 0, sizeof(client));

	while (1) {
		recv_len = recvfrom(sockfd, recv_buff, sizeof(recv_buff), 0, (struct sockaddr *)&client, &client_len);
		if (recv_len < 0) {
			perror("recv data fail");
			sleep(2);
			continue;
		}

		recv_buff[recv_len] = '\0';
		printf("recv %s port:%d len: %d data: %s", inet_ntoa(client.sin_addr), ntohs(client.sin_port), recv_len, recv_buff);
	}

	close(sockfd);
}

void udp_client(void)
{
	int ret;
	struct sockaddr_in server;

	printf("start udp client ... \r\n");

	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0) {
		perror("socket create fail");
		return ;
	}

	char send_buff[512];
	int len, send_len;

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);

	for (int i = 0; i < 100; i++) {
		len = snprintf(send_buff, sizeof(send_buff), "this is a udp test, index: %d\r\n", i);
		send_buff[len] = '\0';
		send_len = 0;

		while (send_len < len) {
			int tmp = sendto(sockfd, &send_buff[send_len], len-send_len, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
			if (tmp < 0) {
				perror("send message fail");
				sleep(2);
				continue;
			}

			send_len += tmp;
		}
	}
	close(sockfd);
}

struct tcp_serv_manage {
	int servfd;
	int clientfd[20]; // server support max 20 client
	int client_cnt;
};

void init_tcp_client(struct tcp_serv_manage *ma) {
	ma->client_cnt = 0;
	ma->servfd = -1;
	for (int i = 0; i < sizeof(ma->clientfd)/sizeof(ma->clientfd[0]); i++) {
		ma->clientfd[i] = -1;
	}
}

void add_tcp_client(struct tcp_serv_manage *ma, int clientsockfd)
{
	for (int i = 0; i < sizeof(ma->clientfd)/sizeof(ma->clientfd[0]); i++) {
		if (ma->clientfd[i] == -1) {
			ma->clientfd[i] = clientsockfd;
			ma->client_cnt += 1;
			break;
		}
	}
}

void del_tcp_client(struct tcp_serv_manage *ma, int clientsockfd)
{
	for (int i = 0; i < ma->client_cnt; i++) {
		if (ma->clientfd[i] == clientsockfd) {
			ma->clientfd[i] = -1;
			ma->client_cnt -= 1;
			break;
		}
	}
}

void tcp_server_select_muli(void)
{
#define MAX_SOCKET_LISTEN 5
	int ret;
	int flag;
	int maxsockfd = 0;
	struct sockaddr_in server;
	struct tcp_serv_manage serv_ma;

	printf("start tcp select server ... \r\n");

	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket create fail");
		return ;
	}

	flag =1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)); //set sockfd option

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(9000); //server port
	server.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr *)&server, sizeof(server));
	if (ret == -1) {
		perror("bind fail");
		close(sockfd);
		return ;
	}

	ret = listen(sockfd, MAX_SOCKET_LISTEN);
	if (ret == -1) {
		perror("listen fail");
		close(sockfd);
		return ;
	}

	init_tcp_client(&serv_ma);
	serv_ma.servfd = sockfd;
	while (1) {
		fd_set rfds, efds;

		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		maxsockfd = 0;

		for (int i = 0; i < serv_ma.client_cnt; i++) {
			FD_SET(serv_ma.clientfd[i], &rfds);
			FD_SET(serv_ma.clientfd[i], &efds);
			maxsockfd = maxsockfd < serv_ma.clientfd[i] ? serv_ma.clientfd[i] : maxsockfd;
		}
		FD_SET(serv_ma.servfd, &rfds);
		FD_SET(serv_ma.servfd, &efds);
		maxsockfd = maxsockfd < serv_ma.servfd ? serv_ma.servfd : maxsockfd;

		struct timeval timeout = {
			.tv_sec = 5,
			.tv_usec = 0,
		};
		ret = select(maxsockfd+1, &rfds, NULL, &efds, &timeout);
		if (ret < 0) {
			perror("select fail");
			sleep(2);
			continue;
		} else if (ret == 0) {
			continue;
		}

		for (int i = 0; i < serv_ma.client_cnt; i++) {
			if (FD_ISSET(serv_ma.clientfd[i], &efds)) {
				perror("client fd fail");
				del_tcp_client(&serv_ma, serv_ma.clientfd[i]);
				sleep(2);
				continue;
			}
		}

		if (FD_ISSET(serv_ma.servfd, &efds)) {
			perror("serv fd fail");
			sleep(2);
			continue;
		}

		if (FD_ISSET(serv_ma.servfd, &rfds)) {
			struct sockaddr_in clientAddr;
			socklen_t len = sizeof(clientAddr);
			memset(&clientAddr, 0, sizeof(clientAddr));
			int newclient = accept(serv_ma.servfd, (struct sockaddr *)&clientAddr, &len);
			if (newclient == -1) {
				perror("accept client fail");
				sleep(2);
				break;
			}
			add_tcp_client(&serv_ma, newclient);
			printf("new client connect %s:%d\r\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		}

		for (int i = 0; i < serv_ma.client_cnt; i++) {
			if (FD_ISSET(serv_ma.clientfd[i], &rfds)) {
				char buff[512];
				int len = recv(serv_ma.clientfd[i], buff, sizeof(buff), 0);
				if (len < 0) {
					perror("recv sockfd fail");
					continue;
				} else if (len == 0) {
					printf("diconnect client\r\n");
					close(serv_ma.clientfd[i]);
					del_tcp_client(&serv_ma, serv_ma.clientfd[i]);
					continue;
				}
				buff[len] = '\0';
				send(serv_ma.clientfd[i], buff, len, 0);
			}
		}
	}
	close(serv_ma.servfd);
}

int main(int argc, char *argv[])
{
	// tcp_server();
	// tcp_client();
	// udp_server();
	// udp_client();
	tcp_server_select_muli();
	//SO_REUSEADDR
	//SO_KEEPALIVE
//IPPROTO_TCP
	//SOL_IP
    return 0;
}
