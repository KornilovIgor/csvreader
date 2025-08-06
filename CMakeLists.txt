cmake_minimum_required(VERSION 3.31)
project(csvreader)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(GTest REQUIRED)
include(CTest)
include(GoogleTest)

add_executable(csvreader
    src/main.cpp
    src/table.cpp
)

add_executable(table_test
    tests/table_test.cpp
    src/table.cpp
)
target_link_libraries(table_test GTest::gtest_main)
gtest_discover_tests(table_test)

find_program(CPPCHECK cppcheck)
if (CPPCHECK)
    set(SOURCE_DIRS 
        ${PROJECT_SOURCE_DIR}/src 
        ${PROJECT_SOURCE_DIR}/include
    )

    add_custom_target(
        cppcheck
        COMMAND ${CPPCHECK}
            --enable=all
            --std=c++17
            --inconclusive
            --suppress=missingIncludeSystem
            -I${PROJECT_SOURCE_DIR}/include
            ${SOURCE_DIRS}
        COMMENT "Running cppcheck static analysis"
    )
else()
    message(WARNING "cppcheck not found! Install it to enable static analysis.")
endif()