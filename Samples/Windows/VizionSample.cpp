#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <windows.h>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "VizionSDK.h"

using namespace cv;

HINSTANCE hs_vizionsdk;

VizionCam* vzcam;
AE_MODE_STATUS ae_mode;
AWB_MODE_STATUS awb_mode;
uint8_t max_fps = 0;
uint8_t jpeg_qual = 0;
uint16_t throughput = 0;
uint8_t exp_gain = 0;
uint32_t exp_time = 0;
uint16_t wb_temp = 0;
uint16_t i2c_addr = 0;
uint16_t i2c_data = 0;

double gamma = 0.0;
double saturation = 0.0;
double contrast = 0.0;
double sharpening = 0.0;
double denoise = 0.0;
double dz_tgt = 0.0;

bool preview = true;
cv::VideoCapture cap;
cv::Mat frame;
cv::Mat resize_frame;

UINT g_width = 0, g_height = 0;
uint8_t* img_arr;
uint8_t* raw_arr;

struct DeviceListData {
	std::wstring deviceName;
	std::wstring hardwareId;
	std::string serialNumber;
	std::string portNumber;
};

std::vector<struct DeviceListData> devlist;

// internal function pointer
#pragma pack(push, 1)
struct VzHeader {
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

struct VzHeaderV3 {   // Header Version 3
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
	uint16_t custom_number;   // or revision number
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

struct VzSensorProfile {
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

enum class FX3_FW_TARGET {
	FW_TARGET_NONE = 0,	/* Invalid target					*/
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
fpVcSetLogLevel VcSetLogLevel;

template <typename FP>
int LoadVcFunc(FP& fp, std::string func_name)
{
	fp = (FP)GetProcAddress(hs_vizionsdk, func_name.c_str());
	if (NULL == fp) { printf("Load %s Fail\n", func_name.c_str()); return -1; }
	return 0;
}

int LoadVizionFunc()
{
	if (LoadVcFunc(VcSetLogLevel, "VcSetLogLevel")) return -1;
	if (LoadVcFunc(VcCreateVizionCamDevice, "VcCreateVizionCamDevice")) return -1;
	if (LoadVcFunc(VcReleaseVizionCamDevice, "VcReleaseVizionCamDevice")) return -1;
	if (LoadVcFunc(VcOpen, "VcOpen")) return -1;
	if (LoadVcFunc(VcClose, "VcClose")) return -1;
	if (LoadVcFunc(VcGetVideoDeviceList, "VcGetVideoDeviceList")) return -1;
	if (LoadVcFunc(VcGetVideoDeviceListWithLocation, "VcGetVideoDeviceListWithLocation")) return -1;
	if (LoadVcFunc(VcGetVizionCamDeviceName, "VcGetVizionCamDeviceName")) return -1;
	if (LoadVcFunc(VcGetCaptureFormatList, "VcGetCaptureFormatList")) return -1;
	if (LoadVcFunc(VcSetCaptureFormat, "VcSetCaptureFormat")) return -1;
	if (LoadVcFunc(VcGetRawImageCapture, "VcGetRawImageCapture")) return -1;
	if (LoadVcFunc(VcGetUniqueSensorID, "VcGetUniqueSensorID")) return -1;
	if (LoadVcFunc(VcGetSensorUID, "VcGetSensorUID")) return -1;
	if (LoadVcFunc(VcGetMaxFPS, "VcGetMaxFPS")) return -1;
	if (LoadVcFunc(VcSetMaxFPS, "VcSetMaxFPS")) return -1;
	if (LoadVcFunc(VcGetAutoExposureMode, "VcGetAutoExposureMode")) return -1;
	if (LoadVcFunc(VcSetAutoExposureMode, "VcSetAutoExposureMode")) return -1;
	if (LoadVcFunc(VcGetExposureTime, "VcGetExposureTime")) return -1;
	if (LoadVcFunc(VcSetExposureTime, "VcSetExposureTime")) return -1;
	if (LoadVcFunc(VcGetExposureGain, "VcGetExposureGain")) return -1;
	if (LoadVcFunc(VcSetExposureGain, "VcSetExposureGain")) return -1;
	if (LoadVcFunc(VcGetCurrentExposureTime, "VcGetCurrentExposureTime")) return -1;
	if (LoadVcFunc(VcGetCurrentExposureGain, "VcGetCurrentExposureGain")) return -1;
	if (LoadVcFunc(VcGetBacklightCompensation, "VcGetBacklightCompensation")) return -1;
	if (LoadVcFunc(VcSetBacklightCompensation, "VcSetBacklightCompensation")) return -1;
	if (LoadVcFunc(VcGetAutoWhiteBalanceMode, "VcGetAutoWhiteBalanceMode")) return -1;
	if (LoadVcFunc(VcSetAutoWhiteBalanceMode, "VcSetAutoWhiteBalanceMode")) return -1;
	if (LoadVcFunc(VcGetTemperature, "VcGetTemperature")) return -1;
	if (LoadVcFunc(VcSetTemperature, "VcSetTemperature")) return -1;
	if (LoadVcFunc(VcGetBrightness, "VcGetBrightness")) return -1;
	if (LoadVcFunc(VcSetBrightness, "VcSetBrightness")) return -1;
	if (LoadVcFunc(VcGetGamma, "VcGetGamma")) return -1;
	if (LoadVcFunc(VcSetGamma, "VcSetGamma")) return -1;
	if (LoadVcFunc(VcGetSaturation, "VcGetSaturation")) return -1;
	if (LoadVcFunc(VcSetSaturation, "VcSetSaturation")) return -1;
	if (LoadVcFunc(VcGetContrast, "VcGetContrast")) return -1;
	if (LoadVcFunc(VcSetContrast, "VcSetContrast")) return -1;
	if (LoadVcFunc(VcGetSharpening, "VcGetSharpening")) return -1;
	if (LoadVcFunc(VcSetSharpening, "VcSetSharpening")) return -1;
	if (LoadVcFunc(VcGetDenoise, "VcGetDenoise")) return -1;
	if (LoadVcFunc(VcSetDenoise, "VcSetDenoise")) return -1;
	if (LoadVcFunc(VcGetFlipMode, "VcGetFlipMode")) return -1;
	if (LoadVcFunc(VcSetFlipMode, "VcSetFlipMode")) return -1;
	if (LoadVcFunc(VcGetEffectMode, "VcGetEffectMode")) return -1;
	if (LoadVcFunc(VcSetEffectMode, "VcSetEffectMode")) return -1;
	if (LoadVcFunc(VcGetDigitalZoomType, "VcGetDigitalZoomType")) return -1;
	if (LoadVcFunc(VcSetDigitalZoomType, "VcSetDigitalZoomType")) return -1;
	if (LoadVcFunc(VcGetDigitalZoomTarget, "VcGetDigitalZoomTarget")) return -1;
	if (LoadVcFunc(VcSetDigitalZoomTarget, "VcSetDigitalZoomTarget")) return -1;
	if (LoadVcFunc(VcGetFlickMode, "VcGetFlickMode")) return -1;
	if (LoadVcFunc(VcSetFlickMode, "VcSetFlickMode")) return -1;
	if (LoadVcFunc(VcGetJpegQual, "VcGetJpegQual")) return -1;
	if (LoadVcFunc(VcSetJpegQual, "VcSetJpegQual")) return -1;

	return 0;
}

void showImage(UINT width, UINT height, VZ_IMAGE_FORMAT format)
{
	int i = 0;
	int ret = 0;
	double time_taken = 0;
	int size = 0;
	int timeout = 3000;

	cv::Mat rawdata;

	g_width = width;
	g_height = height;


	while (true)
	{
		if (!preview)
			break;

		img_arr = new uint8_t[g_width * g_height * 3];

		ret = VcGetRawImageCapture(vzcam, img_arr, &size, timeout);

		if (ret != 0)
		{
			std::cout << "VcGetImageCapture Fail ErrCode: " << ret << std::endl;
			goto done;
		}

		switch (format)
		{
		case VZ_IMAGE_FORMAT::UYVY:
		case VZ_IMAGE_FORMAT::YUY2:
			rawdata = cv::Mat(height, width, CV_8UC2, img_arr);
			break;
		case VZ_IMAGE_FORMAT::NV12:
			rawdata = cv::Mat(height * 3 / 2, width, CV_8UC1, img_arr);
			break;
		default:
			rawdata = cv::Mat(1, size, CV_8UC1, img_arr);
		}

		if (rawdata.empty()) {
			break;
		}

		if (format == VZ_IMAGE_FORMAT::UYVY)
			cv::cvtColor(rawdata, frame, cv::COLOR_YUV2BGR_UYVY);
		else if (format == VZ_IMAGE_FORMAT::YUY2)
			cv::cvtColor(rawdata, frame, cv::COLOR_YUV2BGR_YUY2);
		else if (format == VZ_IMAGE_FORMAT::NV12)
			cv::cvtColor(rawdata, frame, cv::COLOR_YUV2BGR_NV12);
		else if (format == VZ_IMAGE_FORMAT::MJPG)
			cv::imdecode(rawdata, cv::IMREAD_COLOR, &frame);

		if (frame.empty()) {
			std::cout << "ERROR! blank frame grabbed\n";
			break;
		}

		cv::resize(frame, resize_frame, cv::Size(1280, 720));
		imshow("Live", resize_frame);

	done:
		if (waitKey(5) >= 0)
			break;

		if (img_arr != NULL)
		{
			delete[] img_arr;
			img_arr = NULL;
		}
		if (raw_arr != NULL)
		{
			delete[] raw_arr;
			raw_arr = NULL;
		}

	}
	cv::destroyWindow("Live");
}

int main()
{
	uint16_t startup;
	hs_vizionsdk = LoadLibrary(L"VizionSDK.dll");

	if (NULL == hs_vizionsdk)
	{
		printf("Load VizionSDK.dll Fail\n");
		return -1;
	}

	if (LoadVizionFunc() < 0) return -1;

	VcSetLogLevel(VzLogLevel::VZ_LOG_LEVEL_DEBUG);

	vzcam = VcCreateVizionCamDevice(); // Create control object for Vizion Camera

	VcGetVideoDeviceListWithLocation(vzcam, devlist);
	Sleep(1);

	if (devlist.size() == 0) { std::cout << "Cannot Find any Camera!" << std::endl;  return -1; }

	int i = 0;
	std::cout << "Select Camera" << std::endl;
	for (const auto& device : devlist) {
		std::wcout << "[" << i++ << "] " << L"Device Name: " << device.deviceName << L", Hardware ID: " << device.hardwareId << L", Serial Number: " << device.serialNumber.c_str() << std::endl;
	}
	std::cin >> startup;

	if (VcOpen(vzcam, startup) != 0) {
		std::cout << "Open Fail" << std::endl;
		return -1;
	}

	BYTE sensor_id[16] = { 0 };
	VcGetSensorUID(vzcam, sensor_id);
	std::cout << "Sensor UID: " << std::hex << std::uppercase << (int)sensor_id[0] << (int)sensor_id[1] << (int)sensor_id[2] << (int)sensor_id[3]
		<< (int)sensor_id[4] << (int)sensor_id[5] << (int)sensor_id[6] << (int)sensor_id[7] << (int)sensor_id[8] << (int)sensor_id[9] << (int)sensor_id[10]
		<< (int)sensor_id[11] << (int)sensor_id[12] << (int)sensor_id[13] << (int)sensor_id[14] << (int)sensor_id[15] << std::dec << std::endl;

	// select format
	int sel = 0;
	std::vector<VzFormat> vzformatlist;

	VcGetCaptureFormatList(vzcam, vzformatlist);

	for (int i = 0; i < vzformatlist.size(); i++)
		printf("[%d] Width=%d, Height=%d, Framerate=%d\n", i, vzformatlist[i].width, vzformatlist[i].height, vzformatlist[i].framerate);

	std::cout << "Select a Capture Format..Please enter the index of Format." << std::endl;

	std::cin >> sel;

	if (sel > vzformatlist.size() || sel < 0) {
		std::cout << "Select index " << sel << " Fail" << std::endl;
		return -1;
	}

	VcSetCaptureFormat(vzcam, vzformatlist[sel]);

	std::thread t1(showImage, vzformatlist[sel].width, vzformatlist[sel].height, vzformatlist[sel].format);
	t1.detach();

	// Test MJPEG QUAL
	uint16_t qual;

	VcGetJpegQual(vzcam, jpeg_qual);
	printf("Current JPEG Qual: %d\n", jpeg_qual);

	while (true) {
		std::cout << "Enter JPEG Qual: (1 ~ 255, set 0 to quit the program)" << std::endl;
		std::cin >> qual;

		jpeg_qual = (uint8_t)qual;

		if (qual == 0) { std::cout << "Leave qual setting program..." << std::endl;  break; }

		if (qual > 255 || qual < 1) { std::cout << "Just support Range: 1 ~ 255 !" << std::endl;  continue; }

		VcSetJpegQual(vzcam, jpeg_qual);
		VcGetJpegQual(vzcam, jpeg_qual);

		printf("Current JPEG Qual: %d\n", qual);
	}

	// Test Max FPS
	ae_mode = AE_MODE_STATUS::AUTO_EXP;
	VcSetAutoExposureMode(vzcam, ae_mode);
	uint16_t fps = 0;
	VcGetMaxFPS(vzcam, max_fps);
	printf("Get Max FPS: %d fps\n", max_fps);

	while (true) {
		std::cout << "Enter Max FPS : (set 0 to quit the program)" << std::endl;
		std::cin >> fps;

		max_fps = (uint8_t)fps;
		if (max_fps == 0) { std::cout << "Leave expoture setting program..." << std::endl;  break; }

		VcSetMaxFPS(vzcam, max_fps); // Set max fps
		VcGetMaxFPS(vzcam, max_fps);

		printf("Current Max FPS: %d fps\n", max_fps);
	}

	// Test Exposure Time
	VcGetCurrentExposureTime(vzcam, exp_time);
	printf("Get Current AE Exposure Time: %d us\n", exp_time);
	VcSetExposureTime(vzcam, exp_time);

	ae_mode = AE_MODE_STATUS::MANUAL_EXP;
	VcSetAutoExposureMode(vzcam, ae_mode);

	while (true) {
		std::cout << "Enter Expoture Time: (1 ~ 1,000,000 us, set 0 to quit the program)" << std::endl;
		std::cin >> exp_time;

		if (exp_time == 0) { std::cout << "Leave expoture setting program..." << std::endl;  break; }

		if (exp_time > 1000000 || exp_time < 1) { std::cout << "Just support Range: 1 ~ 1,000,000 us!" << std::endl;  continue; }

		VcSetExposureTime(vzcam, exp_time);

		VcGetExposureTime(vzcam, exp_time);
		printf("Current Exposure Time: %d ms\n", exp_time / 1000);
	}

	ae_mode = AE_MODE_STATUS::AUTO_EXP;
	VcSetAutoExposureMode(vzcam, ae_mode);

	// Test Zoom Target
	VcGetDigitalZoomTarget(vzcam, dz_tgt);
	printf("Get Digital Zoom Target: %f x\n", dz_tgt);

	while (true) {
		std::cout << "Enter Digital Zoom Target: (1 ~ 8x, set 0 to quit the program)" << std::endl;
		std::cin >> dz_tgt;

		if (dz_tgt == 0) { std::cout << "Leave zoom setting program..." << std::endl;  break; }

		if (dz_tgt > 8 || dz_tgt < 1) { std::cout << "Just support Range: 1 ~ 8x!" << std::endl;  continue; }

		VcSetDigitalZoomTarget(vzcam, dz_tgt);

		VcGetDigitalZoomTarget(vzcam, dz_tgt);
		printf("Current Digital Zoom Target: %f x\n", dz_tgt);
	}

	preview = false;

	Sleep(10);

	VcClose(vzcam);

	VcReleaseVizionCamDevice(vzcam);

	FreeLibrary(hs_vizionsdk);

	return 0;
}