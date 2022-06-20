#include "http_conn.h"

//所有的 socket 上的事件都被注册到同一个 epoll 对象中
int http_conn::m_epollfd = -1;
//统计用户的数量
int http_conn::m_user_count = 0;

void setnonblocking(int fd) {
    int flag = fcntl(fd, F_GETFL) | O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}

//添加文件描述符到 epoll 中
void addfd(int epollfd, int fd, bool one_shot) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot) {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    //设置文件描述符非阻塞
    setnonblocking(fd);
}

//从 epoll 中删除文件描述符
void removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

void modfd(int epollfd, int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

//初始化连接
void http_conn::init(int sockfd, const sockaddr_in& addr) {
    m_sockfd = sockfd;
    m_address = addr;

    //设置端口复用
    int reuse = 1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    //添加到 epoll 对象中
    addfd(m_epollfd, sockfd, true);

    //用户数加1
    ++m_user_count;
}

//关闭连接
void http_conn::close_conn() {
    if (m_sockfd != -1) {
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        //关闭一个连接，客户数量减1
        --m_user_count;
    }
}

bool http_conn::read() {
    printf("一次性读完数据\n");
    return true;
}

bool http_conn::write() {
    printf("一次性写完数据\n");
    return true;
}

//由线程池的工作线程调用，这是处理 HTTP 请求的入口函数
void http_conn::process() {
    //解析HTTP请求
    printf("parse request, create response\n");

    //生成响应
}