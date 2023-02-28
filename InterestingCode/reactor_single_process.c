#include<stdio.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<assert.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#define BUFFLEN 4096
#define MAX_EVENTS 1024

struct my_event;

//  callback
typedef void (*Callback_t) (int ,struct my_event*);
void recvMsg(int events,struct my_event* m_ev);
void sendMsg(int events,struct my_event* m_ev);
void acceptConn(int ,struct my_event* m_ev);


void myError(int x,char *msg);
int setnonblocking(int fd);

//  在cpp里可简化为构造函数
struct my_event * initMyEvent(int fd,enum EPOLL_EVENTS type,int idx,Callback_t callback);
void setMyEvent(struct my_event *m_ev,int events,Callback_t callback);


int registerEvent(struct my_event *m_ev,int op);
void closeEvent(struct my_event *m_ev);



//  抽象fd 
//  封装fd,events type感兴趣的事件,handler回调,buf缓冲区,idx等
struct my_event
{
    int fd;             //  socket fd
    int events;         //  epoll监听的事件类型
    // void *arg;          //  指向自己的指针
    struct my_event* ptr;   // 指向自己的指针
    Callback_t handler; //  回调函数    
    int st;             //  记录本my_event是否被挂在epoll树上
    int idx;            //  记录下标 用于判断是否my_event数量已达上限
    char buf[BUFFLEN];  //  缓冲区
    int len;            //  缓冲区有效数据长度 
};

//  epoll_fd
int epfd;

//  所有被epoll监听的事件(在epoll树上) 都会被记录在my_events中
    //  只要是epoll感兴趣的都会记录 不一定活跃。
//  也即 : **维护了所有挂在epoll树上的my_events**
    //  首先挂在epoll树上的events 其生命周期我认为当然应当是全局。总不能还挂在epoll树上 这个事件的信息结构就死掉了。
    //  因此 需要有一个全局的数组或者其他data-structure来维护这些结构
    //  因此 就有了my_events
//  作用：
//        1. 用于判断什么时候连接满了。
//        2. 用于将callback和相应的事件 : 建立连接/读/写绑定在一起。
struct my_event my_events[MAX_EVENTS+1];


//  create and add lfd to epoll
//  return lfd
int initListenSocket();


void show_my_events()
{
    for(int i=0;i<=MAX_EVENTS;++i)
    {
        if(my_events[i].st!=0)
        {
            printf("fd = %d\n",my_events[i].fd);
        }
    }
}

int main()
{
    //  create epoll fd
    epfd = epoll_create1(EPOLL_CLOEXEC);

    //  create and add lfd to epoll
    initListenSocket();
    //  epoll_event for epoll_wait
    struct epoll_event events[MAX_EVENTS+1];
    while(1)
    {

        // show_my_events();  打印epoll tree上的event
        
        // demultiplex : epoll_wait
            //  LT模式 
        int nfds = epoll_wait(epfd,events,MAX_EVENTS+1,-1);

        // dispatcher / Reactor : 
        for(int i=0;i<nfds;++i)
        {
            //  活跃的两种fd
                //  listenfd : accept
                //  connfd   : read / write
            struct epoll_event *ev = &events[i];
            struct my_event *m_ev = events[i].data.ptr;

            //  监听到了EPOLLIN事件 且 m_ev的fd确实对这个EPOLLIN感兴趣 按理来说一定是true
            //  ev->events(活跃的事件) 不一定 ==  m_ev->events(实际发生的事件)  
            if((ev->events & EPOLLIN) && (m_ev->events & EPOLLIN))
            {
                //  handler 可能是 lfd的acceptConn ， 也可能是connfd的recvData
                m_ev->handler(ev->events,m_ev);
            }
            
            if((ev->events & EPOLLOUT) && (m_ev->events & EPOLLOUT))
            {
                //  handler只可能是connfd的sendData
                m_ev->handler(ev->events,m_ev);
            }
        }
    }
}

char convert_char(char c) {
  if ('A' <= c && c <= 'Z')
    return c + 32;  // 转换小写
  else if ('a' <= c && c <= 'z')
    return c - 32;  // 转换大写
  else
    return c;  // 其他不变
}


void recvMsg(int events,struct my_event* conn_ev)
{
    int connfd = conn_ev->fd;

    memset(conn_ev->buf,0,sizeof conn_ev->buf);
    int nbytes = read(connfd,conn_ev->buf,BUFFLEN-1);   //  留一个"\0"

    //  非阻塞且没数据
    if(nbytes == -1 && errno == EAGAIN)
    {
        printf("read connfd = %d ; internal error! it should not be woke up in LT\n",connfd);
        // exit(-1);
    }
    //  对端关闭
    else if(nbytes == 0)
    {
        closeEvent(conn_ev);
    }
    //  recive msg
    else if(nbytes > 0)
    {
        int len = strlen(conn_ev->buf);
        conn_ev->buf[len] = 0;
        for(int i=0;i<nbytes;++i)
        {
            conn_ev->buf[i] = convert_char(conn_ev->buf[i]);
        }
        
        printf("server received msg %s\tfrom connfd %d\n",conn_ev->buf,connfd);
        
        //  更改事件类型以及回调
        setMyEvent(conn_ev,EPOLLOUT,sendMsg);
        //  在epoll tree上更改fd的事件类型 mod 因为已经存在
        registerEvent(conn_ev,EPOLL_CTL_MOD);
        //  之后epoll_wait监听的就是connfd的可写事件
        //  由于fd写缓冲区没满 所以会立刻触发 
    }
    //  wrong
    else
    {
        perror("recv internal error!\n");
        exit(-1);
    }
}



void sendMsg(int events,struct my_event* m_ev)
{
    int connfd = m_ev->fd;
    int nbytes = write(connfd,m_ev->buf,m_ev->len);
    //  nbytes < 0
    myError(nbytes,"server send msg error\n");

    //  nbytes > 0
        //  send sucess
        //  EPOLLOUT结束 MOD成EPOLLIN
    printf("server send %d to client\n",nbytes);
        //  更改为events 为 EPOLLIN ；handler = recvMsg
    setMyEvent(m_ev,EPOLLIN,recvMsg);
        //  在epoll上mod
    registerEvent(m_ev,EPOLL_CTL_MOD);


}


//  更改myEvent的回调和事件类型
void setMyEvent(struct my_event *m_ev,int events,Callback_t callback)
{
    //  m_events中读缓冲区的内容就是待会要写给socket 所以m_events也是一个写缓冲区。
    m_ev->events = events;
    m_ev->handler = callback;
    //  update len
    m_ev->len = strlen(m_ev->buf);
}


void resetMyEvent(struct my_event *m_ev)
{
    memset(m_ev,0,sizeof (struct my_event));        //  remove from my_events
}

void removeFromEpoll(int fd)
{
    printf("remove %d from epoll tree\n",fd);
    myError(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL),"epoll_ctl_del error");  //  remove from epoll tree
}

void closeEvent(struct my_event * m_ev)
{
    int fd = m_ev->fd;
    printf("close fd : %d\n",fd);
    resetMyEvent(m_ev);                             //  remove from my_events
    removeFromEpoll(fd);                            //  remove from epoll tree
    close(fd);                                      //  close fd
}

void acceptConn(int events, struct my_event* listen_ev)
{
    int lfd = listen_ev->fd;
    assert(events == listen_ev->events);     //  listen socket 一定时EPOLLIN
    printf("lfd = %d\n",lfd);
    int idx = -1;
    for(int i=0;i<MAX_EVENTS;++i)       //  MAX_EVENTS 是accept的lfd 不能给conn使用
    {
        if(my_events[i].st == 0)
        {
            idx = i;
            break;
        }
    }

    myError(idx,"fds on epoll tree is upto limit\n");

    //  accept 接收连接
    int connfd = accept(lfd,NULL,NULL);
    myError(connfd,"accept error!\n");

    //  设置非阻塞    
    setnonblocking(connfd);
    //  记录在task queue里
    struct my_event* m_ev = initMyEvent(connfd,EPOLLIN,idx,recvMsg);

    //  将connfd的读事件注册到epoll上
    assert(registerEvent(m_ev,EPOLL_CTL_ADD));

}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL,0);
    fcntl(fd,F_SETFL,old_option | O_NONBLOCK);
    return old_option;
}


int registerEvent(struct my_event *m_ev,int op)
{
    int fd = m_ev->fd;
    struct epoll_event event;
    memset(&event,0,sizeof event);
    event.data.ptr = m_ev;
    event.events = m_ev->events;
    printf("add or mod %d on the epoll successfully\n",fd);
    return epoll_ctl(epfd,op,fd,&event) == 0;
}


struct my_event * initMyEvent(int fd,enum EPOLL_EVENTS type,int idx,Callback_t callback)
{
    myError(MAX_EVENTS-idx,"initMyEvent  error : outof bound\n");
    struct my_event *m_ev = &my_events[idx];
    m_ev->fd = fd;
    m_ev->events = type;
    m_ev->ptr = m_ev;
    m_ev->handler = callback;
    m_ev->st = 1;           //  注册在epoll tree上
    m_ev->idx = idx;
    memset(m_ev->buf,0,BUFFLEN);
    m_ev->len = 0;
    return m_ev;
}




int initListenSocket()
{
    //  create
        int lfd = socket(PF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
        myError(lfd,"socket error\n");
        //  reuse
        int reuse = 1;
        setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof reuse);
        struct sockaddr_in server_addr;
        memset(&server_addr,0,sizeof server_addr);
        server_addr.sin_family = AF_INET;           //  protocol
        inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr.s_addr);    //  ip
        server_addr.sin_port = htons(6669);         //  port
        int ret = bind(lfd,(struct sockaddr*)(&server_addr),sizeof server_addr);
        myError(ret,"bind error\n");
        ret = listen(lfd,5);
        myError(ret,"listen error\n");
    //  record
        struct my_event *m_ev = initMyEvent(lfd,EPOLLIN,MAX_EVENTS,acceptConn);
    //  add to epoll
        registerEvent(m_ev,EPOLL_CTL_ADD);
    
    printf("ip = 127.0.0.1 port = 6669\n");
    return lfd;
}


void myError(int x,char *msg)
{
    if(x < 0)
    {
        perror(msg);
        exit(-1);
    }
    return ;
}