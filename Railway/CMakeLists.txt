cmake_minimum_required(VERSION 3.14)
project(Railway)

set(CMAKE_CXX_STANDARD 14)

add_executable(Railway ServiceDesk.cpp railway_method.h other_method.h)