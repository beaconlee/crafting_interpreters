cmake_minimum_required(VERSION 3.20)
project(CraftingInterpreters LANGUAGES CXX)


# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 调试和编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE Debug)


add_subdirectory(src/frontend)
add_subdirectory(src/interpreter)