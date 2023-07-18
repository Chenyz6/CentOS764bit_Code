#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define SOCKET_IP "127.0.0.1"
#define SOCKET_PORT 8888

void  catch_child(int temp)
{
	while(waitpid(0, NULL, WNOHANG) > 0)   // WNOHANG  return immediately if no child has exited
	{
		printf("waitpid...\n");
	}
	return ;
}

int main(  )
{  
	int sfd, cfd, cur;
	int len;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	char buf[1024] =  { 0 };
	char client_ip[128]  = { 0 };
	int pid;

	struct sigaction act;
	act.sa_handler = catch_child;  
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	cur = sigaction(SIGCHLD, &act, NULL);
	if(cur != 0)
	{
		fprintf(stdout,"sigaction error!");
	}

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SOCKET_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	cur = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	cur = listen(sfd, 64);
	printf("Waiting client connect ... ....\n");	
	
	while(1)
	{
		cfd = accept(sfd, (struct sockaddr *)&client_addr, &client_addr_len);
	 	//if(cfd != 0)
		//{
		//	printf("Client IP : %s, Port %d\n", 
		//	inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)),
		//	ntohs(client_addr.sin_port));
		//}

		pid = fork();
		if(pid == 0)
		{
			close(sfd);
			while(1)
			{
				len = read(cfd, buf, sizeof(buf));
			    if(len == 0)
				{
					printf("the other side has been closed.\n");
					break;
				}
				
				//printf("Client IP : %s, Port %d\n", 
				//inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)),
				//ntohs(client_addr.sin_port));
				
				write(1, buf, len);
				for(int i = 0; i < len; i++)
				{
					buf[i] = toupper(buf[i]);
				}
				write(cfd, buf, len);
			}
			close(cfd);
			return 0;
		}else
		{
			close(cfd);
		}

	}
	return 0;
}
