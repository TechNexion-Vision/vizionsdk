#include <iostream>
#include "VizionSDK.h"

int main() {
    std::cout << "VizionSDK Version: " << VIZIONSDK_VERSION << std::endl;
    HelloVizionSDK();
    
    std::vector<std::string> devList;
    int deviceCount = VxDiscoverCameraDevices(devList);
    
    if (deviceCount > 0) {
        std::cout << "Found " << deviceCount << " camera(s):" << std::endl;
        for (size_t i = 0; i < devList.size(); i++) {
            std::cout << "[" << i << "] " << devList[i] << std::endl;
        }
    } else {
        std::cout << "No cameras found" << std::endl;
    }
    
    return 0;
} 