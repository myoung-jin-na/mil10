//***************************************************************************************/
// 
// File name: CameraOnRobotArmCalibration.cpp  
//
// Synopsis:  This program contains an example of 3d robotics calibration using the milcal module.
//            See the PrintHeader() function below for detailed description.
//
// Printable calibration grids in PDF format can be found in your "Matrox Imaging/Images/"
// directory.
//
#include <mil.h>

// DirectX display is only available under Windows.
#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   #define USE_D3D_DISPLAY 1
#else
   #define USE_D3D_DISPLAY 0
#endif

#if USE_D3D_DISPLAY
   // Include DirectX display only under Windows.
   #include "MdispD3D.h"
#endif

#if   M_MIL_USE_LINUX
#define SAVE_PATH MIL_TEXT("")
#elif M_MIL_USE_CE
#define SAVE_PATH MIL_TEXT("\\Userdisk\\")
#else
#define SAVE_PATH MIL_TEXT("")
#endif

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("CameraOnRobotArmCalibration\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example shows how to calibrate a 3d robotics setup where a camera is\n")
             MIL_TEXT("attached to a robotic arm. The calibration module is used to:\n")
             MIL_TEXT(" - Calibrate the camera.\n")
             MIL_TEXT(" - Find the pose of the camera coordinate system with respect to the\n")
             MIL_TEXT("   robot tool coordinate system.\n")
             MIL_TEXT(" - Find the pose of the robot base coordinate system with respect to the\n")
             MIL_TEXT("   absolute coordinate system.\n\n")
 
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, buffer, calibration.\n\n")

             MIL_TEXT("Press <Enter> to start.\n\n"));
   MosGetch();
   }

//****************************************************************************
// Struct containing all necessary informations for a single calibration step:
// image file name of the calibration grid, corner hint and robot pose.
//****************************************************************************
struct SCalibrationData
   {
   const MIL_TEXT_CHAR* m_ImageFile;
   MIL_DOUBLE m_GridCornerHintX;
   MIL_DOUBLE m_GridCornerHintY;
   MIL_DOUBLE m_ToolPositionX;
   MIL_DOUBLE m_ToolPositionY;
   MIL_DOUBLE m_ToolPositionZ;
   MIL_DOUBLE m_ToolRotationX;
   MIL_DOUBLE m_ToolRotationY;
   MIL_DOUBLE m_ToolRotationZ;
   };

//****************************************************************************
// Constants.
//****************************************************************************

// Directory containing all images used in this example.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("CameraOnRobotArmCalibration/")

// File name of the 3d robotics calibration context saved at the end of the example.
static const MIL_TEXT_CHAR* const OUTPUT_CALIBRATION_FILE = SAVE_PATH MIL_TEXT("MilRobotCalibration.mca");

// Number of images used to calibrate the setup.
static const MIL_INT NB_CALIBRATION_IMAGES = 7;

// All necessary informations to calibrate the 3d robotics setup.
static const SCalibrationData CALIBRATION_DATA[NB_CALIBRATION_IMAGES] =
   {
      //           File                              HintX   HintY TranslationX TranslationY TranslationZ    RotationX   RotationY   RotationZ
      {EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid0.mim"), M_NONE, M_NONE,  -29.999479,  700.000122,  510.000092,  174.405594, 28.591669,  91.206627},
      {EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid1.mim"), M_NONE, M_NONE,  -51.989830,  599.020020,  505.920288,  173.120300, 20.788210,  95.883133},
      {EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid2.mim"), M_NONE, M_NONE,  118.010101,  629.020020,  515.919983, -169.119003, 24.478680,  79.661667},
      {EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid3.mim"), M_NONE, M_NONE,  118.009903,  719.020020,  505.920105, -167.463898, 31.302469,  85.128510},
      {EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid4.mim"), M_NONE, M_NONE,  -11.990170,  519.020081,  415.920013,  179.393494, 16.471180,  91.697990},
      {EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid5.mim"), M_NONE, M_NONE,  -29.999969,  399.999786,  509.999786,  175.392303,  0.067751,  97.142853},
      {EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid6.mim"), M_NONE, M_NONE, -130.000000,  399.999786,  510.000000,  164.944305,  7.392438, 115.798599}
   };

// One last pose to use after calibration is done to test accuracy.
static const SCalibrationData TEST_DATA =
      //           File                              HintX   HintY TranslationX TranslationY TranslationZ    RotationX   RotationY   RotationZ
      {EXAMPLE_IMAGE_PATH MIL_TEXT("TestGrid.mim"), M_NONE, M_NONE,   18.009741,  629.019775,  505.919891,  178.899307, 22.548679,  88.419952}; //2

// Parameters for McalGrid(). Depends on the calibration grid used.
static const MIL_INT    ROW_NUMBER     = 20;
static const MIL_INT    COLUMN_NUMBER  = 20;
static const MIL_DOUBLE ROW_SPACING    = 10.05;           // in mm
static const MIL_DOUBLE COLUMN_SPACING = 10.00;           // in mm
static const MIL_INT    GRID_TYPE      = M_CHESSBOARD_GRID;

// Colors used to draw points in the overlay.
static const MIL_DOUBLE PIXEL_COLOR = M_COLOR_GREEN;
static const MIL_DOUBLE WORLD_COLOR = M_COLOR_RED;

// Size of the DirectX display.
static const MIL_INT D3D_DISPLAY_SIZE_X = 600;
static const MIL_INT D3D_DISPLAY_SIZE_Y = 600;

// String used to separate output sections.
static const MIL_TEXT_CHAR* const SEPARATOR = MIL_TEXT("--------------------\n\n");

//****************************************************************************
// Functions declarations.
//****************************************************************************

void MoveRobotPose(MIL_ID MilCalibration, const SCalibrationData& rData);
void AddCalibrationGrid(MIL_ID  MilDisplay,
                        MIL_ID  MilCalibration,
                        MIL_ID  MilDisplayImage,
                        MIL_ID  MilOverlayImage,
                        MIL_INT ImageIndex);
void ShowCalibrationResults(MIL_ID MilDisplay, MIL_ID MilCalibration, MIL_ID MilDisplayImage, MIL_ID MilOverlayImage);
void TestCalibration(MIL_ID  MilDisplay,
                     MIL_ID  MilCalibration,
                     MIL_ID  MilDisplayImage,
                     MIL_ID  MilOverlayImage);

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate MIL objects. 
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

   PrintHeader();

   // Allocate calibration context for 3d robotics.
   MIL_ID MilCalibration = McalAlloc(MilSystem, M_3D_ROBOTICS, M_DEFAULT, M_NULL);

   // Create an image buffer with the right settings and select it to display.
   MIL_INT SizeX = MbufDiskInquire(TEST_DATA.m_ImageFile, M_SIZE_X, M_NULL);
   MIL_INT SizeY = MbufDiskInquire(TEST_DATA.m_ImageFile, M_SIZE_Y, M_NULL);
   MIL_ID MilDisplayImage = MbufAlloc2d(MilSystem, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MbufClear(MilDisplayImage, 0.0);
   MdispSelect(MilDisplay, MilDisplayImage);

   // Prepare overlay for annotations.
   MIL_ID MilOverlayImage;
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   // Add informations for calibration.
   for (MIL_INT ImageIndex = 0; ImageIndex < NB_CALIBRATION_IMAGES; ++ImageIndex)
      {
      MosPrintf(MIL_TEXT("The robot arm is at pose #%d.\n\n"), ImageIndex);

      // Move robot and provide new pose to the calibration module.
      MoveRobotPose(MilCalibration, CALIBRATION_DATA[ImageIndex]);

      // Provide a calibration grid to the calibration module and verify feature extraction.
      AddCalibrationGrid(MilDisplay, MilCalibration, MilDisplayImage, MilOverlayImage, ImageIndex);

      MosPrintf(SEPARATOR);
      }

   MosPrintf(MIL_TEXT("The 3d robotics calibration is performed using all the accumulated data.\n"));

   // Calibrate 3d robotics calibration context using all accumulated informations.
   McalGrid(MilCalibration, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   // Verify that calibration succeeded.
   MIL_INT CalibrationStatus = McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL);

   if (CalibrationStatus == M_CALIBRATED)
      {
      MosPrintf(MIL_TEXT("The calibration is successful.\n\n"));

      // Show each pose, using DirectX display if available.
      ShowCalibrationResults(MilDisplay, MilCalibration, MilDisplayImage, MilOverlayImage);

      MosPrintf(SEPARATOR);

      // Use one last pose to test the accuracy of the calibration.
      TestCalibration(MilDisplay, MilCalibration, MilDisplayImage, MilOverlayImage);
      }
   else
      {
      MosPrintf(MIL_TEXT("The calibration failed.\n\n")
                MIL_TEXT("Press <Enter> to exit.\n\n"));
      MosGetch();
      }

   // Free MIL objects.
   MbufFree(MilDisplayImage);
   McalFree(MilCalibration);

   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//*****************************************************************************
// Sets the tool coordinate system with respect to the robot.
//*****************************************************************************
void MoveRobotPose(MIL_ID MilCalibration, const SCalibrationData& rData)
   {
   // In a real application, this is where the robot should move to a new location.
   // After moving, the robot software should be used to query the location of its
   // tool (or hand). In this example, this information is hardcoded in the given
   // rData structure.

   // Provide the tool pose given by the robot software to the calibration module.
   // Note that the TransformTypes used (M_TRANSLATION, then M_ROTATION_ZYX in this
   // case) is dependent on the robot controller.
   McalSetCoordinateSystem(MilCalibration, 
                           M_TOOL_COORDINATE_SYSTEM, 
                           M_ROBOT_BASE_COORDINATE_SYSTEM,
                           M_TRANSLATION+M_ASSIGN,
                           M_NULL, 
                           rData.m_ToolPositionX,
                           rData.m_ToolPositionY,
                           rData.m_ToolPositionZ,
                           M_DEFAULT);

   McalSetCoordinateSystem(MilCalibration, 
                           M_TOOL_COORDINATE_SYSTEM, 
                           M_TOOL_COORDINATE_SYSTEM,
                           M_ROTATION_ZYX+M_COMPOSE_WITH_CURRENT, 
                           M_NULL, 
                           rData.m_ToolRotationZ,
                           rData.m_ToolRotationY,
                           rData.m_ToolRotationX,
                           M_DEFAULT);
   }

//*****************************************************************************
// Analyzes one more calibration grid, and display extracted features.
//*****************************************************************************
void AddCalibrationGrid(MIL_ID  MilDisplay,
                        MIL_ID  MilCalibration,
                        MIL_ID  MilDisplayImage,
                        MIL_ID  MilOverlayImage,
                        MIL_INT ImageIndex)
   {
   MosPrintf(MIL_TEXT("An image of the calibration grid is taken at that position, and used for\n")
             MIL_TEXT("calibration.\n\n")

             MIL_TEXT("Calling McalGrid(): "));

   const SCalibrationData& rData = CALIBRATION_DATA[ImageIndex];

   // Load the image of the calibration grid to the display.
   MbufLoad(rData.m_ImageFile, MilDisplayImage);

   // Provide a hint of where is the top-left corner of the grid.
   McalControl(MilCalibration, M_GRID_CORNER_HINT_X, rData.m_GridCornerHintX);
   McalControl(MilCalibration, M_GRID_CORNER_HINT_Y, rData.m_GridCornerHintY);

   // Add this grid (note the use of M_ACCUMULATE).
   McalGrid(MilCalibration, MilDisplayImage,
            0.0, 0.0, 0.0,                   // GridOffset
            ROW_NUMBER,   COLUMN_NUMBER,
            ROW_SPACING,  COLUMN_SPACING,
            M_ACCUMULATE, GRID_TYPE);

   // Verify that the operation succeeded.
   MIL_INT CalibrationStatus = McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL);

   // When using M_ACCUMULATE, a succesful call to McalGrid() leaves the calibration context
   // in a partially calibrated state, thus its status should be M_CALIBRATING.
   if (CalibrationStatus == M_CALIBRATING)
      {
      // Draw the features extracted from the grid in the overlay.
      MgraColor(M_DEFAULT, PIXEL_COLOR);
      McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_IMAGE_POINTS, ImageIndex, M_DEFAULT);

      MosPrintf(MIL_TEXT("extracted features are displayed in green.\n")
                MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Clear overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      }
   else
      {
      MosPrintf(MIL_TEXT("the grid was not found.\n")
                MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();
      }
   }

//*****************************************************************************
// Shows each pose, using DirectX display if available, and print error informations.
//*****************************************************************************
void ShowCalibrationResults(MIL_ID MilDisplay, MIL_ID MilCalibration, MIL_ID MilDisplayImage, MIL_ID MilOverlayImage)
   {
   // Print results on global errors.
   MIL_DOUBLE AveragePixelError, MaximumPixelError, AverageWorldError, MaximumWorldError;
   McalInquire( MilCalibration, M_GLOBAL_AVERAGE_PIXEL_ERROR, &AveragePixelError );   
   McalInquire( MilCalibration, M_GLOBAL_MAXIMUM_PIXEL_ERROR, &MaximumPixelError );
   McalInquire( MilCalibration, M_GLOBAL_AVERAGE_WORLD_ERROR, &AverageWorldError );   
   McalInquire( MilCalibration, M_GLOBAL_MAXIMUM_WORLD_ERROR, &MaximumWorldError );

   MosPrintf(MIL_TEXT("Global pixel error\n   average: %.3g pixels\n   maximum: %.3g pixels\n"),
             AveragePixelError, MaximumPixelError);
   MosPrintf(MIL_TEXT("Global world error\n   average: %.3g mm\n   maximum: %.3g mm\n\n"),
             AverageWorldError, MaximumWorldError);

   // Save the calibration context.
   McalSave(OUTPUT_CALIBRATION_FILE, MilCalibration, M_DEFAULT);

   MosPrintf(MIL_TEXT("The calibration context was saved as '"));
   MosPrintf(OUTPUT_CALIBRATION_FILE);
   MosPrintf(MIL_TEXT("'.\nPress <Enter> to verify the calibration accuracy for each pose.\n\n"));
   MosGetch();

   MosPrintf(SEPARATOR);

#if USE_D3D_DISPLAY
   // Allocate DirectX display.
   MIL_DISP_D3D_HANDLE DispHandle;
   DispHandle = McalD3DAlloc(NULL, 0, D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y, 0);
#endif

   // Show each grid (with both types of draw).
   for (MIL_INT ImageIndex = 0; ImageIndex < NB_CALIBRATION_IMAGES; ++ImageIndex)
      {
      MosPrintf(MIL_TEXT("Pose #%d\n"), ImageIndex);
      MosPrintf(MIL_TEXT("-------\n\n"));

      // Load the next image.
      MbufLoad(CALIBRATION_DATA[ImageIndex].m_ImageFile, MilDisplayImage);

      // Move the tool to the pose used for calibration.
      MoveRobotPose(MilCalibration, CALIBRATION_DATA[ImageIndex]);

      // Set all coordinate systems of the image to those used during calibration.
      McalAssociate(MilCalibration, MilDisplayImage, M_DEFAULT);

      // Print results on errors for this pose only.
      McalInquireSingle( MilCalibration, ImageIndex, M_AVERAGE_PIXEL_ERROR, &AveragePixelError );   
      McalInquireSingle( MilCalibration, ImageIndex, M_MAXIMUM_PIXEL_ERROR, &MaximumPixelError );
      McalInquireSingle( MilCalibration, ImageIndex, M_AVERAGE_WORLD_ERROR, &AverageWorldError );   
      McalInquireSingle( MilCalibration, ImageIndex, M_MAXIMUM_WORLD_ERROR, &MaximumWorldError );

      MosPrintf(MIL_TEXT("Pixel error\n   average: %.3g pixels\n   maximum: %.3g pixels\n"),
                AveragePixelError, MaximumPixelError);
      MosPrintf(MIL_TEXT("World error\n   average: %.3g mm\n   maximum: %.3g mm\n\n"),
                AverageWorldError, MaximumWorldError);

      // Draw the features extracted from the grid AND the world points in the overlay.
      MgraColor(M_DEFAULT, PIXEL_COLOR);
      McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_IMAGE_POINTS, ImageIndex, M_DEFAULT);
      MgraColor(M_DEFAULT, WORLD_COLOR);
      McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_WORLD_POINTS, ImageIndex, M_DEFAULT);
      
      MosPrintf(MIL_TEXT("Green: extracted features (pixels).\n")
                MIL_TEXT("Red:   world points converted to pixels using the calibration context.\n\n"));

#if USE_D3D_DISPLAY
      if (DispHandle != NULL)
         {
         // Show the grid image and coordinate systems.
         McalD3DSetImages(DispHandle, &MilDisplayImage, 1);

         MosPrintf(MIL_TEXT("The DirectX display shows the coordinate systems for calibration pose #%d.\n\n"), ImageIndex);
         MdispD3DPrintHelp(DispHandle);

         // Show DirectX window if allocation succeeded and print instructions.
         if (ImageIndex == 0)
            MdispD3DShow(DispHandle);
         }
#endif

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Clear overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      }

#if USE_D3D_DISPLAY
   // Free DirectX display.
   if (DispHandle != NULL)
      {
      MdispD3DHide(DispHandle);
      MdispD3DFree(DispHandle);
      }
#endif
   }

//*****************************************************************************
// Use one last pose to test the accuracy of the calibration.
//*****************************************************************************
void TestCalibration(MIL_ID  MilDisplay,
                     MIL_ID  MilCalibration,
                     MIL_ID  MilDisplayImage,
                     MIL_ID  MilOverlayImage)
   {
   // Move one last time.
   MoveRobotPose(MilCalibration, TEST_DATA);

   // Load image at the new position. This is an image of the same calibration grid used
   // for the calibration, but this particular pose was not used.
   MbufLoad(TEST_DATA.m_ImageFile, MilDisplayImage);

   // Draw on image and show that it goes on the grid not used for calibration

   // This draw operation will show, in the overlay, the position of the world points used
   // during calibration, but taking into account the calibration of the overlay (and thus
   // the current pose).
   MgraColor(M_DEFAULT, WORLD_COLOR);
   McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_WORLD_POINTS, M_DEFAULT, M_DEFAULT);

   // If the calibration is accurate, the marks should be on top of the grid.
   MosPrintf(MIL_TEXT("The robot arm is moved to a new position. McalSetCoordinateSystem() is used\n")
             MIL_TEXT("to provide the new tool pose to the calibration module, thus the system\n")
             MIL_TEXT("remains fully calibrated.\n\n")

             MIL_TEXT("The calibration grid is grabbed at the new camera position. This grabbed\n")
             MIL_TEXT("image was not used during calibration. The corners of the grid are not\n")
             MIL_TEXT("extracted from this image; no image processing is performed.\n\n")

             MIL_TEXT("Instead, the world points of the calibration grid are converted to pixels,\n")
             MIL_TEXT("using the calibration module with the new tool pose. Since these points\n")
             MIL_TEXT("(displayed in red) coincide with the corners of the grabbed grid, the\n")
             MIL_TEXT("calibration is accurate.\n\n")

             MIL_TEXT("Press <Enter> to exit.\n\n"));
   MosGetch();

   // Clear overlay.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   }
