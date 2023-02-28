//  网络模块

#include "ChatServer.h"
#include "ChatService.h"

ChatServer::ChatServer(net::EventLoop *loop,
                       const net::InetAddress &listenAddr,
                       const string &nameArg) : server_(loop, listenAddr, nameArg), loop_(loop)
{
    //  连接回调
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
    //  接收消息回调
    server_.setMessageCallback(std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    //  1 for accept
    //  3 for worker
    server_.setThreadNum(4);
}

void ChatServer::start()
{
    server_.start();
}

void ChatServer::onMessage(const net::TcpConnectionPtr &conn,
                           net::Buffer *buffer,
                           Timestamp time)
{
    string recv_buf = buffer->retrieveAllAsString();
    //  数据反序列化
    json js = json::parse(recv_buf);
    //  根据接收的数据调用相应函数
    //  目的：解耦
    //  原理：回调函数
        //  map 存储 <id - handler>
        //  handler利用function定义，利用bind包装传入
    //  不在这里switch-case。而是将业务代码交给业务模块。达到将网络模块的代码和业务模块的代码解耦的目的。
    //  通过js["msgid"] ---> 获取相应业务的handler ---> 调用

    ChatService *chat_service = ChatService::getInstance();    //  单例 获取 Chat_Serivce业务类
    MsgHandler msg_handler = chat_service->getMsgHandler(MsgType(js["msg_id"].get<int>()));       //  get<int>() 转化成int
    msg_handler(conn,js,time);
    return ;
}


//  在连接建立和断开时回调
void ChatServer::onConnection(const net::TcpConnectionPtr &conn)
{
    if(!conn->connected())
    {
        ChatService::getInstance()->clientCloseException(conn);  //  移除关闭的连接
        conn->shutdown();
    }
}


