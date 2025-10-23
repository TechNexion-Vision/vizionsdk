# VizionSDK – Third-Party Software Notice

Copyright (C) 2025 TechNexion Ltd.  
All rights reserved.

The VizionSDK distribution includes multiple components:

- **VizionSDK core library** (`VizionSDK`)  
- **Command-line utility** (`vizion-ctl`)

Each component includes or depends on third-party open-source software under the terms listed below.  
Standard runtime libraries (e.g., `glibc`, `libstdc++`, `pthread`) are excluded.

---

## Section A – VizionSDK Core Library

### Core Dependencies

- **yaml-cpp (v0.8.0)** — [MIT License](https://github.com/jbeder/yaml-cpp/blob/master/LICENSE)  

- **spdlog (v1.14.1)** — [MIT License](https://github.com/gabime/spdlog/blob/v1.x/LICENSE)  

- **fmt (v10.1.1)** — [MIT License](https://github.com/fmtlib/fmt/blob/master/LICENSE)  

- **rapidjson** — [MIT License](https://github.com/Tencent/rapidjson/blob/master/license.txt)  

- **pugixml** — [MIT License](https://github.com/zeux/pugixml/blob/master/LICENSE.md)  


---

### Linux-Specific Dependencies

- **GStreamer** — [LGPL 2.1](https://gstreamer.freedesktop.org/documentation/frequently-asked-questions/licensing.html)  
  Components: gstreamer-1.0, gstreamer-base-1.0, gstreamer-app-1.0, gstreamer-allocators-1.0  

- **libusb-1.0** — [LGPL 2.1](https://github.com/libusb/libusb/blob/master/COPYING)  

- **libudev** — [LGPL 2.1](https://github.com/systemd/systemd/blob/main/LICENSE.LGPL2.1)  

> **Note:** These libraries are dynamically linked and provided by the Linux distribution.  
> Standard C/C++ runtimes (glibc, libstdc++) are excluded.


---

### Windows-Specific Dependencies

- **Windows Media Foundation** — Proprietary (Microsoft Windows SDK)  

---

### Language Bindings

- **pybind11** — [BSD-style License](https://github.com/pybind/pybind11/blob/master/LICENSE)  

- **SWIG (Simplified Wrapper and Interface Generator)** — [GPL v3 with Runtime Exception](http://www.swig.org/Release/LICENSE)  
  Note: SWIG’s runtime exception allows generated wrapper code to be distributed under proprietary terms.  

- **scikit-build-core (>=0.10)** — [MIT License](https://github.com/scikit-build/scikit-build-core/blob/main/LICENSE)  


- **numpy (>=1.21.0)** — [BSD License](https://github.com/numpy/numpy)  

---


## Section B – vizion-ctl Command-Line Utility

The `vizion-ctl` tool is distributed alongside the SDK and includes additional networking and security dependencies.

### Core Dependencies

- **fmt (v10.1.1)** — [MIT License](https://github.com/fmtlib/fmt/blob/master/LICENSE)  
- **argparse** — [MIT License](https://github.com/p-ranav/argparse/blob/master/LICENSE)
- **inquirer (>=3.4.0)** — [MIT License](https://github.com/magmax/python-inquirer)  

---

### Networking and Security

- **libcurl (v8.5.0)**  
  - License: [curl License (MIT-like)](https://curl.se/docs/copyright.html)  

- **OpenSSL (v3.3.2)**  
  - License: [Apache License 2.0](https://www.openssl.org/source/license.html)  
  - **Notice:**  
    This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit.  
    <https://www.openssl.org/>


---

## Notes

- All listed components are used in compliance with their respective licenses.  
- No modifications were made to third-party source code unless otherwise noted.  
- Binary distributions include these components only in compiled form.  

---

### Additional Attribution (per Apache License 2.0 §4d)

This software includes portions of OpenSSL licensed under the Apache License 2.0.  
A copy of the Apache License 2.0 is available at:  
<https://www.apache.org/licenses/LICENSE-2.0>
