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
    
    // Get and set ISP Image Processing properties
    VX_ISP_IMAGE_PROPERTIES properties[] = {
        VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_BRIGHTNESS,
        VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_CONTRAST,
        VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_SATURATION,
        VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_SHARPNESS,
        VX_ISP_IMAGE_PROPERTIES::ISP_IMAGE_GAMMA
    };
    
    const char* propertyNames[] = {
        "Brightness",
        "Contrast",
        "Saturation",
        "Sharpness",
        "Gamma"
    };
    
    for (size_t i = 0; i < sizeof(properties)/sizeof(properties[0]); i++) {      
        std::cout << "=====================================" << std::endl;  
        // Get property range
        int min, max, step, def;
        if (VxGetISPImageProcessingRange(cam, properties[i], min, max, step, def) != 0) {
            std::cout << "Failed to get " << propertyNames[i] << " range" << std::endl;
            continue;
        }
        
        std::cout << propertyNames[i] << " range: " << min << " - " << max 
                  << " (step: " << step << ", default: " << def << ")" << std::endl;
        
        // Set to middle value
        int targetValue = (min + max) / 2;
        if (VxSetISPImageProcessing(cam, properties[i], targetValue) != 0) {
            std::cout << "Failed to set " << propertyNames[i] << std::endl;
            continue;
        }
        
        // Get current value
        int currentValue, flag;
        if (VxGetISPImageProcessing(cam, properties[i], currentValue, flag) != 0) {
            std::cout << "Failed to get current " << propertyNames[i] << " value" << std::endl;
            continue;
        }
        
        std::cout << "Set " << propertyNames[i] << " to " << targetValue 
                  << ", current value: " << currentValue 
                  << " (auto mode: " << (flag ? "on" : "off") << ")" << std::endl;
    }
    
    VxClose(cam);
    return 0;
} 