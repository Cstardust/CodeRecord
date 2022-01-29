//    在内联函数较多的情况下，为了避免头文件过长、版面混乱，可以将所有的内联函数定义移到一个单独的文件中去，然后再 用#include指令将它包含到类声明的后面（类的头文件的底部）。这样的文件称为一个内联函数定义文件。
//    inl文件中也可以包含头文件的，因为内联函数中可能包含其他文件中定义的东西。
//    按照惯例， 应该将这个文件命名为“filename.inl”，其中“filename”与相应的头文件和实现文件相同。
#ifndef MYUTILITY_INL
#define MYUTILITY_INL

#include<cstdlib>
#include<ctime>

inline
int random(int l,int r) //返回随机数[l+1,r]之间
{
    srand(time(NULL));
    return rand()%(r-l+1) + l;
}



#endif