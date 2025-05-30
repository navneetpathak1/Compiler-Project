cmake_minimum_required(VERSION 3.10)
project(GehuCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Remove -fno-exceptions if set by LLVM or system
string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# Ensure -fexceptions is set for all C++ files
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

# Add debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(gehu
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/semantic_analyzer.cpp
    src/codegen.cpp
)

target_compile_options(gehu PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
set_target_properties(gehu PROPERTIES COMPILE_FLAGS "-fexceptions")

target_link_libraries(gehu
    LLVM
    LLVMCore
    LLVMExecutionEngine
    LLVMOrcJIT
    LLVMSupport
    LLVMX86CodeGen
) 