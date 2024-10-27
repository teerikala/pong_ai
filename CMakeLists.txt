cmake_minimum_required(VERSION 3.5)

project(pong_ai VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)

add_executable(pong_ai gamewindow.hh
    state.hh main.cpp gamewindow.cpp state.cpp
    agent.hh agent.cpp
    filehandle.hh
    filehandle.cpp
    training.hh
    training.cpp

)

target_link_libraries(pong_ai Qt5::Widgets)
