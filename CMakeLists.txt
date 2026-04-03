cmake_minimum_required(VERSION 3.20)
project(untitled10)

set(CMAKE_CXX_STANDARD 17)

# The WIN32 keyword here is what kills the CMD window at the OS level
add_executable(untitled10 WIN32 main.cpp)

# If using MinGW, this flag is a safety net to ensure no console is linked
if(MINGW)
    target_link_options(untitled10 PRIVATE "-mwindows")
endif()