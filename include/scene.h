#ifndef SCENE_H
#define SCENE_H
#include <iostream>
#include <vector>
#include "group.h"
#include "common.h"
#include "color.h"
#include<fstream>
using std::vector;
// 类的命名习惯；类名前面加一个大写的C 并且类名首字母大写
// 函数命名习惯：驼峰命名法
// CScene 掌管游戏所有后部数据，控制游戏进程,画游戏截面。
class CScene
{
    //group 存的是point* 指向_map中的格子，作为判断作用
public:
    typedef vector<my_point> CCommand;
    CScene();
    CScene(const std::string &);
    void generate();            //  随机数生成          借用
    void erasePane(int num=10); //  去除格子             √   
    void start();               //  开始游戏 控制整个游戏界面流程   ×
    point_feature getValueState(unsigned int,unsigned int) const;
    point_feature getCurValueState() const;
    void save();                //  存档                 ×
    bool check() const;
    void revoke();

private:
    //CScene 的数据处理功能
    void init();                //  初始化所有数据          √
    void init(const std::string &);  //  du dang
    void init_part();           //  gonggong part
    void setValueState(unsigned int row,unsigned int col,unsigned int val,State s);    // √ 待改正
    bool setCurValueState(unsigned int val,State s);    // √ 待改正
    void setValue(unsigned int row,unsigned int col,unsigned int val);     //  改变指定坐标的值 ok 
    bool setValue(unsigned int val);     //  改变当前光标所在位置的值  // √
    void setCurPoint(unsigned int x,unsigned int y);    // ok
    //CScene 的界面打印功能
    void show();                //  展示界面    √
    void printLine(unsigned int) const ;   //  打印一行边界    √
    void printNum(unsigned int) const ;    //  打印一行带数字的    √
    
    // CScene has a CGroup  relationship: compoisition
    vector<point_feature> _map; //存储所有格子
    vector<CGroup> _block;      //  9个3*3方阵
    vector<CGroup> _row;        //  每一行
    vector<CGroup> _col;        //  每一列
    CCommand com;       //  the state in the past
    coordinate cur_point;       //  当前坐标
    Modifier mode_initial,mode_user,normal;       // 打印字体的颜色
};



//  改变当前光标所在位置的值
inline 
bool CScene::setValue(unsigned int val)
{
    if(_map[cur_point.first*9+cur_point.second].second==State::INITIAL_FIXED) return false;
    _map[cur_point.first*9+cur_point.second].first = val;
    return true;
}

//  返回the cur_point的{value State}
inline 
point_feature CScene::getCurValueState() const
{
    return _map[cur_point.first*9+cur_point.second];
}


//  return the specific point's {value State} 
inline 
point_feature CScene::getValueState(unsigned int row,unsigned int col) const
{
    return _map[row*9+col];
}

inline 
void CScene::setCurPoint(unsigned int x,unsigned int y)
{
    cur_point = coordinate(x,y);
}
# endif
