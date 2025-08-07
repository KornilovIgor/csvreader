add_executable(csvreader
    main.cpp
    table.cpp
)

target_include_directories(csvreader PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
