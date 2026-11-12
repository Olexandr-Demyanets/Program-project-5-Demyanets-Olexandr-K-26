cmake_minimum_required(VERSION 3.20)
project(Project5FutureSchedule LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

find_package(Threads REQUIRED)

add_executable(work_demo
    src/main.cpp
    src/work.cpp)
target_link_libraries(work_demo PRIVATE Threads::Threads)

if(MINGW)
    target_link_options(work_demo PRIVATE -static)
endif()

if(MSVC)
    target_compile_options(work_demo PRIVATE /W4 /utf-8)
else()
    target_compile_options(work_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
