#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 8888

int main()
{
	int cfd;
	struct sockaddr_in addr;
	char buf[1024];

	cfd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &addr.sin_addr.s_addr);
	int res = connect(cfd, (struct sockaddr *)&addr, sizeof(addr));

	while(1)
	{
		fgets(buf, sizeof(buf), stdin);
		write(cfd, buf, strlen(buf));
		int len = read(cfd, buf, sizeof(buf));
		write(1, buf, len);
	}
}
