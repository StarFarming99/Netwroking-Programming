//TCP 通信服务器

#include<iostream>
#include<arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<cstring>

int main()
{
    //1.创建socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;//协议族
    saddr.sin_addr.s_addr = 0;//全接受
    saddr.sin_port = htons(9999);
    int ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1)
    {
        perror("bind func error");
        return -1;
    }
    //3.监听
    int lret = listen(sockfd, 8);
    if(lret == -1)
    {
        perror("listen func error");
        return -1;
    }

    //4.接受客户端
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int cfd = accept(sockfd,(struct sockaddr*) &clientaddr, &len);
    if(cfd == -1)
    {
        perror("accept error");
        return -1;
    }

    //输出客户端信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientaddr));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    std::cout << "IP " << clientIP << std::endl;
    std::cout << "PORT " << clientPort << std::endl;

    //5.通信
    //获取数据
    char recvBuf[1024] = {0};
    int length = read(cfd, recvBuf, sizeof(recvBuf));
    if(length == -1)
    {
        perror("read func error");
        return -1;
    }
    else if(length > 0)
    {
        std::cout << recvBuf << std::endl; 
    }
    else if(length == 0)
    {
        //断开连接
        std::cout << "断开连接" << std::endl;
    }

    char* data = "This is server";
    write(cfd, data, strlen(data));

    //close
    close(cfd);
    close(sockfd);

    return 0;
}