#ifndef _DB_H_
#define _DB_H_

// 数据访问对象模式(Data Access Object Pattern)或DAO模式
// 用于把低级的数据访问操作从高级的业务服务中分离出来

#include <mysql/mysql.h>
#include <string>
#include <muduo/base/Logging.h>

using std::string;

//  数据库配置信息
static string server = "127.0.0.1";
static string user = "root";
static string pwd = "123456";
static string dbname = "chat";

/*
mysql开发包提供的接口：
初始化连接
    mysql_init
释放数据库连接资源
    mysql_close
发起连接
    mysql_real_connect
执行sql语句
    mysql_query
*/

class MySQLHandler
{
public:
    //  初始化连接
    MySQLHandler();
    //  释放数据库连接资源
    ~MySQLHandler();
    //  发起连接
    bool connect();
    //  insert  / delete ...
    bool update(string sql_command); 
    //  select
    MYSQL_RES *query(string sql_command);  
    //  获取连接
    MYSQL *getConnection();
private:
    MYSQL *conn_;
};

#endif