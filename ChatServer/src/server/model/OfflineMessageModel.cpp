#include"OfflineMessageModel.h"
#include"DB.h"

bool OfflineMsgModel::insert(int user_id,string msg)
{
    char sql[1024];
    sprintf(sql,"insert into offlinemessage values (%d, '%s')",user_id,msg.c_str());
    
    MySQLHandler mysql_handler;
    if(mysql_handler.connect())     //  连接数据库
    {
        return mysql_handler.update(sql);
    }
    return false;
}


bool OfflineMsgModel::remove(int user_id)
{
    char sql[1024];
    sprintf(sql,"delete from offlinemessage where userid = %d",user_id);

    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {   
        return mysql_handler.update(sql);
    }
    return false;
}

vector<string> OfflineMsgModel::query(int user_id)
{
    char sql[1024];
    sprintf(sql,"select message from offlinemessage where userid = %d",user_id);

    MySQLHandler mysql_handler;
    vector<string> vec;
    if(mysql_handler.connect())
    {
        MYSQL_RES *res = mysql_handler.query(sql);
        if(res)
        {
            MYSQL_ROW row;
            while((row=mysql_fetch_row(res))!=nullptr)  //  逐行从结果集中读取
            {
                vec.push_back(row[0]);  //  row[0] : char * -> string
            }
            mysql_free_result(res);     //  释放结果集内存
        }
    }
    return vec;
}

