//客户端

#include<iostream>
#include<arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<cstring>

int main()
{
    //1.套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);
    int res = connect(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
    if(res == -1)
    {
        perror("connet error");
        return -1;
    }

    //通信
    char* data = "hello, i am client!";
    write(sockfd, data, strlen(data));

    char recvBuf[1024] = {0};
    int length = read(sockfd, recvBuf, sizeof(recvBuf));
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

    //close
    close(sockfd);
    return 0;
}