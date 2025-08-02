cmake_minimum_required(VERSION 3.31)

project(csv_reader)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

include_directories(include)

add_executable(csvreader src/main.cpp src/csv_parser.cpp src/evaluator.cpp)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    enable_testing()
    add_subdirectory(test)
endif()