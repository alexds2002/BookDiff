cmake_minimum_required(VERSION 3.16)
project(BookDiff)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Manualy add new files
set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
