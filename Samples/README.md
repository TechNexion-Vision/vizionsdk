# VizionSample

This is a sample project demonstrating how to use CMake to build and run a project called VizionSample.

## Prerequisites

Before building and running this project, ensure you have the following installed:

- CMake (version 3.0 or newer)
- C++ compiler (supporting C++11 standard)

## Building

### Windows

1. Open Command Prompt or PowerShell.
2. Navigate to the root directory of the project.
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake: `cmake ..`
5. Build the project using the generated build files: `cmake --build . --config Release`

### Linux

1. Open Terminal.
2. Navigate to the root directory of the project.
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake: `cmake ..`
5. Build the project using the generated build files: `make`

## Running

### Windows

- After successful building, you will find the executable `VizionSample.exe` in the `build` directory. You can run it from the command line or by double-clicking on it in the File Explorer.
- Ensure that you also copy the VizionSDK.dll file next to the executable VizionSample.exe. This DLL file is required for the proper execution of the application.

### Linux

- After successful building, you will find the executable `VizionSample` in the `build` directory. You can run it from the terminal by executing `./VizionSample`.

