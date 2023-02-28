# ChatServer
## 基于muduo

## 基于nginx的tcp负载均衡

## 基于redis作为message queue

## 编译
```bash
./autobuild.sh

or

cd build
rm -rf ./*
cmake ..
make
```

## 效果
- ChatServer
```bash
shc@shc-virtual-machine:~/ChatServer/bin$ ./ChatServer 127.0.0.1 6666
20220831 05:20:41.030118Z 49732 INFO  TcpServer::newConnection [MyChatServer] - new connection [MyChatServer-127.0.0.1:6666#1] from 127.0.0.1:54752 - TcpServer.cc:80
connect redis-server success!
20220831 05:21:19.714554Z 49733 INFO  do login service !  - ChatService.cpp:49
20220831 05:21:19.721324Z 49733 INFO  connect successfully - DB.cpp:29
20220831 05:21:19.728755Z 49733 INFO  connect successfully - DB.cpp:29
20220831 05:21:19.737419Z 49733 INFO  connect successfully - DB.cpp:29
20220831 05:21:19.744650Z 49733 INFO  connect successfully - DB.cpp:29
20220831 05:21:19.751272Z 49733 INFO  connect successfully - DB.cpp:29
20220831 05:21:19.759688Z 49733 INFO  connect successfully - DB.cpp:29
20220831 05:21:37.222174Z 49959 INFO  connect successfully - DB.cpp:29
20220831 05:22:06.632894Z 49733 INFO  connect successfully - DB.cpp:29
20220831 05:22:09.743268Z 49732 INFO  TcpServer::removeConnectionInLoop [MyChatServer] - connection MyChatServer-127.0.0.1:6666#1 - TcpServer.cc:109

shc@shc-virtual-machine:~/ChatServer/bin$ ./ChatServer 127.0.0.1 6667
20220831 05:20:15.131960Z 49780 INFO  TcpServer::newConnection [MyChatServer] - new connection [MyChatServer-127.0.0.1:6667#1] from 127.0.0.1:44936 - TcpServer.cc:80
connect redis-server success!
20220831 05:20:46.890288Z 49781 INFO  do login service !  - ChatService.cpp:49
20220831 05:20:46.916497Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:20:46.936300Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:20:46.953651Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:20:46.962213Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:20:46.971192Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:20:46.982104Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:21:08.693222Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:21:08.700159Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:21:37.213340Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:21:42.331239Z 49781 INFO  connect successfully - DB.cpp:29
20220831 05:21:50.685400Z 49780 INFO  TcpServer::removeConnectionInLoop [MyChatServer] - connection MyChatServer-127.0.0.1:6667#1 - TcpServer.cc:109
```

- ChatClient
```bash
======================MENU======================                                          │======================MENU======================
1. login                                                                                  │1. login
2. register                                                                               │2. register
3. quit                                                                                   │3. quit
input your choice : 1                                                                     │input your choice : 1
id : 22                                                                                   │id : 23
password : 123456                                                                         │input your choice : 1
======================Current User Data======================                             │id : 23
current login user ---> id : 22 name : shc                                                │password : 55
======================Friend List======================                                   │======================Current User Data======================
23 ss offline                                                                             │current login user ---> id : 23 name : ss
======================Group List======================                                    │======================Friend List======================
4 save yourself study!                                                                    │22 shc online
22 shc creator                                                                            │======================Group List======================
23 ss normal                                                                              │4 save yourself study!
                                                                                          │22 shc creator
================Instructions==============                                                │23 ss normal
addgroup === 加入群组 -- addgroup:groupid                                                 │=============PtoP Chat============
creategroup === 创建群组 -- creategroup:groupname:groupdesc                               │Time : 20220831 01:27:41.767874
groupchat === 群聊 -- groupchat:groupid:message                                           │From name : shc
chat === 发送消息 -- chat:to_id:msg                                                       │        msg : uu
loginout === 注销登录 -- loginout                                                         │=============PtoP Chat============
addfriend === 添加好友 -- addFriend:friend_id                                             │Time : 20220831 05:21:08.688306
help === 获取帮助 -- help                                                                 │From name : shc
                                                                                          │        msg : hello
chat:23:hello                                                                             │
READ IN chat:23:hello                                                                     │================Instructions==============
PARSE chat 23:hello                                                                       │addgroup === 加入群组 -- addgroup:groupid
                                                                                          │creategroup === 创建群组 -- creategroup:groupname:groupdesc
===================chat===================                                                │groupchat === 群聊 -- groupchat:groupid:message
3 23:hello                                                                                │chat === 发送消息 -- chat:to_id:msg
                                                                                          │loginout === 注销登录 -- loginout
chat:23:bye                                                                               │addfriend === 添加好友 -- addFriend:friend_id
READ IN chat:23:bye                                                                       │help === 获取帮助 -- help
PARSE chat 23:bye                                                                         │
                                                                                          │=============PtoP Chat============
===================chat===================                                                │Time : 20220831 05:21:37.208555
3 23:bye                                                                                  │From name : shc
                                                                                          │        msg : bye
loginout                                                                                  │loginout
READ IN loginout                                                                          │READ IN loginout
======================MENU======================                                          │======================MENU======================
1. login                                                                                  │1. login
2. register                                                                               │2. register
3. quit                                                                                   │3. quit
input your choice : 3                                                                     │input your choice : 3
```
