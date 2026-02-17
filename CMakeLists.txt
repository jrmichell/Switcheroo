cmake_minimum_required(VERSION 4.2)
project(Switcheroo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/switcheroo.cpp

    src/converter.cpp
    src/converter.hpp

    src/menu.cpp
    src/menu.hpp
)
add_library(jsoncons INTERFACE)
target_include_directories(jsoncons INTERFACE external/jsoncons/include)

add_executable(switcheroo
               src/switcheroo.cpp
               src/converter.cpp
               src/menu.cpp
)

target_link_libraries(switcheroo PRIVATE jsoncons)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
