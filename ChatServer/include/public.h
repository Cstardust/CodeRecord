#ifndef _PUBLIC_H_
#define _PUBLIC_H_

// client 和 server的公共头文件

enum MsgType{
    LGOIN_MSG = 1,      //  登录
    LGOIN_MSG_ACK,
    REG_MSG,            //  注册
    REG_MSG_ACK,
    LOGINOUT_MSG,       //  登出
    LOGINOUT_MSG_ACK,
    PTOP_CHAT_MSG,           //  peer to peer chat
    PTOP_CHAT_MSG_ACK,
    ADD_FRIEND_MSG,     //  加好友
    ADD_FRIEND_MSG_ACK,
    CREATE_GROUP_MSG,   //  创建群组
    CREATE_GROUP_MSG_ACK,
    ADD_GROUP_MSG,      //  加入群组
    ADD_GROUP_MSG_ACK,
    GROUP_CHAT_MSG,
    GROUP_CHAT_MSG_ACK, //  群组聊天
};

#endif