#include <iostream>
#include <dlfcn.h>
#include <vector>
#include <unistd.h>

using namespace std;

enum class AE_MODE_STATUS
{
  MANUAL_EXP = 0,
  AUTO_EXP = 0xC,
};

enum class AWB_MODE_STATUS
{
  MANUAL_TEMPERATURE_WB = 0x7,
  AUTO_WB = 0xF,
};

enum class DZ_MODE_STATUS
{
  DZ_IMMEDIATE = 0x8000,
  DZ_SLOW = 0x0040,
  DZ_FAST = 0x0200,
};

enum VZ_IMAGE_FORMAT
{
  NONE = 0,
  YUY2,
  UYVY,
  NV12,
  MJPG,
};

struct VzFormat
{
  uint8_t mediatype_idx;
  uint16_t width;
  uint16_t height;
  uint16_t framerate;
  VZ_IMAGE_FORMAT format;
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

class VizionCam;

typedef VizionCam *(*fpVcCreateVizionCamDevice)();
typedef int (*fpVcReleaseVizionCamDevice)(VizionCam *);
typedef int (*fpVcOpen)(VizionCam *, int dev_idx);
typedef int (*fpVcClose)(VizionCam *);
typedef void (*fpVcSetLogLevel)(VzLogLevel);
typedef int (*fpVcGetRawImageCapture)(VizionCam *, uint8_t *, int *size, uint16_t);
typedef int (*fpVcGetCaptureFormatList)(VizionCam *, std::vector<VzFormat> &);
typedef int (*fpVcSetCaptureFormat)(VizionCam *, VzFormat);

typedef int (*fpVcGetAutoExposureMode)(VizionCam *, AE_MODE_STATUS &);
typedef int (*fpVcSetAutoExposureMode)(VizionCam *, AE_MODE_STATUS);
typedef int (*fpVcGetExposureTime)(VizionCam *, uint32_t &);
typedef int (*fpVcSetExposureTime)(VizionCam *, uint32_t);
typedef int (*fpVcGetExposureGain)(VizionCam *, uint8_t &);
typedef int (*fpVcSetExposureGain)(VizionCam *, uint8_t);
typedef int (*fpVcGetMaxFPS)(VizionCam *, uint8_t &);
typedef int (*fpVcSetMaxFPS)(VizionCam *, uint8_t);
typedef int (*fpVcGetAutoWhiteBalanceMode)(VizionCam *, AWB_MODE_STATUS &);
typedef int (*fpVcSetAutoWhiteBalanceMode)(VizionCam *, AWB_MODE_STATUS);
typedef int (*fpVcGetTemperature)(VizionCam *, uint16_t &);
typedef int (*fpVcSetTemperature)(VizionCam *, uint16_t);

typedef int (*fpVcGetGamma)(VizionCam *, double &);
typedef int (*fpVcSetGamma)(VizionCam *, double);
typedef int (*fpVcGetSaturation)(VizionCam *, double &);
typedef int (*fpVcSetSaturation)(VizionCam *, double);
typedef int (*fpVcGetContrast)(VizionCam *, double &);
typedef int (*fpVcSetContrast)(VizionCam *, double);
typedef int (*fpVcGetSharpening)(VizionCam *, double &);
typedef int (*fpVcSetSharpening)(VizionCam *, double);
typedef int (*fpVcGetDenoise)(VizionCam *, double &);
typedef int (*fpVcSetDenoise)(VizionCam *, double);

typedef int (*fpGetDigitalZoomType)(VizionCam *, DZ_MODE_STATUS &);
typedef int (*fpSetDigitalZoomType)(VizionCam *, DZ_MODE_STATUS);
typedef int (*fpGetDigitalZoomTarget)(VizionCam *, double &);
typedef int (*fpSetDigitalZoomTarget)(VizionCam *, double);

typedef int (*fpVcGetVideoDeviceList)(VizionCam *, std::vector<std::string> &);
typedef int (*fpVcGetVizionCamDeviceName)(VizionCam *, char *);
typedef int (*fpVcGetUniqueSensorID)(VizionCam *, char *);
typedef int (*fpVcGetUSBFirmwareVersion)(VizionCam *, char *);
typedef int (*fpVcGetTEVSFirmwareVersion)(VizionCam *, char *);

typedef int (*fpVcGotoSaveOSPProfile)(VizionCam *, uint8_t);
typedef int (*fpVcCheckOSPProfile)(VizionCam *);

fpVcCreateVizionCamDevice VcCreateVizionCamDevice;
fpVcReleaseVizionCamDevice VcReleaseVizionCamDevice;
fpVcOpen VcOpen;
fpVcClose VcClose;
fpVcSetLogLevel VcSetLogLevel;
fpVcGetRawImageCapture VcGetRawImageCapture;
fpVcGetCaptureFormatList VcGetCaptureFormatList;
fpVcSetCaptureFormat VcSetCaptureFormat;

// AE.AWB
fpVcGetAutoExposureMode VcGetAutoExposureMode;
fpVcSetAutoExposureMode VcSetAutoExposureMode;
fpVcGetExposureTime VcGetExposureTime;
fpVcSetExposureTime VcSetExposureTime;
fpVcGetExposureGain VcGetExposureGain;
fpVcSetExposureGain VcSetExposureGain;
fpVcGetMaxFPS VcGetMaxFPS;
fpVcSetMaxFPS VcSetMaxFPS;
fpVcGetAutoWhiteBalanceMode VcGetAutoWhiteBalanceMode;
fpVcSetAutoWhiteBalanceMode VcSetAutoWhiteBalanceMode;
fpVcGetTemperature VcGetTemperature;
fpVcSetTemperature VcSetTemperature;
// Tunning
fpVcGetGamma VcGetGamma;
fpVcSetGamma VcSetGamma;
fpVcGetSaturation VcGetSaturation;
fpVcSetSaturation VcSetSaturation;
fpVcGetContrast VcGetContrast;
fpVcSetContrast VcSetContrast;
fpVcGetSharpening VcGetSharpening;
fpVcSetSharpening VcSetSharpening;
fpVcGetDenoise VcGetDenoise;
fpVcSetDenoise VcSetDenoise;

// Digital Zoom
fpGetDigitalZoomType VcGetDigitalZoomType;
fpSetDigitalZoomType VcSetDigitalZoomType;
fpGetDigitalZoomTarget VcGetDigitalZoomTarget;
fpSetDigitalZoomTarget VcSetDigitalZoomTarget;

fpVcGetVideoDeviceList VcGetVideoDeviceList;
fpVcGetVizionCamDeviceName VcGetVizionCamDeviceName;
fpVcGetUniqueSensorID VcGetUniqueSensorID;
fpVcGetUSBFirmwareVersion VcGetUSBFirmwareVersion;
fpVcGetTEVSFirmwareVersion VcGetTEVSFirmwareVersion;

fpVcGotoSaveOSPProfile VcGotoSaveOSPProfile;
fpVcCheckOSPProfile VcCheckOSPProfile;

void *hs_vizionsdk;
AE_MODE_STATUS ae_mode;
AWB_MODE_STATUS awb_mode;
uint8_t max_fps = 0;
uint8_t exp_gain = 0;
uint32_t exp_time = 0;
uint16_t wb_temp = 0;

/**
 * Loads a function from a dynamic library and assigns it to a function pointer.
 *
 * @param fp reference to the function pointer to be assigned
 * @param func_name name of the function to be loaded
 *
 * @return 0 if the function is loaded successfully, -1 otherwise
 *
 * @throws None
 */
template <typename FP>
int LoadVcFunc(FP &fp, std::string func_name)
{
  fp = (FP)dlsym(hs_vizionsdk, func_name.c_str());
  if (NULL == fp)
  {
    printf("Load %s Fail\n", func_name.c_str());
    return -1;
  }
  return 0;
}

/**
 * Loads the Vizion function and its associated functions.
 *
 * @return 0 if successful, -1 if an error occurs
 */
int LoadVizionFunc()
{
  std::cout << "++++LoadVizionFunc++++" << std::endl;
  std::string libname("libVizionSDK.so");
  hs_vizionsdk = dlopen(libname.c_str(), RTLD_LAZY);
  const char *error = dlerror();
  if (error != nullptr)
  {
    std::cout << "dlopen error: " << error << std::endl;
  }
  if (LoadVcFunc(VcCreateVizionCamDevice, "VcCreateVizionCamDevice"))
    return -1;
  if (LoadVcFunc(VcReleaseVizionCamDevice, "VcReleaseVizionCamDevice"))
    return -1;
  if (LoadVcFunc(VcOpen, "VcOpen"))
    return -1;
  if (LoadVcFunc(VcClose, "VcClose"))
    return -1;
  if (LoadVcFunc(VcSetLogLevel, "VcSetLogLevel"))
    return -1;
  if (LoadVcFunc(VcGetRawImageCapture, "VcGetRawImageCapture"))
    return -1;
  if (LoadVcFunc(VcGetCaptureFormatList, "VcGetCaptureFormatList"))
    return -1;
  if (LoadVcFunc(VcSetCaptureFormat, "VcSetCaptureFormat"))
    return -1;

  if (LoadVcFunc(VcGetAutoExposureMode, "VcGetAutoExposureMode"))
    return -1;
  if (LoadVcFunc(VcSetAutoExposureMode, "VcSetAutoExposureMode"))
    return -1;
  if (LoadVcFunc(VcGetExposureTime, "VcGetExposureTime"))
    return -1;
  if (LoadVcFunc(VcSetExposureTime, "VcSetExposureTime"))
    return -1;
  if (LoadVcFunc(VcGetExposureGain, "VcGetExposureGain"))
    return -1;
  if (LoadVcFunc(VcSetExposureGain, "VcSetExposureGain"))
    return -1;
  if (LoadVcFunc(VcGetMaxFPS, "VcGetMaxFPS"))
    return -1;
  if (LoadVcFunc(VcSetMaxFPS, "VcSetMaxFPS"))
    return -1;
  if (LoadVcFunc(VcGetAutoWhiteBalanceMode, "VcGetAutoWhiteBalanceMode"))
    return -1;
  if (LoadVcFunc(VcSetAutoWhiteBalanceMode, "VcSetAutoWhiteBalanceMode"))
    return -1;
  if (LoadVcFunc(VcGetTemperature, "VcGetTemperature"))
    return -1;
  if (LoadVcFunc(VcSetTemperature, "VcSetTemperature"))
    return -1;

  if (LoadVcFunc(VcGetGamma, "VcGetGamma"))
    return -1;
  if (LoadVcFunc(VcSetGamma, "VcSetGamma"))
    return -1;
  if (LoadVcFunc(VcGetSaturation, "VcGetSaturation"))
    return -1;
  if (LoadVcFunc(VcSetSaturation, "VcSetSaturation"))
    return -1;
  if (LoadVcFunc(VcGetContrast, "VcGetContrast"))
    return -1;
  if (LoadVcFunc(VcSetContrast, "VcSetContrast"))
    return -1;
  if (LoadVcFunc(VcGetSharpening, "VcGetSharpening"))
    return -1;
  if (LoadVcFunc(VcSetSharpening, "VcSetSharpening"))
    return -1;
  if (LoadVcFunc(VcGetDenoise, "VcGetDenoise"))
    return -1;
  if (LoadVcFunc(VcSetDenoise, "VcSetDenoise"))
    return -1;

  if (LoadVcFunc(VcGetDigitalZoomType, "VcGetDigitalZoomType"))
    return -1;
  if (LoadVcFunc(VcSetDigitalZoomType, "VcSetDigitalZoomType"))
    return -1;
  if (LoadVcFunc(VcGetDigitalZoomTarget, "VcGetDigitalZoomTarget"))
    return -1;
  if (LoadVcFunc(VcSetDigitalZoomTarget, "VcSetDigitalZoomTarget"))
    return -1;

  if (LoadVcFunc(VcGetVideoDeviceList, "VcGetVideoDeviceList"))
    return -1;
  if (LoadVcFunc(VcGetVizionCamDeviceName, "VcGetVizionCamDeviceName"))
    return -1;
  if (LoadVcFunc(VcGetUniqueSensorID, "VcGetUniqueSensorID"))
    return -1;
  if (LoadVcFunc(VcGetUSBFirmwareVersion, "VcGetUSBFirmwareVersion"))
    return -1;
  if (LoadVcFunc(VcGetTEVSFirmwareVersion, "VcGetTEVSFirmwareVersion"))
    return -1;

  if (LoadVcFunc(VcCheckOSPProfile, "VcCheckOSPProfile"))
    return -1;
  if (LoadVcFunc(VcGotoSaveOSPProfile, "VcGotoSaveOSPProfile"))
    return -1;

  std::cout << "++++LoadVizionFunc  finish++++" << std::endl;
  return 0;
}

int main()
{
  if (LoadVizionFunc() < 0)
    return -1;

  VizionCam *vizionCam = VcCreateVizionCamDevice();

  // Set log level
  VzLogLevel logLevel = VzLogLevel::VZ_LOG_LEVEL_OFF;
  VcSetLogLevel(logLevel);

  int count = 0;
  std::vector<std::string> deviceNames;
  VcGetVideoDeviceList(vizionCam, deviceNames);
  if (deviceNames.empty())
  {
    std::cout << "No camera connected." << std::endl;
    return -1;
  }
  for (const auto &deviceName : deviceNames)
  {
    std::cout << "[" << count++ << "] " << deviceName << std::endl;
  }

  int index = -1;
  std::cout << "Please select camera: " << std::endl;
  std::cin >> index;
  if (index >= 0 && index < deviceNames.size())
  {
    if (VcOpen(vizionCam, index) < 0)
    {
      std::cout << "Error opening camera" << std::endl;
      return -1;
    }
    std::cout << "Opening camera finish" << std::endl;
  }

  char vizionDeviceName[256];
  VcGetVizionCamDeviceName(vizionCam, vizionDeviceName);
  std::cout << "Device Name: " << vizionDeviceName << std::endl;

  std::vector<VzFormat> captureFormats;
  VcGetCaptureFormatList(vizionCam, captureFormats);

  count = 0;
  for (const auto &format : captureFormats)
  {
    std::cout << "[" << count++ << "] " << format.width << " x " << format.height << " framerate: " << format.framerate << std::endl;
  }

  index = -1;
  std::cout << "Please select format: " << std::endl;
  std::cin >> index;
  VzFormat chosenFormat;
  if (index >= 0 && index < captureFormats.size())
  {
    chosenFormat = captureFormats[index];
  }

  if (VcSetCaptureFormat(vizionCam, chosenFormat) != 0)
    return -1;

  int dataSize = 0;
  int timeout = 3000;
  uint8_t *imageData = new uint8_t[chosenFormat.width * chosenFormat.height * 3];

  if (VcGetRawImageCapture(vizionCam, imageData, &dataSize, timeout) != 0)
  {
    std::cout << "Capture image fail" << std::endl;
  }
  std::cout << "Image data size: " << dataSize << std::endl;

  FILE *file = fopen("image.raw", "wb");
  fwrite(imageData, dataSize, 1, file);
  fclose(file);
  std::cout << "Save image finish" << std::endl;

  delete[] imageData;

  AE_MODE_STATUS aeMode = AE_MODE_STATUS::MANUAL_EXP;
  VcSetAutoExposureMode(vizionCam, aeMode);
  VcGetAutoExposureMode(vizionCam, aeMode);
  if (aeMode == AE_MODE_STATUS::AUTO_EXP)
    printf("Get AE Mode: AUTO_EXP\n");
  else if (aeMode == AE_MODE_STATUS::MANUAL_EXP)
    printf("Get AE Mode: MANUAL_EXP\n");

  uint8_t maxFps = 60;
  VcSetMaxFPS(vizionCam, maxFps);
  VcGetMaxFPS(vizionCam, maxFps);
  printf("Get Max FPS: %d\n", maxFps);

  VcSetExposureTime(vizionCam, 16667);
  uint32_t expTime;
  VcGetExposureTime(vizionCam, expTime);
  printf("Get AE Exposure Time: %d us\n", expTime);

  VcSetExposureGain(vizionCam, 3);
  uint8_t expGain;
  VcGetExposureGain(vizionCam, expGain);
  printf("Get Exp Gain: %d\n", expGain);

  auto awbMode = AWB_MODE_STATUS::MANUAL_TEMPERATURE_WB;
  VcSetAutoWhiteBalanceMode(vizionCam, awbMode);
  VcGetAutoWhiteBalanceMode(vizionCam, awbMode);

  VcClose(vizionCam);
  std::cout << "Close the camera" << std::endl;
  return 0;
}
