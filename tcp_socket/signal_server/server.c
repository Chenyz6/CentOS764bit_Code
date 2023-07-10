#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>  // toupper()
#include <unistd.h>   // STDOUT_FILENO

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main()
{
	int sfd, cfd, res; 
	struct sockaddr_in server_addr,client_addr;	
	socklen_t clientLen = sizeof(client_addr);
	char buf[1024], client_IP[256];	

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1)
	{
		fprintf(stderr, "socket error!");
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8888);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	res = bind(sfd,(struct socketaddr *)&server_addr, sizeof(server_addr)); 
	if(res == -1)
	{
		fprintf(stderr, "bind error!");
	}
		
	res = listen(sfd,64);
	if(res == -1)
	{
		fprintf(stderr, "listen error!");
	}

	cfd = accept(sfd,(struct socketaddr *)&client_addr,&clientLen);
	if(res == -1)
	{
		fprintf(stderr, "accept error!");		
	}
    printf("ip:%s,port:%d\n", 
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)),
		ntohs(client_addr.sin_port));
	printf("123456");
	
	while(1)
	{
		int len = read(cfd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, len);
		for(int i = 0; i < len; i++)
		{
			buf[i] = toupper(buf[i]);
		}
		write(cfd, buf, len);
	}

	close(cfd);
	close(sfd);
	return 0;
}


