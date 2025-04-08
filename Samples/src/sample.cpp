#include "sample.h"
#include <iostream>

VizionSample::VizionSample()
    : cam(nullptr) {}

VizionSample::~VizionSample() { Cleanup(); }

void VizionSample::Cleanup() {
    if (cam) {
        VxClose(cam);
        cam = nullptr;
    }
}

int VizionSample::ListDevices() {
    std::vector<std::string> devList;
    int deviceCount = VxDiscoverCameraDevices(devList);

    if (deviceCount == 0) {
        std::cout << "No cameras found" << std::endl;
        return -1;
    }

    std::cout << "Found " << deviceCount << " camera(s):" << std::endl;
    for (size_t i = 0; i < devList.size(); i++) {
        std::cout << "[" << i << "] " << devList[i] << std::endl;
    }

    return 0;
}

int VizionSample::OpenDevice(int deviceIndex) {
    std::vector<std::string> devList;
    int canNum = VxDiscoverCameraDevices(devList);

    if (canNum == 0 || deviceIndex >= devList.size()) {
        std::cout << "No cameras found or invalid device index" << std::endl;
        return -1;
    }

    Cleanup();  // Ensure clean state

    cam = VxInitialCameraDevice(deviceIndex);
    if (!cam || VxOpen(cam) != 0) {
        std::cout << "Failed to initialize/open camera" << std::endl;
        return -1;
    }

    std::cout << "Device opened successfully" << std::endl;
    return 0;
}

int VizionSample::ShowDeviceInfo(int devIdx) {
    if (!cam) {
        std::cout << "Camera not initialized" << std::endl;
        return -1;
    }

    std::string deviceName;
    if (VxGetDeviceName(cam, deviceName) == 0) {
        std::cout << "Device name: " << deviceName << std::endl;
    }

    std::string hwId;
    if (VxGetHardwareID(cam, hwId) == 0) {
        std::cout << "Hardware ID: " << hwId << std::endl;
    }

    return 0;
}