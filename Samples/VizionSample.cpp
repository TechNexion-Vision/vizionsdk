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

#ifdef _WIN32
UINT g_width = 0, g_height = 0;
#else 
uint8_t g_width = 0, g_height = 0;
#endif
uint8_t* img_arr;
uint8_t* raw_arr;

#ifdef _WIN32
struct DeviceListData
{
	std::wstring deviceName;
	std::wstring hardwareId;
	std::string serialNumber;
	std::string portNumber;
};
std::vector<DeviceListData> devlist;
#else
struct DeviceListData
{
	std::string deviceName;
	std::string hardwareID;
	std::string serialNumber;
	std::string portNumber;
};
std::vector<DeviceListData> devlist;
#endif



// internal function pointer
#pragma pack(push, 1)
struct VzHeader
{
	uint8_t header_version;
	uint16_t content_offset;
	uint8_t product_name[64];
	uint8_t product_version;
	uint8_t head_info[64];
	uint8_t lens_version;
	uint8_t content_version;
	uint32_t content_checksum;
	uint32_t content_len;
	uint16_t pll_bootdata_len;
};

struct VzHeaderV3
{ // Header Version 3
	// Fixed Area
	uint8_t header_version;
	uint16_t content_offset;
	uint16_t sensor_type;
	uint8_t sensor_fuseid[16];
	uint8_t product_name[64];
	uint8_t lens_id[16];
	uint16_t fix_checksum;
	// Dynamic update area
	uint8_t tn_fw_version[2];
	uint16_t vendor_fw_version;
	uint16_t custom_number; // or revision number
	uint8_t build_year;
	uint8_t build_month;
	uint8_t build_day;
	uint8_t build_hour;
	uint8_t build_minute;
	uint8_t build_second;
	uint16_t mipi_datarate;
	uint32_t content_len;
	uint16_t content_checksum;
	uint16_t total_checksum;
};

struct VzSensorProfile
{
	uint8_t profile_check;
	uint16_t profile_len;
	uint16_t pre_width;
	uint16_t pre_height;
	uint8_t pre_format;
	uint8_t sensor_mode;
	uint16_t pre_throughput;
	uint8_t pre_maxfps;
	uint32_t pre_ae_upper;
	uint32_t pre_ae_max;
	uint16_t pre_hinf_ctrl;
	uint8_t ae_mode;
	uint32_t exp_time;
	uint8_t exp_gain;
	uint16_t backlight_comp;
	uint8_t awb_mode;
	uint16_t awb_tmp;
	uint16_t brightness;
	uint16_t contrast;
	uint16_t saturation;
	uint16_t gamma;
	uint16_t denoise;
	uint16_t sharpen;
	uint8_t flip;
	uint8_t effect;
	uint16_t zoom_type;
	uint16_t zoom_times;
	uint16_t ct_x;
	uint16_t ct_y;
	uint16_t trigger_mode;
	uint16_t flick_ctrl;
	uint8_t jpeg_qual;
	uint16_t reserved[9];
	uint16_t profile_checksum;
};
#pragma pack(pop)

enum class FX3_FW_TARGET
{
	FW_TARGET_NONE = 0, /* Invalid target					*/
	FW_TARGET_RAM,		/* Program firmware (hex) to RAM	*/
	FW_TARGET_I2C,		/* Program firmware (hex) to EEPROM	*/
	FW_TARGET_SPI		/* Program firmware (hex) to SPI	*/
};

enum class VzLogLevel
{
	VZ_LOG_LEVEL_TRACE,
	VZ_LOG_LEVEL_DEBUG,
	VZ_LOG_LEVEL_INFO,
	VZ_LOG_LEVEL_WARN,
	VZ_LOG_LEVEL_ERROR,
	VZ_LOG_LEVEL_CRITICAL,
	VZ_LOG_LEVEL_OFF,
};

typedef int (*fpVcSetLogLevel)(VzLogLevel);
typedef int (*fpVcGetVideoDeviceListWithLocation)(VizionCam*, std::vector<struct DeviceListData>&);
typedef int (*fpVcGetUSBFirmwareVersion)(VizionCam*, char*);
typedef int (*fpVcGetBootdataHeader)(VizionCam*, VzHeader*);
typedef int (*fpVcGetBootdataHeaderV3)(VizionCam*, VzHeaderV3*);
typedef int (*fpVcCheckHeaderVer)(VizionCam*);
typedef int (*fpVcGetTEVSFirmwareVersion)(VizionCam*, char*);

fpVcSetLogLevel VcSetLogLevel;
fpVcGetVideoDeviceListWithLocation VcGetVideoDeviceListWithLocation;
fpVcGetUSBFirmwareVersion VcGetUSBFirmwareVersion;
fpVcGetBootdataHeader VcGetBootdataHeader;
fpVcGetBootdataHeaderV3 VcGetBootdataHeaderV3;
fpVcCheckHeaderVer VcCheckHeaderVer;
fpVcGetTEVSFirmwareVersion VcGetTEVSFirmwareVersion;

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

typedef BOOL(WINAPI* LPGetFileVersionInfoSizeW)(
	LPCWSTR lptstrFilename,
	LPDWORD lpdwHandle);

typedef BOOL(WINAPI* LPGetFileVersionInfoW)(
	LPCWSTR lptstrFilename,
	DWORD dwHandle,
	DWORD dwLen,
	LPVOID lpData);

typedef BOOL(WINAPI* LPVerQueryValueW)(
	LPCVOID pBlock,
	LPCWSTR lpSubBlock,
	LPVOID* lplpBuffer,
	PUINT puLen);

std::string GetFileVersion(const std::wstring& filePath)
{
	HMODULE hModule = LoadLibrary(L"Version.dll");
	if (hModule == NULL)
	{
		std::cerr << "Error loading Version.dll" << std::endl;
		return "";
	}

	LPGetFileVersionInfoSizeW lpGetFileVersionInfoSizeW = reinterpret_cast<LPGetFileVersionInfoSizeW>(GetProcAddress(hModule, "GetFileVersionInfoSizeW"));
	LPGetFileVersionInfoW lpGetFileVersionInfoW = reinterpret_cast<LPGetFileVersionInfoW>(GetProcAddress(hModule, "GetFileVersionInfoW"));
	LPVerQueryValueW lpVerQueryValueW = reinterpret_cast<LPVerQueryValueW>(GetProcAddress(hModule, "VerQueryValueW"));

	if (lpGetFileVersionInfoSizeW == NULL || lpGetFileVersionInfoW == NULL || lpVerQueryValueW == NULL)
	{
		std::cerr << "Error getting addresses of Version.dll functions" << std::endl;
		FreeLibrary(hModule);
		return "";
	}

	DWORD dwHandle = 0;
	DWORD dwSize = lpGetFileVersionInfoSizeW(filePath.c_str(), &dwHandle);
	if (dwSize == 0)
	{
		std::cerr << "Error in GetFileVersionInfoSize: " << GetLastError() << std::endl;
		FreeLibrary(hModule);
		return "";
	}

	BYTE* pBuffer = new BYTE[dwSize];
	if (!lpGetFileVersionInfoW(filePath.c_str(), dwHandle, dwSize, pBuffer))
	{
		std::cerr << "Error in GetFileVersionInfo: " << GetLastError() << std::endl;
		delete[] pBuffer;
		FreeLibrary(hModule);
		return "";
	}

	VS_FIXEDFILEINFO* pFileInfo = nullptr;
	UINT puLenFileInfo = 0;
	if (!lpVerQueryValueW(pBuffer, L"\\", reinterpret_cast<LPVOID*>(&pFileInfo), &puLenFileInfo))
	{
		std::cerr << "Error in VerQueryValue: " << GetLastError() << std::endl;
		delete[] pBuffer;
		FreeLibrary(hModule);
		return "";
	}

	int majorVersion = HIWORD(pFileInfo->dwProductVersionMS);
	int minorVersion = LOWORD(pFileInfo->dwProductVersionMS);
	int revision = HIWORD(pFileInfo->dwProductVersionLS);
	int buildNumber = LOWORD(pFileInfo->dwProductVersionLS);

	delete[] pBuffer;
	FreeLibrary(hModule);

	return std::to_string(majorVersion) + "." + std::to_string(minorVersion) + "." + std::to_string(revision) + "." + std::to_string(buildNumber);
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
	std::wstring filePath = L"VizionSDK.dll";
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
	std::cout << "Select Camera" << std::endl;
	for (const auto& device : devlist)
	{
#ifdef _WIN32
		std::wcout << "[" << i++ << "] " << L"Device Name: " << device.deviceName << L", Hardware ID: " << device.hardwareId << L", Serial Number: " << device.serialNumber.c_str() << std::endl;
#else
		std::cout << "[" << i++ << "] " << "Device Name: " << device.deviceName << ", Hardware ID: " << device.hardwareID << ", Serial Number: " << device.serialNumber.c_str() << std::endl;
#endif
	}
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