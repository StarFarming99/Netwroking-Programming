#include<iostream>
#include<unistd.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<cstring>
#include<sys/select.h>

int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = 0;

    bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    listen(lfd, 8);

    //创建fd_set的集合，存放需要检测的描述符
    fd_set rdset, tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;
    while(1)
    {
        tmp = rdset;
        //调用select，让内核帮助检测
        int ret = select(maxfd + 1, &tmp, nullptr, nullptr, nullptr);//永久阻塞
        if(ret == -1)
        {
            perror("select func error");
            return -1;
        }
        else if(ret == 0)
        {
            continue;
        }
        else if(ret > 0)
        {
            //检测到文件描述符缓冲区发生改变
            if(FD_ISSET(lfd, &tmp))
            {
                //表示有新客户端连接
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr*)&clientaddr, &len);
                FD_SET(cfd, &rdset);
                maxfd = maxfd > cfd ? maxfd : cfd; 
            }
            for(int i = lfd + 1; i <= maxfd; i++)
            {
                if(FD_ISSET(i, &tmp))
                {
                    //客户端发来数据
                    char buf[1024] = {0};
                    int length = read(i, buf, sizeof(buf));
                    if(length == -1)
                    {
                        perror("read func error");
                        return -1;
                    }
                    else if(length == 0)
                    {
                        std::cout << "client exit" << std::endl;
                        close(i);
                        FD_CLR(i, &rdset);
                    }
                    else if(length > 0)
                    {
                        std::cout << "read buf = " << buf << std::endl;
                        write(i, buf, strlen(buf) + 1);//回写
                    }
                }
            }
        }
    }

    close(lfd);

    return 0;
}