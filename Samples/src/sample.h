#pragma once
#include <memory>
#include <string>
#include <vector>
#include "vizionsdk/VizionSDK.h"

class VizionSample {
public:
    VizionSample();
    ~VizionSample();

    // Basic operations
    static int ListDevices();
    int OpenDevice(int deviceIndex);
    int ShowDeviceInfo(int deviceIndex);
    void Cleanup();

private:
    std::shared_ptr<VxCamera> cam;
};