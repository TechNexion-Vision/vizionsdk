# VizionSDK Samples

This repository contains three sample projects demonstrating how to use the VizionSDK:

1. **Hello VizionSDK** - Basic setup and initialization
2. **Camera Capture** - Basic camera operations and image capture
3. **Camera Control** - Advanced camera control features

## Prerequisites

Before building and running these projects, ensure you have the following installed:

### CMake (version 3.0 or newer)
- **Windows**: 
  - You can download the latest CMake installer from the [official CMake website](https://cmake.org/download/) and follow the installation instructions provided.
  - Ensure that CMake is added to your system PATH during installation to enable command-line usage.

- **Linux**: 
  - On Debian-based systems (e.g., Ubuntu), you can install CMake using apt:
    ```
    sudo apt update
    sudo apt install cmake
    ```
  - If sudo apt install cmake is not available. You can manually download and install [CMake](https://cmake.org/download/) from the official website:

    If you're working with NXP, Nvidia, or TI platforms  look for the Linux aarch64 binary distribution. Download the appropriate tar.gz file for your platform.
    1. Extract the downloaded file:
    ```
    tar -zxvf cmake-[version]-linux-aarch64.tar.gz
    ```
    2. Move the extracted directory:
       - Move the extracted files to a directory like /opt for easier management:
         ```
         sudo mv cmake-[version]-linux-aarch64 /opt/cmake
         ```
    3. Update the PATH environment variable:
       - Add CMake to your system PATH by modifying your ~/.bashrc or ~/.profile file:
         ```
          export PATH=/opt/cmake/bin:$PATH
         ```
        - Save the file and reload your profile with
          ```
           source ~/.profile
          ```
### C++ compiler (supporting C++11 standard)

- **Windows**: 
  - For Windows, you might need to install a C++ compiler separately. You can use Microsoft Visual Studio, which includes the MSVC compiler.
    - When installing Visual Studio, make sure to select the "Desktop development with C++" workload during the installation process. This will include the necessary C++ compiler, nmake, and other essential tools for C++ development on Windows. 
  - Alternatively, you can use MinGW, a port of the GNU Compiler Collection (GCC), which provides a C++ compiler for Windows.

- **Linux**: 
    - Most Linux distributions come with a C++ compiler pre-installed. However, if it's not available, you can install it using your package manager.
  On Debian-based systems (e.g., Ubuntu), you can install the GNU Compiler Collection (GCC) using apt:
    ```
    sudo apt update
    sudo apt install build-essential
    ```
   - Embedded systems:
    Embedded systems typically do not require a compiler. Development is done using cross-compilation on a host machine, and the resulting binaries are deployed to the embedded device.
   
## Building

### Windows

For each sample:
1. Open Command Prompt or PowerShell.
2. Navigate to the sample directory (e.g., sample1-hello_vizionsdk)
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake:
  ```
  cmake ..
  ```
5. Build the project using the generated build files: `cmake --build . --config Release`

### Linux

### CMakeLists.txt Configuration

In this project, we use library from the installation way for example. If you like to use VizionSDK in the local folder( ../Downloads/vizionsdk-{version}-{arch}/lib), please make sure the CMakeLists.txt is correct.

- **Link the Installation library** (default)  
Linked the library which is installed by **.exe**, **.deb** or using **apt install**
```cmake

# Find vizionsdk package
find_package(vizionsdk REQUIRED)

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
```

- **Link the Local library**
```cmake
# Set path to the SDK library directory
# NOTE: Update this path to match your SDK location
target_link_directories(VizionSample
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Downloads/vizionsdk-{version}-{arch}/lib
)

# Link libraries
target_link_libraries(VizionSample
  PRIVATE
  VizionSDK
)
```
### Building with cmake

For each sample:
1. Open Terminal.
2. Navigate to the sample directory (e.g., sample1-hello_vizionsdk)
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake:
  ```
  cmake ..
  ```
5. Build the project using the generated build files: `make`

## Running

### Windows

For each sample:
- After successful building, you will find the executable in the `build` directory. You can run it from the command line or by double-clicking on it in the File Explorer.

### Linux

For each sample:
- After successful building, you will find the executable in the `build` directory. You can run it from the terminal by executing `./VizionSample`.

## Sample Descriptions

### 1. Hello VizionSDK
This sample demonstrates the basic setup and initialization of the VizionSDK. It shows how to:
- Initialize the SDK
- Discover available camera devices

### 2. Camera Capture
This sample focuses on basic camera operations:
- Discover available camera devices
- Opening a camera connection
- Capturing images

### 3. Camera Control
This sample demonstrates advanced camera features:
- Discover available camera devices
- Opening a camera connection
- ISP Image Processing


