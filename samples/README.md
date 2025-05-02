# VizionSDK Samples

This repository contains three sample projects demonstrating how to use the VizionSDK:

1. **Hello VizionSDK** - Basic setup and initialization
2. **Camera Capture** - Basic camera operations and image capture
3. **Camera Control** - Advanced camera control features

### Prerequisites

Before building, ensure you have:

1.  **CMake (Version 3.0 or newer):**
    * CMake is required to configure the build.
    * If you don't have it, download and install it from the [official CMake website](https://cmake.org/download/).
    * Make sure the `cmake` executable is in your system's PATH. You can verify by running `cmake --version` in your terminal.

2.  **C++ Compiler (Supporting C++11):**
    * A C++ compiler compatible with the C++11 standard is needed (e.g., GCC, Clang, MSVC). Install one suitable for your platform if necessary (e.g., `build-essential` on Ubuntu/Debian, Visual Studio C++ workload on Windows).

If you're using Ubuntu, install with: 
```
sudo apt install cmake build-essential
```

---

## Building

For each sample (Windows/Linux):

1. Open the terminal/command prompt:
   - Windows: Command Prompt or PowerShell
   - Linux: Your preferred terminal

2. Navigate to the sample directory:  
   e.g., `cd sample1-hello_vizionsdk`

3. Create a build directory:
   ```bash
   mkdir build && cd build
   ```

4. Generate build files using CMake:
   ```bash
   cmake ..
   ```

5. Build the project:
   ```bash
   cmake --build .
   ```

---

## Configuring CMakeLists.txt

### When Using System-Installed SDK (.exe or .deb Installed)

If you've installed VizionSDK using the installer (e.g., .exe on Windows or .deb on Linux), you can simply use `find_package`:

```cmake
# Find the SDK package
find_package(vizionsdk REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  vizionsdk::VizionSDK
)
```

### Using Extracted SDK Archive

If you're using the SDK directly from the extracted folder (without system installation), update the path manually:

```cmake
# Set include and lib paths
# NOTE: Adjust the path to match your actual SDK location
target_include_directories(${PROJECT_NAME}
  PRIVATE
  <YOUR_SDK_PATH>/include/vizionsdk
)

target_link_directories(${PROJECT_NAME}
  PRIVATE
  <YOUR_SDK_PATH>/lib
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  VizionSDK
)
```

> 📌 Replace `<YOUR_SDK_PATH>` with the actual path to your extracted SDK folder.

---

## Running

After building, you'll find the executable in the `build` directory.  
Run it from the command line or terminal:

- **Windows**:  
  ```bash
  .\hello_vizionsdk.exe
  ```

- **Linux**:  
  ```bash
  ./hello_vizionsdk
  ```

> 🔁 Replace `hello_vizionsdk` with `camera_capture` or `camera_control` depending on the sample you're building.

---

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


