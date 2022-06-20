#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "locker.h"
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

class http_conn {
public:
    //所有的 socket 上的事件都被注册到同一个 epoll 对象中
    static int m_epollfd;
    //统计用户的数量
    static int m_user_count;

    http_conn() {}

    ~http_conn() {}

    void process(); //处理客户端请求

    void init(int sockfd, const sockaddr_in& addr); //初始化新接收的连接

    void close_conn();//关闭连接

    bool read();//非阻塞的读

    bool write();//非阻塞的写

private:
    int m_sockfd;          //该 HTTP 连接的 socket
    sockaddr_in m_address; //通信的 socket 地址
};

#endif