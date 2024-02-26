#define __GLIBC_USE
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

void print_hostent(struct hostent *ent) 
{
	int i = 0;
	printf("name:%s len:%d\n", ent->h_name, ent->h_length);
	for (i = 0; ent->h_addr_list[i] != NULL; i++) {
		char buf[INET_ADDRSTRLEN];
		printf("addr:%s\n", inet_ntop(ent->h_addrtype, ent->h_addr_list[i], buf, sizeof(buf)));
	}
	for (i = 0; ent->h_aliases[i] != NULL; i++) {
		printf("alias: %s\n", ent->h_aliases[0]);
	}
}

void print_netent(struct netent *ent)
{
	printf("name:%s addr_type:%x\r\n", ent->n_name, ent->n_addrtype);
	printf("netNumber:%X\r\n", ent->n_net);
	for (int i = 0; ent->n_aliases[i] != NULL; i++) {
		printf("addr:%s\r\n", ent->n_aliases[i]);
	}
}

void print_servent(struct servent *ent)
{
	printf("name: %s \r\n", ent->s_name);
	printf("port:%d protol:%s\r\n", ntohs(ent->s_port), ent->s_proto);
	int i;
	for (i = 0; ent->s_aliases[i] != NULL; i++) {
		printf("alias:%s\r\n", ent->s_aliases[i]);
	}
}

void print_protoent(struct protoent *ent)
{
	printf("name:%s protol:%d\r\n", ent->p_name, ent->p_proto);
	int i;
	for (i = 0; ent->p_aliases[i] != NULL; i++) {
		printf("alias:%s\r\n", ent->p_aliases[i]);
	}
}

static void test_addr_convert(void)
{
	struct hostent *ent;
	sethostent(0);
	while ((ent=gethostent()) != NULL) {
		print_hostent(ent);
	}
	endhostent();

	struct hostent *h = gethostbyname("etcfly.top");
	if (h != NULL) {
		print_hostent(h);
	}

	in_addr_t addr = inet_network("47.174.205.116");
	h = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	if (h != NULL) {
		print_hostent(h);
	} else {
		herror("by addr");
	}

    sethostent(1);
    struct hostent res;
    char buf[1024];
    int errcode = 0;
    int ret = 0;
	ent = NULL;
    while ((ret=gethostent_r(&res, buf, sizeof(buf), &ent, &errcode)) == 0) {
        if (ent != NULL) {
			print_hostent(ent);
		}
    }
    endhostent();
	struct in_addr inaddr;
	inaddr.s_addr = inet_addr("116.205.174.47");
	ent = NULL;
	ret = gethostbyaddr_r(&inaddr, sizeof(inaddr), AF_INET, &res, buf, sizeof(buf), &ent, &errcode);
	if (ret == 0 && ent != NULL) {
		print_hostent(ent);
	} else {
		herror("error:");
	}

	ret = gethostbyname_r("etcfly.top", &res, buf, sizeof(buf), &ent, &errcode);
	if (ret == 0 && ent != NULL) {
		print_hostent(ent);
	} else {
		herror("error:");
	}

	//! find database in /etc/networks
	setnetent(0);
	struct netent *netent;
	while ((netent=getnetent()) != NULL) {
		print_netent(netent);
	}
	endnetent();

	netent = getnetbyname("link-local");
	if (NULL != netent) {
		print_netent(netent);
	} else {
		herror("netname fail");
	}
}

void test_server(void)
{
	//! find database in /etc/services
	setservent(0);
	struct servent *servent;
	while ((servent=getservent()) != NULL) {
		print_servent(servent);
	}
	endservent();

	servent = getservbyname("ssh", "tcp");
	if (servent != NULL) {
		print_servent(servent);
	} else {
		herror("get servernt fail");
	}
	servent = getservbyport(htons(22), "tcp");
	if (servent != NULL) {
		print_servent(servent);
	} else {
		herror("get servernt fail");
	}
}

void test_protocol(void)
{
	//! find database in /etc/protocols
	setprotoent(0);
	struct protoent *protoent;
	while ((protoent=getprotoent()) != NULL) {
		print_protoent(protoent);
	}
	endprotoent();

	protoent = getprotobyname("icmp");
	if (NULL != protoent) {
		print_protoent(protoent);
	}
	protoent = getprotobynumber(6);
	if (NULL != protoent) {
		print_protoent(protoent);
	}
}

static char *show_proto_family(int family)
{
	switch (family) {
		case AF_INET:  return "AF_INET";
		case AF_INET6: return "AF_INET6";
	}
	return "Unknow";
}

static char * show_socktype(int socktype)
{
	switch (socktype) {
		case SOCK_STREAM: return "SOCK_STREAM";
		case SOCK_DGRAM:  return "SOCK_DGRAM";
		case SOCK_RAW:    return "SOCK_RAM";
	}
	char buff[100];
	snprintf(buff, sizeof(buff), "SOCK_Unknow[%d]", socktype);
	return strdup(buff);
}

static char *show_proto(int proto)
{
	switch (proto) {
		case IPPROTO_TCP: return "tcp";
		case IPPROTO_ICMP: return "icmp";
		case IPPROTO_IP:  return "ip";
		case IPPROTO_UDP: return "udp";
	}
	char buff[100];
	snprintf(buff, sizeof(buff), "PROTO_Unknow[%d]", proto);
	return strdup(buff);
}

void test_getaddinfo(void)
{
	#include <errno.h>
	struct addrinfo *result = NULL;
	struct addrinfo hint;
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags |= AI_PASSIVE | AI_NUMERICSERV | AI_CANONNAME ;
	//hint.ai_family = AF_INET;
	hint.ai_protocol = IPPROTO_TCP;
	//"47.174.205.116"
	int ret = getaddrinfo("etcfly.top", NULL, &hint, &result);
	if (ret != 0) {
		printf("error code:%s", gai_strerror(ret));
		return ;
	}

	if (NULL == result) {
		return ;
	}

	// SOCK_STREAM
	// IPPROTO_ICMP
	while (result != NULL) {
		struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
		printf("aiflag:%d family:%s\r\n", result->ai_flags, show_proto_family(result->ai_family));
		printf("socktype:%s aiprotocol:%s\r\n", show_socktype(result->ai_socktype), \
														show_proto(result->ai_protocol));
		printf("ai_canonname:%s\r\n", result->ai_canonname);

		char buff[sizeof "255.255.255.255."];
		printf("addr:%s len:%d port:%d\r\n", inet_ntop(addr->sin_family, &addr->sin_addr, buff, sizeof(buff)), result->ai_addrlen, ntohs(addr->sin_port));
		result = result->ai_next;
	}
	freeaddrinfo(result);
}

void test_getnameinfo(void)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	inet_pton(AF_INET, "116.205.174.47", &addr.sin_addr.s_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(23);

	char hostname[100];
	char servname[100];
	int flags = 0;

	flags |= NI_DGRAM;
	int ret = getnameinfo((struct sockaddr *)&addr, sizeof(addr), \
	hostname, sizeof(hostname), servname, sizeof(servname), flags);
	if (ret != 0) {
		printf("errcode:%s\r\n", gai_strerror(ret));
		return ;
	}

	printf("hostname:%s\r\n", hostname);
	printf("servname:%s\r\n", servname);
}

void test_select(void)
{
	int sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockid < 0) {
		perror("socket fail");
		return ;
	}
	
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(9008);
	inet_aton("116.205.174.47", &server.sin_addr);
	// printf("s:addr:%d\r\n", server.sin_addr.s_addr);
	// inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // 使用 inet_pton 函数转换 IP 地址

	int flags = fcntl(sockid, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(sockid, F_SETFL, flags);

	int ret = connect(sockid, (struct sockaddr *)&server, sizeof(server));
	if (ret < 0 && errno != EINPROGRESS) {
		perror("connect fail");
		close(sockid);
		return ;
	}
	printf("ret:%d\r\n", ret);
	fd_set rset, wset, eset;
	FD_ZERO(&wset);
	FD_ZERO(&eset);
	FD_ZERO(&rset);
	FD_SET(sockid, &rset);
	FD_SET(sockid, &wset);
	FD_SET(sockid, &eset);
	ret = select(sockid+1, NULL, &wset, &eset, NULL);
	if (ret < 0) {
		close(sockid);
		return ;
	}

	if (ret == 0) {
		printf("connect timeout \r\n");
		close(sockid);
		return ;
	}

	if (FD_ISSET(sockid, &wset) != 0) {
		printf("wset set\r\n");
	}
	if (FD_ISSET(sockid, &rset) != 0) {
		printf("rset set\r\n");
	}

	if (FD_ISSET(sockid, &eset) != 0) {
		printf("eset set\r\n");
	}
	printf("select ret: %d\r\n", ret);

	char buf[100];
	while (1) {
		FD_ZERO(&wset);
		FD_ZERO(&eset);
		FD_ZERO(&rset);

		FD_SET(sockid, &rset);
		FD_SET(sockid, &wset);
		FD_SET(sockid, &eset);
		memset(buf, 0, sizeof(buf));

		struct timeval timeout = {
			.tv_sec = 5,
			.tv_usec = 0,
		};
		ret = select(sockid+1, &rset, NULL, &eset, &timeout);
		if (ret < 0) {
			perror("select fail\r\n");
			break;
		}

		if (ret == 0) {
			printf("timeout date:%ld\r\n", time(NULL));
			continue;
		}

		printf("timeout date:%ld ret:%d\r\n", time(NULL), ret);

		if (FD_ISSET(sockid, &eset) != 0) {
			printf("err set\r\n");
			int optval = 0;
			socklen_t optlen = sizeof(optval);
			int ret = getsockopt(sockid, SOL_SOCKET, SO_ERROR, &optval, &optlen);
			if (ret == -1) {
				perror("getsock fail");
			} else {
				if (optval != 0) {
					printf("err: %s\r\n", strerror(optval));
				} else {
					printf("no sock err\r\n");
				}
			}
			break;
		}

		if (FD_ISSET(sockid, &rset) != 0) {
			printf("sockid rset\r\n");
			if ((ret=recv(sockid, buf, sizeof(buf), 0)) < 0) {
				printf("ret:%d %s\r\n", ret, strerror(errno));
				break;
			} else if (ret == 0) {
				printf("socket disconnect\r\n");
				break;
			}
			printf("recv ret: %d %s", ret, buf);
		}

		if (FD_ISSET(sockid, &wset) != 0) {
			FD_CLR(sockid, &wset);
			printf("wset sockid\r\n");
		}
	}
}

void test_poll(void)
{
	int sockid;
	sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockid < 0) {
		perror("sockid fail");
		return ;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9008);
	inet_pton(AF_INET, "116.205.174.47", &server_addr.sin_addr);

	int ret = connect(sockid, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0) {
		perror("connect fail");
		close(sockid);
		return ;
	}

	while (1) {
		struct pollfd fds;
		fds.fd = sockid;
		fds.events = POLLIN | POLLPRI | POLLERR;
		fds.revents = 0;
		ret = poll(&fds, 1, 5000);
		if (ret < 0) {
			perror("poll fail");
			break;
		}

		if (ret == 0) {
			printf("timeout connect:%ld\r\n", time(NULL));
			continue;
		}

		if (fds.revents & POLLIN) {
			char buf[1000];
			memset(buf, 0, sizeof(buf));
			int len = recv(sockid, buf, sizeof(buf), 0);
			if (len < 0) {
				perror("recv fail\r\n");
				break;
			}

			if (len == 0) {
				printf("link disconnect\r\n");
				break;
			}
			printf("data recv len:%d %s", len, buf);
		}

		if (fds.revents & POLLERR) {
			printf("poll err\r\n");
		}
		printf("poll:%d\r\n", fds.revents);
	}
	close(sockid);
}

void test_select_accept(void)
{
	int sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockid < 0) {
		perror("sock fail");
		return ;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9008);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	int ret = bind(sockid, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret == -1) {
		perror("bind fail");
		close(sockid);
		return ;
	}

	ret = listen(sockid, 5);
	if (ret == -1) {
		perror("listen fail");
		close(sockid);
		return ;
	}

	fd_set rfds, efds;
again:
	FD_ZERO(&rfds);
	FD_ZERO(&efds);

	FD_SET(sockid, &rfds);
	FD_SET(sockid, &efds);

	struct timeval timeout = {
		.tv_sec = 5,
		.tv_usec = 0,
	};

	ret = select(sockid+1, &rfds, NULL, &efds, &timeout);
	if (ret < 0) {
		perror("select fail");
		close(sockid);
		return ;
	}

	if (ret == 0) {
		perror("select timeout");
		goto again;
	}

	if (FD_ISSET(sockid, &efds) != 0) {
		printf("efds set\r\n");
	}

	if (FD_ISSET(sockid, &rfds) != 0) {
		ret = accept(sockid, NULL, NULL);
		if (ret < 0) {
			perror("accept fail");
		} else {
			const char *psend = "this is a test";
			send(ret, psend, strlen(psend), 0);
			close(ret);
		}
	}
	close(sockid);
	return ;
}

void test_udp_seg(void)
{
	int sockid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockid < 0) {
		perror("sock fail");
		return ;
	}
	struct sockaddr_in server_addr;
	socklen_t len = sizeof(server_addr);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9008);
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	// int ret = connect(sockid, (const struct sockaddr *)&server_addr, len);
	// if (ret < 0) {
	// 	perror("connect fail");
	// }
	// printf("connect ret:%d\r\n", ret);
	int flag = 1;
	setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
	int ret = bind(sockid, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0) {
		perror("connect fail");
		close(sockid);
		return ;
	}

	fd_set rfds, efds;
	while (1) {
		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		FD_SET(sockid, &rfds);
		FD_SET(sockid, &efds);

		struct timeval timeout = {
			.tv_sec = 5,
			.tv_usec = 0,
		};
		int ret = select(sockid+1, &rfds, NULL, &efds, &timeout);
		if (ret < 0) {
			perror("select fail");
			break;
		}

		if (ret == 0) {
			printf("select timeout\r\n");
			sleep(1);
			continue;
		}

		if (FD_ISSET(sockid, &efds) != 0) {
			printf("efds fail\r\n");
			sleep(1);
			continue ;
		}

		memset(&server_addr, 0, sizeof(server_addr));
		if (FD_ISSET(sockid, &rfds)) {
			char buf[4];
			memset(buf, 0, sizeof(buf));
			int ret = recvfrom(sockid, buf, sizeof(buf), 0, (struct sockaddr *)&server_addr, &len);
			if (ret < 0) {
				perror("recv fail\r\n");
				break;
			}

			char net_buf[] = "255.255.255.255";
			memset(net_buf, 0, sizeof(net_buf));
			inet_ntop(AF_INET, &server_addr.sin_addr, net_buf, sizeof(net_buf));
			printf("recv ip:%s port:%d\r\n", net_buf, ntohs(server_addr.sin_port));
			printf("recv len:%d %s\r\n", ret, buf);
		}
	}   
}

void test_byteorder(void)
{
	struct in_addr inaddr;
	inaddr.s_addr = inet_addr("127.1.2.3");
	in_addr_t lin_addr = inet_lnaof(inaddr);
	printf("%s\r\n", inet_ntoa(*(struct in_addr *)(&lin_addr)));

	inaddr.s_addr = inet_addr("127.1.2.3");
	lin_addr = inet_netof(inaddr);
	printf("%s\r\n", inet_ntoa(*(struct in_addr *)(&lin_addr)));
}

static void test_socket_api(void)
{
    int ret, flag;

    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd < 0) {
        perror("sock creat fail\r\n");
        return ;
    }
    printf("sock open ok!\r\n");

    flag = 1;
    ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if (ret == -1) {
        perror("set reuseaddr fail\r\n");
    }
    struct sockaddr_in server;
    struct sockaddr_in6 server6;
    struct sockaddr server_unnone;
    printf("ipv4:%ld - ipv6:%ld - unnone:%ld\r\n", sizeof(server), sizeof(server6), sizeof(server_unnone));

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY;
    server.sin_port = htons(11000);
    server.sin_family = AF_INET;

    socklen_t len = sizeof(server);
    ret = bind(sock_fd, (struct sockaddr *)&server, len);
    if (ret == -1) {
        perror("bind fail\r\n");
        close(sock_fd);
        return ;
    }

    printf("bind server ok\r\n");

    ret = listen(sock_fd, 5);
    if (ret == -1) {
        perror("listen fail \r\n");
        close(sock_fd);
    }
    printf("listen sock %d ok\r\n", sock_fd);

    while (1) {
        int client_sock = accept(sock_fd, NULL, NULL);
        if (client_sock < 0) {
            perror("client connect fail");
            continue;
        }

        ret = setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
        if (ret == -1) {
            perror("set nagle disable fail\r\n");
        }

        printf("new client connect\r\n");
        char buf[1024];
        // while ((ret=recv(client_sock, buf, sizeof(buf), 0)) != 0) {
        //     if (ret > 0) {
        //         printf("recv: %s\r\n", buf);
        //     } else {
        //         usleep(100*1000);
        //     }
        // }

        while (1) {
            char *sret = fgets((char *)buf, sizeof(buf), stdin);
            // if (sret != NULL) {

            // }
            int ret = send(client_sock, sret, strlen(sret), 0);
            if (ret == 0) {
                break;
            }
        }
        printf("client %d disconnect\r\n", client_sock);
        close(client_sock);
    }
    close(sock_fd);
}
#include <sys/select.h>
#include <sys/poll.h>
int main(void)
{
    test_addr_convert();
	test_server();
	//test_protocol();
	//test_byteorder();
	//test_getaddinfo();
	//test_getnameinfo();
    //test_socket_api();
	//test_select();
	//test_poll();
	//test_select_accept();
	// test_udp_seg();
    return 0;
}
