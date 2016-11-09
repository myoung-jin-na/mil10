//***************************************************************************************
//
// File name: CameraLaserCalibration.cpp
//
// Synopsis:  This program contains an example of camera-laser calibration using milcal and
//            mil3dmap modules. See the PrintHeader() function below for detailed description.
//
// Printable calibration grids in PDF format can be found in your "Matrox Imaging/Images/"
// directory.
//
// When considering a laser-based 3D reconstruction system, the file "3D Setup Helper.xls"
// can be used to accelerate prototyping by choosing an adequate hardware configuration
// (angle, distance, lens, camera, ...). The file is located in your
// "Matrox Imaging/Tools/" directory.
//

#include <mil.h>
#include "interactiveexample.h"
#include "standaloneexample.h"

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
             MIL_TEXT("CameraLaserCalibration\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example demonstrates how to create a robust calibration of a\n")
             MIL_TEXT("3d reconstruction setup consisting of a camera and a laser line.\n\n")

             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, digitizer, buffer, graphic,\n")
             MIL_TEXT("              image processing, calibration, 3d reconstruction.\n\n"));
   }

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// File names

// Name of the camera calibration file to save (or to reload).
static const MIL_TEXT_CHAR* const CAMERA_CALIBRATION_FILE = SAVE_PATH MIL_TEXT("MilCalibration.mca");

// Name of the image file of the calibration grid to save.
static const MIL_TEXT_CHAR* const CALIBRATION_GRID_FILE   = SAVE_PATH MIL_TEXT("CalibrationGrid.mim");

// Name of the 3d reconstruction context file to save.
static const MIL_TEXT_CHAR* const CALIBRATION_3D_FILE     = SAVE_PATH MIL_TEXT("MilLaser.m3d");

// Pattern for file name of the images of laser lines used to calibrate the
// 3d reconstruction context.
static const MIL_TEXT_CHAR* const REFERENCE_PLANE_FILE_PATTERN = SAVE_PATH MIL_TEXT("ReferencePlane%d.mim");

//*****************************************************************************
// Parameters for McalGrid(). Depends on the calibration grid used.

static const MIL_INT    ROW_NUMBER     = 15;
static const MIL_INT    COLUMN_NUMBER  = 16;
static const MIL_DOUBLE ROW_SPACING    = 5.0;             // in mm
static const MIL_DOUBLE COLUMN_SPACING = 5.0;             // in mm
static const MIL_INT    GRID_TYPE      = M_CIRCLE_GRID;

//*****************************************************************************
// Parameters used during 3d reconstruction setup calibration.

// Z-position of the first reference plane.
static const MIL_DOUBLE INITIAL_CALIBRATION_DEPTH   =   0.0; // in mm

// Z difference between consecutive reference planes. This is negative to indicate that
// each reference plane is higher that the previous one (since the Z axis points downwards).
static const MIL_DOUBLE CALIBRATION_DEPTH_PER_PLANE = -12.0; // in mm

// Conversion constant that could be used to generate depth maps in M_DEPTH_CORRECTION mode.
// It allows to convert from a world Z position to an output depth map gray level.
static const MIL_DOUBLE GRAY_LEVEL_PER_WORLD_UNIT = -1000.0; // in gray level/mm

//*****************************************************************************
// Functions declarations.
//*****************************************************************************

MIL_ID CalibrateCamera(CExampleInterface* pExample);
bool DiagnoseCameraCalibration(CExampleInterface* pExample, MIL_ID MilCalibration);
void SetupLineExtractionParameters(CExampleInterface* pExample,
                                   MIL_INT            CalibrationMode,
                                   MIL_INT*           pMinIntensity,
                                   MIL_INT*           pPeakWidth);
bool CalibrateLaser(CExampleInterface* pExample, MIL_ID MilLaser, MIL_ID MilCameraCalibration);
bool DiagnoseFullCalibration(CExampleInterface* pExample, MIL_ID MilLaser);
bool DiagnoseDepthCalibration(CExampleInterface* pExample, MIL_ID MilLaser);

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Print example description.
   PrintHeader();

   bool RunInteractiveExample = CExampleInterface::AskInteractiveExample();

   // Allocate MIL objects: application, system, display and digitizer (if needed).
   CExampleInterface* pExample;
   if (RunInteractiveExample)
      pExample = new CInteractiveExample;
   else
      pExample = new CStandAloneExample;

   // If any allocation failed (mainly, the system or digitizer), then exit now.
   if (!pExample->IsValid())
      {
      MosPrintf(MIL_TEXT("Unable to allocate all MIL objects (system, display, digitizer).\n\n")
                MIL_TEXT("Press <Enter> to exit.\n\n"));
      MosGetch();
      delete pExample;
      return -1;
      }

   // Choose 3dmap mode.
   MIL_INT CalibrationMode = pExample->AskCalibrationMode();

   // Calibrate the camera if needed.
   MIL_ID MilCameraCalibration = M_NULL;
   if (CalibrationMode == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      MilCameraCalibration = CalibrateCamera(pExample);

   // Allocate display image buffer, select it to display and prepare overlay for
   // annotations if not done already.
   pExample->ShowDisplay();

   // Choose values for M_MIN_INTENSITY and M_PEAK_WIDTH.
   // For now, this example leave M_PEAK_WIDTH to its default value.
   MIL_INT MinIntensity, PeakWidth;
   SetupLineExtractionParameters(pExample, CalibrationMode, &MinIntensity, &PeakWidth);

   // Calibration loop until successful.
   bool CalibrationSuccessful = false;
   while (!CalibrationSuccessful)
      {
      // Create a new 3d reconstruction context.
      MIL_ID MilLaser = M3dmapAlloc(pExample->GetMilSystem(), M_LASER, CalibrationMode, M_NULL);

      M3dmapControl(MilLaser, M_DEFAULT, M_MIN_INTENSITY, MinIntensity);
      M3dmapControl(MilLaser, M_DEFAULT, M_PEAK_WIDTH   , PeakWidth   );

      // Call calibration routine.
      CalibrationSuccessful = CalibrateLaser(pExample, MilLaser, MilCameraCalibration);

      // If calibration succeeded, save the 3d reconstruction context before exiting.
      if (CalibrationSuccessful)
         {
         pExample->HideDisplay();

         M3dmapSave(CALIBRATION_3D_FILE, MilLaser, M_DEFAULT);
         MosPrintf(MIL_TEXT("Calibration was successful.\n")
                   MIL_TEXT("3D reconstruction context was saved as '"));
         MosPrintf(CALIBRATION_3D_FILE);
         MosPrintf(MIL_TEXT("'\n\nPress <Enter> to exit.\n\n"));
         MosGetch();
         }

      M3dmapFree(MilLaser);
      }

   // Free MIL objects.
   if (MilCameraCalibration != M_NULL)
      McalFree(MilCameraCalibration);
   delete pExample;

   return 0;
   }

//*****************************************************************************
// Allocates a new camera calibration context and grabs a calibration grid to
// calibrate it. The resulting calibration context is returned when successful.
//*****************************************************************************
MIL_ID CalibrateCamera(CExampleInterface* pExample)
   {
   MosPrintf(MIL_TEXT("Calibrating the camera\n")
             MIL_TEXT("======================\n\n"));

   // First, let's look if there is a calibration file in local directory.
   // Try to reload it, and if it works, ask user if he wants it.
   MIL_ID MilCalibration = pExample->TryToReloadCameraCalibration(CAMERA_CALIBRATION_FILE);

   if (MilCalibration == M_NULL)
      {
      // Allocate display image buffer, select it to display and prepare overlay for
      // annotations if not done already.
      pExample->ShowDisplay();

      // The calibration context was not reloaded, create one here.
      // Allocate calibration context in 3D mode.
      McalAlloc(pExample->GetMilSystem(), M_TSAI_BASED, M_DEFAULT, &MilCalibration);

      // Calibration loop until successful.
      bool CalibrationSuccessful = false;
      while (!CalibrationSuccessful)
         {
         // Grab continuously until user presses <Enter>.
         pExample->CopyInOverlay(CExampleInterface::eCalibrateCameraImage);
         pExample->GrabCalibrationGrid();

         // Calibrate the camera.
         MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
         McalGrid(MilCalibration, pExample->GetMilDisplayImage(), 0.0, 0.0, 0.0, ROW_NUMBER, COLUMN_NUMBER,
                  ROW_SPACING, COLUMN_SPACING, M_DEFAULT, GRID_TYPE);
         MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

         MIL_INT CalibrationStatus = McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL);

         switch (CalibrationStatus)
            {
            case M_CALIBRATED:
               // When calibration is successful, use diagnostic functionalities to ensure
               // it is accurate.
               CalibrationSuccessful = DiagnoseCameraCalibration(pExample, MilCalibration);
               break;

            case M_GRID_NOT_FOUND:
               MosPrintf(MIL_TEXT("The camera calibration failed because the grid was not found.\n\n"));
               break;

            case M_PLANE_ANGLE_TOO_SMALL:
               MosPrintf(MIL_TEXT("The camera calibration failed because the camera's optical axis is not\n")
                         MIL_TEXT("sufficiently inclined (inclination should be at least 30 degrees).\n\n"));
               break;

            default:
               MosPrintf(MIL_TEXT("Calibration failed for unexpected reasons.\n\n"));
               break;
            }

         pExample->PauseInStandAloneMode();
         if (!CalibrationSuccessful)
            MosPrintf(SEPARATOR);

         // Clear overlay.
         MdispControl(pExample->GetMilDisplay(), M_OVERLAY_CLEAR, M_DEFAULT);
         }

      pExample->HideDisplay();

      // Save objects related to camera calibration.
      McalSave(CAMERA_CALIBRATION_FILE, MilCalibration, M_DEFAULT);
      MbufSave(CALIBRATION_GRID_FILE, pExample->GetMilDisplayImage());
      MosPrintf(MIL_TEXT("The camera calibration was successful. The calibration context was saved as\n'"));
      MosPrintf(CAMERA_CALIBRATION_FILE);
      MosPrintf(MIL_TEXT("' and the calibration grid image was saved as\n'"));
      MosPrintf(CALIBRATION_GRID_FILE);
      MosPrintf(MIL_TEXT("'.\n\nPress <Enter> to continue.\n\n"));
      MosGetch();
      }

   return MilCalibration;
   }

//*****************************************************************************
// Use diagnostic functionalities to ensure given camera calibration context is
// accurate. If so, return true.
//*****************************************************************************
bool DiagnoseCameraCalibration(CExampleInterface* pExample, MIL_ID MilCalibration)
   {
   // Both conditions must be true for the camera calibration to be good.
   bool ExtractionIsAccurate = false;
   bool CalibrationIsAccurate = false;

   // Draw calibration points in green.
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   McalDraw(M_DEFAULT, MilCalibration, pExample->GetMilOverlayImage(), M_DRAW_IMAGE_POINTS, M_DEFAULT, M_DEFAULT);
   pExample->CopyInOverlay(CExampleInterface::eCalibrateCameraImage);
   MosPrintf(MIL_TEXT("Calibration points extracted from the image are displayed in green.\n\n"));

   // Asks user if extraction is OK.
   ExtractionIsAccurate = pExample->AskIfFeatureExtractionAccurate();

   if (ExtractionIsAccurate)
      {
      // Show some error information.
      MIL_DOUBLE AveragePixelError, MaximumPixelError, AverageWorldError, MaximumWorldError;
      McalInquire(MilCalibration, M_AVERAGE_PIXEL_ERROR, &AveragePixelError);
      McalInquire(MilCalibration, M_MAXIMUM_PIXEL_ERROR, &MaximumPixelError);
      McalInquire(MilCalibration, M_AVERAGE_WORLD_ERROR, &AverageWorldError);
      McalInquire(MilCalibration, M_MAXIMUM_WORLD_ERROR, &MaximumWorldError);

      MosPrintf(MIL_TEXT("Calibration points, transformed using the calibration context, are shown in red.\n"));
      MosPrintf(MIL_TEXT("Pixel error\n   average: %.3g pixels\n   maximum: %.3g pixels\n"), AveragePixelError, MaximumPixelError);
      MosPrintf(MIL_TEXT("World error\n   average: %.3g mm\n   maximum: %.3g mm\n\n"), AverageWorldError, MaximumWorldError);

      // Draw coordinate system in gray.
      MgraColor(M_DEFAULT, M_COLOR_LIGHT_GRAY);
      McalDraw(M_DEFAULT, MilCalibration, pExample->GetMilOverlayImage(), M_DRAW_ABSOLUTE_COORDINATE_SYSTEM+M_DRAW_AXES, M_DEFAULT, M_DEFAULT);

      // Draw calibration points in red.
      MgraColor(M_DEFAULT, M_COLOR_RED);
      McalDraw(M_DEFAULT, MilCalibration, pExample->GetMilOverlayImage(), M_DRAW_WORLD_POINTS, M_DEFAULT, M_DEFAULT);
      pExample->CopyInOverlay(CExampleInterface::eCalibrateCameraImage);

      // Asks user if calibration is accurate, i.e. if red and green marks coincide.
      CalibrationIsAccurate = pExample->AskIfCameraCalibrationAccurate();
      }

   // Calibration is OK if user answered YES to both questions.
   return (ExtractionIsAccurate && CalibrationIsAccurate);
   }

//*****************************************************************************
// Choose values for M_MIN_INTENSITY and M_PEAK_WIDTH.
// For now, this example leave M_PEAK_WIDTH to its default value.
//*****************************************************************************
void SetupLineExtractionParameters(CExampleInterface* pExample,
                                   MIL_INT            CalibrationMode,
                                   MIL_INT*           pMinIntensity,
                                   MIL_INT*           pPeakWidth)
   {
   MosPrintf(MIL_TEXT("Setting up the laser line extraction parameters\n")
             MIL_TEXT("===============================================\n\n"));

   // Create a child for green annotations.
   MgraColor(M_DEFAULT, 255.0);
   MIL_ID MilOverlayGreenBand = MbufChildColor(pExample->GetMilOverlayImage(), M_GREEN, M_NULL);

   // Create 3dmap objects.
   MIL_ID MilLaser = M3dmapAlloc(pExample->GetMilSystem(), M_LASER, CalibrationMode, M_NULL);
   MIL_ID MilScan  = M3dmapAllocResult(pExample->GetMilSystem(), M_LASER_DATA, M_DEFAULT, M_NULL);

   // Initialize M_MIN_INTENSITY and M_PEAK_WIDTH variables to the module default values.
   M3dmapInquire(MilLaser, M_DEFAULT, M_MIN_INTENSITY+M_TYPE_MIL_INT+M_DEFAULT, pMinIntensity);
   M3dmapInquire(MilLaser, M_DEFAULT, M_PEAK_WIDTH   +M_TYPE_MIL_INT+M_DEFAULT, pPeakWidth   );

   pExample->PrintExplanationForMinIntensity();

   // Loop until user is satisfied with the M_MIN_INTENSITY setting.
   bool IsFinished = false;
   while (!IsFinished)
      {
      // Allow user to change M_MIN_INTENSITY by using (1,2,4,5); <Enter> breaks from the loop.
      // This is not a blocking call.
      IsFinished = pExample->AskMinIntensityAdjust(pMinIntensity);

      // Grabs a single image of the laser line.
      pExample->GrabLaserLineToAdjustIntensity();

      // Extract laser line from grabbed image, using current value for M_MIN_INTENSITY.
      M3dmapControl(MilLaser, M_DEFAULT, M_MIN_INTENSITY, *pMinIntensity);
      M3dmapAddScan(MilLaser, MilScan, pExample->GetMilDisplayImage(), M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

      // Draw extracted line (temporarily disable overlay for faster refresh rate).
      MdispControl(pExample->GetMilDisplay(), M_UPDATE, M_DISABLE);
      MdispControl(pExample->GetMilDisplay(), M_OVERLAY_CLEAR, M_DEFAULT);
      M3dmapDraw(M_DEFAULT, MilScan, MilOverlayGreenBand, M_DRAW_PEAKS_LAST, M_DEFAULT, M_DEFAULT);
      pExample->CopyInOverlay(CExampleInterface::eAdjustMinIntensityImage);
      MdispControl(pExample->GetMilDisplay(), M_UPDATE, M_ENABLE);

      pExample->PauseInStandAloneMode();

      // Remove extracted line to avoid accumulating useless data.
      M3dmapAddScan(MilLaser, MilScan, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_REMOVE_LAST_SCAN);
      }

   MosPrintf(MIL_TEXT("\n\n"));

   // Clear overlay.
   MbufFree(MilOverlayGreenBand);
   MdispControl(pExample->GetMilDisplay(), M_OVERLAY_CLEAR, M_DEFAULT);

   // Free MIL objects.
   M3dmapFree(MilScan);
   M3dmapFree(MilLaser);
   }

//*****************************************************************************
// Grab images of laser lines at different heights to calibrate a given 3d
// reconstruction context. If it is M_CALIBRATED_CAMERA_LINEAR_MOTION, then
// the camera calibration context is also passed as parameter, else it is M_NULL.
//*****************************************************************************
bool CalibrateLaser(CExampleInterface* pExample, MIL_ID MilLaser, MIL_ID MilCameraCalibration)
   {
   MosPrintf(MIL_TEXT("Calibrating the 3d reconstruction setup\n")
             MIL_TEXT("=======================================\n\n"));

   // Create a child for green annotations.
   MgraColor(M_DEFAULT, 255.0);
   MIL_ID MilOverlayGreenBand = MbufChildColor(pExample->GetMilOverlayImage(), M_GREEN, M_NULL);

   MIL_INT SizeX = MbufInquire(pExample->GetMilDisplayImage(), M_SIZE_X, M_NULL);
   MIL_INT SizeY = MbufInquire(pExample->GetMilDisplayImage(), M_SIZE_Y, M_NULL);

   // Inquire the calibration mode to set up some different behaviors.
   MIL_INT CalibrationMode = M3dmapInquire(MilLaser, M_DEFAULT, M_LASER_CONTEXT_TYPE, M_NULL);

   // In M_CALIBRATED_CAMERA_LINEAR_MOTION mode:
   // - If only one reference plane is used, the laser plane is assumed to be perfectly vertical.
   // - Corrected depths are given in world units.
   // In M_DEPTH_CORRECTION mode:
   // - At least two reference planes are needed.
   // - Corrected depths are given in desired gray levels of the eventual output depth maps.
   //   -> Because of this, we initialize a factor to allow converting real heights to gray levels.

   MIL_INT MinReferencePlanes;
   MIL_DOUBLE Factor;
   if (CalibrationMode == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
      MinReferencePlanes = 1;
      Factor = 1.0; // keep heights in mm.
      }
   else
      {
      MinReferencePlanes = 2;
      Factor = GRAY_LEVEL_PER_WORLD_UNIT; // transform heights from mm to gray levels.
      }

   // To show some useful diagnostic informations, we need to know the number of "columns"
   // in which to locate the laser line. This depends on the M_ORIENTATION settings.
   MIL_INT Orientation = M3dmapInquire(MilLaser, M_DEFAULT, M_ORIENTATION, M_NULL);
   if (Orientation == M_DEFAULT)
      Orientation = M3dmapInquire(MilLaser, M_DEFAULT, M_ORIENTATION+M_DEFAULT, M_NULL);
   MIL_INT NbColumns = (Orientation == M_1D_COLUMNS ? SizeX : SizeY);

   // Allocate the result that will be used to calibrate the 3d reconstruction context.
   MIL_ID MilScan = M3dmapAllocResult(pExample->GetMilSystem(), M_LASER_DATA, M_DEFAULT, M_NULL);

   // If you need more that 1024 reference planes, you should control M_MAX_FRAMES here.
   MIL_INT MaxReferencePlanes = M3dmapInquire(MilScan, M_DEFAULT, M_MAX_FRAMES, M_NULL);

   // This will be used to count the number of reference planes used until now.
   MIL_INT ReferencePlaneIndex = 0;

   // Loop to add more reference planes until the user is satisfied.
   bool ReadyToCalibrate = false;
   while (!ReadyToCalibrate)
      {
      // Computed the hardcoded depth of the current reference plane.
      // Change INITIAL_CALIBRATION_DEPTH and CALIBRATION_DEPTH_PER_PLANE to suit your needs.
      MIL_DOUBLE CalibrationDepth = INITIAL_CALIBRATION_DEPTH + ReferencePlaneIndex*CALIBRATION_DEPTH_PER_PLANE;

      // Verify that the maximum number of reference planes has not been reached.
      bool MaxPlaneIsReached = (ReferencePlaneIndex >= MaxReferencePlanes);
      if (CalibrationMode == M_DEPTH_CORRECTION)
         {
         // It is not possible to provide a value outside the depth map range to
         // M_CORRECTED_DEPTH in M_DEPTH_CORRECTION mode.
         if (Factor*CalibrationDepth < 0.0 || Factor*CalibrationDepth > 65534.0)
            MaxPlaneIsReached = true;
         }

      // Break the loop if we have the maximum number of reference planes.
      if (MaxPlaneIsReached)
         {
         MosPrintf(MIL_TEXT("The maximum number of reference planes of this example has been reached.\n")
                   MIL_TEXT("Please modify the source code to use more reference planes.\n\n")
                   MIL_TEXT("Press <Enter> to continue.\n\n"));
         MosGetch();
         break;
         }

      // If we already have enough planes to calibrate, then ask the user if he wants to stop.
      bool ShouldAskIfFinished = (ReferencePlaneIndex >= MinReferencePlanes);

      // Grabs continuously until the user press <Enter> to add a new reference plane
      // or 's' if he has enough reference planes and wants to stop.
      pExample->CopyInOverlay(CExampleInterface::eCalibrateLaserImage);
      ReadyToCalibrate = pExample->GrabCalibrationLaserLine(ReferencePlaneIndex, CalibrationDepth, ShouldAskIfFinished);

      if (!ReadyToCalibrate)
         {
         // Add a new reference plane. First set the calibration depth of the plane.
         M3dmapControl(MilLaser, M_DEFAULT, M_CORRECTED_DEPTH, Factor*CalibrationDepth);

         // Extract the laser line and draw it in the overlay.
         M3dmapAddScan(MilLaser, MilScan, pExample->GetMilDisplayImage(), M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);
         M3dmapDraw(M_DEFAULT, MilScan, MilOverlayGreenBand, M_DRAW_PEAKS_LAST, M_DEFAULT, M_DEFAULT);
         pExample->CopyInOverlay(CExampleInterface::eCalibrateLaserImage);

         // Print the number of columns where the laser line could not be detected.
         MIL_INT NbColumnsWithMissingData;
         M3dmapGetResult(MilScan, M_DEFAULT, M_NUMBER_OF_MISSING_DATA_LAST_SCAN+M_TYPE_MIL_INT, &NbColumnsWithMissingData);

         MosPrintf(MIL_TEXT("Extracted laser line is displayed in green.\n"));
         MosPrintf(MIL_TEXT("Number of columns with missing data: %4d / %d (%.1f%%)\n\n"),
                   NbColumnsWithMissingData, NbColumns,
                   100.0*static_cast<MIL_DOUBLE>(NbColumnsWithMissingData)/static_cast<MIL_DOUBLE>(NbColumns));

         // Asks the user if he is satisfied with the laser line extraction.
         bool ExtractionIsAccurate = pExample->AskIfLineExtractionAccurate();
         pExample->PauseInStandAloneMode();

         // Clear the overlay.
         MdispControl(pExample->GetMilDisplay(), M_OVERLAY_CLEAR, M_DEFAULT);

         if (ExtractionIsAccurate)
            {
            // If the image is OK, save it.
            const MIL_INT MAX_FILENAME_LENGTH = 32;
            MIL_TEXT_CHAR ImageName[MAX_FILENAME_LENGTH];
            MosSprintf(ImageName, MAX_FILENAME_LENGTH, REFERENCE_PLANE_FILE_PATTERN, ReferencePlaneIndex);
            MbufSave(ImageName, pExample->GetMilDisplayImage());
            MosPrintf(MIL_TEXT("Image was saved as '%s'\n\n"), ImageName);

            ++ReferencePlaneIndex;
            }
         else
            {
            // Image is not OK, remove it from the result.
            M3dmapAddScan(MilLaser, MilScan, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_REMOVE_LAST_SCAN);
            }

         MosPrintf(SEPARATOR);
         }
      }

   // Calibrate the 3d reconstruction context using the result with all the extracted laser
   // lines that were kept by the user and using the camera calibration context, if necessary.
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   M3dmapCalibrate(MilLaser, MilScan, MilCameraCalibration, M_DEFAULT);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   MIL_INT CalibrationStatus = M3dmapInquire(MilLaser, M_DEFAULT, M_CALIBRATION_STATUS, M_NULL);

   bool CalibrationSuccessful = false;
   switch (CalibrationStatus)
      {
      case M_CALIBRATED:
         // When calibration is successful, use diagnostic functionalities to ensure
         // it is accurate.
         if (CalibrationMode == M_CALIBRATED_CAMERA_LINEAR_MOTION)
            CalibrationSuccessful = DiagnoseFullCalibration(pExample, MilLaser);
         else
            CalibrationSuccessful = DiagnoseDepthCalibration(pExample, MilLaser);
         break;

      case M_LASER_LINE_NOT_DETECTED:
         MosPrintf(MIL_TEXT("Calibration failed because laser line could not be detected in the calibration\n")
                   MIL_TEXT("image.\n\n"));
         break;

      case M_NOT_ENOUGH_MEMORY:
         MosPrintf(MIL_TEXT("Calibration failed because there was not enough available memory.\n\n"));
         break;

      default:
         MosPrintf(MIL_TEXT("Calibration failed for unexpected reasons.\n\n"));
         break;
      }

   pExample->PauseInStandAloneMode();

   // Clear overlay.
   MbufFree(MilOverlayGreenBand);
   MdispControl(pExample->GetMilDisplay(), M_OVERLAY_CLEAR, M_DEFAULT);

   // Free MIL objects.
   M3dmapFree(MilScan);

   // Returns whether the given 3d reconstruction context has been successfully calibrated or not.
   return CalibrationSuccessful;
   }

//*****************************************************************************
// Use diagnostic functionalities to ensure the accuracy of the given 3d
// reconstruction context in M_CALIBRATED_CAMERA_LINEAR_MOTION mode. If it is
// accurate, return true.
//*****************************************************************************
bool DiagnoseFullCalibration(CExampleInterface* pExample, MIL_ID MilLaser)
   {
   // Clear display and overlay.
   MbufClear(pExample->GetMilDisplayImage(), 0.0);
   MdispControl(pExample->GetMilDisplay(), M_OVERLAY_CLEAR, M_DEFAULT);

   // Show fitted lines in red.
   MgraColor(M_DEFAULT, M_COLOR_RED);
   M3dmapDraw(M_DEFAULT, MilLaser, pExample->GetMilOverlayImage(), M_DRAW_CALIBRATION_LINES, M_DEFAULT, M_DEFAULT);

   // Show all extracted laser lines in green.
   MgraColor(M_DEFAULT, 255.0);
   MIL_ID MilOverlayGreenBand = MbufChildColor(pExample->GetMilOverlayImage(), M_GREEN, M_NULL);
   M3dmapDraw(M_DEFAULT, MilLaser, MilOverlayGreenBand, M_DRAW_CALIBRATION_PEAKS, M_DEFAULT, M_DEFAULT);
   MbufFree(MilOverlayGreenBand);

   MosPrintf(MIL_TEXT("The laser plane has been fitted on the extracted laser line(s).\n")
             MIL_TEXT("   Green: extracted laser line(s).\n")
             MIL_TEXT("   Red:   expected line(s) on the fitted laser plane.\n\n"));

   // Print fit RMS error.
   MIL_DOUBLE FitRMSError;
   M3dmapInquire(MilLaser, M_DEFAULT, M_FIT_RMS_ERROR, &FitRMSError);
   MosPrintf(MIL_TEXT("Fit RMS error: %.3g mm\n\n"), FitRMSError);

   // Ask user if he is satisfied with the calibration.
   bool CalibrationIsAccurate = pExample->AskIfLaserCalibrationAccurate();
   return CalibrationIsAccurate;
   }

//*****************************************************************************
// Use diagnostic functionalities to ensure the accuracy of the given 3d
// reconstruction context in M_DEPTH_CORRECTION mode. If it is accurate,
// return true.
//*****************************************************************************
bool DiagnoseDepthCalibration(CExampleInterface* pExample, MIL_ID MilLaser)
   {
   // Clear display and overlay.
   MbufClear(pExample->GetMilDisplayImage(), 0.0);
   MdispControl(pExample->GetMilDisplay(), M_OVERLAY_CLEAR, M_DEFAULT);

   // Show all different image regions according to their calibration state.
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   M3dmapDraw(M_DEFAULT, MilLaser, pExample->GetMilOverlayImage(), M_DRAW_REGION_VALID       , M_DEFAULT, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_YELLOW);
   M3dmapDraw(M_DEFAULT, MilLaser, pExample->GetMilOverlayImage(), M_DRAW_REGION_INTERPOLATED, M_DEFAULT, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_GRAY);
   M3dmapDraw(M_DEFAULT, MilLaser, pExample->GetMilOverlayImage(), M_DRAW_REGION_UNCALIBRATED, M_DEFAULT, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_RED);
   M3dmapDraw(M_DEFAULT, MilLaser, pExample->GetMilOverlayImage(), M_DRAW_REGION_MISSING_DATA, M_DEFAULT, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_MAGENTA);
   M3dmapDraw(M_DEFAULT, MilLaser, pExample->GetMilOverlayImage(), M_DRAW_REGION_INVERTED    , M_DEFAULT, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_WHITE);
   M3dmapDraw(M_DEFAULT, MilLaser, pExample->GetMilOverlayImage(), M_DRAW_CALIBRATION_PEAKS  , M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("The calibration state of each pixel of the camera image is shown:\n")
             MIL_TEXT(" Green:   Calibrated region        (will generate corrected depths)\n")
             MIL_TEXT(" Gray:    Uncalibrated region      (will generate missing data)\n")
             MIL_TEXT("          Cause: outside the region between lowest and highest planes.\n")
             MIL_TEXT(" Yellow:  Interpolated region      (will generate less accurate depths)\n")
             MIL_TEXT("          Cause: some laser line pixels were missed.\n")
             MIL_TEXT(" Red:     Missing calibration data (will generate missing data)\n")
             MIL_TEXT("          Cause: some laser line pixels were missed on lowest or highest plane.\n")
             MIL_TEXT(" Magenta: Inversion                (could generate erroneous depths)\n")
             MIL_TEXT("          Cause: some laser line pixels of a lower reference plane were\n")
             MIL_TEXT("                 detected above a higher reference plane.\n\n"));

   // Print some diagnostic informations.
   MIL_INT NbColumns                = M3dmapInquire(MilLaser, M_DEFAULT, M_NUMBER_OF_COLUMNS                  , M_NULL);
   MIL_INT NbColumnsWithMissingData = M3dmapInquire(MilLaser, M_DEFAULT, M_NUMBER_OF_COLUMNS_WITH_MISSING_DATA, M_NULL);
   MIL_INT NbColumnsWithInversions  = M3dmapInquire(MilLaser, M_DEFAULT, M_NUMBER_OF_COLUMNS_WITH_INVERSIONS  , M_NULL);

   MosPrintf(MIL_TEXT("Number of columns with missing data: %4d / %d (%.1f%%)\n")
             MIL_TEXT("Number of columns with inversions:   %4d / %d (%.1f%%)\n\n"),
             NbColumnsWithMissingData, NbColumns,
             100.0*static_cast<MIL_DOUBLE>(NbColumnsWithMissingData)/static_cast<MIL_DOUBLE>(NbColumns),
             NbColumnsWithInversions, NbColumns,
             100.0*static_cast<MIL_DOUBLE>(NbColumnsWithInversions)/static_cast<MIL_DOUBLE>(NbColumns));

   // Ask user if he is satisfied with the calibration.
   bool CalibrationIsAccurate = pExample->AskIfLaserCalibrationAccurate();
   return CalibrationIsAccurate;
   }
