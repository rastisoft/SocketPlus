cmake_minimum_required (VERSION 3.0)

project(socketplus)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
set(CMAKE_CXX_COMPILER g++)

include_directories(
        ${CMAKE_SOURCE_DIR}/SocketPlus/include
        )
        
file(GLOB_RECURSE SRC_FILES 
    ${CMAKE_SOURCE_DIR}/SocketPlus/src/*.cpp
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/examples/bin)
add_library(socketplus STATIC ${SRC_FILES})

add_executable(tcpserver ${CMAKE_SOURCE_DIR}/examples/tcp/server.cpp ${CMAKE_SOURCE_DIR}/examples/common/common.cpp)
target_link_libraries(tcpserver socketplus)

add_executable(tcpclient ${CMAKE_SOURCE_DIR}/examples/tcp/client.cpp ${CMAKE_SOURCE_DIR}/examples/common/common.cpp)
target_link_libraries(tcpclient socketplus)