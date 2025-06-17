cmake_minimum_required(VERSION 3.14)

# Declare a project for tests
project(Tests LANGUAGES CXX)

include(FetchContent)

# Fetch Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4 # Use latest stable or specific version
)

FetchContent_MakeAvailable(Catch2)

# Add test sources
add_executable(unit_tests
  test_rectangle.cpp
  test_intersections.cpp
  test_helpers.cpp
  ../Rectangle.cpp
  ../IntersectionFinder.cpp
)

# Link to the main project source and Catch2
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

# Include paths
target_include_directories(unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Enable CTest if using `ctest`
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
