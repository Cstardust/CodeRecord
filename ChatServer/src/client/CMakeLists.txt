

aux_source_directory(./ SRC_LIST)       # 3. 源文件

add_executable(ChatClient ${SRC_LIST})  # 1. 生成可执行文件

target_link_libraries(ChatClient pthread)   # 6. 链接库