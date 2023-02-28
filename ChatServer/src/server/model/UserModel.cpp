#include"UserModel.h"
#include"User.hpp"
#include"DB.h"
#include<iostream>

//  向User表中插入数据
bool UserModel::insert(User &user)
{
    // std::cout<<"UserModel insert"<<std::endl;
//  mysql_insert_id 返回上一个查询为AUTO_INCREMENT列生成的 ID
    //  组成sql语句
    char sql[1024]={0};
    sprintf(sql, 
            "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(),user.getPwd().c_str(),user.getState().c_str());
    
    MySQLHandler mysql_handler;
    if(mysql_handler.connect()){        //  connect：发起连接
        //  获取该条用户数据插入时自动生成的主键id
        mysql_handler.update(sql);
        user.setId(mysql_insert_id(mysql_handler.getConnection()));
        return true;
    }else{
        //  ...
    }
    return false;
}

//  根据id返回User
User UserModel::query(int id)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from user where id = %d",id);
    
    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        MYSQL_RES *res = mysql_handler.query(sql);      //  mysql_query , mysql_use_query
        if(res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
            if(row!=nullptr)
            {
                return User(atoi(row[0]),row[1],row[2],row[3]);
            }
        }
    }
    return User();      //  default id = -1 返回User'id = -1 代表 用户没注册过 或者 连接sql server失败
}

// Return Values
// 下一行的MYSQL_ROW结构，或NULL。 NULL返回的含义取决于在mysql_fetch_row()之前调用了哪个函数：
// 在mysql_store_result()之后使用时，如果没有更多行可检索，则mysql_fetch_row()返回NULL。
// 在mysql_use_result()之后使用时，如果没有更多行可检索或发生错误，则mysql_fetch_row()返回NULL。要确定是否发生错误，请检查mysql_error()是否返回非空字符串或mysql_errno()返回非零。



bool UserModel::upState(User &user)
{
    char sql[1024]={0};
    sprintf(sql,"update user set state = '%s' where id = %d",user.getState().c_str(),user.getId());

    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        if(mysql_handler.update(sql))
        {
            return true;
        }
    }
    return false;
}


void UserModel::resetState()
{
    const char *sql = " update user set state = 'offline' where state = 'online' ";
    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        mysql_handler.update(sql);
    }

}   