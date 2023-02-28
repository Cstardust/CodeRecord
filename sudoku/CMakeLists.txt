# CMake最小版本要求为3.0
cmake_minimum_required(VERSION 3.0)

project(SUDOKU)

# g++ complie paramater
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11" )

# attached with tiaoshi information
set(CMAKE_BUILD_TYPE Debug)

# 将/usr/include/myincludefolder 和 ./include 添加到头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main main.cpp src/color.cpp src/group.cpp src/scene.cpp)

