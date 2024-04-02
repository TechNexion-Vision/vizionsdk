#pragma once

enum VZ_IMAGE_FORMAT
{
	NONE = 0,
	YUY2,
	UYVY,
	NV12,
	MJPG,
};

enum class AE_MODE_STATUS
{
	MANUAL_EXP = 0,
	AUTO_EXP = 0xC,
};

enum class AWB_MODE_STATUS
{
	MANUAL_QXQY_WB = 0,
	MANUAL_TEMPERATURE_WB = 0x7,
	AUTO_WB = 0xF,
};

enum class DZ_MODE_STATUS
{
	DZ_IMMEDIATE = 0x8000,
	DZ_SLOW = 0x0040,
	DZ_FAST = 0x0200,
};

enum class DZ_TARGET_STATUS
{
	DZ_TGT_1X = 1,
	DZ_TGT_2X = 2,
	DZ_TGT_4X = 4,
	DZ_TGT_NO_SCALING = 0xFF,
};

enum class FLIP_MODE
{
	FLIP_NORMAL = 0,
	FLIP_H_MIRROR,
	FLIP_V_MIRROR,
	FLIP_ROTATE_180,
};

enum class EFFECT_MODE
{
	NORMAL_MODE = 0x00,
	BLACK_WHITE_MODE = 0x03,
	GRAYSCALE_MODE = 0x06,
	NEGATIVE_MODE = 0x07,
	SKETCH_MODE = 0x0F,
};

enum class FLICK_MODE {
	DISABLE = 0x0000,
	_50HZ = 0x3201,
	_60HZ = 0x3C01,
	AUTO = 0x000E,
};

struct VzFormat {
	uint8_t mediatype_idx;
	uint16_t width;
	uint16_t height;
	uint16_t framerate;
	VZ_IMAGE_FORMAT format;
};

class VizionCam;

#ifndef _VIZIONSDK_FP
typedef VizionCam* (*fpVcCreateVizionCamDevice)();
typedef int (*fpVcReleaseVizionCamDevice)(VizionCam*);
typedef int (*fpVcOpen)(VizionCam*, int dev_idx);
typedef int (*fpVcClose)(VizionCam*);
typedef int(*fpVcGetVideoDeviceList)(VizionCam*, std::vector<std::pair<std::wstring, std::wstring>>&);
typedef int (*fpVcGetVizionCamDeviceName)(VizionCam*, wchar_t*);

typedef int (*fpVcGetCaptureFormatList)(VizionCam*, std::vector<VzFormat>&);
typedef int (*fpVcSetCaptureFormat)(VizionCam*, VzFormat);
typedef int (*fpVcGetRawImageCapture)(VizionCam*, uint8_t*, int* size, uint16_t);

typedef int (*fpVcGetUniqueSensorID)(VizionCam*, char*);
typedef int (*fpVcGetSensorUID)(VizionCam*, BYTE*);

typedef int (*fpVcResetUVC)(VizionCam*);
typedef int (*fpVcReadTimeStamp)(VizionCam*, uint32_t&, uint32_t&);

typedef int (*fpVcGetMaxFPS)(VizionCam*, uint8_t&);
typedef int (*fpVcSetMaxFPS)(VizionCam*, uint8_t);
typedef int (*fpVcGetAutoExposureMode)(VizionCam*, AE_MODE_STATUS&);
typedef int (*fpVcSetAutoExposureMode)(VizionCam*, AE_MODE_STATUS);
typedef int (*fpVcGetExposureTime)(VizionCam*, uint32_t&);
typedef int (*fpVcSetExposureTime)(VizionCam*, uint32_t);
typedef int (*fpVcGetExposureGain)(VizionCam*, uint8_t&);
typedef int (*fpVcSetExposureGain)(VizionCam*, uint8_t);
typedef int (*fpVcGetCurrentExposureTime)(VizionCam*, uint32_t&);
typedef int (*fpVcGetCurrentExposureGain)(VizionCam*, uint8_t&);
typedef int (*fpVcGetBacklightCompensation)(VizionCam*, double&);
typedef int (*fpVcSetBacklightCompensation)(VizionCam*, double);

typedef int (*fpVcGetAutoWhiteBalanceMode)(VizionCam*, AWB_MODE_STATUS&);
typedef int (*fpVcSetAutoWhiteBalanceMode)(VizionCam*, AWB_MODE_STATUS);
typedef int (*fpVcGetTemperature)(VizionCam*, uint16_t&);
typedef int (*fpVcSetTemperature)(VizionCam*, uint16_t);

typedef int (*fpVcGetBrightness)(VizionCam*, double&);
typedef int (*fpVcSetBrightness)(VizionCam*, double);
typedef int (*fpVcGetGamma)(VizionCam*, double&);
typedef int (*fpVcSetGamma)(VizionCam*, double);
typedef int (*fpVcGetSaturation)(VizionCam*, double&);
typedef int (*fpVcSetSaturation)(VizionCam*, double);
typedef int (*fpVcGetContrast)(VizionCam*, double&);
typedef int (*fpVcSetContrast)(VizionCam*, double);
typedef int (*fpVcGetSharpening)(VizionCam*, double&);
typedef int (*fpVcSetSharpening)(VizionCam*, double);
typedef int (*fpVcGetDenoise)(VizionCam*, double&);
typedef int (*fpVcSetDenoise)(VizionCam*, double);

typedef int (*fpVcGetFlipMode)(VizionCam*, FLIP_MODE&);
typedef int (*fpVcSetFlipMode)(VizionCam*, FLIP_MODE);
typedef int (*fpVcGetEffectMode)(VizionCam*, EFFECT_MODE&);
typedef int (*fpVcSetEffectMode)(VizionCam*, EFFECT_MODE);

typedef int (*fpGetDigitalZoomType)(VizionCam*, DZ_MODE_STATUS&);
typedef int (*fpSetDigitalZoomType)(VizionCam*, DZ_MODE_STATUS);
typedef int (*fpGetDigitalZoomTarget)(VizionCam*, double&);
typedef int (*fpSetDigitalZoomTarget)(VizionCam*, double);

typedef int (*fpVcGetFlickMode)(VizionCam*, FLICK_MODE&);
typedef int (*fpVcSetFlickMode)(VizionCam*, FLICK_MODE);

typedef int (*fpVcGetJpegQual)(VizionCam*, uint8_t&);
typedef int (*fpVcSetJpegQual)(VizionCam*, uint8_t);

fpVcCreateVizionCamDevice VcCreateVizionCamDevice;
fpVcReleaseVizionCamDevice VcReleaseVizionCamDevice;
fpVcOpen VcOpen;
fpVcClose VcClose;
fpVcGetVideoDeviceList VcGetVideoDeviceList;
fpVcGetVizionCamDeviceName VcGetVizionCamDeviceName;

fpVcGetCaptureFormatList VcGetCaptureFormatList;
fpVcSetCaptureFormat VcSetCaptureFormat;
fpVcGetRawImageCapture VcGetRawImageCapture;

fpVcGetUniqueSensorID VcGetUniqueSensorID;
fpVcGetSensorUID VcGetSensorUID;

fpVcResetUVC VcResetUVC;
fpVcReadTimeStamp VcReadTimeStamp;

// AE.AWB
fpVcGetMaxFPS VcGetMaxFPS;
fpVcSetMaxFPS VcSetMaxFPS;
fpVcGetAutoExposureMode VcGetAutoExposureMode;
fpVcSetAutoExposureMode VcSetAutoExposureMode;
fpVcGetExposureTime VcGetExposureTime;
fpVcSetExposureTime VcSetExposureTime;
fpVcGetExposureGain VcGetExposureGain;
fpVcSetExposureGain VcSetExposureGain;
fpVcGetCurrentExposureTime VcGetCurrentExposureTime;
fpVcGetCurrentExposureGain VcGetCurrentExposureGain;
fpVcGetBacklightCompensation VcGetBacklightCompensation;
fpVcSetBacklightCompensation VcSetBacklightCompensation;
fpVcGetAutoWhiteBalanceMode VcGetAutoWhiteBalanceMode;
fpVcSetAutoWhiteBalanceMode VcSetAutoWhiteBalanceMode;
fpVcGetTemperature VcGetTemperature;
fpVcSetTemperature VcSetTemperature;
// Tunning
fpVcGetBrightness    VcGetBrightness;
fpVcSetBrightness    VcSetBrightness;
fpVcGetGamma		 VcGetGamma;
fpVcSetGamma		 VcSetGamma;
fpVcGetSaturation    VcGetSaturation;
fpVcSetSaturation    VcSetSaturation;
fpVcGetContrast      VcGetContrast;
fpVcSetContrast      VcSetContrast;
fpVcGetSharpening    VcGetSharpening;
fpVcSetSharpening    VcSetSharpening;
fpVcGetDenoise		 VcGetDenoise;
fpVcSetDenoise		 VcSetDenoise;

fpVcGetFlipMode VcGetFlipMode;
fpVcSetFlipMode VcSetFlipMode;
fpVcGetEffectMode VcGetEffectMode;
fpVcSetEffectMode VcSetEffectMode;

// Digital Zoom
fpGetDigitalZoomType   VcGetDigitalZoomType;
fpSetDigitalZoomType   VcSetDigitalZoomType;
fpGetDigitalZoomTarget VcGetDigitalZoomTarget;
fpSetDigitalZoomTarget VcSetDigitalZoomTarget;

// FLICK Ctrl
fpVcGetFlickMode VcGetFlickMode;
fpVcSetFlickMode VcSetFlickMode;

// JPEG Qual
fpVcGetJpegQual VcGetJpegQual;
fpVcSetJpegQual VcSetJpegQual;

#endif