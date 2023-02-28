#include"FriendModel.h"
#include"DB.h"

bool FriendModel::insert(int user_id,int friend_id)
{
    char sql[512]={0};
    sprintf(sql,"insert into friend values(%d , %d)",user_id,friend_id);

    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        return mysql_handler.update(sql);
    }
    return false;
}


//  返回user_id的好友列表 每个好友的信息是 id name state  
vector<User> FriendModel::query(int user_id)
{
    char sql[512];
    sprintf(sql,"select a.id,a.name,a.state from user a inner join friend b on b.friendid = a.id where b.userid = %d",user_id);

    vector<User> vec;
    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        MYSQL_RES * res = mysql_handler.query(sql);
        if(res)
        {
            MYSQL_ROW row;
            while( (row=mysql_fetch_row(res)) != nullptr)
            {
                User usr;
                usr.setId(atoi(row[0]));
                usr.setName(row[1]);
                usr.setState(row[2]);
                vec.push_back(usr);
            }
            mysql_free_result(res);
        }
    }
    return vec;
}