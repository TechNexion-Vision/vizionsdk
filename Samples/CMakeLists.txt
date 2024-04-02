cmake_minimum_required(VERSION 3.0)
project(VizionSample)

set(CMAKE_CXX_STANDARD 11)

# Add source files
set(SOURCES
    VizionSample.cpp
)

# Add header files
set(HEADERS
    VizionSDK.h
)

# Add platform-specific configurations
if(WIN32)
    add_definitions(-D_WIN32)
    add_definitions(-DUNICODE)
elseif(UNIX)
    add_definitions(-D_LINUX)
endif()

# Add executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
