#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>

#define IP "127.0.0.1"
#define PORT 8888

int main()
{
    int res,listenfd,connectfd;
    struct sockaddr_in clientaddr, serveraddr;
    char ip[32], buf[1024], len;
    socklen_t clientlen = sizeof(clientaddr);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
        perror("listen error!");
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    res = bind(listenfd, (struct socketaddr *)&serveraddr, sizeof(serveraddr));
    if(res == -1)
        perror("bind error!");

    res = listen(listenfd, 1024);
    if(res == -1)
        perror("listen error!");

    int epfd, num;
    struct epoll_event temp, ep[1024];
    epfd = epoll_create(1024);
    if(epfd == -1)
        perror("epoll_create error!");

    temp.events = EPOLLIN;
    temp.data.fd = listenfd;
    res = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &temp);
    if(res == -1)
        perror("epoll_ctl error!");

    while (1)
    {
        num = epoll_wait(epfd, ep, 1024, -1);
        if(num == -1)
            perror("epoll_wait error");
        
        for(int i = 0; i < num; i++)
        {
            if(!(ep[i].events & EPOLLIN))
                continue;
            if(ep[i].data.fd == listenfd)
            {
                connectfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
                if(connectfd == -1)
                    perror("accept error!");
                	
				int flag = fcntl(connectfd, F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(connectfd, F_SETFL, flag);

				printf("Client IP:%s, PORT:%d\n",
                                inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip)),
                                ntohs(clientaddr.sin_port));

                temp.events = EPOLLIN;
                temp.data.fd = connectfd;
                res = epoll_ctl(epfd, EPOLL_CTL_ADD, connectfd, &temp);
                if(res == -1)
                    perror("epoll_ctl error!");
            }else
            {
                len = read(ep[i].data.fd, buf, sizeof(buf));
                if(len == -1){
                    perror("read error!");
                }else if(len == 0){
                    res = epoll_ctl(epfd, EPOLL_CTL_DEL, ep[i].data.fd, NULL);
                    if(res == -1)
                        perror("epoll_ctl error!");
                    printf("Client exit!\n");
                    close(ep[i].data.fd);
                }else{
                    for(int i = 0; i < len; i++){
                        buf[i] = toupper(buf[i]);
                    }
                    write(1, buf, len);
                    write(ep[i].data.fd, buf, len);
                }
            }
        }
    }
        
    return 0;
}
