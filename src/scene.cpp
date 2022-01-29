#include"scene.h"
#include"common.h"
#include"mygetch.h"
#include"myutility.inl"
#include<cstring>
#include<unordered_map>
#include<fstream>
#include<sstream>
#include<string>
#include<stdexcept>
using std::make_pair;

// Effective中 提到过，尽量不把构造函数作为inline函数 因为可能有继承关系？
// inline 
CScene::CScene():_map(MAX_SIZE),_block(MAX_LEN),_row(MAX_LEN),_col(MAX_LEN),cur_point(coordinate(0,0)),mode_initial(Color::FG_BLUE),mode_user(Color::FG_RED),normal(Color::RESET)
{
    init();
}

//  i think that jiaohu in the constructor isn't good. it will casue exception
CScene::CScene(const std::string& file):_map(MAX_SIZE),_block(MAX_LEN),_row(MAX_LEN),_col(MAX_LEN),mode_initial(Color::FG_BLUE),mode_user(Color::FG_RED),normal(Color::RESET)
{
    init(file);// read the archive(r kai v)
}


// read the archive(r kai v)
void CScene::init(const std::string& file)
{
    std::ifstream ifs(file);
    std::string line;
    unsigned int _map_idx = 0;
    bool flag = false;
    while(getline(ifs,line))
    {
        std::istringstream ss(line);    //  !!!!! attached the iss to the line we read just now
        if(!flag)   // read the cur_point
        {
            unsigned int x,y;
            ss>>x>>y;
            assert(x>=0&&x<9&&y>=0&&y<9);
            setCurPoint(x,y);
            flag = true;
            continue;
        }

        if(_map_idx<MAX_SIZE)   //read the _map
        {
            unsigned int para[2];
            int t = 0;
            while(t<2&&ss>>para[t]&&_map_idx<MAX_SIZE)    //sstringstream transform str to int or others
                ++t;
            _map[_map_idx] = point_feature(para[0],State(para[1]));
            ++_map_idx;
        }
        else //read the ccommand
        {
            unsigned int para[4];    //use the array seems better than the before
            int idx = 0;    
            while(ss>>para[idx]&&idx<4)
                ++idx;
            com.push_back(my_point(coordinate(para[0],para[1]),point_feature(para[2],State(para[3]))));
        }
    }
    ifs.close();
    return ;
}


//  数据初始化，格子装入map, row col组的指针指向map中的格子
void CScene::init()
{
    int n = MAX_LEN;
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<n;++j)
        {                           // 坐标         状态
            _map[i*9+j] = point_feature(0,State(0));  //  初始化坐标 
        }
    }
    // 用new吗？？回头再试试。
    
    init_part();    //  hhhh mingzijuele

}
//vector与map，set不同 map，set当不存在时 使用[]代表创建
//而vector则会越界



void CScene::init_part()
{
    //  添加_row _col
    for(unsigned int i=0;i!=MAX_LEN;++i)
    {
        for(unsigned int j=0;j!=MAX_LEN;++j)
        {
            _row[i].push_back(std::make_shared<point_feature>(_map[i*9+j]));  //row 行的每组指针都指向该指向的位置
            _col[j].push_back(std::make_shared<point_feature>(_map[i*9+j]));  //col 列的每组指针都指向该指向的位置  
        }
    }
    
    // 添加block
    for(unsigned int k=0;k!=MAX_LEN;++k)    //  第几个小九宫格
    {
        for(int i=0;i<3;++i)
        {
            for(int j=0;j<3;++j)
            {
                _block[k].push_back(std::make_shared<point_feature>(_map[k/3*27+i*9+j+k%3*3]));
            }
        }
    }
}


//  数独生成算法，先借用一下，流程写完再优化。
//  初始化随机数
void CScene::generate()
{
        // XXX: pseudo random
    static char map_pattern[10][10] = {
        "ighcabfde",
        "cabfdeigh",
        "fdeighcab",
        "ghiabcdef",
        "abcdefghi",
        "defghiabc",
        "higbcaefd",
        "bcaefdhig",
        "efdhigbca"};

    std::vector<char> v = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};

    // 产生字母到数字的随机映射
    std::unordered_map<char, unsigned int> hash_map;
    for (int i = 1; i <= 9; ++i)
    {
        int r = random(0, v.size() - 1); //v.size()=9 生成随机数 r∈[0,8]。作为v的下标去选取hash_map的关键字
        hash_map[v[r]] = i;     //确定字符[a,i]和随机数[1,9]的映射关系
        v.erase(v.begin() + r);     //干什么用的？？ 没用啊xd！！！？
    }

    // 填入场景
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {                                   // generate时 生成的格子的状态其中State置为FIXED
            setValueState(row,col,hash_map[map_pattern[row][col]],State(1));
        }
    }
#if 0
    // debug
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            std::cout<<_map[i*9+j].second.first<<" ";
        }
        std::cout<<std::endl;
    }
#endif
}


//  抹去num个格子的数据，使其由fixed变为erased（即可填充）
//  ok
void CScene::erasePane(int num)
{
    while(num--)
    {
        int row = rand()%MAX_LEN; //[0,8]
        int col = rand()%MAX_LEN; //[0,8]
        if(_map[row*9+col].second==State::INITIAL_FIXED) //如果状态为fixed，即可以抹去（还未抹去过）
            setValueState(row,col,0,State::ERASED);  
        else 
            ++num;  //否则本次失败，下次继续。
        
        assert(col>=0&&col<9);
        assert(row>=0&&row<9);
    }
#if 0
    // debug
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            // _map[row*9+col] = my_point(coordinate(row,col),point_feature(val,s));
            std::cout<<_map[i*9+j].second.first<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl<<std::endl;
#endif
}

//  控制整个游戏流程中的界面。
void CScene::start()
{
    show();
    char ch;
    std::unordered_map<char,mov> m={ {'a',mov(0,-1)},{'w',mov(-1,0)},{'s',mov(1,0)},{'d',mov(0,1)} };
    while(true)
    {
        ch = getch();
        if(isdigit(ch)) // 填数字
        {   
            int val = ch-'0';
            my_point mp = my_point(cur_point,getCurValueState());
            if(!setCurValueState(val,State::USER_FIXED))  
            {
                std::cout<<"the number can't be changed"<<std::endl;  //  it will be clear
            }
            else  com.push_back( mp );
        }
        else if(m.find(ch)!=m.end())    //改变光标
        {
            mov loc =  m[ch];
            unsigned int x = cur_point.first+loc.first;
            unsigned int y = cur_point.second+loc.second;
            if(x>=0&&x<MAX_LEN&&y>=0&&y<MAX_LEN) 
            {
                com.push_back( my_point(cur_point,getCurValueState()));
                cur_point.first=x,cur_point.second=y;
            }
        }
        else if(ch=='q'||ch=='Q')   // 退出动作 ok
        {
            std::cout<<"do you want to quit? (y/n) "<<std::endl;
            char choice;
            std::cin>>choice;
            if(choice!='y'&&choice!='Y') continue;
            std::cout<<"do you want to save? (y/n) "<<std::endl;
            std::cin>>choice;
            if(choice=='y'||choice=='Y') this->save();   //保留场景数据  功能未添加
            break;
        }
        else if(ch=='u')    //  撤销动作 ok
        {
            if(com.empty()) continue;
            my_point mp = com.back();
            com.pop_back();
            cur_point = make_pair(mp.first.first,mp.first.second);  //  set the cur point
            setCurValueState(mp.second.first,mp.second.second);
        }
        else if(ch=='o')    //  check whether the player has won.
        {
            bool flag = this->check();
            if(flag)   
                std::cout<<"Victory"<<std::endl;
            else
                std::cout<<"You hasn't won"<<std::endl;
            std::cout<<"input the enter to continue"<<std::endl;
            getchar();
            if(flag) break;
        }
        else if(ch=='p')    // rapid cleared by the customs
        {
            std::cout<<"the function hasn't been finished"<<std::endl;
        }
        show();   //更新过后,重新画一遍 
    }
    return ;
}

// 打印一行 
void CScene::show()
{
    std::cout<<"show"<<std::endl;
    system("clear");  //  清屏
    printLine(-1);  //  最上面一行
    for(unsigned int row = 0;row!=MAX_LEN;++row)
    {
        printNum(row);      // 一行数字
        printLine(row);     // 一行线 
    }
}


// ╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━^━╋━━━╋
void CScene::printLine(unsigned int row) const
{
    for(unsigned int col=0;col!=MAX_LEN;++col)
    {
        std::cout<<"\u254B"<<"\u2501";
        std::cout<< ((row==cur_point.first&&col==cur_point.second) ? "^" : "\u2501");  //打印╋━^━  或者╋━━━
        std::cout<<"\u2501";
    }
    std::cout<<"\u254B"<<std::endl; //收尾转到下一行
   
}


//  打印一行，如果值为0，则不打印。
void CScene::printNum(unsigned int row) const
{
    for(unsigned int col=0;col<MAX_LEN;++col)
    {
        assert(col>=0&&col<9);
        assert(row>=0&&row<9);
        std::cout<<"\u2503"; // 打印|
        point_feature pf = getValueState(row,col);
        State st = pf.second;
        int val = pf.first;
        if(st==State::ERASED)    // 值为0 则显示空格
            std::cout<<"   ";
        else if(st==State::INITIAL_FIXED)   //<< "\033["<<34<<"m"<<"
            std::cout  << mode_initial << " " << val << " " << normal;// 颜色开启 数字 颜色关闭
            // std::cout<< "\033["<<34<<"m"<<" "<<val<<" "<<"\033[0m";    
        else if(st==State::USER_FIXED)
            std::cout << mode_user << " " << val << " " << normal;
    }
    std::cout<<"\u2503"<<std::endl;
}

//  设置格子的所有状态
void CScene::setValueState(unsigned int row,unsigned int col,unsigned int val,State s)
{
    _map[row*9+col] = point_feature(val,s);
}

//  set the value and state of cur_point
bool CScene::setCurValueState(unsigned int val,State s)
{
    if(getCurValueState().second==State::INITIAL_FIXED) return false;
    int row = cur_point.first;
    int col = cur_point.second;
    _map[row*9+col] = point_feature(val,s);
    return true;
}


//  改变指定坐标的值
// inline
void CScene::setValue(unsigned int row,unsigned int col,unsigned int val)
{
    _map[row*9+col].first = val;    
}

void CScene::save()
{
    std::cout<<"input the name of the file you'd like to reference to"<<std::endl;
    std::string file;
    std::cin>>file;
    file = PATH + file; 
    std::ofstream ofs;
    try{
        ofs.open(file);
        ofs<<cur_point.first<<" "<<cur_point.second<<std::endl; //  save the current point
   
        for(const auto& item:_map)  // save the information of the scene
        {
            ofs<<item.first<<" "<<as_integer(item.second)<<std::endl;   // as_integer zhuanhuan meijuleiixing template
        }
        
        for(const my_point& mp:com) //  save the command the player has executed 
        {
            coordinate cd = mp.first;
            point_feature pf = mp.second;
            ofs<<cd.first<<" "<<cd.second<<" "<<pf.first<<" "<<as_integer(pf.second)<<std::endl;
        }
        
    }catch(std::exception e){
        ofs.close();
        std::cout<<e.what()<<std::endl;
    }
    ofs.close();
    return ;
}

bool CScene::check() const
{
    for(const CGroup& item : _block)
        if(!item.check()) return false;
    for(const CGroup& item : _col )
        if(!item.check()) return false;
    for(const CGroup& item : _row)
        if(!item.check()) return false;
    return true; 
}
