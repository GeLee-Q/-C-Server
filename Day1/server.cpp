#include <stdio.h>
#include <sys.socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM，0)；
    
    /**
     * 对于客户端 ，服务器存在的唯一标识是一个IP地址和port；
     * 需要将套接字绑定到相应的IP地址和port上；
    */
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sockfd, SOMAXCONN);

    /**
     * 接受一个链接，都要使用accept，同时也要保存客户端socket地址信息；
     * 
     */
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(socketlen_t);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd  = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    
    printf("new client fd %d! IP: %s Port: %d\n", 
        clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    return 0;
    
}