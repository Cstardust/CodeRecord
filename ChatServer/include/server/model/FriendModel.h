#ifndef _FRIENDMODEL_H_
#define _FRIENDMODEL_H_

#include<vector>
#include"User.hpp"

using std::vector;

//  操作friend好友表的工具类
    //  insert
    //  query
class FriendModel
{
public:
    //  向好友表中插入一对好友
    bool insert(int user_id,int friend_id);
    //  返回user_id的好友列表
    vector<User> query(int user_id);    
private:
};

#endif