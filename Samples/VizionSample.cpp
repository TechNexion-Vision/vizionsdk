#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#include <limits.h>
#include <link.h>
typedef unsigned char BYTE;
#endif

#include "VizionSDK.h"

#ifdef _WIN32
HINSTANCE hs_vizionsdk;
#else
void* hs_vizionsdk;
#endif

VizionCam* vzcam;
AE_MODE_STATUS ae_mode;
AWB_MODE_STATUS awb_mode;

uint16_t g_width = 0, g_height = 0;
uint8_t* img_arr;

std::vector<DeviceListData> devlist;

template <typename FP>
int LoadVcFunc(FP& fp, std::string func_name)
{
#ifdef _WIN32
	fp = (FP)GetProcAddress(hs_vizionsdk, func_name.c_str());
#else
	fp = (FP)dlsym(hs_vizionsdk, func_name.c_str());
#endif
	if (NULL == fp)
	{
		printf("Load %s Fail\n", func_name.c_str());
		return -1;
	}
	return 0;
}

int LoadVizionFunc()
{
	if (LoadVcFunc(VcSetLogLevel, "VcSetLogLevel"))
		return -1;
	if (LoadVcFunc(VcCreateVizionCamDevice, "VcCreateVizionCamDevice"))
		return -1;
	if (LoadVcFunc(VcReleaseVizionCamDevice, "VcReleaseVizionCamDevice"))
		return -1;
	if (LoadVcFunc(VcOpen, "VcOpen"))
		return -1;
	if (LoadVcFunc(VcClose, "VcClose"))
		return -1;
	if (LoadVcFunc(VcGetVideoDeviceListWithLocation, "VcGetVideoDeviceListWithLocation"))
		return -1;
	if (LoadVcFunc(VcGetVizionCamDeviceName, "VcGetVizionCamDeviceName"))
		return -1;
	if (LoadVcFunc(VcGetCaptureFormatList, "VcGetCaptureFormatList"))
		return -1;
	if (LoadVcFunc(VcSetCaptureFormat, "VcSetCaptureFormat"))
		return -1;
	if (LoadVcFunc(VcGetRawImageCapture, "VcGetRawImageCapture"))
		return -1;
	if (LoadVcFunc(VcGetUSBFirmwareVersion, "VcGetUSBFirmwareVersion"))
		return -1;
	if (LoadVcFunc(VcGetBootdataHeader, "VcGetBootdataHeader"))
		return -1;
	if (LoadVcFunc(VcGetBootdataHeaderV3, "VcGetBootdataHeaderV3"))
		return -1;
	if (LoadVcFunc(VcCheckHeaderVer, "VcCheckHeaderVer"))
		return -1;
	if (LoadVcFunc(VcGetTEVSFirmwareVersion, "VcGetTEVSFirmwareVersion"))
		return -1;

	return 0;
}

#ifdef _WIN32
std::string GetFileVersion(const std::string filePath)
{
	std::string ret_str;
	DWORD dwHandle = 0;
	DWORD dwSize = GetFileVersionInfoSizeA(filePath.c_str(), &dwHandle);
	BYTE* pBuffer;
	if (dwSize == 0)
	{
		std::cerr << "Error in GetFileVersionInfoSize: " << GetLastError() << std::endl;
		goto done;
	}

	pBuffer = new BYTE[dwSize];

	if (!GetFileVersionInfoA(filePath.c_str(), dwHandle, dwSize, pBuffer))
	{
		std::cerr << "Error in GetFileVersionInfo: " << GetLastError() << std::endl;
		goto done;
	}

	VS_FIXEDFILEINFO* pFileInfo = nullptr;
	if (!VerQueryValueA(pBuffer, "\\", (LPVOID*)(&pFileInfo), NULL))
	{
		std::cerr << "Error in VerQueryValue: " << GetLastError() << std::endl;
		goto done;
	}

	ret_str = std::to_string(HIWORD(pFileInfo->dwProductVersionMS)) + "."
			+ std::to_string(LOWORD(pFileInfo->dwProductVersionMS)) + "."
			+ std::to_string(HIWORD(pFileInfo->dwProductVersionLS)) + "."
			+ std::to_string(LOWORD(pFileInfo->dwProductVersionLS));

done:
	if(pBuffer != nullptr)
		delete[] pBuffer;
	return ret_str;
}
#else 
std::string GetFileVersion(void* handle)
{
	std::string version = "";

	struct link_map* map;
	if (dlinfo(handle, RTLD_DI_LINKMAP, &map) == -1) {
		return version;
	}

	char buf[PATH_MAX];
	char* res = realpath(map->l_name, buf);
	if (res) {
		std::string libraryPath(buf);
		size_t pos = libraryPath.find(".so.");
		if (pos != std::string::npos) {
			version = libraryPath.substr(pos + 4); // Assuming version starts after ".so."
		}
	}

	return version;
}
#endif

int main()
{
	uint16_t startup;
#ifdef _WIN32
	hs_vizionsdk = LoadLibrary(L"VizionSDK.dll");
	if (NULL == hs_vizionsdk)
	{
		printf("Load VizionSDK.dll Fail\n");
		return -1;
	}
#else
	std::string libname("libVizionSDK.so");
	hs_vizionsdk = dlopen(libname.c_str(), RTLD_LAZY);
	const char* error = dlerror();
	if (error != nullptr)
	{
		std::cout << "dlopen error: " << error << std::endl;
		return -1;
	}
#endif

	if (LoadVizionFunc() < 0)
		return -1;

#ifdef _WIN32
	std::string filePath = "VizionSDK.dll";
	std::string version = GetFileVersion(filePath);
	if (!version.empty())
	{
		std::cout << "VizionSDK Version: " << version << std::endl;
	}
#else
	std::string version = GetFileVersion(hs_vizionsdk);
	if (!version.empty()) {
		std::cout << "VizionSDK Version: " << version << std::endl;
	}
#endif

	// Set Log Level
	VcSetLogLevel(VzLogLevel::VZ_LOG_LEVEL_OFF);

	// Create control object for Vizion Camera
	vzcam = VcCreateVizionCamDevice();

	VcGetVideoDeviceListWithLocation(vzcam, devlist);

	if (devlist.size() == 0)
	{
		std::cout << "Cannot Find any Camera!" << std::endl;
		return -1;
	}

	int i = 0;
	std::cout << "Select Camera:" << std::endl;
	for (const auto& device : devlist)
		_STDOUT << "[" << i++ << "] " << "Device Name: " << device.deviceName \
		<< ", Hardware ID: " << device.hardwareId << \
		", Serial Number: " << device.serialNumber << std::endl;

	std::cin >> startup;

	if (VcOpen(vzcam, startup) != 0)
	{
		std::cout << "Open Fail" << std::endl;
		return -1;
	}

	// UVC Version
	char usb_fw_ver[16];
	if (VcGetUSBFirmwareVersion(vzcam, usb_fw_ver) == 0)
	{
		std::cout << "UVC Version: " << usb_fw_ver << std::endl;
	}

	// ISP Version
	int ver = VcCheckHeaderVer(vzcam);
	if (ver == 3)
	{
		VzHeaderV3 header_v3;
		if (VcGetBootdataHeaderV3(vzcam, &header_v3) == 0)
		{
			char fw_ver[16];
			snprintf(fw_ver, 16, "%02d.%02d.%02d.%02d", header_v3.tn_fw_version[0], header_v3.tn_fw_version[1], header_v3.vendor_fw_version, header_v3.custom_number);
			std::cout << "ISP Version: " << fw_ver << std::endl;
		}
	}
	else if (ver == 2)
	{
		VzHeader header_v2;
		if (VcGetBootdataHeader(vzcam, &header_v2) == 0)
		{
			std::cout << "Lens Version: " << static_cast<int>(header_v2.lens_version) << std::endl;
			std::cout << "Content Version: " << static_cast<int>(header_v2.content_version) << std::endl;
		}
	}
	else
	{
		std::cout << "Unknown ISP Version" << std::endl;
	}

	// Tevs Version
#ifdef _WIN32
	if (devlist[0].deviceName.find(L"TEVS") != std::string::npos || devlist[0].deviceName.find(L"VLS") != std::string::npos)
#else
	if (devlist[0].deviceName.find("TEVS") != std::string::npos || devlist[0].deviceName.find("VLS") != std::string::npos)
#endif
	{
		char tevsVersion[16];
		if (VcGetTEVSFirmwareVersion(vzcam, tevsVersion) == 0)
		{
			std::cout << "TEVS Version: " << tevsVersion << std::endl;
		}
	}

	// select format
	int sel = 0;
	std::vector<VzFormat> vzformatlist;

	VcGetCaptureFormatList(vzcam, vzformatlist);

	for (int i = 0; i < vzformatlist.size(); i++)
		printf("[%d] Width=%d, Height=%d, Framerate=%d\n", i, vzformatlist[i].width, vzformatlist[i].height, vzformatlist[i].framerate);

	std::cout << "Select a Capture Format..Please enter the index of Format." << std::endl;

	std::cin >> sel;

	if (sel > vzformatlist.size() || sel < 0)
	{
		std::cout << "Select index " << sel << " Fail" << std::endl;
		return -1;
	}

	VcSetCaptureFormat(vzcam, vzformatlist[sel]);
#ifdef _WIN32
	Sleep(10);
#else
	usleep(10);
#endif
	int size = 0;
	int timeout = 3000;
	g_width = vzformatlist[sel].width;
	g_height = vzformatlist[sel].height;
	int retry_count = 0;
	const int max_retry = 10;
	int ret = -1;
	img_arr = new uint8_t[g_width * g_height * 3];
	while (retry_count < max_retry)
	{
		
		ret = VcGetRawImageCapture(vzcam, img_arr, &size, timeout);

		if (ret == 0)
		{
			std::cout << "VcGetImageCapture Success " << std::endl;
			std::cout << "Image data size: " << size << std::endl;
			break;
		}
		else
		{
			retry_count++;
			if (retry_count >= max_retry)
			{
				std::cout << "VcGetImageCapture Fail ErrCode: " << ret << std::endl;
				std::cout << "count: " << retry_count << std::endl;
				return -1;
			}
		}
#ifdef _WIN32
		Sleep(10);
#else
		usleep(10);
#endif
	}

	// Save image
#ifdef _WIN32
	HANDLE hFile = CreateFileA("image.raw", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten;
		BOOL result = WriteFile(hFile, img_arr, size, &bytesWritten, NULL);
		if (result != FALSE)
			std::cout << "Save image finish" << std::endl;
		else
			std::cerr << "Failed to write to file" << std::endl;
		CloseHandle(hFile);
	}
	else
	{
		std::cerr << "Failed to create file" << std::endl;
	}
#else
	FILE* file = fopen("image.raw", "wb");
	if (file != NULL)
	{
		size_t elements_written = fwrite(img_arr, size, 1, file);
		if (elements_written == 1)
			std::cout << "Save image finish" << std::endl;
		else
			std::cerr << "Failed to write to file" << std::endl;
	}
	else
		std::cerr << "Failed to create file" << std::endl;

	fclose(file);
#endif

	std::cout << "Press any key to exit..." << std::endl;
	std::cin.ignore();
	std::cin.get();
	std::cout << "Exiting program..." << std::endl;

	VcClose(vzcam);

	VcReleaseVizionCamDevice(vzcam);

#ifdef _WIN32
	FreeLibrary(hs_vizionsdk);
#else
	dlclose(hs_vizionsdk);
#endif
	return 0;
}