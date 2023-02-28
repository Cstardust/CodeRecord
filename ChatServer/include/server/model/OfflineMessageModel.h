#ifndef _OFFLINEMESSAGEMODEL_H_
#define _OFFLINEMESSAGEMODEL_H_

#include<string>
#include<vector>

using std::string;
using std::vector;

//  操作offlineMessage离线信息表的工具类
    //  insert
    //  remove
    //  query
class OfflineMsgModel
{
public:
    //  插入应提供给id的离线信息
    bool insert(int user_id, string msg);
    //  删除提供给id的离线信息
    bool remove(int user_id);
    //  查询id的离线信息
    vector<string> query(int user_id);
private:
};

#endif



