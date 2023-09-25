cmake_minimum_required(VERSION 3.20)
project(twothreadmutex)

find_package(fmt)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE fmt::fmt)
