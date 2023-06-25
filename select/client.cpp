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

    int num = 0;
    while(1)
    {
        char sendBuf[1024] = {0};
        sprintf(sendBuf, "send data %d", num++);
        write(sockfd, sendBuf, strlen(sendBuf) + 1);

        int len = read(sockfd, sendBuf, sizeof(sendBuf));
        if(len == -1)
        {
            perror("read");
            return -1;
        }
        else if(len > 0)
        {
            std::cout << "receive message: " << sendBuf << std::endl;
        }
        else
        {
            std::cout << "server is out of connection" << std::endl;
            break;
        }
        sleep(1);
    }

    //close
    close(sockfd);
    return 0;
}