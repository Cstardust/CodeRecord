#ifndef _REDIS_H_
#define _REDIS_H_

#include<hiredis/hiredis.h>
#include<thread>
#include<functional>
#include<string>
#include<iostream>


using std::string;
using std::function;
using std::cerr;
using std::cout;
using std::endl;
using std::thread;



class Redis
{
public:
    Redis();
    ~Redis();
    
    //  连接 redis server
    bool connect();

    //  向redis指定的通道channel发布消息
    bool publish(int channel,string message);

    //  向redis指定的通道subscribe订阅消息
    bool subscribe(int channel);
    
    //  向redis指定的通道unsubscribe取消订阅消息
    bool unsubscribe(int channel);

    //  在独立线程中接收订阅通道的消息
    void observer_channel_message();

    //  初始化向业务层上报通道消息的回调对象
    void init_notify_handler(function<void(int,string)> f);

private:    
    //  hiredis同步上下文对象 负责publish消息
    redisContext *publish_context_;
    //  hiredis同步上下文对象 负责subscribe消息
    redisContext *subscribe_context_;
    //  回调操作
    function<void(int,string)> notify_message_handler_;
};



#endif