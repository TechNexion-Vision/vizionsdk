# VizionSDK
VizionSDK is a software development kit designed for Windows and Linux platforms, supporting Technexion AR Series Cameras on various platforms like Windows 10/11, Ubuntu, Yocto, NXP-i.MX, and NVIDIA Jetson. It empowers companies and individuals to easily control these cameras and develop custom applications for diverse purposes.

## What is included in the repository
- Samples : C/C++ samples project source code.
- config : A config file of TechNexion Camera series for VizionSDK to recognize the devices.

## Supported TechNexion Cameras

| Sensor | MIPI           | FPD-LinkIII      | UVC            |
|--------|----------------|------------------|----------------|
| AR0144 | ✓ TEVS-AR0144  | ✓ VLS3-AR0144    | ✓ VCI-AR0144   |
| AR0145 | ✓ TEVS-AR0145  | ✓ VLS3-AR0145    |                |
| AR0234 | ✓ TEVS-AR0234  | ✓ VLS3-AR0234    | ✓ VCI-AR0234   |
| AR0521 | ✓ TEVS-AR0521  | ✓ VLS3-AR0521    | ✓ VCI-AR0521   |
| AR0522 | ✓ TEVS-AR0522  | ✓ VLS3-AR0522    | ✓ VCI-AR0522   |
| AR0821 | ✓ TEVS-AR0821  | ✓ VLS3-AR0821    | ✓ VCI-AR0821   |
| AR0822 | ✓ TEVS-AR0822  | ✓ VLS3-AR0822    | ✓ VCI-AR0822   |
| AR1335 | ✓ TEVS-AR1335  | ✓ VLS3-AR1335    | ✓ VCI-AR1335   |

## Supported Platforms

#### [Windows x64](https://developer.technexion.com/docs/vizionsdk-cpp-installation#windowsx64)
- Windows 10 and 11 (64-bit)
#### [Linux x64](https://developer.technexion.com/docs/vizionsdk-cpp-installation#linuxx64)
- Ubuntu Desktop 20.04 and 22.04 (64-bit)
- UP Squared Pro 7000
#### [Linux ARM64](https://developer.technexion.com/docs/vizionsdk-cpp-installation#linuxarm64)
- NVIDIA Jetson (JetPack 5 or later)
- NXP-i.MX8MM, NXP-i.MX8MQ, NXP-i.MX8MP, NXP-i.MX93, NXP-i.MX95
- TI-TDA4VM

## vizion-ctl

[vizion-ctl](https://developer.technexion.com/docs/vizion-ctl) is a cross-platform command-line tool for controlling TechNexion cameras.

It supports camera parameter control, information retrieval, and firmware updates via terminal commands.

Two versions are available:
- **vizion-ctl** – uses the C++ `VizionSDK`
- **pyvizion-ctl** – uses the Python `pyvizionsdk`(no firmware update support)
  
## VizionViewer™

[VizionViewer™](https://developer.technexion.com/docs/vizionviewer-overview) is a cross-platform camera software for video streaming and image capturing from camera devices with user-friendly Graphical User Interface based on the **VizionSDK**.

![output](./doc/resources/VizionViewer.png)


## Documents
- Github Pages: https://github.com/TechNexion
- VizionSDK Overview: https://developer.technexion.com/docs/vizionsdk-overview
- VizionSDK Installation: https://developer.technexion.com/docs/vizionsdk-cpp-installation
- VizionSDK API User Guide: https://developer.technexion.com/docs/vizionsdk-api-log-file-setting
- VizionViewer Overview: https://developer.technexion.com/docs/vizionviewer-overview

## Related links
- [Technexion Main Page](https://www.technexion.com/)
