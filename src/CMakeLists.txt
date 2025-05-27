cmake_minimum_required(VERSION 3.10)
project(gehu_standalone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

add_executable(gehu_standalone
    main.cpp
    lexer.cpp
    parser.cpp
    codegen.cpp
)

target_include_directories(gehu_standalone PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(gehu_standalone PRIVATE -Wall -Wextra -Wpedantic -fexceptions) 