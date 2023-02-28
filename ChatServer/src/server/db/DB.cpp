#include "DB.h"

MySQLHandler::MySQLHandler()
{
    conn_ = mysql_init(nullptr);
}

MySQLHandler::~MySQLHandler()
{
    if (conn_)
    {
        mysql_close(conn_);
    }
}

bool MySQLHandler::connect()
{
    MYSQL *p = mysql_real_connect(conn_,
                                  server.c_str(),
                                  user.c_str(),
                                  pwd.c_str(),
                                  dbname.c_str(),
                                  3306, //  mysql server的端口号
                                  nullptr,
                                  0);
    if (p != nullptr)
    {
        mysql_query(conn_, "set names gbk"); //  支持中文
        LOG_INFO << "connect successfully";
        return true;
    }
    else
    {
        LOG_INFO << "failed to connect";
        return false;
    }
}

//  update
bool MySQLHandler::update(string sql_command)
{
    if (mysql_query(conn_, sql_command.c_str()))
    {
        LOG_INFO << __FILE__ << " : " << __LINE__ << " : " << sql_command << " failed to update";
        return false;
    }
    return true;
}

// select 
MYSQL_RES *MySQLHandler::query(string sql_command)
{
    if (mysql_query(conn_, sql_command.c_str()))
    {
        LOG_INFO << __FILE__ << " : " << __FILE__ << " : " << sql_command << " failed to query";
        return nullptr;
    }
    return mysql_use_result(conn_);
    //  调用mysql_query()或mysql_real_query()之后，必须为成功生成结果集的每个语句(SELECT，SHOW，DESCRIBE，EXPLAIN，CHECK TABLE等等)
    //  调用mysql_store_result()或mysql_use_result()。
    //  完成结果集后，还必须调用mysql_free_result()。
}

MYSQL *MySQLHandler::getConnection()
{
    return conn_;
}