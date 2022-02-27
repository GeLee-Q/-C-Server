# Acceptor类

- 用于管理新建链接的逻辑放在勒Server中



- 类存在于事件驱动的EventLoop ，即Reactor中的 main-reactor
- socket fd就是服务器监听的socket fd, 每一个Acceptor 对应一个socket fdl
- 通过一个独有的Channel负责分发epoll, Channel的事件处理函数会调用Acceptor中的接受链接函数来新建一个TCP链接