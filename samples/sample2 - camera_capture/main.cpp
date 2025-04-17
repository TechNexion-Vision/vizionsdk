#include <iostream>
#include <memory>
#include "VizionSDK.h"

int main() {
    // List available cameras
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
    
    // Open camera
    auto cam = VxInitialCameraDevice(0);
    if (!cam || VxOpen(cam) != 0) {
        std::cout << "Failed to initialize/open camera" << std::endl;
        return -1;
    }
    
    // Get and set format
    std::vector<VxFormat> fmtList;
    if (VxGetFormatList(cam, fmtList) != 0) {
        std::cout << "Failed to get format list" << std::endl;
        VxClose(cam);
        return -1;
    }
    
    if (VxSetFormat(cam, fmtList[0]) != 0) {
        std::cout << "Failed to set format" << std::endl;
        VxClose(cam);
        return -1;
    }
    
    // Start streaming
    if (VxStartStreaming(cam) != 0) {
        std::cout << "Failed to start streaming" << std::endl;
        VxClose(cam);
        return -1;
    }
    
    // Capture 5 frames
    std::shared_ptr<uint8_t[]> buffer(new uint8_t[fmtList[0].width * fmtList[0].height * 3]);
    int dataSize;
    
    for (int i = 0; i < 5; i++) {
        VX_CAPTURE_RESULT result = VxGetImage(cam, buffer.get(), &dataSize, 1000);
        if (result == VX_CAPTURE_RESULT::VX_SUCCESS) {
            std::cout << "Successfully captured frame " << i + 1 << " of size: " << dataSize << " bytes" << std::endl;
        } else {
            std::cout << "Failed to capture frame " << i + 1 << std::endl;
            break;
        }
    }
    
    // Cleanup
    VxStopStreaming(cam);
    VxClose(cam);
    
    return 0;
} 