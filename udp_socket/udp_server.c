#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/socket.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 8888

int main()
{
    int sockfd, res, len;
    struct sockaddr_in listenaddr, clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    char buf[1024],client_IP[64];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
        perror("listen error!");
    
    listenaddr.sin_family = AF_INET;
    listenaddr.sin_port = htons(PORT);
    listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    res = bind(sockfd, (struct sockaddr *)&listenaddr, sizeof(listenaddr));
    if(res == -1)
        perror("bind error!");
    
    res = listen(sockfd, 1024);
    if(res == -1)
        perror("listen error!");

    printf("waiting connect... ...\n");

    while (1)
    {
        len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &clientaddr_len);
        if(len == -1){
            perror("recvfrom error!");
        }else if(len == 0){
            printf("client exit!\n");
            close(sockfd);
        }else{
            printf("ip:%s,port:%d\n", 
		        inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, client_IP, sizeof(client_IP)),
		        ntohs(clientaddr.sin_port));
            write(1, buf, len);    
            for(int i = 0; i < len; i++){
                buf[i] = toupper(buf[i]);
            }
            sendto(sockfd, buf, len, 0, (struct sockaddr *)&clientaddr, clientaddr_len);
        }
    }
    close(sockfd);      
}






