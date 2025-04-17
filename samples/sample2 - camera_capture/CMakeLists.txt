cmake_minimum_required(VERSION 3.10)
project(camera_capture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vizionsdk package
find_package(vizionsdk REQUIRED)

# Add source files
add_executable(camera_capture main.cpp)

# Link against VizionSDK
target_link_libraries(camera_capture PRIVATE vizionsdk::VizionSDK) 