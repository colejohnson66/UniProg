cmake_minimum_required(VERSION 3.14)
project(upro
    VERSION 0.1.0
    DESCRIPTION "Companion software for UniProg"
)
configure_file(configure.h.in configure.h)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)


add_executable(upro
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/getopt.c"
)

target_include_directories(
    upro PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
)