find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(table_test
    table_test.cpp
    ../src/table.cpp
)

target_include_directories(table_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(table_test GTest::gtest_main)

gtest_discover_tests(table_test)
