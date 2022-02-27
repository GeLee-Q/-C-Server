#include "Epoll.h"
#include "util.h"
#include <unistd.h>
#include <string.h>

#define MAX_EVENTS 1000

Epoll::Epoll(): epfd(-1),events(nullptr){
    epfd = epoll_create1(0);
    //此时epoll的事件还没有被添加到epoll红黑树中;
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS]; // events 数组 存储触发的所有的事件
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

// Epoll::addFd(int fd, uint32_t op){
//     struct epoll_event ev;                //将触发的sockfd事件方法到ev,然后方法epfd中；
//     bzero(&ev, sizeof(ev));
//     ev.data.fd = fd;
//     ev.events = op;
//     errif(epoll_ctl(epfd, EPOLL_CTL_ADD,fd, &ev) == -1, "epoll add event error");
// }


// std::vector<epoll_event> Epoll::poll(int timeout){
//     std::vector<epoll_event> activeEvents; //将epfd中所有的wait事件放入设置的vector中；
//     int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
//     errif(nfds == -1, "epoll wait error");
//     for(int i = 0 ; i < nfds ; ++i){
//         activeEvents.push_back(events[i]);
//     }
//     return activeEvents;
// }

std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeEvents; 
    //如果epoll_wait检测到事件，就会把所有就绪的事件从内核事件表中复制到events。
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0 ; i < nfds ; ++i){
        Channel * ch = (Channel*)events[i].data.ptr; // 获取当前的channel
        ch->setRevents(events[i].events); //设置对文件描述符发生的事件
        activeEvents.push_back(ch);
    }
    return activeEvents;
}


void Epoll::updateChannel(Channel * channel ){
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel; //传入这个channel的指针 可以绑定到 epoll_events->epoll_date->ptr上
    ev.events = channel->getEvents();//得到要监听这个文件描述符的哪些操作
    if(!channel->getInEpoll()){
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) ==-1, "epoll add error");
        channel->setInEpoll();
    }else{
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}