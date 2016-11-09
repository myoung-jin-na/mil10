Running this example will show how to convert the output of a SICK RangerC 
camera to the expected input format of the MIL 3D reconstruction module. 
Camera output images have been saved on disk and will be reloaded, then 
converted to MIL format and provided to the 3D reconstruction module to 
generate the depth map of an object.

To run the example using an actual SICK RangerC camera, open cameraconfig.h 
and set USE_REAL_CAMERA to 1. Also change other defines there, if necessary 
(especially CAMERA_COM_PORT). Note that you will need to install the camera 
SDK to be able to link the program with USE_REAL_CAMERA set to 1 (see 
sickcamera_rangerc.cpp for the linker pragma).

Here is the configuration on which this example has been tested:
- 32 bits Windows OS
- Compiler: Microsoft Visual Studio 2005
- Frame grabber: Matrox Solios eCL/XCL (CameraLink)
- SICK iCon SDK (3D Camera Development Software): 3D Camera v3.2.0.17
- Matrox Imaging Library: MIL 9.0 Processing Pack 2

Note that, after installing the camera SDK, you will also need to add the 
paths to the include files and libraries in Visual Studio:
- Include files: C:\SICKIVP\3D Cameras\icon\inc
- Library files: C:\SICKIVP\3D Cameras\icon\lib
(you may need to change these paths to reflect your software installation)
