cmake_minimum_required(VERSION 3.10)
project(gehu_test)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add test executable
add_executable(gehu_test test.cpp)

# Set compiler options
target_compile_options(gehu_test PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

# Link with required libraries
target_link_libraries(gehu_test PRIVATE
    gehu_lib
    ${llvm_libs}
    pthread
)

# Set include directories
target_include_directories(gehu_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${LLVM_INCLUDE_DIRS}
)

# Add test to CTest
add_test(NAME gehu_test COMMAND gehu_test) 