# VizionSDK
VizionSDK is a cross-platform software development kit (SDK) for Windows and Linux, designed to support TechNexion AR Series Cameras across platforms such as Windows 10/11, Ubuntu, Yocto, NXP i.MX, and NVIDIA Jetson. It enables developers and companies to easily integrate, control, and build custom applications using these cameras.

## What is included in the repository
- `samples/` : C++ samples project source code.
- `config/` : A config file of TechNexion Camera series for VizionSDK to recognize the devices.

## Documentation
- [Install VizionSDK-C++](https://developer.technexion.com/docs/vizionsdk-cpp-installation)
- [Install VizionSDK-Python](https://developer.technexion.com/docs/vizionsdk-python-installation)
- [VizionSDK Documentation](https://developer.technexion.com/docs/vizionsdk-overview)
- [VizionSDK API User Guide](https://developer.technexion.com/docs/vizionsdk-api-log-file-setting)

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
- **pyvizion-ctl** – uses the Python `pyvizionsdk`
##### (Note: pyvizion-ctl currently does not support firmware updates.)

## VizionViewer™

[VizionViewer™](https://developer.technexion.com/docs/vizionviewer-overview) is a cross-platform camera software for video streaming and image capturing from camera devices with user-friendly Graphical User Interface based on the **VizionSDK**.

![VizionViewer GUI interface](./doc/resources/VizionViewer.png)

## Related links
- [TechNexion Main Page](https://www.technexion.com/)
- [TechNexion Github](https://github.com/TechNexion)
