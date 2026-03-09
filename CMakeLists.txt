cmake_minimum_required(VERSION 4.2)
project(Switcheroo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(jsoncons INTERFACE)
target_include_directories(jsoncons INTERFACE external/jsoncons/include)

add_executable(switcheroo
    src/switcheroo.cpp
    src/main_window.cpp
    src/converter.cpp
    src/validator.cpp
)

set_target_properties(switcheroo PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(switcheroo PRIVATE jsoncons Qt6::Widgets)

if (MSVC)
    target_compile_options(switcheroo PRIVATE /W4)
else()
    target_compile_options(switcheroo PRIVATE -Wall -Wextra -Wpedantic)
endif()
