//***************************************************************************************/
// 
// File name: Kinect.cpp  
//
// Synopsis:  This program demonstrate how to interface the Microsoft Kinect camera to create
//            depth maps in MIL.
//            See the PrintHeader() function below for detailed description.
//

#include <mil.h>
#include <Windows.h>
#include <float.h>
#include <math.h>
#include "KinectCamera.h"
#include "KinectCameraStandalone.h"
#include "McalTransformCoordinate3dListMP.h"
#include "BestPlaneFitter.h"
#include "DepthDataMgr.h"

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
/* Include DirectX display only on Windows. */
#include "MdispD3D.h"

/* D3D display parameters */
static const MIL_INT D3D_DISPLAY_SIZE_X = 640;
static const MIL_INT D3D_DISPLAY_SIZE_Y = 480;
static const MIL_DOUBLE MIN_Z           = 0;
static const MIL_DOUBLE MAX_Z           = 3000; // 3000 mm is the maximum Z in near mode
static const MIL_DOUBLE MAX_DISTANCE_Z  = 100;  // 3d points with distance > 100mm are not linked in the 3d display  
#endif

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("Microsoft Kinect\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example illustrates how to interface with the Microsoft Kinect to \n")
             MIL_TEXT("create and use depth maps in MIL. Specifically, this example shows:\n")
             MIL_TEXT("   - Kinect camera system calibration.\n")
             MIL_TEXT("   - Acquisition of color and depth images.\n")
             MIL_TEXT("   - Mapping of the color image on the depth image.\n")
             MIL_TEXT("   - Extraction of the depth map and mapping of the color image.\n")
             MIL_TEXT("   - Plane fitting of the floor.\n")
             MIL_TEXT("   - Localization of the 3D bounding box of an object on the floor.\n\n")
  
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: Application, system, image, calibration, 3dmap\n\n")
  
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

// User's processing function hook data structures.
struct SGetDepthProcFuncData
   {
   CKinectCameraInterface* pKinectCamera;   
   MIL_ID MilColorImage;
   MIL_ID MilDepthImage;
   };

struct SGetDepthColorProcFuncData
   {   
   SGetDepthProcFuncData GetDepthData;
   MIL_ID MilColorCameraCalibration;

   CDepthDataMgr* pDepthDataMgr;
   
   MIL_ID MilColorDepthOrWorldImage;
   bool IsDepthMapLut;
   };

struct SGet3DMeshColorProcFuncData
   {   
   SGetDepthColorProcFuncData GetDepthColorData;   
   MIL_ID MilColorGraList;
   MIL_ID MilColorDisplay;
   MIL_ID MilDepthMapImage;
   MIL_ID MilDepthMapDisplay;
   MIL_ID MilColorDepthMapImage;
   MIL_ID MilColorDepthMapDisplay;

   CBestPlaneFitter* pBestPlaneFitter;
   MIL_ID MilBinDepthMapImage;
   MIL_ID MilBlobResult;
   MIL_ID MilBlobFeatureList;
   
   MIL_DISP_D3D_HANDLE DispD3DHandle;
   MIL_ID Mil3dDepthMapImage;
   MIL_ID Mil3dColorDepthMapImage;
   MIL_ID MilD3DSetImagesThread;
   MIL_ID MilD3DUpdateEvent;
   volatile bool StopUpdate3dDisplay;
   };

// Box result structure.
struct SBox
   {
   MIL_DOUBLE CenterPosX;
   MIL_DOUBLE CenterPosY;
   MIL_DOUBLE CenterPosZ;
   MIL_DOUBLE ZAxisAngle;
   MIL_DOUBLE Width;
   MIL_DOUBLE Length;
   MIL_DOUBLE Height;
   };

// Camera calibration functions.
void CalibrateCamera(CKinectCameraInterface* pKinectCamera,
                     MIL_ID MilCameraCalibration,
                     MIL_ID MilGrabImage,
                     MIL_ID MilDisplay,
                     ColorStreamTypeEnum ColorStreamType);

void MoveColorToolCoordSystemOnIRCameraCoordSystem(MIL_ID MilSystem,
                                                     MIL_ID MilIRCameraCalibration,
                                                     MIL_ID MilColorCameraCalibration);


// Object finding functions.
bool FindObjectOnFloor(MIL_ID MilDepthMapImage,
                       MIL_ID MilBinDepthMapImage,
                       MIL_ID MilBlobResult,
                       MIL_ID MilBlobFeatureList,
                       MIL_DOUBLE FloorZ,
                       MIL_DOUBLE ThresholdZ,
                       SBox* pFoundBox);

void DrawFound3dBox(MIL_ID MilGraContext, MIL_ID MilDrawDest, MIL_ID MilCalibration, const SBox& rFoundBox);

// 3d display update functions.
MIL_UINT32 MFTYPE Update3dDisplayThread(void *UserDataPtr);
void Update3dDisplay(const SGet3DMeshColorProcFuncData* pGet3DMeshColorData);

// Helper functions.
void SetupDisplay(MIL_ID MilDisplay, MIL_CONST_TEXT_PTR WindowTitle, MIL_INT WindowPosX, MIL_INT WindowPosY);
void FatalError(CKinectCameraInterface* pKinectCamera, const MIL_TEXT_CHAR* pMsg);
void FillDisplayLut(MIL_ID MilDisplayLut);

// User's processing functions. 
MIL_INT MFTYPE CopyFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr);
MIL_INT MFTYPE GetDepthProcFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr);
MIL_INT MFTYPE GetDepthColorProcFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr);
MIL_INT MFTYPE Get3DMeshColorProcFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr);

// Depth map generation parameters.
static const MIL_INT    DEPTH_MAP_SIZE_X =   400;
static const MIL_INT    DEPTH_MAP_SIZE_Y =   300;
static const MIL_INT    CLOSE_NB_ITER    =   1;

// The (0,0) world position is at the pinhole of the Kinect camera.
static const MIL_DOUBLE WORLD_POS_X       = -1000.0;                                                       // in mm 
static const MIL_DOUBLE WORLD_POS_Y       = WORLD_POS_X * (MIL_DOUBLE)DEPTH_MAP_SIZE_Y / DEPTH_MAP_SIZE_X; // in mm 
static const MIL_DOUBLE WORLD_POS_Z       = 400.0;                                                         // in mm
static const MIL_DOUBLE PIXEL_SIZE_X      = (2 * -WORLD_POS_X) / DEPTH_MAP_SIZE_X;                         // in mm/pixel        
static const MIL_DOUBLE PIXEL_SIZE_Y      = PIXEL_SIZE_X;                                                  // in mm/pixel        
static const MIL_DOUBLE GRAY_LEVEL_SIZE_Z = 1.0;                                                           // in mm/gray val
static const SDepthMapGenParam KINECT_DEPTH_PARAM = {WORLD_POS_X, WORLD_POS_Y, WORLD_POS_Z,
                                                     PIXEL_SIZE_X, PIXEL_SIZE_Y, GRAY_LEVEL_SIZE_Z};

// The top view parameters.
static const MIL_DOUBLE TOP_VIEW_WORLD_POS_Z = -1500;   // in mm 
static const MIL_DOUBLE OBJECT_THRESHOLD_Z   = 100;      // in mm
static const SDepthMapGenParam TOP_DEPTH_PARAM = {WORLD_POS_X, WORLD_POS_Y, TOP_VIEW_WORLD_POS_Z,
                                                  PIXEL_SIZE_X, PIXEL_SIZE_Y, GRAY_LEVEL_SIZE_Z};

// The calibration grid parameters.
static const MIL_DOUBLE GRID_WORLD_OFFSET_X  = 0.0; // in mm
static const MIL_DOUBLE GRID_WORLD_OFFSET_Y  = 0.0; // in mm
static const MIL_INT    GRID_NB_ROWS         = 15;   
static const MIL_INT    GRID_NB_COLUMNS      = 11;
static const MIL_DOUBLE GRID_SPACING         = 50;  // in mm

// The minimum requirement of the selected blobs.
static const MIL_DOUBLE MIN_BLOB_AREA      = 20000; // in mm2
static const MIL_DOUBLE MIN_BLOB_FERET_MIN = 100;   // in mm
static const MIL_DOUBLE MAX_SIGMA_PIXEL    = 75;    // in mm 
static const MIL_DOUBLE MIN_RECTANGULARITY = 0.7;    // in percentage 

// The blob extraction parameters.
static const MIL_INT NB_FERETS = 180;

// The display text line offset.
static const MIL_INT DISPLAY_TEXT_LINE_OFFSET = 16;

// Display windows positions.
static const MIL_INT WINDOWS_OFFSET_X = 15;
static const MIL_INT WINDOWS_OFFSET_Y = 38;
static const MIL_INT DEPTH_DISPLAY_X = 0;
static const MIL_INT DEPTH_DISPLAY_Y = 0;
static const MIL_INT COLOR_DISPLAY_X = KINECT_DEPTH_IMAGE_SIZE_X + 100;
static const MIL_INT COLOR_DISPLAY_Y = 0;
static const MIL_INT COLOR_DEPTH_DISPLAY_X = 100;
static const MIL_INT COLOR_DEPTH_DISPLAY_Y = 0;
static const MIL_INT DEPTH_MAP_DISPLAY_X = 0;
static const MIL_INT DEPTH_MAP_DISPLAY_Y = D3D_DISPLAY_SIZE_Y + WINDOWS_OFFSET_Y;
static const MIL_INT COLOR_MAP_DISPLAY_X = DEPTH_MAP_SIZE_X + WINDOWS_OFFSET_X;
static const MIL_INT COLOR_MAP_DISPLAY_Y = D3D_DISPLAY_SIZE_Y + WINDOWS_OFFSET_Y;


//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate the application.
   MIL_ID MilApplication = MappAlloc(M_DEFAULT, M_NULL);
   
   // Allocate the system.
   MIL_ID MilSystem = MsysAlloc(M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);

   // Allocate the displays.
   MIL_ID MilDepthImageDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
   SetupDisplay(MilDepthImageDisplay, MIL_TEXT("Depth Image"), DEPTH_DISPLAY_X, DEPTH_DISPLAY_Y);
   MIL_ID MilColorDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
   SetupDisplay(MilColorDisplay, MIL_TEXT("Color Image"), COLOR_DISPLAY_X, COLOR_DISPLAY_Y);
   MIL_ID MilColorDepthImageDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
   SetupDisplay(MilColorDepthImageDisplay, MIL_TEXT("Color Depth Image"), COLOR_DEPTH_DISPLAY_X, COLOR_DEPTH_DISPLAY_Y);
   MIL_ID MilDepthMapDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
   SetupDisplay(MilDepthMapDisplay, MIL_TEXT("Depth Map"), DEPTH_MAP_DISPLAY_X, DEPTH_MAP_DISPLAY_Y);
   MIL_ID MilColorDepthMapDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
   SetupDisplay(MilColorDepthMapDisplay, MIL_TEXT("Color Depth Map"), COLOR_MAP_DISPLAY_X, COLOR_MAP_DISPLAY_Y);

   // Allocate the graphic list associated to the color display.
   MIL_ID MilColorGraList = MgraAllocList(MilSystem, M_DEFAULT, M_NULL);
   MdispControl(MilColorDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, MilColorGraList);
         
   MosPrintf(MIL_TEXT("MICROSOFT KINECT:\n"));
   MosPrintf(MIL_TEXT("---------------------------------------\n\n"));
   
   // Print the header.
   PrintHeader();

   // Create a Kinect camera and initialize it.
   CKinectCameraInterface* pKinectCamera;
#if USE_REAL_CAMERA
   pKinectCamera = new CKinectCamera(MilSystem);
#else
   pKinectCamera = new CKinectCameraStandalone(MilSystem);
#endif
   if(pKinectCamera->GetCameraStatus() != KINECT_CAMERA_OK)
      FatalError(pKinectCamera, MIL_TEXT("Unable to create camera sensor.\n"));
      
   // Allocate the color images.
   MIL_ID MilColorImage = MbufAllocColor(MilSystem, 3, KINECT_COLOR_IMAGE_SIZE_X, KINECT_COLOR_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MIL_ID MilColorDepthImage = MbufAllocColor(MilSystem, 3, KINECT_DEPTH_IMAGE_SIZE_X, KINECT_DEPTH_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP+M_BGR32+M_PACKED, M_NULL);
   MIL_ID MilColorWorldImageContainer = MbufAllocColor(MilSystem, 3, KINECT_DEPTH_IMAGE_SIZE_X*KINECT_DEPTH_IMAGE_SIZE_Y, 1, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP+M_BGR32+M_PACKED, M_NULL);
   MIL_ID MilColorWorldImage = MbufChild1d(MilColorWorldImageContainer, 0, 1, M_NULL);
   MIL_ID MilColorDepthMapImage = MbufAllocColor(MilSystem, 3, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_BGR32+M_PACKED+M_DISP, M_NULL);  
   MIL_ID Mil3dColorDepthMapImage = MbufAllocColor(MilSystem, 3, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_BGR32+M_PACKED+M_DISP, M_NULL);

   // Allocate the images for the depth.
   MIL_ID MilDepthImage = MbufAlloc2d(MilSystem, KINECT_DEPTH_IMAGE_SIZE_X, KINECT_DEPTH_IMAGE_SIZE_Y, 16+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MIL_ID MilDepthMapImage = MbufAlloc2d(MilSystem, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 16+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MIL_ID MilBinDepthMapImage = MbufAlloc2d(MilSystem, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID Mil3dDepthMapImage = MbufAlloc2d(MilSystem, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 16+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);

   // Allocate blob to find the objects box and set its parameters and features.
   MIL_ID MilBlobResult = MblobAllocResult(MilSystem, M_NULL);
   MIL_ID MilBlobFeatureList = MblobAllocFeatureList(MilSystem, M_NULL);
   MblobControl(MilBlobResult, M_RESULT_OUTPUT_UNITS, M_WORLD);
   MblobControl(MilBlobResult, M_INPUT_SELECT_UNITS, M_WORLD);
   MblobControl(MilBlobResult, M_NUMBER_OF_FERETS, NB_FERETS);
   MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY + M_BINARY);
   MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_ANGLE);
   MblobSelectFeature(MilBlobFeatureList, M_RECTANGULARITY);
   MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
   MblobSelectFeature(MilBlobFeatureList, M_SIGMA_PIXEL);
   MblobSelectFeature(MilBlobFeatureList, M_WORLD_BOX);

   // Allocate the calibration objects of both cameras of the Kinect.
   MIL_ID MilColorCameraCalibration = McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, M_NULL);
   MIL_ID MilIRCameraCalibration = McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, M_NULL);

   // Create the display lut and set it on the display.
   MIL_ID MilDisplayLut = MbufAllocColor(MilSystem, 3, MIL_UINT16_MAX, 1, 8+M_UNSIGNED, M_LUT, M_NULL);
   FillDisplayLut(MilDisplayLut);
   MdispLut(MilDepthImageDisplay, MilDisplayLut);
   MdispLut(MilDepthMapDisplay, MilDisplayLut);

   // Select the IR image on the display.
   MbufClear(MilDepthImage, 0.0);
   MdispSelect(MilColorDisplay, MilDepthImage);
   MdispControl(MilColorDisplay, M_VIEW_MODE, M_BIT_SHIFT);
   MdispControl(MilColorDisplay, M_VIEW_BIT_SHIFT, 8);

    // Calibrate the IR Camera.
   CalibrateCamera(pKinectCamera, MilIRCameraCalibration, MilDepthImage, MilColorDisplay, enIR);
 
   // Select the color image.    
   MdispSelect(MilColorDisplay, MilColorImage);
   MdispControl(MilColorDisplay, M_VIEW_MODE, M_DEFAULT);
         
   // Calibrate the color camera.
   CalibrateCamera(pKinectCamera, MilColorCameraCalibration, MilColorImage, MilColorDisplay, enColor);

   // Remove the link between the tool and the camera coordinate system.
   McalControl(MilColorCameraCalibration, M_LINK_TOOL_AND_CAMERA, M_DISABLE);

   // Move some coordinate systems on the IRCamera.
   MoveColorToolCoordSystemOnIRCameraCoordSystem(MilSystem, MilIRCameraCalibration, MilColorCameraCalibration);
         
   // Reinitialize the camera to grab the right type of image.
   if(pKinectCamera->InitCamera(enColor, true) != KINECT_CAMERA_OK)   
      FatalError(pKinectCamera, MIL_TEXT("Unable to initialize camera system.\n"));
           
   // Fill FrameGrabberProcess hook function structure for the GetDepthProcFunc.
   SGetDepthProcFuncData GetDepthData;
   GetDepthData.pKinectCamera = pKinectCamera;
   GetDepthData.MilColorImage = MilColorImage;
   GetDepthData.MilDepthImage = MilDepthImage;
   
   // Select the depth image on the display.
   MdispSelect(MilDepthImageDisplay, MilDepthImage);

   // Grab depth image continuously.
   pKinectCamera->FrameGrabberProcess(M_START, M_ASYNCHRONOUS, M_DEFAULT, &GetDepthProcFunc, &GetDepthData);

   MosPrintf(MIL_TEXT("The depth images from the Kinect are displayed.\n")
             MIL_TEXT("Press <Enter> to stop the grab.\n\n"));
   MosGetch();
   
   // Stop the grab.
   pKinectCamera->FrameGrabberProcess(M_STOP, M_DEFAULT);   
   
   // Fill FrameGrabberProcess hook function structure for the GetDepthColorProcFunc.
   SGetDepthColorProcFuncData GetDepthColorData;
   GetDepthColorData.GetDepthData               = GetDepthData;
   GetDepthColorData.MilColorCameraCalibration  = MilColorCameraCalibration;
   GetDepthColorData.MilColorDepthOrWorldImage  = MilColorDepthImage;
   GetDepthColorData.IsDepthMapLut = false;
      
   // Allocate the depth data manager.
   CDepthDataMgr* pDepthDataMgr = new CDepthDataMgr(MilSystem, KINECT_DEPTH_IMAGE_SIZE_X, KINECT_DEPTH_IMAGE_SIZE_Y);
   pDepthDataMgr->CalculateMultipliers(MilIRCameraCalibration);
   GetDepthColorData.pDepthDataMgr = pDepthDataMgr;   

   // Select the color depth image on the display.
   MdispSelect(MilColorDepthImageDisplay, MilColorDepthImage);
   
   // Grab colored depth continuously.
   pKinectCamera->FrameGrabberProcess(M_START, M_ASYNCHRONOUS, M_DEFAULT, &GetDepthColorProcFunc, &GetDepthColorData);

   MosPrintf(MIL_TEXT("The colored depth images are displayed.\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to stop the grab.\n\n"));
   MosGetch();

   // Stop the grab.
   pKinectCamera->FrameGrabberProcess(M_STOP, M_DEFAULT);
  
   // Create the depth map.
   if(pDepthDataMgr->GetNbWorld())
      {
      // Generate the color to world luts and move the color world image child.
      pDepthDataMgr->GenColorLuts(MilColorCameraCalibration, true);
      MbufChildMove(MilColorWorldImage, 0, 0, pDepthDataMgr->GetNbWorld(), 1, M_DEFAULT);
      
      // Create the color world image.
      pDepthDataMgr->WarpColorImage(MilColorImage, MilColorWorldImage, true);

      // Create the depth map.
      pDepthDataMgr->CreateDepthMap(MilColorWorldImage, MilDepthMapImage, MilColorDepthMapImage, KINECT_DEPTH_PARAM);
      }

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   MIL_DISP_D3D_HANDLE DispD3DHandle = M_NULL;
   
   // Try to allocate D3D display.       
   DispD3DHandle = MdepthD3DAlloc(MilDepthMapImage, MilColorDepthMapImage,
                                  D3D_DISPLAY_SIZE_X,
                                  D3D_DISPLAY_SIZE_Y,
                                  M_DEFAULT,
                                  M_DEFAULT,
                                  M_DEFAULT,
                                  MIN_Z,
                                  MAX_Z,
                                  MAX_DISTANCE_Z,
                                  M_NULL);

   if (DispD3DHandle != NULL)
      {
      // Show the display and print the help.
      MdispD3DShow(DispD3DHandle);
      MdispD3DPrintHelp(DispD3DHandle);
      }

#endif

   // Fill FrameGrabberProcess hook function structure for the Get3DMeshColorProcFunc.
   SGet3DMeshColorProcFuncData Get3DMeshColorData;
   Get3DMeshColorData.GetDepthColorData         = GetDepthColorData;
   Get3DMeshColorData.MilColorGraList           = MilColorGraList;
   Get3DMeshColorData.MilColorDisplay           = MilColorDisplay;
   Get3DMeshColorData.MilDepthMapImage          = MilDepthMapImage;
   Get3DMeshColorData.MilDepthMapDisplay        = MilDepthMapDisplay;
   Get3DMeshColorData.MilColorDepthMapImage     = MilColorDepthMapImage;
   Get3DMeshColorData.MilColorDepthMapDisplay   = MilColorDepthMapDisplay;
   Get3DMeshColorData.Mil3dDepthMapImage        = Mil3dDepthMapImage;
   Get3DMeshColorData.Mil3dColorDepthMapImage   = Mil3dColorDepthMapImage;
   Get3DMeshColorData.DispD3DHandle             = DispD3DHandle;
   
   // Allocate the best plane fitter and set the pointer in the structure.
   CBestPlaneFitter* pBestPlaneFitter = new CBestPlaneFitter(MilSystem, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y);
   Get3DMeshColorData.pBestPlaneFitter = pBestPlaneFitter;

   Get3DMeshColorData.MilBinDepthMapImage = MilBinDepthMapImage;
   Get3DMeshColorData.MilBlobResult       = MilBlobResult;
   Get3DMeshColorData.MilBlobFeatureList  = MilBlobFeatureList;

   // Set the generation of the color image to be a color map.
   Get3DMeshColorData.GetDepthColorData.IsDepthMapLut = true;
   Get3DMeshColorData.GetDepthColorData.MilColorDepthOrWorldImage = MilColorWorldImage;
   
   // Start the 3d display update thread.
   Get3DMeshColorData.StopUpdate3dDisplay = false;
   MthrAlloc(MilSystem, M_EVENT, M_NOT_SIGNALED + M_AUTO_RESET, M_NULL, M_NULL, &Get3DMeshColorData.MilD3DUpdateEvent);
   MthrAlloc(MilSystem, M_THREAD, M_DEFAULT, Update3dDisplayThread,
             &Get3DMeshColorData, &Get3DMeshColorData.MilD3DSetImagesThread);
   
   // Deselect the color depth image display since it is not generated anymore.
   MdispSelect(MilColorDepthImageDisplay, M_NULL);

   // Select the depth and color map on the displays
   MdispSelect(MilColorDepthMapDisplay, MilColorDepthMapImage);
   MdispSelect(MilDepthMapDisplay, MilDepthMapImage);
   
   // Grab depth maps continuously.
   pKinectCamera->FrameGrabberProcess(M_START, M_ASYNCHRONOUS, M_DEFAULT, &Get3DMeshColorProcFunc, &Get3DMeshColorData);
   
   MosPrintf(MIL_TEXT("The corrected and colored depth map as well as the fitted floor\n")
             MIL_TEXT("and the 3D bounding box are displayed live.\n")
             MIL_TEXT("Press <Enter> to stop the grab.\n\n"));
   MosGetch();
   
   // Stop the grab.
   pKinectCamera->FrameGrabberProcess(M_STOP, M_DEFAULT);   

   MosPrintf(MIL_TEXT("The final corrected and colored depth map as well as the fitted floor \n")
             MIL_TEXT("and the 3D bounding box are displayed.\n")
             MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();
   
   // Stop the update of the 3d display and free the event and thread.
   Get3DMeshColorData.StopUpdate3dDisplay = true;
   MthrControl(Get3DMeshColorData.MilD3DUpdateEvent, M_EVENT_SET, M_SIGNALED);
   MthrWait(Get3DMeshColorData.MilD3DSetImagesThread, M_THREAD_END_WAIT, M_NULL);
   MthrFree(Get3DMeshColorData.MilD3DSetImagesThread);
   MthrFree(Get3DMeshColorData.MilD3DUpdateEvent);

   // Free the 3d display.
#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   if (DispD3DHandle != M_NULL)
      {
      MdispD3DHide(DispD3DHandle);
      MdispD3DFree(DispD3DHandle);
      }
#endif

   // Delete the plane fitter.
   delete pBestPlaneFitter;

   // Delete the depth data manager.
   delete pDepthDataMgr;

   // Close down the camera
   pKinectCamera->CloseDown();

   // Delete the camera.
   delete pKinectCamera;
 
   // Free the calibrations.
   McalFree(MilColorCameraCalibration);
   McalFree(MilIRCameraCalibration);
  
   // Free the buffers.
   MbufFree(MilDisplayLut);
   MbufFree(MilColorDepthMapImage);
   MbufFree(MilColorWorldImage);
   MbufFree(MilColorWorldImageContainer);
   MbufFree(MilColorDepthImage);
   MbufFree(MilColorImage);
   MbufFree(MilDepthMapImage);
   MbufFree(MilDepthImage);
   MbufFree(Mil3dColorDepthMapImage);
   MbufFree(Mil3dDepthMapImage);
   MbufFree(MilBinDepthMapImage);

   // Free blob
   MblobFree(MilBlobResult);
   MblobFree(MilBlobFeatureList);

   // Free the graphic list.
   MgraFree(MilColorGraList);

   // Free the displays.     
   MdispFree(MilColorDisplay);
   MdispFree(MilDepthImageDisplay);
   MdispFree(MilColorDepthImageDisplay);
   MdispFree(MilDepthMapDisplay);
   MdispFree(MilColorDepthMapDisplay);

   // Free the system.
   MsysFree(MilSystem);

   // Free the application.
   MappFree(MilApplication);

   return 0;
   }

//*****************************************************************************
// Hook function for FrameGrabberProcess(). Copy the Kinect image.
//*****************************************************************************
MIL_INT MFTYPE CopyFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr)
   {
   MIL_ID* pMilImage = (MIL_ID*) UserDataPtr;
   MbufCopy(MilKinectImage, *pMilImage);
   return M_NULL;
   }

//*****************************************************************************
// Hook function for FrameGrabberProcess(). Converts the depth buffer data returned 
// by the Kinect to a depth image.
//*****************************************************************************
MIL_INT MFTYPE GetDepthProcFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr)
   {
   SGetDepthProcFuncData* pGetDepthData = static_cast<SGetDepthProcFuncData*>(UserDataPtr);

   // If the image grabbed by the Kinect is the color image.
   if(MbufInquire(MilKinectImage, M_SIZE_BAND, M_NULL) == 3)
      MbufCopy(MilKinectImage, pGetDepthData->MilColorImage);
   else
      pGetDepthData->pKinectCamera->ConvertKinectDepthToMilDepth(MilKinectImage, pGetDepthData->MilDepthImage);   

   return M_NULL;
   }

//*****************************************************************************
// Hook function for FrameGrabberProcess(). Converts the depth buffer data returned 
// by the Kinect and map the colors of the color image on the depth image.
//*****************************************************************************
MIL_INT MFTYPE GetDepthColorProcFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr)
   {
   SGetDepthColorProcFuncData* pGetDepthColorData = static_cast<SGetDepthColorProcFuncData*>(UserDataPtr);
   SGetDepthProcFuncData& GetDepthData = pGetDepthColorData->GetDepthData;
   CDepthDataMgr* pDepthDataMgr = pGetDepthColorData->pDepthDataMgr;

   // Call the hook to get the depth.
   GetDepthProcFunc(HookType, MilKinectImage, &GetDepthData);
   
   // If the image grabbed by the Kinect is the depth image.
   if(MbufInquire(MilKinectImage, M_SIZE_BAND, M_NULL) == 1)
      {
      // Calculate the world points.
      if(pGetDepthColorData->IsDepthMapLut)
         pDepthDataMgr->CalculateWorldPoints<false>(GetDepthData.MilDepthImage);
      else
         pDepthDataMgr->CalculateWorldPoints<true>(GetDepthData.MilDepthImage);

      // Generate the luts.
      pDepthDataMgr->GenColorLuts(pGetDepthColorData->MilColorCameraCalibration, pGetDepthColorData->IsDepthMapLut);
      
      // Create the color depth or world image.
      if(pDepthDataMgr->GetNbWorld())
         {
         if(pGetDepthColorData->IsDepthMapLut)
            MbufChildMove(pGetDepthColorData->MilColorDepthOrWorldImage, 0, 0, pDepthDataMgr->GetNbWorld(), 1, M_DEFAULT);

         pDepthDataMgr->WarpColorImage(GetDepthData.MilColorImage, pGetDepthColorData->MilColorDepthOrWorldImage,
                                       pGetDepthColorData->IsDepthMapLut);          
         }
      }
   return M_NULL;
   }

//*****************************************************************************
// Hook function for FrameGrabberProcess(). Converts the depth buffer data returned 
// by the Kinect to a MIL depth map with its associated color depth map. A plane is
// fitted on the floor data and the bounding box of an object is localized. The 3d
// display is updated live.
//*****************************************************************************
MIL_INT MFTYPE Get3DMeshColorProcFunc(MIL_INT HookType, MIL_ID MilKinectImage, void* UserDataPtr)
   {
   SGet3DMeshColorProcFuncData* pGet3DMeshColorData = static_cast<SGet3DMeshColorProcFuncData*>(UserDataPtr);
   SGetDepthColorProcFuncData& GetDepthColorData = pGet3DMeshColorData->GetDepthColorData;
   CBestPlaneFitter* pBestPlaneFitter = pGet3DMeshColorData->pBestPlaneFitter;
   CDepthDataMgr* pDepthDataMgr = GetDepthColorData.pDepthDataMgr;

   // Call the hook function to get the world points and the color associated to each point.
   GetDepthColorProcFunc(HookType, MilKinectImage, &GetDepthColorData);

   // If the image grabbed by the Kinect is the depth image.
   if(MbufInquire(MilKinectImage, M_SIZE_BAND, M_NULL) == 1)
      { 
      // Disable the display updates.
      MdispControl(pGet3DMeshColorData->MilDepthMapDisplay, M_UPDATE, M_DISABLE);
      MdispControl(pGet3DMeshColorData->MilColorDepthMapDisplay, M_UPDATE, M_DISABLE);
      MdispControl(pGet3DMeshColorData->MilColorDisplay, M_UPDATE, M_DISABLE);

      // Create the depth map and color depth map.
      pDepthDataMgr->CreateDepthMap(GetDepthColorData.MilColorDepthOrWorldImage,
                                    pGet3DMeshColorData->MilDepthMapImage,
                                    pGet3DMeshColorData->MilColorDepthMapImage,
                                    KINECT_DEPTH_PARAM);

      // Initialize the found box to 0.
      SBox FoundBox = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

      // Fit the floor plane.
      if(pBestPlaneFitter->CalculateBestPlane(pGet3DMeshColorData->MilDepthMapImage))
         {
         // Update the 3d display.
         Update3dDisplay(pGet3DMeshColorData);    

         // Move the relative coordinate system on the fitted plane. The fitted plane was found in
         // the IR camera coordinate system, which is where we had put the M_TOOL_COORDINATE_SYSTEM
         // of the color camera calibration.
         pBestPlaneFitter->MoveCoordinateSystemOnPlane(GetDepthColorData.MilColorCameraCalibration,
                                                       M_RELATIVE_COORDINATE_SYSTEM, M_TOOL_COORDINATE_SYSTEM);
         
         // Transform the chains of the plane region which were expressed in the M_TOOL_COORDINATE_SYSTEM.
         pBestPlaneFitter->MoveWorldChains(GetDepthColorData.MilColorCameraCalibration,
                                           M_TOOL_COORDINATE_SYSTEM, M_RELATIVE_COORDINATE_SYSTEM);

         // Get the overlay of the color display and clear it.
         MIL_ID MilOverlay = MdispInquire(pGet3DMeshColorData->MilColorDisplay, M_OVERLAY_ID, M_NULL);
         MdispControl(pGet3DMeshColorData->MilColorDisplay, M_OVERLAY_CLEAR, M_DEFAULT); 
         MgraClear(M_DEFAULT, pGet3DMeshColorData->MilColorGraList);
         
         // Set the graphic context and draw the plane in the image.
         MgraColor(M_DEFAULT, M_COLOR_RED);
         MgraControl(M_DEFAULT, M_INPUT_UNITS, M_WORLD);
         McalAssociate(GetDepthColorData.MilColorCameraCalibration, MilOverlay, M_DEFAULT);
         MIL_INT TransparentColor = MdispInquire(pGet3DMeshColorData->MilColorDisplay, M_TRANSPARENT_COLOR, M_NULL);
         pBestPlaneFitter->DrawPlaneInImage(M_DEFAULT,
                                            MilOverlay,
                                            TransparentColor);

         // Move the world points in the plane coordinate system.
         pDepthDataMgr->MoveWorldPoints(GetDepthColorData.MilColorCameraCalibration,
                                        M_TOOL_COORDINATE_SYSTEM, M_RELATIVE_COORDINATE_SYSTEM);

         // Create the depth map seen from above the floor.
         pDepthDataMgr->CreateDepthMap(GetDepthColorData.MilColorDepthOrWorldImage,
                                       pGet3DMeshColorData->MilDepthMapImage,
                                       M_NULL,
                                       TOP_DEPTH_PARAM);

         // Find the object on the floor and draw its bounding box.         
         if(FindObjectOnFloor(pGet3DMeshColorData->MilDepthMapImage,
                              pGet3DMeshColorData->MilBinDepthMapImage,
                              pGet3DMeshColorData->MilBlobResult,
                              pGet3DMeshColorData->MilBlobFeatureList,
                              -TOP_VIEW_WORLD_POS_Z,
                              OBJECT_THRESHOLD_Z,
                              &FoundBox))
            {  
            MgraColor(M_DEFAULT, M_COLOR_CYAN);
            DrawFound3dBox(M_DEFAULT, pGet3DMeshColorData->MilColorGraList,
                           GetDepthColorData.MilColorCameraCalibration, FoundBox);
            }  
         else
            MgraColor(M_DEFAULT, M_COLOR_RED);
         }

      // Draw the calculated volume.
      MgraControl(M_DEFAULT, M_INPUT_UNITS, M_DISPLAY);
      MIL_DOUBLE BoxVolume = FoundBox.Width * FoundBox.Length * FoundBox.Height / 1000.0;
      MIL_TEXT_CHAR DisplayText[256];
      MosSprintf(DisplayText, 256, MIL_TEXT(" Box width:  %8.1f cm   "), FoundBox.Width / 10.0);
      MgraText(M_DEFAULT, pGet3DMeshColorData->MilColorGraList, 0, 0, DisplayText);
      MosSprintf(DisplayText, 256, MIL_TEXT(" Box length: %8.1f cm   "), FoundBox.Length / 10.0);
      MgraText(M_DEFAULT, pGet3DMeshColorData->MilColorGraList, 0, DISPLAY_TEXT_LINE_OFFSET, DisplayText);
      MosSprintf(DisplayText, 256, MIL_TEXT(" Box height: %8.1f cm   "), FoundBox.Height / 10.0);
      MgraText(M_DEFAULT, pGet3DMeshColorData->MilColorGraList, 0, 2 * DISPLAY_TEXT_LINE_OFFSET, DisplayText);
      MosSprintf(DisplayText, 256, MIL_TEXT(" Box volume: %8.1f cm^3 "), BoxVolume);
      MgraText(M_DEFAULT, pGet3DMeshColorData->MilColorGraList, 0, 3 * DISPLAY_TEXT_LINE_OFFSET, DisplayText);
                  
      // Enable the display updates.
      MdispControl(pGet3DMeshColorData->MilColorDisplay, M_UPDATE, M_ENABLE);
      MdispControl(pGet3DMeshColorData->MilColorDepthMapDisplay, M_UPDATE, M_ENABLE);
      MdispControl(pGet3DMeshColorData->MilDepthMapDisplay, M_UPDATE, M_ENABLE);     
      }
   return M_NULL;
   }

//*****************************************************************************
// Function to calibrate the Kinect cameras.
//*****************************************************************************
void CalibrateCamera(CKinectCameraInterface* pKinectCamera,
                     MIL_ID MilCameraCalibration,
                     MIL_ID MilGrabImage,
                     MIL_ID MilDisplay,
                     ColorStreamTypeEnum ColorStreamType)
   {
   MIL_CONST_TEXT_PTR CameraFlag = (ColorStreamType == enColor) ? MIL_TEXT("color") : MIL_TEXT("infrared");
      
   // Reinitialize the camera to grab the right type of image.
   if(pKinectCamera->InitCamera(ColorStreamType, false) != KINECT_CAMERA_OK)
      FatalError(pKinectCamera, MIL_TEXT("Unable to initialize camera system.\n"));

   MIL_ID MilDrawDest = MdispInquire(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, M_NULL);
   MgraClear(M_DEFAULT, MilDrawDest);
  
#if USE_REAL_CAMERA && CALIBRATE_WITH_REAL_CAMERA
   // Grab image continuously.
   pKinectCamera->FrameGrabberProcess(M_START, M_ASYNCHRONOUS, &CopyFunc, &MilGrabImage);
   
   // Print message.
   MosPrintf(MIL_TEXT("The grabbed %s image from the Kinect is displayed.\n\n"), CameraFlag);
   if(ColorStreamType == enIR)
      MosPrintf(MIL_TEXT("Place a tilted chessboard calibration grid to calibrate the %s camera.\n"), CameraFlag);
   else
      MosPrintf(MIL_TEXT("Use the same tilted chessboard calibration grid to calibrate the %s camera.\n"), CameraFlag);   
   MosPrintf(MIL_TEXT("Press <Enter> to grab and calibrate the %s camera.\n\n"), CameraFlag);
   MosGetch();

   // Stop the grab.
   pKinectCamera->FrameGrabberProcess(M_STOP, M_DEFAULT);
#else
   MbufLoad(ColorStreamType == enColor? COLOR_GRID : IR_GRID, MilGrabImage);
#endif

   // Calibrate the IR camera.
   McalGrid(MilCameraCalibration,
            MilGrabImage,
            GRID_WORLD_OFFSET_X,
            GRID_WORLD_OFFSET_Y,
            0.0,
            GRID_NB_ROWS,
            GRID_NB_COLUMNS,
            GRID_SPACING,
            GRID_SPACING, 
            M_DEFAULT,
            M_CIRCLE_GRID);

   if(McalInquire(MilCameraCalibration, M_CALIBRATION_STATUS, M_NULL) != M_CALIBRATED)
      FatalError(pKinectCamera, MIL_TEXT("Unable to calibrate the camera."));

   // Display the calibration points.
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   McalDraw(M_DEFAULT, MilCameraCalibration, MilDrawDest, M_DRAW_WORLD_POINTS, M_DEFAULT, M_DEFAULT);
   MosPrintf(MIL_TEXT("The calibration of the %s camera is displayed.\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"),
             CameraFlag);
   MosGetch();

   MgraClear(M_DEFAULT, MilDrawDest);
   }

//*****************************************************************************
// Function to move the color camera tool coordinate system on the 
// camera coordinate system of the IR camera. Here is the list of coordinate system
// and their usage.
//
//                              | ColorCameraCalibration | IRCameraCalibration |
// M_ABSOLUTE_COORDINATE_SYSTEM |             Same for both cameras            |
// M_RELATIVE_COORDINATE_SYSTEM | Put on the floor plane |        Unused       |
// M_TOOL_COORDINATE_SYSTEM     | Put on the IR camera   |        Unused       |
//*****************************************************************************
void MoveColorToolCoordSystemOnIRCameraCoordSystem(MIL_ID MilSystem,
                             MIL_ID MilIRCameraCalibration,
                             MIL_ID MilColorCameraCalibration)
   {
   // Allocate the homogeneous matrix.
   MIL_ID MilAbsoluteToIRCameraMatrix = MbufAlloc2d(MilSystem, 4, 4, 32+M_FLOAT, M_ARRAY, M_NULL);

   // Get the homogeneous matrix between the absolute and the IR camera coordinate system.
   McalGetCoordinateSystem(MilIRCameraCalibration,
                           M_CAMERA_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM,
                           M_HOMOGENEOUS_MATRIX,
                           MilAbsoluteToIRCameraMatrix,
                           M_NULL, 
                           M_NULL, 
                           M_NULL,
                           M_NULL);
      
   // Set the tool coordinate system of the Color calibration on the IR calibration camera coordinate system.
   McalSetCoordinateSystem(MilColorCameraCalibration,
                           M_TOOL_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM,
                           M_HOMOGENEOUS_MATRIX,
                           MilAbsoluteToIRCameraMatrix,
                           M_DEFAULT,
                           M_DEFAULT,
                           M_DEFAULT,
                           M_DEFAULT);

   // Free the homogeneous matrix.
   MbufFree(MilAbsoluteToIRCameraMatrix);
   }

//*****************************************************************************
// Function to find the object on the floor.
//*****************************************************************************
bool FindObjectOnFloor(MIL_ID MilDepthMapImage,
                       MIL_ID MilBinDepthMapImage,
                       MIL_ID MilBlobResult,
                       MIL_ID MilBlobFeatureList,
                       MIL_DOUBLE FloorZ,
                       MIL_DOUBLE ThresholdZ,
                       SBox* pFoundBox)
   {
   // Binarize the depth map.
   MimBinarize(MilDepthMapImage, MilBinDepthMapImage, M_LESS, FloorZ - ThresholdZ, M_NULL);

   // Calculate the blobs.
   MblobCalculate(MilBinDepthMapImage, MilDepthMapImage, MilBlobFeatureList, MilBlobResult);

   // Select the blobs that meet the area and feret criterion
   MblobSelect(MilBlobResult, M_INCLUDE_ONLY, M_AREA, M_GREATER_OR_EQUAL, MIN_BLOB_AREA, M_NULL);
   MblobSelect(MilBlobResult, M_EXCLUDE, M_FERET_MIN_DIAMETER, M_LESS, MIN_BLOB_FERET_MIN, M_NULL);
   MblobSelect(MilBlobResult, M_EXCLUDE, M_SIGMA_PIXEL, M_GREATER, MAX_SIGMA_PIXEL, M_NULL);
   MblobSelect(MilBlobResult, M_EXCLUDE, M_RECTANGULARITY, M_LESS, MIN_RECTANGULARITY, M_NULL);

   MIL_INT NbBlobs;
   if(MblobGetNumber(MilBlobResult, &NbBlobs))
      {
      MIL_INT* pBlobLabel = new MIL_INT[NbBlobs];
      MIL_INT* pBlobArea = new MIL_INT[NbBlobs];
      MIL_DOUBLE* pBlobCogX = new MIL_DOUBLE[NbBlobs];
      MIL_DOUBLE* pBlobCogY = new MIL_DOUBLE[NbBlobs];
      
      // Get the labels of the blobs
      MblobGetResult(MilBlobResult, M_LABEL_VALUE + M_TYPE_MIL_INT, pBlobLabel);

      // Get the area and positions of the blob
      MblobGetResult(MilBlobResult, M_AREA + M_TYPE_MIL_INT, pBlobArea);
      MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_X + M_BINARY, pBlobCogX);
      MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_Y + M_BINARY, pBlobCogY);

      // Calculate a pseudo score for each blob based on the area and Manhattan distance
      // from plane origin and keep the best blob.
      MIL_DOUBLE ManhattanDistance = (abs(pBlobCogX[0]) + abs(pBlobCogY[0]));
      MIL_DOUBLE MaxScore = pBlobArea[0] / (ManhattanDistance + 100);
      MIL_INT MaxScoreIdx = 0;
      for(MIL_INT BlobIdx = 1; BlobIdx < NbBlobs; BlobIdx++)
         {
         ManhattanDistance = (abs(pBlobCogX[BlobIdx]) + abs(pBlobCogY[BlobIdx]));
         MIL_DOUBLE CurScore = pBlobArea[BlobIdx] / (ManhattanDistance + 100);
         if(CurScore > MaxScore)
            {
            MaxScore = CurScore;
            MaxScoreIdx = BlobIdx;
            }
         }         
      
      // Get the Z results.
      MIL_DOUBLE BoxTopZ;
      MblobGetResultSingle(MilBlobResult, pBlobLabel[MaxScoreIdx], M_MEAN_PIXEL, &BoxTopZ);
      pFoundBox->Height = FloorZ - BoxTopZ;
      pFoundBox->CenterPosZ = -pFoundBox->Height/2;
      MblobGetResultSingle(MilBlobResult, pBlobLabel[MaxScoreIdx], M_FERET_MIN_ANGLE, &pFoundBox->ZAxisAngle);
      
      // Use the world box to get the box center width and length.
      McalFixture(MilBinDepthMapImage, M_NULL, M_MOVE_RELATIVE, M_POINT_AND_ANGLE,
                  M_DEFAULT, 0.0, 0.0, pFoundBox->ZAxisAngle, M_DEFAULT);
      MblobCalculate(MilBinDepthMapImage, M_NULL, MilBlobFeatureList, MilBlobResult);
      MblobGetResultSingle(MilBlobResult, pBlobLabel[MaxScoreIdx], M_WORLD_BOX_X_MIN, &pFoundBox->CenterPosX);
      MblobGetResultSingle(MilBlobResult, pBlobLabel[MaxScoreIdx], M_WORLD_BOX_Y_MIN, &pFoundBox->CenterPosY);
      MblobGetResultSingle(MilBlobResult, pBlobLabel[MaxScoreIdx], M_WORLD_FERET_X, &pFoundBox->Width);
      MblobGetResultSingle(MilBlobResult, pBlobLabel[MaxScoreIdx], M_WORLD_FERET_Y, &pFoundBox->Length);
      pFoundBox->CenterPosX += pFoundBox->Width/2;
      pFoundBox->CenterPosY += pFoundBox->Length/2;
      
      McalTransformCoordinate3dList(MilBinDepthMapImage,
                                    M_RELATIVE_COORDINATE_SYSTEM,
                                    M_ABSOLUTE_COORDINATE_SYSTEM,
                                    1,
                                    &pFoundBox->CenterPosX,
                                    &pFoundBox->CenterPosY, 
                                    M_NULL,
                                    &pFoundBox->CenterPosX,
                                    &pFoundBox->CenterPosY,
                                    M_NULL,
                                    M_DEFAULT);

      // Delete the arrays
      delete [] pBlobCogY;
      delete [] pBlobCogX;
      delete [] pBlobArea;
      delete [] pBlobLabel;

      return true;
      }
   return false;   
   }

//*****************************************************************************
// Function that draw the 3d box in the image
//*****************************************************************************
void DrawFound3dBox(MIL_ID MilGraContext, MIL_ID MilDrawDest, MIL_ID MilCalibration, const SBox& rFoundBox)
   {
   // Move the coordinate system on the box.
   McalSetCoordinateSystem(MilCalibration,
                           M_RELATIVE_COORDINATE_SYSTEM,
                           M_RELATIVE_COORDINATE_SYSTEM,
                           M_TRANSLATION + M_COMPOSE_WITH_CURRENT,
                           M_NULL,
                           rFoundBox.CenterPosX,
                           rFoundBox.CenterPosY,
                           rFoundBox.CenterPosZ,
                           M_DEFAULT);

   McalSetCoordinateSystem(MilCalibration,
                           M_RELATIVE_COORDINATE_SYSTEM,
                           M_RELATIVE_COORDINATE_SYSTEM,
                           M_ROTATION_Z + M_COMPOSE_WITH_CURRENT,
                           M_NULL,
                           -rFoundBox.ZAxisAngle,
                           M_DEFAULT,
                           M_DEFAULT,
                           M_DEFAULT);

   // Get all the corners of the box. The top corners are first.
   MIL_DOUBLE CornersX[8] = {-rFoundBox.Width/2 , rFoundBox.Width/2  , rFoundBox.Width/2  , -rFoundBox.Width/2 ,
                             -rFoundBox.Width/2 , rFoundBox.Width/2  , rFoundBox.Width/2  , -rFoundBox.Width/2 };  
   MIL_DOUBLE CornersY[8] = {-rFoundBox.Length/2, -rFoundBox.Length/2, rFoundBox.Length/2 , rFoundBox.Length/2 ,
                             -rFoundBox.Length/2, -rFoundBox.Length/2, rFoundBox.Length/2 , rFoundBox.Length/2 };
   MIL_DOUBLE CornersZ[8] = {-rFoundBox.Height/2, -rFoundBox.Height/2, -rFoundBox.Height/2, -rFoundBox.Height/2,
                             rFoundBox.Height/2 , rFoundBox.Height/2 , rFoundBox.Height/2 , rFoundBox.Height/2 };
   MIL_DOUBLE TopCameraCornersX[4]; 
   MIL_DOUBLE TopCameraCornersY[4]; 
   MIL_DOUBLE TopCameraCornersZ[4]; 

   // Transform the 4 top corners in the camera coordinate system.
   McalTransformCoordinate3dList(MilCalibration,
                                 M_RELATIVE_COORDINATE_SYSTEM,
                                 M_CAMERA_COORDINATE_SYSTEM,
                                 4,
                                 CornersX,
                                 CornersY,
                                 CornersZ,
                                 TopCameraCornersX,
                                 TopCameraCornersY,
                                 TopCameraCornersZ,
                                 M_DEFAULT);

   // Get the corner that is the closest to the camera.
   MIL_INT ClosestCornerIdx = 0;
   MIL_DOUBLE MinCornerDistSquare = TopCameraCornersX[0] * TopCameraCornersX[0] +
                                    TopCameraCornersY[0] * TopCameraCornersY[0] +
                                    TopCameraCornersZ[0] * TopCameraCornersZ[0];  
   for(MIL_INT CornerIdx = 1; CornerIdx < 4; CornerIdx++)
      {
      MIL_DOUBLE CurCornerDistSquare = TopCameraCornersX[CornerIdx] * TopCameraCornersX[CornerIdx] +
                                       TopCameraCornersY[CornerIdx] * TopCameraCornersY[CornerIdx] +
                                       TopCameraCornersZ[CornerIdx] * TopCameraCornersZ[CornerIdx];
      if(CurCornerDistSquare < MinCornerDistSquare)
         {
         MinCornerDistSquare = CurCornerDistSquare;
         ClosestCornerIdx = CornerIdx;
         }
      }

   // Transform all the corners to the pixel coordinate system.
   McalTransformCoordinate3dList(MilCalibration,
                                 M_RELATIVE_COORDINATE_SYSTEM,
                                 M_PIXEL_COORDINATE_SYSTEM,
                                 8,
                                 CornersX,
                                 CornersY,
                                 CornersZ,
                                 CornersX,
                                 CornersY,
                                 M_NULL,
                                 M_DEFAULT);

   // Set the input units.
   MgraControl(MilGraContext, M_INPUT_UNITS, M_PIXEL);
   
   // Draw the top polygon.
   MgraLines(MilGraContext, MilDrawDest, 4, CornersX, CornersY, M_NULL, M_NULL, M_POLYGON);

   // Draw the previous polygon.
   MIL_DOUBLE PrevDrawCornersX[4] = {CornersX[ClosestCornerIdx]                , CornersX[(ClosestCornerIdx + 3) % 4],
                                     CornersX[((ClosestCornerIdx + 3) % 4) + 4], CornersX[ClosestCornerIdx + 4]      };
   MIL_DOUBLE PrevDrawCornersY[4] = {CornersY[ClosestCornerIdx]                , CornersY[(ClosestCornerIdx + 3) % 4],
                                     CornersY[((ClosestCornerIdx + 3) % 4) + 4], CornersY[ClosestCornerIdx + 4]      };
   MgraLines(MilGraContext, MilDrawDest, 4, PrevDrawCornersX, PrevDrawCornersY, M_NULL, M_NULL, M_POLYGON);
   
   // Draw the next polygon.
   MIL_DOUBLE NextDrawCornersX[4] = {CornersX[ClosestCornerIdx]                , CornersX[(ClosestCornerIdx + 1) % 4],
                                     CornersX[((ClosestCornerIdx + 1) % 4) + 4], CornersX[ClosestCornerIdx + 4]      };
   MIL_DOUBLE NextDrawCornersY[4] = {CornersY[ClosestCornerIdx]                , CornersY[(ClosestCornerIdx + 1) % 4],
                                     CornersY[((ClosestCornerIdx + 1) % 4) + 4], CornersY[ClosestCornerIdx + 4]      };
   MgraLines(MilGraContext, MilDrawDest, 4, NextDrawCornersX, NextDrawCornersY, M_NULL, M_NULL, M_POLYGON);   
   }

//*****************************************************************************
// Function that updates the 3d display.
//*****************************************************************************
void Update3dDisplay(const SGet3DMeshColorProcFuncData* pGet3DMeshColorData)
   {
   MbufCopy(pGet3DMeshColorData->MilDepthMapImage, pGet3DMeshColorData->Mil3dDepthMapImage);
   MbufCopy(pGet3DMeshColorData->MilColorDepthMapImage, pGet3DMeshColorData->Mil3dColorDepthMapImage);
   if(pGet3DMeshColorData->MilD3DSetImagesThread)
      MthrControl(pGet3DMeshColorData->MilD3DUpdateEvent, M_EVENT_SET, M_SIGNALED);   
   else
      MdepthD3DSetImages(pGet3DMeshColorData->DispD3DHandle, pGet3DMeshColorData->Mil3dDepthMapImage,
                         pGet3DMeshColorData->Mil3dColorDepthMapImage);
   }
//*****************************************************************************
// Thread function that effectively updates the 3d display.
//*****************************************************************************
MIL_UINT32 MFTYPE Update3dDisplayThread(void *UserDataPtr)
   {
   SGet3DMeshColorProcFuncData* pGet3DMeshColorData = (SGet3DMeshColorProcFuncData*) UserDataPtr;
   while(!pGet3DMeshColorData->StopUpdate3dDisplay)
      {
      // Wait for the update event.
      MthrWait(pGet3DMeshColorData->MilD3DUpdateEvent, M_EVENT_WAIT, M_NULL);

      // If we need to update.
      if(!pGet3DMeshColorData->StopUpdate3dDisplay)
         MdepthD3DSetImages(pGet3DMeshColorData->DispD3DHandle, pGet3DMeshColorData->Mil3dDepthMapImage,
         pGet3DMeshColorData->Mil3dColorDepthMapImage);
      }
   return 0;
   }

//*****************************************************************************
// Function that sets the title and position of the display.
//*****************************************************************************
void SetupDisplay(MIL_ID MilDisplay, MIL_CONST_TEXT_PTR WindowTitle, MIL_INT WindowPosX, MIL_INT WindowPosY)
   {
   MdispControl(MilDisplay, M_TITLE, M_PTR_TO_DOUBLE(WindowTitle));
   MdispControl(MilDisplay, M_WINDOW_INITIAL_POSITION_X, (MIL_DOUBLE)WindowPosX);
   MdispControl(MilDisplay, M_WINDOW_INITIAL_POSITION_Y, (MIL_DOUBLE)WindowPosY);
   }

//*****************************************************************************
// Simple function to fill the display lut. The colors of the luts are based on 
// MIL M_PSEUDO display lut.
//*****************************************************************************
void FillDisplayLut(MIL_ID MilDisplayLut)
   {
   MbufClear(MilDisplayLut, 0);
   MIL_ID MilLutRed = MbufChildColor(MilDisplayLut, M_RED, M_NULL);
   MIL_ID MilLutGreen = MbufChildColor(MilDisplayLut, M_GREEN, M_NULL);
   MIL_ID MilLutBlue = MbufChildColor(MilDisplayLut, M_BLUE, M_NULL);

   MgenLutRamp(MilLutGreen, 1, 0, 491, 127);
   MgenLutRamp(MilLutGreen, 492, 128, 995, 0);
   MgenLutRamp(MilLutGreen, 996, 0, 1499, 0);
   MgenLutRamp(MilLutGreen, 1500, 0, 2999, 255);

   MgenLutRamp(MilLutBlue, 1, 0, 995, 191);
   MgenLutRamp(MilLutBlue, 996, 192, 1745, 0);
   MgenLutRamp(MilLutBlue, 1746, 0, 1991, 0);
   MgenLutRamp(MilLutBlue, 1992, 0, 2999, 255);   

   MgenLutRamp(MilLutRed, 1, 0, 491, 0);
   MgenLutRamp(MilLutRed, 492, 0, 1499, 255);
   MgenLutRamp(MilLutRed, 1500, 255, 2999, 255);

   MbufFree(MilLutRed);
   MbufFree(MilLutGreen);
   MbufFree(MilLutBlue);
   }

//*****************************************************************************
// Simple function that terminates the program abruptly when an error occurs.
//*****************************************************************************
void FatalError(CKinectCameraInterface* pKinectCamera, const MIL_TEXT_CHAR* pMsg)
   {
   pKinectCamera->CloseDown();
   delete pKinectCamera;
   MosPrintf(pMsg);
   MosPrintf(MIL_TEXT("\nPress <Enter> to end.\n"));
   MosGetch();
   exit(-1);
   }

