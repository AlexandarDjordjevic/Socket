cmake_minimum_required(VERSION 3.8)

set(this Socket)

project(${this})

set(CMAKE_CXX_STANDARD 11)

set(Src 
    src/Socket.cpp
)

set(Include 
    include/Network/Socket.hpp
)

add_library(${this} STATIC ${Src} ${Include})

set_target_properties(${this} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${this} PUBLIC include)
