cmake_minimum_required(VERSION 3.31)
project(csvreader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_library(csv_lib
    src/csv_parser.cpp
    src/evaluator.cpp
)

target_include_directories(csv_lib PUBLIC include)

add_executable(csvreader src/main.cpp)
target_link_libraries(csvreader PRIVATE csv_lib)

add_executable(evaluator_test tests/evaluator_test.cpp)
target_link_libraries(evaluator_test PRIVATE csv_lib GTest::gtest_main)

add_executable(parser_test tests/test_csv_parser.cpp)
target_link_libraries(parser_test PRIVATE csv_lib GTest::gtest_main)

include(CTest)
include(GoogleTest)
gtest_discover_tests(evaluator_test)
gtest_discover_tests(parser_test)