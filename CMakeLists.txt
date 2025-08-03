cmake_minimum_required(VERSION 3.31)
project(csvreader)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(GTest REQUIRED)
include(CTest)
include(GoogleTest)

add_executable(csvreader
    src/main.cpp
    src/csv_parser.cpp
    src/evaluator.cpp
)

add_executable(evaluator_test
    tests/evaluator_test.cpp
    src/csv_parser.cpp
    src/evaluator.cpp
)
target_link_libraries(evaluator_test GTest::gtest_main)
gtest_discover_tests(evaluator_test)

add_executable(parser_test
    tests/test_csv_parser.cpp
    src/csv_parser.cpp
)
target_link_libraries(parser_test GTest::gtest_main)
gtest_discover_tests(parser_test)
