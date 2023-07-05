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
	int len, i;
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

	cur = listen(lfd, 64);
	printf("Waiting client connect ... ....\n");	
	
	fd_set rset, allset;
	FD_ZERO(&allset);
	FD_SET(lfd, &allset);
	int maxfd = lfd;
	char buf[1024];
	int num = 0;

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
			FD_SET(cfd, &allset);
		
			if(cfd > maxfd)
			{
				maxfd = cfd;
			}
			if(num == 1)
			{
				continue;
			}
		}
		for(int i = lfd + 1; i <= maxfd; i++)
		{
			if(FD_ISSET(i, &rset))
			{
				len = read(i, buf, sizeof(buf));
				if(len == 0)
				{
					printf("close client ...");
					close(i);
					FD_CLR(i, &allset);
					break;
				}
				for(int j = 0; j < len; j++)
				{
					buf[j] = toupper(buf[j]);
				}
				write(i, buf, len);
				write(1, buf, len);
			}
		}
	}
	return 0;
}
