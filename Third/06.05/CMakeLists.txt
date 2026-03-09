cmake_minimum_required(VERSION 3.10)
project(Loader)

find_package(Boost REQUIRED COMPONENTS dll system)

add_library(test1 SHARED lib1.cpp)
add_library(test2 SHARED lib2.cpp)

add_executable(loader main.cpp)
target_link_libraries(loader PRIVATE Boost::dll Boost::system)

# Help executable find libraries in build directory
set_target_properties(loader PROPERTIES
    BUILD_RPATH "${CMAKE_CURRENT_BINARY_DIR}")
