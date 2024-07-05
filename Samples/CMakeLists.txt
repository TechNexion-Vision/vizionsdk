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

# Define platform options
option(PLATFORM "Choose the platform: IMX, Nvidia, Ubuntu, Windows" "")

if(NOT PLATFORM)
    message(FATAL_ERROR "PLATFORM is not set. Please specify PLATFORM as IMX, Nvidia, Ubuntu, or Windows.")
endif()

# Construct platform-specific directory path
set(PLATFORM_DIR "${CMAKE_SOURCE_DIR}/../lib/${PLATFORM}/")

# Check if PLATFORM_DIR exists
if(NOT EXISTS ${PLATFORM_DIR})
    message(FATAL_ERROR "PLATFORM_DIR does not exist: ${PLATFORM_DIR}")
endif()

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add platform-specific configurations
if(WIN32)
    add_definitions(-D_WIN32)
    add_definitions(-DUNICODE)
    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "Configurations" FORCE)
    target_link_libraries(${PROJECT_NAME} "Version.lib")
  if(EXISTS "${PLATFORM_DIR}VizionSDK.dll")
    # Copy the DLL to the build output directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PLATFORM_DIR}/VizionSDK.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
 else()
     message(FATAL_ERROR "VizionSDK.dll not found in ${PLATFORM_DIR}")
   endif()
elseif(UNIX)
    add_definitions(-D_LINUX)
    include_directories(${PLATFORM_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE -ldl -lusb-1.0 -pthread)
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,${PLATFORM_DIR}")
endif()
