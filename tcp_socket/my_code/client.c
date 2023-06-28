#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT 8888
#define	SERVER_IP "127.0.0.1"

int main()
{
	int fd, res;
	struct sockaddr_in addr;
	char buf[1024];

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)
	{
		fprintf(stderr,"socket error!");
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	inet_pton(AF_INET, SERVER_IP, &addr.sin_addr.s_addr);
	res = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
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

