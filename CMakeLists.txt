# Specify the minimum CMAKE version required
cmake_minimum_required(VERSION 3.5.0)
project(RandomWalk)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories("/opt/homebrew/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Source related variables
set(MAIN_SRC src/main.cpp)
add_executable(main ${MAIN_SRC})
set_property(TARGET main PROPERTY CXX_STANDARD 11)
