#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPoll.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false){
    ep = new Epoll;
    threadPoll = new ThreadPoll;
}


EventLoop::~EventLoop()
{
    delete ep;
}

//loop就是ep->poll的死循环
void EventLoop::loop(){
    while(!quit){
        std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it){
            (*it)->handleEvents();
        }
    }
}




void EventLoop::updataChannel(Channel *ch){
    ep->updateChannel(ch);
}


void EventLoop::addThread(std::function<void()> func){
    threadPoll->add(func);
}