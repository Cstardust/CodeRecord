#ifndef COMMON_H
#define COMMON_H
#include<assert.h>
#include<utility>
#include<string>
#include<unistd.h>  //  check whether the file exists
static const unsigned int MAX_LEN = 9;
static const unsigned int MAX_SIZE = 81;
static std::string PATH = "../players/";
using std::pair;

//  check whether the file exists
inline 
bool isFileExists_access(std::string& name) {
    return (access(name.c_str(), F_OK ) == 0 );
}

//  状态枚举类型 每个数的状态 固定FIXED还是未固定ERASED
enum class State:unsigned int    //指定底层使用的数据类型 int
{
    ERASED = 0,
    INITIAL_FIXED = 1,
    USER_FIXED = 2
};

//  难度枚举类型 
enum class Difficulty:int
{
    EASY = 1,
    NORMAL,
    DIFFICULT,
};

template<class Enumeration>
typename std::underlying_type<Enumeration>::type as_integer (const Enumeration& re)
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(re);
}


typedef pair<unsigned int,unsigned int> coordinate;    //  格子坐标
typedef pair<int,int> mov;
typedef pair<int,State> point_feature; //格子特征 含有格子属性{value,State}
typedef pair<coordinate,point_feature> my_point;
#endif

