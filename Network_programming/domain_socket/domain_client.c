#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stddef.h>

#define SERVER_SOCKET "server.socket"
#define CLIENT_SOCKET "client.socket"

int main()
{
	int fd, res, offset;
	struct sockaddr_un serveraddr, clientaddr;
	char buf[1024];

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd == -1)
	{
		fprintf(stderr,"socket error!");
	}
	
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, CLIENT_SOCKET);
	offset = offsetof(struct sockaddr_un, sun_path) + strlen(clientaddr.sun_path);
	unlink(CLIENT_SOCKET);
	bind(fd, (struct sockaddr *)&clientaddr, offset);

	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, SERVER_SOCKET);
	offset = offsetof(struct sockaddr_un, sun_path) + strlen(serveraddr.sun_path);
	res = connect(fd, (struct sockaddr *)&serveraddr, offset);
	if(res == -1)
    {
        fprintf(stderr,"connect error!");
    }
	

	while(1)
	{
		fgets(buf, sizeof(buf), stdin);
		write(fd, buf, strlen(buf));
		int len = read(fd , buf, sizeof(buf));
		write(STDOUT_FILENO, buf, len); 
	}

	close(fd);
	return 0;
}

