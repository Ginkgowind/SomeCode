#include <stdio.h>
#include <arpa/inet.h>  //htons() sockaddr_in
#include <string.h>     //bzero()
#include <sys/socket.h> //socket
#include <unistd.h>
#include <stdlib.h>   //exit()
#include <sys/wait.h> //waitpid();
#include <pthread.h>
#define BUFFER_SIZE 1024
struct IP_data
{
    int cfd;
    struct sockaddr_in cliaddr;
};
void *mytcp(void *args)
{
    char ip[24] = {0};
    struct IP_data ip_data = *(struct IP_data *)args;
    int cfd = ip_data.cfd;
    struct sockaddr_in cliaddr = ip_data.cliaddr;
    // 1. 创建监听的套接字
    while (1)
    {

        // 接收数据
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = read(cfd, buf, sizeof(buf));
        if (len > 0)
        {
            printf("客户端 %s 发送数据: %s\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)), buf);
            write(cfd, buf, len);
        }
        else
        {
            printf("客户端 %s 断开了连接...\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)));
            close(cfd);
            return;
        }
    }
}
void *myudp1(void *args)
{

    int serfd = 0;
    serfd = socket(AF_INET, SOCK_DGRAM, 0);

    int ret = 0;
    struct sockaddr_in seraddr = {0};
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("192.168.40.124");
    seraddr.sin_port = htons(8888);
    while (bind(serfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
        ;
    // 接收发送自客户端的消息
    while (1)
    {
        int addrlen = 0;
        char buf[1024] = {0};
        struct sockaddr_in clientaddr = {0};
        addrlen = sizeof(clientaddr);
        ret = recvfrom(serfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &addrlen);
        if (ret > 0)
        {
            printf("UDP协议连接成功 客户端的IP地址: %s,端口：%u ", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
            printf("发送数据: %s\n", buf);
            sendto(serfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, addrlen);
        }
    }
    close(serfd);
}
void *myudp2(void *args)
{

    int serfd = 0;
    serfd = socket(AF_INET, SOCK_DGRAM, 0);

    int ret = 0;
    struct sockaddr_in seraddr = {0};
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("192.168.1.50");
    seraddr.sin_port = htons(8888);
    while (bind(serfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
        ;
    // 接收发送自客户端的消息
    while (1)
    {
        int addrlen = 0;
        char buf[1024] = {0};
        struct sockaddr_in clientaddr = {0};
        addrlen = sizeof(clientaddr);
        ret = recvfrom(serfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &addrlen);
        if (ret > 0)
        {
            printf("UDP协议连接成功 客户端的IP地址: %s,端口：%u ", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
            printf("发送数据: %s\n", buf);

            sendto(serfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, addrlen);
        }
    }
    close(serfd);
}
void *net(void *args)
{
    pthread_t th1;

    int iRet = -1;
    struct IP_data ip_data;
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    // 2. 将socket()返回值和本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080); // 大端端口
    // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY; // 这个宏的值为0 == 0.0.0.0
    iRet = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &iRet, sizeof(int));
    while (iRet < 0)
    {
        iRet = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &iRet, sizeof(int));
    }
    while (bind(lfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        ;
    // 3. 设置监听
    while (listen(lfd, 128) == -1)
        ;
    // 4. 阻塞等待并接受客户端连接
    while (1)
    {

        struct sockaddr_in cliaddr;
        int clilen = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &clilen);
        // 打印客户端的地址信息
        char ip[24] = {0};
        printf("TCP协议连接成功 客户端的IP地址: %s, 端口: %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(cliaddr.sin_port));
        // 5. 和客户端通信
        ip_data.cfd = cfd;
        ip_data.cliaddr = cliaddr;
        pthread_create(&th1, NULL, mytcp, (void *)(&ip_data));
    }
    return;
}
int main()
{
    char command[256];

    char interface_name[] = "eth0";       // 以太网接口名字
    char ip_address[] = "192.168.40.124"; // 设置的IP地址
    char netmask[] = "255.255.255.0";     // 子网掩码
    char gateway[] = "192.168.0.1";       // 默认网关

    sprintf(command, "ifconfig %s %s netmask %s up", interface_name, ip_address, netmask);
    system(command);

    // 配置默认网关
    sprintf(command, "route add default gw %s %s", gateway, interface_name);
    system(command);

    char interface_name1[] = "eth1";     // 以太网接口名字
    char ip_address1[] = "192.168.1.50"; // 设置的IP地址
    char netmask1[] = "255.255.255.0";   // 子网掩码
    char gateway1[] = "192.168.1.1";     // 默认网关

    sprintf(command, "ifconfig %s %s netmask %s up", interface_name1, ip_address1, netmask1);
    system(command);
    pthread_t th_net;
    pthread_t th2;
    pthread_t th3;
    pthread_create(&th2, NULL, myudp1, NULL);
    pthread_create(&th3, NULL, myudp2, NULL);
    pthread_create(&th_net, NULL, net, NULL);
    while (1)
        ;
    return 0;
}
