# VizionViewer™

VizionViewer™ is a cross-platform camera software for video streaming and image capturing from camera devices with user-friendly Graphical User Interface based on the **VizionSDK**.

## Supported TechNexion Cameras

### MIPI Cameras

- TEVS-AR0144
- TEVS-AR0234
- TEVS-AR0521
- TEVS-AR0522
- TEVS-AR0821
- TEVS-AR0822
- TEVS-AR1335
- TEVI-AR0144 (NRND)
- TEVI-AR0234 (NRND)
- TEVI-AR0521 (NRND)
- TEVI-AR0522 (NRND)
- TEVI-AR0821 (NRND)
- TEVI-AR0822 (NRND)
- TEVI-AR1335 (NRND)

### FPD-LinkIII Cameras

- VLS-AR0144
- VLS-AR0234
- VLS-AR0521
- VLS-AR0522
- VLS-AR0821
- VLS-AR0822
- VLS-AR1335

### UVC Cameras

- VCI-AR0144
- VCI-AR0234
- VCI-AR0521
- VCI-AR0522
- VCI-AR0821
- VCI-AR0822
- VCI-AR1335

## How to Install VizionViewer™

### Windows

1. Download the [VizionViewer™ installer](https://download.technexion.com/vizionviewer/windows_x64/) for Windows.
2. Once it is downloaded, run the installer (vizionviewersetup_{version}.exe).
3. By default, VizionViewer™ is installed under C:\Program Files\VizionViewer.

### Linux

#### Offline Installation

1. Download the [.deb packages](https://download.technexion.com/vizionviewer/linux_x64/) for Ubuntu.
2. Uncompress and install .deb packages, You can do this either through the graphical software center if available or via the command line using:

```shell
tar -xf ./vizionviewersetup_{version}_amd64.tar.xz
sudo apt install ./vizionsdk.deb
sudo apt install ./vizionviewer.deb
```

3. Installing the .deb package will automatically install the apt repository and signing key to enable auto-updating using the system's package manager.

##### Online Installation

The repository and key can also be installed manually with the following script:

```shell
sudo apt-get install wget gpg
wget -qO- https://download.technexion.com/apt/technexion.asc | gpg --dearmor > packages.technexion.gpg
sudo install -D -o root -g root -m 644 packages.technexion.gpg /etc/apt/keyrings/packages.technexion.gpg
sudo sh -c 'echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/packages.technexion.gpg] https://download.technexion.com/apt/vizionsdk/ stable main" > /etc/apt/sources.list.d/vizionsdk.list'
sudo sh -c 'echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/packages.technexion.gpg] https://download.technexion.com/apt/vizionviewer/ stable main" >> /etc/apt/sources.list.d/vizionsdk.list'
```
Then update the package cache and install the package using:

```shell
sudo apt update
sudo apt install vizionsdk vizionviewer
```

## More Information about VizionViewer

click here !! [VizionViewer™ Overview](https://developer.technexion.com/docs/installation)
