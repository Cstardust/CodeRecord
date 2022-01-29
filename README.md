# 简单控制台游戏--数独

## 编译指令
```bash
$ cd build
$ cmake ..
$ make
```
## 效果
- `./main -h`：帮助
```bash
shc@shc-virtual-machine:~/code/sudoku/build$ ./main -h

sudoku - a little game in command line

Usage:
         sudoku [-l <progressFile>]

Options:
         -l <path>       specify path of progress file to load, optional.

When playing:
         'q' to quit the game 
         'u' to the back 
         'o' to check whether you've won
         'p' to get the answer 
         and sth hasn't finished yet 
```

- 游戏
```bash
shc@shc-virtual-machine:~/code/sudoku/build$ ./main
设置难度：1简单 2普通 3困难
input your choice
1

╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃   ┃ 6 ┃   ┃ 7 ┃   ┃   ┃ 4 ┃ 9 ┃ 8 ┃
╋━^━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃ 7 ┃ 1 ┃   ┃ 4 ┃ 9 ┃ 8 ┃ 2 ┃   ┃   ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃   ┃   ┃   ┃ 2 ┃ 6 ┃   ┃ 7 ┃ 1 ┃ 3 ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃ 6 ┃ 5 ┃ 2 ┃ 1 ┃ 3 ┃ 7 ┃ 9 ┃   ┃ 4 ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃ 1 ┃ 3 ┃ 7 ┃ 9 ┃ 8 ┃ 4 ┃ 6 ┃   ┃ 2 ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃ 9 ┃ 8 ┃ 4 ┃ 6 ┃ 5 ┃ 2 ┃ 1 ┃ 3 ┃ 7 ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃ 5 ┃ 2 ┃ 6 ┃ 3 ┃ 7 ┃ 1 ┃   ┃ 4 ┃   ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃ 3 ┃ 7 ┃   ┃ 8 ┃   ┃ 9 ┃ 5 ┃ 2 ┃   ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
┃ 8 ┃   ┃ 9 ┃   ┃ 2 ┃ 6 ┃ 3 ┃ 7 ┃ 1 ┃
╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋
do you want to quit? (y/n) 
y
do you want to save? (y/n) 
n
```
- 玩家存档保存在`player`文件夹下

## 收获
- 熟悉c++语法。
- 复习git
- 详见博客
