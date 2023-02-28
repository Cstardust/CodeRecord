#ifndef _GROUPMODEL_H_
#define _GROUPMODEL_H_

#include"Group.hpp"
#include"GroupUser.h"

//  维护群组信息的工具类
    //  操作AllGroup表和GroupUser表
class GroupModel
{
public:
    //  创建群组   insert into AllGroup
    bool createGroup(Group &group);
    //  用户加入群组 
    bool addIntoGroup(int user_id,int group_id,const string& role);
    //  查询用户所在群组信息
    vector<Group> queryGroups(int user_id);
    //  根据指定的groupid查询群组用户id列表，除了user_id自己，主要用户群聊有任务给群组其他成员发群消息
    vector<int> queryGroupUsers(int user_id,int group_id);
private:
};


#endif