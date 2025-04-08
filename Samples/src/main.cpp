#include "sample.h"
#include <iostream>

int main(int argc, char* argv[]) {
    VizionSample sample;

    // List all devices
    std::cout << "\nListing all devices:" << std::endl;
    sample.ListDevices();

    // Try to open first device
    std::cout << "\nTrying to open device 0:" << std::endl;
    if (sample.OpenDevice(0) == 0) {
        // Show device info
        std::cout << "\nDevice information:" << std::endl;
        sample.ShowDeviceInfo(0);
    }

    std::cout << "Press Enter to exit..." << std::endl;
	std::cin.get();
	std::cout << "Exiting program..." << std::endl;

    return 0;
}