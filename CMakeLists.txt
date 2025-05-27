cmake_minimum_required(VERSION 3.15)
project(gehu VERSION 0.1.0 LANGUAGES C CXX)

# Enable exceptions globally
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fexceptions")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fexceptions")

# Remove -fno-exceptions if present
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM definitions and include directories
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# Add source files from src directory
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
)

# Add header files from src directory
set(HEADERS
    src/lexer.hpp
    src/parser.hpp
    src/semantic_analyzer.hpp
    src/codegen.hpp
)

# Create executable
add_executable(gehu ${SOURCES} ${HEADERS})

# Link against LLVM libraries
target_link_libraries(gehu
    ${LLVM_AVAILABLE_LIBS}
)

# Add include directories
target_include_directories(gehu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Enable warnings
if(MSVC)
    target_compile_options(gehu PRIVATE /W4)
else()
    target_compile_options(gehu PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Force exceptions for the gehu target
target_compile_options(gehu PRIVATE -fexceptions)

# Ensure -fexceptions is set after any other flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fexceptions")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fexceptions")

# Remove -fno-exceptions if present
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

# Install
install(TARGETS gehu DESTINATION bin)
install(FILES lib/std.gehu DESTINATION lib/gehu)

# Testing
enable_testing()
add_subdirectory(test)

# LLVM Optimization Passes
function(add_llvm_optimization_pass target pass_name)
    target_compile_definitions(${target} PRIVATE
        LLVM_ENABLE_OPTIMIZATION_${pass_name}=1
    )
endfunction()

# Add optimization passes
add_llvm_optimization_pass(gehu CONSTANT_FOLDING)
add_llvm_optimization_pass(gehu DEAD_CODE_ELIMINATION)
add_llvm_optimization_pass(gehu STRENGTH_REDUCTION)

# Create shared library
add_library(gehu_lib SHARED ${SOURCES} ${HEADERS})

# Link against LLVM libraries
target_link_libraries(gehu_lib
    ${LLVM_AVAILABLE_LIBS}
)

# Add include directories
target_include_directories(gehu_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Enable warnings
if(MSVC)
    target_compile_options(gehu_lib PRIVATE /W4)
else()
    target_compile_options(gehu_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Force exceptions for the gehu_lib target
target_compile_options(gehu_lib PRIVATE -fexceptions)

# Ensure -fexceptions is set after any other flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fexceptions")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fexceptions")

# Remove -fno-exceptions if present
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}") 