#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>
#define READ_BUFFER 1024

Connection::Connection(EventLoop * _loop, Socket * _sock)
    :loop(_loop), sock(_sock){
    channel = new Channel(loop,sock->getFD());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFD());
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection(){
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd){
    char buf[READ_BUFFER];
    while (true) { //由于使用非阻塞IO，需要不断读取，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }else if(read_bytes == 0){
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }else if(read_bytes == -1 && errno == EINTR){
            //正常中断，继续读取；
            printf("continue reading");
            continue;
        }else if(read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK){
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb){
    deleteConnectionCallback = _cb;
}