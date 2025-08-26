cmake_minimum_required(VERSION 4.1.0)

project(ntr)

add_executable(ntr 
    main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/env.cpp
    src/interpreter.cpp
    src/utils.cpp
)

target_include_directories(ntr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)