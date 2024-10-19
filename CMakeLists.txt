cmake_minimum_required(VERSION 3.16)
project(BookDiff)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-O3)

# Manualy add new files
set(SOURCES main.cpp
            PricePointBook/OrderBook.cpp
            PricePointBook/OrderBook.h
            PricePointBook/PriceBook.cpp
            PricePointBook/PriceBook.h
            Include/json.hpp
            Include/benchmark_component.h)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
