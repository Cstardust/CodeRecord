#ifndef GROUP_H
#define GROUP_H

#include"common.h"
#include<vector>
#include<memory>
// CGroup row block col的类型

class CGroup
{
public:
    // friend class scene; 友元好像也没有必要
    void push_back(std::shared_ptr<point_feature> );
    bool check() const;
    bool isFull() const;
    // void show() const;
private:
    std::vector<std::shared_ptr<point_feature>> v;   //一组9格 无论row block 还是col都是这样
};

//  inline函数定义在头文件内
inline
void CGroup::push_back(std::shared_ptr<point_feature> mp)
{
    v.push_back(mp);
}

inline
bool CGroup::isFull() const
{
    assert(v.size()>=0&&v.size()<=9);
    return v.size()==9;
}
#endif