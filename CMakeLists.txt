cmake_minimum_required(VERSION 2.8)

# set the project name
project(Demo2)

include_directories(./include)
#include_directories(./lualib)

aux_source_directory(./src DIR_SRCS)
# aux_source_directory(./lualib LUA_SRCS)
# set(SRC_LIST ${DIR_SRCS} ${LUA_SRCS})

# add the executable
add_executable(Demo2 ${DIR_SRCS})

#link lib
target_link_libraries(Demo2 pthread)