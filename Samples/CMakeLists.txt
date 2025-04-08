cmake_minimum_required(VERSION 3.14)
project(vizionsdk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vizionsdk package
find_package(vizionsdk REQUIRED)

# Add executable
add_executable(VizionSample
    src/main.cpp
    src/sample.cpp
)

# Include directories
target_include_directories(VizionSample
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(VizionSample
    PRIVATE
    vizionsdk::VizionSDK
)