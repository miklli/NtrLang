cmake_minimum_required(VERSION 3.15)

project(ntr)

# Включаем стандарт C++17, чтобы работал std::variant и вложенные namespace
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ntr 
    main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/utils.cpp
    src/compiler.cpp
)

target_include_directories(ntr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
