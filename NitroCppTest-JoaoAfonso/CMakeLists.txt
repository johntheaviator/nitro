cmake_minimum_required(VERSION 3.8)
project(RectangleIntersectionFinder VERSION 1.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the executable and sources
add_executable(intersection_finder
    main.cpp
    Rectangle.cpp
    IntersectionFinder.cpp
)

# Include current directory for headers (Rectangle.h, IntersectionFinder.h, json.hpp)
target_include_directories(intersection_finder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})