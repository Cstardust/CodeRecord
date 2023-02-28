#include "GroupModel.h"
#include "DB.h"
#include <vector>

using std::vector;

//  创建群组
bool GroupModel::createGroup(Group &group)
{
    char sql[512];
    sprintf(sql,
            "insert into allgroup(groupname,groupdesc) values ('%s','%s')",
            group.getName().c_str(), group.getDesc().c_str());

    MySQLHandler mysql_handler;
    if (mysql_handler.connect())
    {
        //  插入成功 更新id
        if(mysql_handler.update(sql))
        {
            group.setId(mysql_insert_id(mysql_handler.getConnection()));
            return true;
        }
    }
    return false;
}

//  user_id 加入 group_id
bool GroupModel::addIntoGroup(int user_id, int group_id, const string &role)
{
    char sql[512];
    sprintf(sql,
            "insert into groupuser values (%d , %d , '%s')",
            group_id, user_id, role.c_str());

    MySQLHandler mysql_handler;
    if (mysql_handler.connect())
    {
        return mysql_handler.update(sql);
    }
    return false;
}

//  查询user_id所在的群组消息
//  一个用户可能在多个群组
vector<Group> GroupModel::queryGroups(int user_id)
{
    char sql[512];

    //  allgroup 和 groupuser 联合查询
    //  根据传入的user_id 查询出 user_id所属的所有群组的 id name 和 desc
    sprintf(sql,
            "select a.id , a.groupname , a.groupdesc from allgroup a inner join groupuser b on a.id = b.groupid where b.userid = %d",
            user_id);

    vector<Group> groups; //  存储user_id所属的所有群组对象
    MySQLHandler mysql_handler;
    if (mysql_handler.connect())
    {
        MYSQL_RES *res = mysql_handler.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                groups.emplace_back(atoi(row[0]), row[1], row[2]);
            }
            mysql_free_result(res);
        }
    }

    for (Group &g : groups) //  引用
    {
        //  查询属于组groupid = g.getId()的 所有成员的详细信息:id name state role
        // memset(sql,0,sizeof sql);
        sprintf(sql,
                "select a.id,a.name,a.state ,b.grouprole from user a inner join groupuser b on b.userid = a.id where b.groupid = %d",
                g.getId());
        MYSQL_RES *res = mysql_handler.query(sql);
        if (res)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) //  mysql_fetch_row 也可以封装进MySQLHandler
            {
                GroupUser gu;
                gu.setId(atoi(row[0]));
                gu.setName(row[1]);
                gu.setState(row[2]);
                gu.setRole(row[3]);
                
                g.getUsers().push_back(gu);
            }
            mysql_free_result(res); //  mysql_free_result 也可以封装进MySQLHandler
        }
    }

    return groups; //  groups包含id所含的所有group。且每个group的id name desc users
}

vector<int> GroupModel::queryGroupUsers(int user_id, int group_id)
{
    char sql[512];
    sprintf(sql,
            "select userid from groupuser where groupid = %d and userid != %d",
            group_id,
            user_id);
    
    vector<int> ids;
    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        MYSQL_RES *res = mysql_handler.query(sql);
        if(res!=nullptr)
        {
            MYSQL_ROW row;
            while((row=mysql_fetch_row(res))!=nullptr)
            {
                ids.push_back(atoi(row[0]));
            }
        }
        mysql_free_result(res);
    }
    return ids;
}
