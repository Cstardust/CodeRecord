#include<iostream>
#include "scene.h"
#include"common.h"

static void printHelp()
{
    std::cout << std::endl;
    std::cout << "sudoku - a little game in command line" << std::endl
              << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t sudoku [-l <progressFile>]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t -l <path> \t specify path of progress file to load, optional." << std::endl
              << std::endl;
    std::cout << "When playing:" 
        << std::endl << "\t 'q' to quit the game " 
        << std::endl << "\t 'u' to the back "
        << std::endl << "\t 'o' to check whether you've won"
        << std::endl << "\t 'p' to get the answer "
        << std::endl << "\t and sth hasn't finished yet "
        << std::endl << std::endl;
}

// 处理输入
static int getChoice()
{
    int grids_to_erase = 0;
    while(true)
    {
        std::cout<<"设置难度：1简单 2普通 3困难"<<std::endl;
        std::cout<<"input your choice"<<std::endl;
        char ch;
        int x = 0;
        std::cin>>ch;
        if(isdigit(ch)) x = ch-'0';
        else{
            std::cout<<"input error"<<std::endl;
            continue;
        }  
        std::cout<<x<<std::endl;
        // Difficulty level = static_cast<Difficulty>(ch);
        Difficulty level = Difficulty(x);  //  将int转化成Difficulty
        switch(level)
        {
            case Difficulty::EASY:
                grids_to_erase = 20;
                break;
            case Difficulty::NORMAL:
                grids_to_erase = 35;
                break;
            case Difficulty::DIFFICULT:
                grids_to_erase = 45;
                break;
            default:
                std::cout<<"input error"<<std::endl;
        }
        if(grids_to_erase) break;
    }
    return grids_to_erase;
}

int main(int argc,char *argv[]) //argv[0]默认为可执行文件路径
{
    if (argc==1)
    {   
        CScene s;   //init 生成场景背后的容器
        //生成数据
        int num = getChoice();
        s.generate();
        s.erasePane(num);
        s.start();  //开始显示画面
    }
    else if(argc==2)    // use space to gekai argv
    {
        printHelp();
    }
    else if(argc==3)
    {
        std::string file = argv[2];
        file = PATH+file;
        if(!isFileExists_access(file)) 
        {
            std::cout<<"the file doesn't exists"<<std::endl;
            return 0;
        }
        // copy constructor 
        CScene s(file);
        s.start();
    }
    
    // getchar();  //stop for a moment
}