# VizionSample

This is a sample project demonstrating how to use CMake to build and run a project called VizionSample.

## Prerequisites

Before building and running this project, ensure you have the following installed:

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
  - On Debian-based systems (e.g., Ubuntu), you can install the GNU Compiler Collection (GCC) using apt:
    ```
    sudo apt update
    sudo apt install build-essential
    ```
## Building

### Windows

1. Open Command Prompt or PowerShell.
2. Navigate to the Samples directory of the project:
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake (replace your_platform with **Windows**, **Ubuntu**, **IMX**, **TI**, or **Nvidia**):
  ```
  cmake -DPLATFORM=[your_platform] ..
  ```
5. Build the project using the generated build files: `cmake --build . --config Release`

### Linux

1. Open Terminal.
2. Navigate to the Samples directory of the project:
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake (replace your_platform with **Windows**, **Ubuntu**, **IMX**, **TI**, or **Nvidia**):
  ```
  cmake -DPLATFORM=[your_platform] ..
  ```
5. Build the project using the generated build files: `make`

## Running

### Windows

- After successful building, you will find the executable `VizionSample.exe` in the `build` directory. You can run it from the command line or by double-clicking on it in the File Explorer.
- Ensure that you also copy the VizionSDK.dll file next to the executable VizionSample.exe. This DLL file is required for the proper execution of the application.

### Linux

- After successful building, you will find the executable VizionSample in the build directory. Before running it, navigate to the SDK directory in the terminal using cd path_to_sdk, then execute sudo cp VizionSDK.so* /usr/lib/ to copy the necessary SDK files to the system's library directory.
- After successful building, you will find the executable `VizionSample` in the `build` directory. You can run it from the terminal by executing `./VizionSample`.

