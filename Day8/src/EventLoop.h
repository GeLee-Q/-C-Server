#pragma once
class Epoll;
class Channel;

/**
 * EventLoop就是把 Epoll打包后，增加了不停的调用loop
 * loop()即开始事件驱动
*/

class EventLoop{
private:
    Epoll * ep;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updataChannel(Channel*);
};
