#include"ChatServer.h"
#include"ChatService.h"
#include<signal.h>
#include<sys/types.h>


//  对于Server的ctrl + c异常退出
    //  业务类应当负责重置一些状态和信息
        //  比如sql表中用户是否在线的状态
void sigIntHandelr(int sig)     //  ctrl + c SIG_INT
{
    ChatService::getInstance()->reset();
    exit(0);
}


int main(int argc,char *argv[])
{

    if(argc!=3){
        printf("Usage : ./ChatServer 127.0.0.1 port\n");
        exit(0);
    }

    //  捕捉信号
    signal(SIGINT,sigIntHandelr);

    net::EventLoop loop;
    net::InetAddress addr(argv[1],atoi(argv[2]));
    ChatServer server(&loop,addr,"MyChatServer");


    server.start();     //  epoll_ctl 将 lfd 添加到epoll上      int epoll_ctl(int epfd , int op , int fd , struct epoll_event * event );  epfd = EPOLL_CTL_ADD     
    loop.loop();        //  epoll_wait以阻塞方式等待新用户的连接 和 已连接用户的读写事件等

    return 0;
}