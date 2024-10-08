#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>

#define _TYPE_STRING std::wstring
#define _STDOUT std::wcout
#define _STR(str) L#str
#define _SLEEP(ms) Sleep(ms)
#define LIBRARY_NAME L"VizionSDK.dll"
#define loadLibrary() LoadLibrary(LIBRARY_NAME)
HINSTANCE hs_vizionsdk;
#else
#include <dlfcn.h>
#include <unistd.h>
#include <limits.h>
#include <link.h>

typedef unsigned char BYTE;

#define _TYPE_STRING std::string
#define _STDOUT std::cout
#define _STR(str) #str
#define _SLEEP(ms) usleep(ms * 1000)
#define LIBRARY_NAME "libVizionSDK.so"
#define loadLibrary() dlopen(LIBRARY_NAME, RTLD_LAZY)
void *hs_vizionsdk;
#endif

#include "VizionSDK.h"

VizionCam *vzcam;
AE_MODE_STATUS ae_mode;
AWB_MODE_STATUS awb_mode;

uint16_t g_width = 0, g_height = 0;
uint8_t *img_arr;

std::vector<DeviceListData> devlist;

template <typename FP>
int LoadVcFunc(FP &fp, std::string func_name)
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

void SaveImageFile(const char *filename, const unsigned char *data, int size)
{
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	file.write(reinterpret_cast<const char *>(data), size);

	if (!file.good())
	{
		std::cerr << "Failed to write to file: " << filename << std::endl;
		file.close();
		return;
	}

	file.close();

	std::cout << "Save Image Successfully!" << std::endl;
}

std::string GetFormatName(VZ_IMAGE_FORMAT format)
{
	switch (format)
	{
	case VZ_IMAGE_FORMAT::NONE:
		return "None";
	case VZ_IMAGE_FORMAT::YUY2:
		return "YUY2";
	case VZ_IMAGE_FORMAT::UYVY:
		return "UYVY";
	case VZ_IMAGE_FORMAT::NV12:
		return "NV12";
	case VZ_IMAGE_FORMAT::MJPG:
		return "MJPG";
	default:
		return "Unknown Format";
	}
}

#ifdef _WIN32
std::string GetFileVersion(const std::string filePath)
{
	std::string ret_str;
	DWORD dwHandle = 0;
	DWORD dwSize = GetFileVersionInfoSizeA(filePath.c_str(), &dwHandle);
	BYTE *pBuffer;
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

	VS_FIXEDFILEINFO *pFileInfo = nullptr;
	if (!VerQueryValueA(pBuffer, "\\", (LPVOID *)(&pFileInfo), NULL))
	{
		std::cerr << "Error in VerQueryValue: " << GetLastError() << std::endl;
		goto done;
	}

	ret_str = std::to_string(HIWORD(pFileInfo->dwProductVersionMS)) + "." + std::to_string(LOWORD(pFileInfo->dwProductVersionMS)) + "." + std::to_string(HIWORD(pFileInfo->dwProductVersionLS)) + "." + std::to_string(LOWORD(pFileInfo->dwProductVersionLS));

done:
	if (pBuffer != nullptr)
		delete[] pBuffer;
	return ret_str;
}
#else
std::string GetFileVersion(void *handle)
{
	std::string version = "";

	struct link_map *map;
	if (dlinfo(handle, RTLD_DI_LINKMAP, &map) == -1)
	{
		return version;
	}

	char buf[PATH_MAX];
	char *res = realpath(map->l_name, buf);
	if (res)
	{
		std::string libraryPath(buf);
		size_t pos = libraryPath.find(".so.");
		if (pos != std::string::npos)
		{
			version = libraryPath.substr(pos + 4); // Assuming version starts after ".so."
		}
	}

	return version;
}
#endif

int main()
{
	uint16_t index;

	hs_vizionsdk = loadLibrary();

	if (hs_vizionsdk == nullptr)
		std::cout << "Load VizionSDK Fail" << std::endl;

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
	if (!version.empty())
	{
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
	for (const auto &device : devlist)
		_STDOUT << "[" << i++ << "] " << "Device Name: " << device.deviceName << std::endl;

	std::cin >> index;

	if (VcOpen(vzcam, index) < 0)
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
	if (devlist[0].deviceName.find(_STR("TEVS")) != std::string::npos || devlist[0].deviceName.find(_STR("VLS")) != std::string::npos)
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
		if (vzformatlist[i].format != VZ_IMAGE_FORMAT::NONE)
			printf("[%d] Width=%d, Height=%d, Framerate=%d, Format=%s\n", i, vzformatlist[i].width, vzformatlist[i].height, vzformatlist[i].framerate, GetFormatName(vzformatlist[i].format).c_str());

	std::cout << "Select a Capture Format..Please enter the index of Format." << std::endl;

	std::cin >> sel;

	if (sel > vzformatlist.size() || sel < 0)
	{
		std::cout << "Select index " << sel << " Fail" << std::endl;
		return -1;
	}

	VcSetCaptureFormat(vzcam, vzformatlist[sel]);
	_SLEEP(10);

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
		_SLEEP(10);
	}

	// Save image
	SaveImageFile("image.raw", img_arr, size);

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
