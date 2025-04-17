cmake_minimum_required(VERSION 3.10)
project(camera_control)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vizionsdk package
find_package(vizionsdk REQUIRED)

# Add source files
add_executable(camera_control main.cpp)

# Link against VizionSDK
target_link_libraries(camera_control PRIVATE vizionsdk::VizionSDK) 