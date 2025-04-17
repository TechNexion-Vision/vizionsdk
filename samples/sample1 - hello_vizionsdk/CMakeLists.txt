cmake_minimum_required(VERSION 3.10)
project(hello_vizionsdk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vizionsdk package
find_package(vizionsdk REQUIRED)

# Add source files
add_executable(hello_vizionsdk main.cpp)

# Link against VizionSDK
target_link_libraries(hello_vizionsdk PRIVATE vizionsdk::VizionSDK) 