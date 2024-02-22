# Steps to run the VizionSDK sample code:

## Download the sample code: 
Go to the [VizionSDK Sample](https://github.com/TechNexion-Vision/vizionsdk/Samples) download page and download the latest version of the Sample.
## Install the VizionSDK: 
Go to the [VizionSDK](https://github.com/TechNexion-Vision/vizionsdk) download page and download the latest version of the SDK.

Copy VizionSDK to the /usr/lib directory.
```
sudo cp -RP libVizionSDK.so* /usr/lib/
```
## Build the sample code: 
Open a terminal window and navigate to the Sample directory of the VizionSDK installation. Run the following command to build the sample code:
```
make
```
## Run the sample code: Run the following command to run the sample code:
```
./Sample
```

## Verify the results: 
The sample code will capture an image and save it to a file named image.raw in the Sample directory. You can verify that the sample code is working correctly by opening the image.raw file in an image viewer.

## Additional information:

For more information on the VizionSDK, please refer to the VizionSDK documentation: [VisionSDK documentation](https://developer.technexion.com/docs/vizionsdk-overview).

Note:

The steps above are for running the VizionSDK sample code on a Linux system. The steps for running the sample code on a Windows system are slightly different. Please refer to the [VizionSDK documentation](https://developer.technexion.com/docs/vizionsdk-overview) for more information.
