#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define SOCKET_PORT 8888

struct s_info
{
	struct sockaddr_in client_addr;
	int client_fd;
};

void * pthread_do(void * temp)
{
	struct s_info * client_info = (struct s_info *)temp;
	char buf[1024] =  { 0 };
	int len;
	char client_ip[128]  = { 0 };

	while(1)
	{
		len = read(client_info->client_fd, buf, sizeof(buf));
		if(len == 0)
		{
			printf("the client %d closed...\n", client_info->client_fd);
			break;
		}

		printf("Client IP: %s, PORT: %d\n", 
						inet_ntop(AF_INET, &client_info->client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)),
						ntohs(client_info->client_addr.sin_port));

		for(int i = 0; i < len; i++)
		{
			buf[i] = toupper(buf[i]);
		}
		write(1, buf, len);
		write(client_info->client_fd, buf, len);
	}

	close(client_info->client_fd);
	return (void *) 0;
}

int main(  )
{  
	int sfd, cfd, cur;
	int len, i;
	struct sockaddr_in server_addr, client_addr;
	struct s_info client_info[256];
	socklen_t client_addr_len = sizeof(client_addr);
	pthread_t tid;

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));


	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SOCKET_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	cur = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	cur = listen(sfd, 64);
	printf("Waiting client connect ... ....\n");	
	
	while(1)
	{
		cfd = accept(sfd, (struct sockaddr *)&client_addr, &client_addr_len);
		client_info[i].client_addr = client_addr;
		client_info[i].client_fd = cfd;
		pthread_create(&tid, NULL, pthread_do, (void *)&client_info[i]);
		pthread_detach(tid);
		i++;
	}
	return 0;
}
