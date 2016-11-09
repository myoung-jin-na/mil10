Running this example will show how to convert the output of a SICK RangerE 
camera to the expected input format of the MIL 3D reconstruction module. 
Camera output images have been saved on disk and will be reloaded, then 
converted to MIL format and provided to the 3D reconstruction module to 
generate a depth map and color map of an object.

To run the example using an actual SICK RangerE camera, open cameraconfig.h 
and set USE_REAL_CAMERA to 1. Also change other defines there, if necessary 
(especially CAMERA_IP). Note that you will need to install the camera 
SDK to be able to link the program with USE_REAL_CAMERA set to 1. The SDK
installation will define the environnement variable IVP_3D_CAMERA and the linking
will be done automatically by the icon_auto_link.h header included in 
sickcamera_rangere.

Here is the configuration on which this example has been tested:
- 64 bits Windows OS
- Compiler: Microsoft Visual Studio 2010
- SICK iCon SDK (3D Camera Development Software): 3D Camera v4.3.0.108
- Matrox Imaging Library: MIL 10.0 

