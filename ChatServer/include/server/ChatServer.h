#ifndef _CHAT_SERVER_H_
#define _CHAT_SERVER_H_


#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/Buffer.h>
#include <functional>
#include "json.hpp"

using namespace muduo;
using json = nlohmann::json;


//  ChatServer的网络模块
class ChatServer
{
public:
    ChatServer(net::EventLoop *loop,
               const net::InetAddress &listenAddr,
               const string &nameArg);
    
    void start();

private:
    //  连接时调用的回调函数
    void onConnection(const net::TcpConnectionPtr& );

    //  接收信息时调用的回调函数
    void onMessage(const net::TcpConnectionPtr &,net::Buffer*,Timestamp st);

    net::TcpServer server_; //  TcpServer
    net::EventLoop *loop_;  //  事件循环
};

#endif 