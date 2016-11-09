Running this example will show how to convert the output of a PhotonFocus 
MV-D1024E-3D01-160-CL camera to the expected input format of the MIL 3D 
reconstruction module. Camera output images have been saved on disk and will 
be reloaded, then converted to MIL format and provided to the 3D 
reconstruction module to generate the depth map of an object.

To run the example using an actual PhotonFocus camera, open cameraconfig.h 
and set USE_REAL_CAMERA to 1. Also change other defines there, if necessary 
(especially CAMERA_COM_PORT). Note that you will need to install the camera 
SDK to be able to link the program with USE_REAL_CAMERA set to 1 (see 
pfocuscamera.cpp for the linker pragma).

Here is the configuration on which this example has been tested:
- 32 bits Windows OS
- Compiler: Microsoft Visual Studio 2005
- Frame grabber: Matrox Solios eCL/XCL (CameraLink)
- PhotonFocus Camera Software: PFInstaller 2.7
- Matrox Imaging Library: MIL 9.0 Processing Pack 2

Note that, after installing the camera SDK, you will also need to add the 
paths to the include files and libraries in Visual Studio:
- Include files: C:\Program Files\Photonfocus\PFRemote\SDK\include
- Library files: C:\Program Files\Photonfocus\PFRemote\SDK\lib
(you may need to change these paths to reflect your software installation)

Some DLLs must also be accessible by Windows to run the executable. Either add 
C:\Program Files\Photonfocus\PFRemote to the PATH environment variable or 
copy the following DLLs alongside the executable (or in C:\WINDOWS\system32):
- C:\Program Files\Photonfocus\PFRemote\pfcam.dll
- C:\Program Files\Photonfocus\PFRemote\comdll.dll
- C:\Program Files\Photonfocus\PFRemote\mv_d1024e_3d01_160.dll
