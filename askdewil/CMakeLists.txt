cmake_minimum_required(VERSION 3.3)
project(askdewil)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcpprest")

include_directories(include)
set(SOURCE_FILES main.cpp src/NetworkLayer.cpp src/ProtocolLayer.cpp src/BusinessLogic.cpp)
add_executable(askdewil ${SOURCE_FILES})