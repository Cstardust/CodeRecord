#ifndef _GROUPUSER_H_
#define _GROUPUSER_H_


#include<string>
#include"User.hpp"

using std::string;

//  为啥没有group_id？
class GroupUser:public User
{
public:
    void setRole(string role){role_ = role;}
    string getRole() const {return role_;}
private:    
    string role_;
};


#endif