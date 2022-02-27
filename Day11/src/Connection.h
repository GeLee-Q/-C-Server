#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection{
private:
    EventLoop * loop;
    Socket * sock;
    Channel * channel;
    std::function<void(Socket*)> deleteConnectionCallback;
    std::string *inBuffer;
    Buffer * readBuffer;

public:
    Connection(EventLoop * loop, Socket * sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)>)
}