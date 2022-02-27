#pragma once
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel
{
private:
    Eventloop * loop;
    int fd;
    uint32_t events; //表示监听了哪些事件
    uint32_t revents; //表示正在发生哪些事件
    bool inEpoll;     //这个channel是否在红黑树中
    std::function<void()> callback;
public:
    Channel(EventLoop *_loop, int _fd); //channel应该与那个epoll 以及fd绑定
    ~Channel();

    void enableReading(); //监听该channel上发生的读事件
    void handleEvent();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    // void setEvents(uint32_t);
    void setRevents(uint32_t);
    void setCallback(std::function<void()>);
};