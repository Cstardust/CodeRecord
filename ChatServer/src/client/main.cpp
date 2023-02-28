//  client connect to Load Balancer based on nginx (负载均衡器) port is 8000
//  Load Balancer distribute connections to ChatServer (负载均衡器将连接分发给ChatServer)
//  负载均衡器负责的ChatServer的port是6666,6667
//  ChatServer连接Redis(作为Message Queue) 通过发布请阅机制进行跨服务器通信 (我觉得也是一种保持信息一致性)
//  Port of Redis is 6379
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <unordered_map>
#include <functional>
#include <thread>
#include <atomic>
#include "json.hpp"
#include "User.hpp"
#include "Group.hpp"
#include "public.h"

using nlohmann::json;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::function;
using std::unordered_map;
using std::vector;

//  记录当前系统登陆的用户信息
User current_user;
//  记录当前用户的好友信息
vector<User> current_user_friendlist;
//  记录当前登录用户的群组列表信息
vector<Group> current_user_grouplist;
//  是否已登录状态
bool isOnLine = false;



//  显示当前登录成功的用户信息
void showCurrentUserData();
//  接收消息线程 (主线程用作发送消息线程)
void recvTaskHandler(int);
//  获取系统时间
string getCurrentTime();
//  主聊天页面程序
void showMainMenu(int);

void printUsage();

bool addFriend(int fd, const string &);

bool chat(int fd, const string &);

bool createGroup(int, const string &);

bool addGroup(int, const string &);

bool groupChat(int, const string &);

bool loginout(int, const string &s = "");

void help(int, const string &s = "");

unordered_map<string, string> commandUsage{
    {"help", "获取帮助 -- help"},
    {"addfriend", "添加好友 -- addFriend:friend_id"},
    {"chat", "发送消息 -- chat:to_id:msg"},
    {"creategroup", "创建群组 -- creategroup:groupname:groupdesc"},
    {"addgroup", "加入群组 -- addgroup:groupid"},
    {"groupchat", "群聊 -- groupchat:groupid:message"},
    {"loginout", "注销登录 -- loginout"}};

unordered_map<string, function<void(int, string)>> commandMap{
    {"help", help},
    {"addfriend", addFriend},
    {"creategroup", createGroup},
    {"addgroup", addGroup},
    {"groupchat", groupChat},
    {"chat", chat},
    {"loginout", loginout},
};




//  聊天客户端实现，main线程用作发送消息线程，子线程用作接收线程
int main(int argc, char *argv[])
{
    cout << "argc " << argc << endl;
    if (argc != 3)
    {
        cout << "Usage : "
             << "ChatClient 127.0.0.1 "
             << " 8000 " << endl;
        exit(1);
    }
    const char *ip = argv[1];
    unsigned short port = atoi(argv[2]);

    //  1.  创建socket
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    assert(client_fd >= 0);

    //  2.  创建要连接的server 结构体address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof server_address);
    server_address.sin_family = AF_INET;              //  ip类型
    inet_pton(AF_INET, ip, &server_address.sin_addr); //  IP
    server_address.sin_port = htons(port);            //  port

    //  3.  发起连接
    int ret = connect(client_fd, (sockaddr *)&server_address, sizeof server_address);
    if (ret == -1)
    {
        cout << "client failed to connect server" << endl;
        exit(1);
    }

    while (!isOnLine)
    {
        cout << "======================MENU======================" << endl;
        cout << "1. login " << endl;
        cout << "2. register" << endl;
        cout << "3. quit" << endl;
        cout << "input your choice : ";
        //  读取选择
        int ch;
        cin >> ch;
        cin.get(); //  读掉缓冲区残留的回车

        switch (ch)
        {
        //  login登录业务
        case 1:
        {
            int id = 0;
            char pwd[50] = {0};

            cout << "id : ";
            cin >> id;
            cin.get(); //  读掉缓冲区残余的回车
            cout << "password : ";
            cin.getline(pwd, 50);

            json send_js;
            send_js["msg_id"] = LGOIN_MSG;
            send_js["id"] = id;
            send_js["password"] = pwd;
            string request = send_js.dump();

            int ret = send(client_fd, request.c_str(), request.size(), 0);
            if (ret == -1)
            {
                cout << "client failed to send login msg" << endl;
                break;
            }
            else
            {
                char recv_buf[1024] = {0};
                int len = recv(client_fd, recv_buf, 1024, 0);
                if (len == -1)
                {
                    cout << "client failed to recv response for login" << endl;
                    break;
                }
                else
                {
                    //  序列化成json
                    json res_js = json::parse(recv_buf);
                    //  登陆失败
                    if (res_js["errno"].get<int>() == 2)
                    {
                        cerr << "client failed to login" << endl;
                        cerr << res_js["errmsg"].get<string>() << endl;
                        break;
                    }
                    //  登陆成功
                    else
                    {
                        current_user.setId(res_js["id"].get<int>());
                        current_user.setName(res_js["name"]);
                        current_user.setPwd(pwd);
                        current_user.setState("online");

                        //  a. 存下好友列表
                        vector<string> friend_list = res_js["friends"];
                        // cout<<"data debug friend_list "<<friend_list.size()<<endl;
                        if (!friend_list.empty())
                        {
                            for (string &s : friend_list)
                            {
                                //  利用json格式传送的数据，来构造user对象
                                json js = json::parse(s);
                                User user;
                                user.setId(js["id"].get<int>());
                                user.setName(js["name"].get<string>());
                                user.setState(js["state"]);

                                //  存好友
                                current_user_friendlist.push_back(user);
                            }
                        }

                        //  b. 存下群组列表
                        vector<string> group_list = res_js["groups"];
                        // cout<<"data debug group_list "<<group_list.size()<<endl;
                        if (!group_list.empty())
                        {
                            for (string &s : group_list)
                            {
                                Group g;
                                json js = json::parse(s);
                                g.setId(js["group_id"].get<int>());
                                g.setName(js["group_name"]);
                                g.setDesc(js["group_desc"]);
                                vector<string> group_users = js["group_users"];
                                for (string &s : group_users)
                                {
                                    json js = json::parse(s);
                                    GroupUser u;
                                    u.setId(js["id"]);
                                    u.setName(js["name"]);
                                    u.setRole(js["role"]);
                                    g.getUsers().push_back(u);
                                }
                                //  存组
                                current_user_grouplist.push_back(g);
                            }
                        }

                        //  d. 开启接收线程 且该接收线程只启动一次
                        static int recvThreadFlag = 0;
                        if(recvThreadFlag==0)
                        {
                            std::thread recvTaskThread(recvTaskHandler, client_fd);
                            recvTaskThread.detach();
                            ++recvThreadFlag;
                        } 

                        //  e. 显示登录用户的基本信息
                        showCurrentUserData();

                        //  c. offline msg
                        vector<string> vec_offlinemsg = res_js["offlinemsg"];
                        if (!vec_offlinemsg.empty())
                        {
                            for (string &s : vec_offlinemsg)
                            {
                                //  序列化
                                json js = json::parse(s);
                                MsgType msg_ack = js["msg_id"].get<MsgType>();
                                if (msg_ack == PTOP_CHAT_MSG_ACK)
                                {
                                    cout << "=============PtoP Chat============" << endl;
                                    cout << "Time : " << js["time"].get<string>() << endl;
                                    cout << "From name : " << js["name"].get<string>() << endl;
                                    cout << "\tmsg : " << js["msg"].get<string>() << endl;
                                }
                                else if (msg_ack == GROUP_CHAT_MSG_ACK)
                                {
                                    cout << "=============Group Chat============" << endl;
                                    cout << "Time : " << js["time"].get<string>() << endl;
                                    cout << "From group : " << js["group_id"].get<int>() << endl;
                                    cout << "\tFrom : " << js["id"].get<int>() << " " << js["name"].get<string>() << endl;
                                    cout << "\t\tmsg : " << js["msg"].get<string>() << endl;
                                }
                                else
                                {
                                    cout << "sth else happened" << endl;
                                }
                                // cout << js["time"]
                                //      << " [" << js["id"] << "] " << js["name"]
                                //      << " said: " << js["msg"] << endl;
                            }
                        }
                    }

                    //  f. 聊天界面
                    isOnLine = true;
                    showMainMenu(client_fd);
                }
            }
            break;
        }
        //  register 注册业务
        case 2:
        {
            //  id是server分配的
            //  name,password是client设置的
            string name;
            string password;
            cout << "input your name : (without space)";
            cin >> name;
            cout << "input your password : (without space)";
            cin >> password;

            json send_js;
            send_js["msg_id"] = REG_MSG;
            send_js["name"] = name;
            send_js["password"] = password;
            string request = send_js.dump();

            int ret = send(client_fd, request.c_str(), request.size(), 0);
            if (ret == -1)
            {
                cout << "client failed to send register msg" << endl;
                break;
            }
            else
            {
                char buf[1024] = {0};
                int len = recv(client_fd, buf, sizeof buf, 0);
                if (len == -1)
                {
                    cout << "client failed to recv response for register" << endl;
                    break;
                }
                else
                {
                    json res_js = json::parse(buf);
                    if (res_js["errno"].get<int>() != 0)
                    {
                        cerr << "client failed to register" << endl;
                        cerr << res_js["errmsg"] << endl;
                        break;
                    }
                    else
                    {
                        cout << "Register Success! Please remember your id : " << res_js["id"].get<int>() << endl;
                        break;
                    }
                }
            }
            break;
        }
        //  quit
        case 3:
        {
            close(client_fd);
            current_user_friendlist.clear();
            current_user_grouplist.clear();
            exit(0);
            break;
        }
        default:
            cout << "invalid input!" << endl;
        }
    }
}

void showCurrentUserData()
{
    cout << "======================Current User Data======================" << endl;

    cout << "current login user ---> id : " << current_user.getId() << " name : " << current_user.getName() << endl;

    cout << "======================Friend List======================" << endl;
    for (const User &u : current_user_friendlist)
    {
        cout << u.getId() << " " << u.getName() << " " << u.getState() << endl;
    }

    cout << "======================Group List======================" << endl;
    for (Group &g : current_user_grouplist)
    {
        cout << g.getId() << " " << g.getName() << " " << g.getDesc() << endl;
        for (GroupUser &u : g.getUsers())
        {
            cout << u.getId() << " " << u.getName() << " " << u.getRole() << endl;
        }
    }
}

void printUsage()
{
    cout << "\n================Instructions==============" << endl;
    for (unordered_map<string, string>::iterator iter = commandUsage.begin(); iter != commandUsage.end(); ++iter)
    {
        cout << iter->first << " === "
             << iter->second << endl;
    }
    cout << endl;
}

// {"help","获取帮助 -- help"},
// {"addfriend","添加好友 -- addFriend:friend_id"},
// {"chat","发送消息 -- chat:to_id:msg"},
// {"creategroup","创建群组 -- creategroup:groupname:groupdesc"},
// {"addgroup","加入群组 -- addgroup:groupid"},
// {"groupchat","群聊 -- groupchat:groupid:message"},
// {"loginout","注销登录 -- loginout"}
bool createGroup(int fd, const string &s)
{
    cout << "===========createGroup==========" << endl;
    cout << s << endl;
    int idx = s.find(':');
    if (idx == string::npos)
    {
        cout << "type help to look through the instructions" << endl;
        return false;
    }
    string group_name = s.substr(0, idx);
    string group_desc = s.substr(idx + 1);
    cout << group_name << endl;
    cout << group_desc << endl;
    json js;
    js["msg_id"] = CREATE_GROUP_MSG;
    js["id"] = current_user.getId();
    js["groupname"] = group_name;
    js["groupdesc"] = group_desc;
    string buf = js.dump();

    int ret = send(fd, buf.c_str(), buf.size(), 0);
    if (ret == -1)
    {
        cout << "failed to send msg to server" << endl;
        return false;
    }

    return true;
}

bool addGroup(int fd, const string &s)
{
    int group_id = atoi(s.c_str());
    json js;
    js["msg_id"] = ADD_GROUP_MSG;
    js["id"] = current_user.getId();
    js["group_id"] = group_id;
    const string &buf = js.dump();

    int ret = send(fd, buf.c_str(), buf.size(), 0);
    if (ret == -1)
    {
        cout << "failed to send msg to server" << endl;
        return false;
    }
    return true;
}

// {"groupchat","群聊 -- groupchat:groupid:message"},
bool groupChat(int fd, const string &s)
{
    int idx = s.find(':');
    if (idx == string::npos)
    {
        cout << "type help to look through the instructions" << endl;
        return false;
    }
    string group_id = s.substr(0, idx);
    string msg = s.substr(idx + 1);

    json js;
    js["msg_id"] = GROUP_CHAT_MSG;
    js["group_id"] = atoi(group_id.c_str());
    js["id"] = current_user.getId();
    js["name"] = current_user.getName();
    js["msg"] = msg;
    string buf = js.dump();

    int ret = send(fd, buf.c_str(), buf.size(), 0);
    if (ret == -1)
    {
        cout << "failed to send msg to server" << endl;
        return false;
    }
    return true;
}

// user只需要输入friend id即可。其余的info如user_id msg_id由client程序自己填充给
bool addFriend(int fd, const string &msg)
{
    // {"msg_id":7,"from":"bbb","id":26,"friend_id":25}
    cout << endl;
    cout << "===================addFriend===================" << endl;
    cout << fd << " " << msg << endl;
    cout << endl;

    int friend_id = atoi(msg.c_str());
    json js;
    js["msg_id"] = ADD_FRIEND_MSG;
    js["id"] = current_user.getId();
    js["friend_id"] = friend_id;
    const string &buf = js.dump();

    int ret = send(fd, buf.c_str(), buf.size(), 0);
    if (ret == -1)
    {
        cout << "failed to send msg to server" << endl;
        return false;
    }
    return true;
}

bool chat(int fd, const string &s)
{
    cout << endl;
    cout << "===================chat===================" << endl;
    cout << fd << " " << s << endl;
    cout << endl;

    int idx = s.find(':');
    string to_id_str = s.substr(0, idx);
    string msg = s.substr(idx + 1);

    json js;
    js["msg_id"] = PTOP_CHAT_MSG;
    js["id"] = current_user.getId();
    js["name"] = current_user.getName();
    js["to_id"] = atoi(to_id_str.c_str());
    js["msg"] = msg;
    // js["time"] = getCurrentTime(); time在Server端加
    string buf = js.dump();

    int ret = send(fd, buf.c_str(), buf.size(), 0);
    if (ret == -1)
    {
        cout << "failed to send msg to server" << endl;
        return false;
    }

    return true;
}

//  简化处理了 这里client一直用的都是同一条长连接
//  正常应该一个账户用一个连接 loginout之后这个连接就应该断开
//  但是挺麻烦。还得重新socket，还得发信号kill掉当前fd的读线程。
//  就先让loginout不断开连接，指示让server删除它的登录信息。
//  之后登录的账号也都是用client的这一条连接。 
bool loginout(int fd, const string &)
{
    // // close(fd);
    // current_user_friendlist.clear();
    // current_user_grouplist.clear();
    // exit(0);
    json js;
    js["msg_id"] = LOGINOUT_MSG;
    js["id"] = current_user.getId();
    string buf = js.dump();
    int ret = send(fd,buf.c_str(),buf.size(),0);
    if (ret == -1)
    {
        cout << "failed to send msg to server" << endl;
        return false;
    }

    //  清空记录
    current_user_friendlist.clear();
    current_user_grouplist.clear();
    isOnLine = false;
    return true;
}

void help(int, const string &)
{
    printUsage();
}

void showMainMenu(int client_fd)
{
    printUsage();
    while (isOnLine)
    {
        //  从终端读取命令
        char buf[128] = {0};
        cin.getline(buf, 128);
        string command(buf);
        cout << "READ IN " << command << endl;
        //  解析命令 parse command (通过:)
        //  help / loginout
        int idx = command.find(':');
        if (idx == string::npos)
        {
            auto iter = commandMap.find(command);
            if (iter == commandMap.end())
            {
                printf("instruction error! Please type help and read Usage\n");
                continue;
            }
            iter->second(client_fd, "");
            continue;
        }
        //  others
        string instruction = command.substr(0, idx);
        string arg = command.substr(idx + 1);
        cout << "PARSE " << instruction << " " << arg << endl;
        auto iter = commandMap.find(instruction);
        if (iter == commandMap.end())
        {
            printf("instruction error! Please type help and read Usage\n");
            continue;
        }
        iter->second(client_fd, arg);
    }
}

string getCurrentTime()
{
}






//  接收消息线程函数
void recvTaskHandler(int client_fd)
{
    while (1)
    {
        char buf[1024] = {0};
        int ret = recv(client_fd, buf, 1024, 0);
        if (ret == -1)
        {
            cout << "failed to recv msg from server" << endl;
            continue;
        }
        else
        {
            json js = json::parse(buf);
            MsgType msg_ack = js["msg_id"].get<MsgType>();
            if (msg_ack == PTOP_CHAT_MSG_ACK)
            {
                cout << "=============PtoP Chat============" << endl;
                cout << "Time : " << js["time"].get<string>() << endl;
                cout << "From name : " << js["name"].get<string>() << endl;
                cout << "\tmsg : " << js["msg"].get<string>() << endl;
            }
            else if (msg_ack == GROUP_CHAT_MSG_ACK)
            {
                cout << "=============Group Chat============" << endl;
                cout << "Time : " << js["time"].get<string>() << endl;
                cout << "From group : " << js["group_id"].get<int>() << endl;
                cout << "\tFrom : " << js["id"].get<int>() << " " << js["name"] << endl;
                cout << "\t\tmsg : " << js["msg"].get<string>() << endl;
            }
            else
            {
                cout << "sth else happened" << endl;
            }
        }
    }
}

//  1. 好习惯 cin.get()
//  从缓冲区读了一个整数
//  接着要读掉缓冲区中的回车 cin.get();
//  因为我们键入时键入的是 int 和 一个 回车
//  如果我们不读取这个残留的回车，那么流中下一个如果想读的是字符串就读不到了，只能读到一个回车。

//   cin >> name不能读取空格 遇到空格就会结束读取
//  所以读用户名应当用 cin.getline() 遇到回车结束读取

//  好友列表、组列表其实应当存在客户端