#include"color.h"
#include"common.h"
ostream& operator<<(ostream& os,const Modifier& md)
{
    // cout  << "\033["<<34<<"m" ;  // static_cast<int>()
    return os << "\033[" << as_integer(md.c) << "m";  // 颜色控制开启 写入输出流
}