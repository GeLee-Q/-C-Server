#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop * _loop) : loop(_loop),sock(nullptr), 
    acceptChannel(nullptr){

    sock  = new Socket();
    addr  = new InetAddress('127.0.0.1',1234);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    
    acceptChannel = new Channel(loop,sock->getFD());

    /**
     * std::function 是一个可调用对象包装器，简化调用
     * std::bind 函数适配器
     * 1.将可调用对象和其参数绑定成一个仿函数；
     * 2.只绑定部分参数，减少可调用对象传入的参数。
     * bind的结果由function保存
     *
     */
    std::function<void()> cb = 
                std::bind(&Acceptor::acceptConnection,this);

    acceptChannel->setCallback(cb); //设置回调函数
    acceptChannel->enableReading();
    delete addr;
}

Acceptor::~Acceptor(){
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    InetAddress *clnt_addr = new InetAddress();      
    Socket *clnt_sock = new Socket(sock->accept(clnt_addr));      
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb){
    newConnectCallback = _cb;
}