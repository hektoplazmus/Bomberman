cmake_minimum_required(VERSION 3.3)
project(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    CGame.cpp
    CGame.hpp
    cli.cpp
    CMap.cpp
    CMap.hpp
    CObjects.cpp
    CObjects.hpp
    CPlayer.cpp
    CPlayer.hpp
    main.cpp
    srv.cpp)

add_executable(src ${SOURCE_FILES})