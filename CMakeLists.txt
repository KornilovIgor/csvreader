cmake_minimum_required(VERSION 3.14)
project(csvreader)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(tests)

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
