#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/socket.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 8888

int main()
{
    int res, clientfd, len;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    char buf[1024],client_IP[64];

    clientfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientfd == -1)
        perror("socket error!");
    
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &clientaddr.sin_addr.s_addr);

    bind(clientfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr));

    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        res = sendto(clientfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        if(res == -1)
            perror("sendto error!");
        len = recvfrom(clientfd, buf, sizeof(buf), 0, NULL, 0);
        if(len == -1)
            perror("recvfrom error!");
        write(1, buf, len);
    }
    
}



