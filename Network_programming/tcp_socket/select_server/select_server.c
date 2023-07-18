#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>

#define SOCKET_PORT 8888

int main(  )
{  
	int lfd, cfd, cur;
	int len, k;
	struct sockaddr_in server_addr, client_addr;
	char client_ip[256];
	socklen_t client_addr_len = sizeof(client_addr);

	lfd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SOCKET_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	cur = bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	cur = listen(lfd, 128);
	printf("Waiting client connect ... ....\n");	
	
	fd_set rset, allset;
	FD_ZERO(&allset);
	FD_SET(lfd, &allset);
	int maxfd = lfd;
	char buf[1024];
	int num = 0;
	
	int client[1024];
	for(int i = 0; i < 1024; i++)
	{
		client[i] = -1;
	}
	int maxi = -1;
	int sockfd = -1;

	while(1)
	{
		rset = allset;
		num = select(maxfd + 1, &rset, NULL, NULL, NULL);		
		if(num < 0)
		{
			printf("select error!");
		}
		if(FD_ISSET(lfd, &rset))
		{
			cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addr_len);
			printf("Client IP = %s, PORT = %d\n",
					inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)),
					ntohs(client_addr.sin_port));
		
			for(k = 0; k < 1024; k++)
			{
				if(client[k] < 0)
				{
					client[k] = cfd;
					break;
				}
			}
			if(k == 1024)
			{
				printf("max error!");
				return 0;
			}

			FD_SET(cfd, &allset);
			if(cfd > maxfd)
			{
				maxfd = cfd;
			}

			if(k > maxi)
			{
				maxi = k;
			}
			if(num == 1)
			{
				continue;
			}
		}
		for(int i = 0; i <= maxi; i++)
		{
			if((sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd, &rset))
			{
				len = read(sockfd, buf, sizeof(buf));
				if(len == 0)
				{
					printf("close client ...");
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}else
				{
					for(int j = 0; j < len; j++)
					{
						buf[j] = toupper(buf[j]);
					}
					write(sockfd, buf, len);
					write(1, buf, len);
				}
				if(num == 1)
					break;
			}
		}
	}
	close(lfd);
	return 0;
}
