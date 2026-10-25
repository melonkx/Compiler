cmake_minimum_required(VERSION 3.20)
project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Front/Lexing)
add_subdirectory(Front/Parsing)


add_executable(Compiler
    main.cpp    
)

target_link_libraries(Compiler
    PRIVATE
        Parsing
)
