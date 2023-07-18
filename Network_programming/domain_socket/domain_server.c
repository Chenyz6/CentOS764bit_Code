#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>  // toupper()
#include <unistd.h>   // STDOUT_FILENO
#include <sys/un.h>
#include <stddef.h>

#define SERVER_SOCKET "server.socket"

int main()
{
	int sfd, cfd, res, offset; 
	struct sockaddr_un server_addr,client_addr;	
	socklen_t clientLen = sizeof(client_addr);
	char buf[1024];

	sfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(sfd == -1)
	{
		fprintf(stderr, "socket error!");
	}
	
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SERVER_SOCKET);

	offset = offsetof(struct sockaddr_un, sun_path) + strlen(server_addr.sun_path);

	unlink(SERVER_SOCKET);
	res = bind(sfd,(struct sockaddr *)&server_addr, (socklen_t)offset); 
	if(res == -1)
	{
		fprintf(stderr, "bind error!");
	}
		
	res = listen(sfd,64);
	if(res == -1)
	{
		fprintf(stderr, "listen error!");
	}

	printf("Waiting connect... ...\n");
	
	while(1)
	{
		cfd = accept(sfd,(struct sockaddr *)&client_addr, (socklen_t *)&clientLen);
		if(res == -1)
		{
			fprintf(stderr, "accept error!");		
		}

		clientLen -= offsetof(struct sockaddr_un, sun_path);
		client_addr.sun_path[clientLen] = '\0';
		printf("client bind filename %s\n", client_addr.sun_path);

		while (1)
		{
			int len = read(cfd, buf, sizeof(buf));
			write(STDOUT_FILENO, buf, len);
			for(int i = 0; i < len; i++)
			{
				buf[i] = toupper(buf[i]);
			}
			write(cfd, buf, len);
		}
		

	}

	close(cfd);
	close(sfd);
	return 0;
}


