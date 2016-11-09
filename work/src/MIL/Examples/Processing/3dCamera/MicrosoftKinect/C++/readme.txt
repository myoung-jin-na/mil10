Running this example will show how to interface the Microsoft Kinect to 
get the depth and color images and work with them using MIL to get the
3d world points, map the color on those 3d world point and generate depth
maps. The camera output images have been saved on disk and will be reloaded.
Note that the standalone mode of this example uses Windows multimedia
timer(Winmm.lib) and thus can only be run on Windows.

To run the example using an actual Microsoft Kinect camera, open 
KinectCameraInfterface.h and set USE_REAL_CAMERA to 1. Note that you will need to 
install the camera SDK to be able to link the program with USE_REAL_CAMERA set to 1.

Here is the configuration on which this example has been tested:
- 64 bits Windows OS
- Compiler: Microsoft Visual Studio 2010
- Microsoft Kinect SDK v1.6
- Matrox Imaging Library: MIL 10.0 