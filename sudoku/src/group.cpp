#include"group.h"
#include"common.h"
#include<unordered_set>


//  检验本组是否符合要求
bool CGroup::check() const
{
    if(v.size()<9) return false;    // 如果本组还没填满 那么一定不行
    std::unordered_set<int> s;
    for(const std::shared_ptr<point_feature>& m:v)
    {
        int val = m->first;
        if(s.find(val)!=s.end()) return false;  //如果之前在该组已经存在 那么返回false
        s.insert(val);
    }
    return true;
}
