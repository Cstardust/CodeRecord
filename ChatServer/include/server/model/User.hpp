#ifndef _USER_HPP_
#define _USER_HPP_

#include<string>

using std::string;

//  User表的ORM类
class User{
public:
    User(int id = -1,
        string name="",
        string pwd="",
        string state="offline"):id_(id),name_(name),pwd_(pwd),state_(state){}    
    
    int getId() const{
        return id_;
    }

    string getName() const{
        return name_;
    }

    string getPwd() const{
        return pwd_;
    }

    string getState() const{
        return state_;
    }

    void setId(int id){
        id_ = id;
    }

    void setName(string name){
        name_ = name;
    }
    void setPwd(string pwd){
        pwd_ = pwd;
    }
    void setState(string state){
        state_ = state;
    }
private:
    int id_;            //  primary key 、auto_increment
    string name_;       //  not null 、 unique
    string pwd_;        //  not null
    string state_;      //  online / offline(default)
};

#endif