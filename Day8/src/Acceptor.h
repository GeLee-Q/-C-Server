#pragma once
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor{
private:
    EventLoop * loop;
    Socket * sock;
    Channel * acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;

public:
    Acceptor(EventLoop * _loop);
    ~Acceptor();

    //新建连接时，只需要调用这个函数即可
    void acceptConnection();

    //回调函数的本身的实现在Server类中
    void setNewConnectionCallback(std::function<void(Socket*)>); 
};