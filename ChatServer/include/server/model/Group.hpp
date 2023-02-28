#ifndef _GROUP_H_
#define _GROUP_H_

#include <string>
#include <vector>
#include "GroupUser.h"

using std::string;
using std::vector;

//  对于一个Group 其所具有的属性
class Group
{
public:
    Group(int group_id = -1, const string &group_name = "", const string &group_desc = "")
        : group_id_(group_id), group_name_(group_name), group_desc_(group_desc) {}

    void setId(int id) { group_id_ = id; }
    void setName(string name) { group_name_ = name; }
    void setDesc(string desc) { group_desc_ = desc; }

    int getId() const { return group_id_; }
    string getName() const { return group_name_; }
    string getDesc() const { return group_desc_; }
    vector<GroupUser> &getUsers() { return users; }

private:
    int group_id_;
    string group_name_;
    string group_desc_;
    vector<GroupUser> users;
};

#endif
