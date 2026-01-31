cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(onon_soft_tracer LANGUAGES CXX)

add_subdirectory(deps)
add_subdirectory(src)
add_subdirectory(examples)
