
PROJECT(server_version)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 14)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
SET( CMAKE_CXX_FLAGS "-std=c++11 -O3")


#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")
set(SRCS 

${CMAKE_CURRENT_SOURCE_DIR}/src/menu_config/menu_config.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/socket_server/HttpServer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/funs/funs.cpp
)


#exe
add_executable(server_version ${SRCS})
target_link_libraries(server_version pthread jsoncpp)