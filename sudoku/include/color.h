#ifndef COLOR_H
#define COLOR_H
#include<iostream>
using std::ostream;

enum class Color:int
{
    BOLD = 1,
    RESET = 0,
    BG_BLUE = 44,
    BG_DEFAULT = 49,
    BG_GREEN = 42,
    BG_RED = 41,
    FG_BLACK = 30,
    FG_BLUE = 34,
    FG_CYAN = 36,
    FG_DARK_GRAY = 90,
    FG_DEFAULT = 39,
    FG_GREEN = 32,
    FG_LIGHT_BLUE = 94,
    FG_LIGHT_CYAN = 96,
    FG_LIGHT_GRAY = 37,
    FG_LIGHT_GREEN = 92,
    FG_LIGHT_MAGENTA = 95,
    FG_LIGHT_RED = 91,
    FG_LIGHT_YELLOW = 93,
    FG_MAGENTA = 35,
    FG_RED = 31,
    FG_WHITE = 97,
    FG_YELLOW = 33,
};

// 输出修饰类 功能：1. 负责改变输出流颜色
class Modifier
{
friend ostream& operator<<(ostream& os,const Modifier&);    //  由于重载的<<()函数需要访问内部数据，所以声明为友元函数
public:
    Modifier(const Color &t):c(t){} 
private:
    Color c;
};

#endif