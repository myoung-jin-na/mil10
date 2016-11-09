//***************************************************************************************
//
// File name: standaloneexample.cpp
//
// Synopsis:  Implementation for CStandAloneExample.
//

#include <mil.h>
#include "standaloneexample.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

// File names of calibration grid images used to calibrate the camera.
static const MIL_TEXT_CHAR* const CALIBRATION_GRID_FILES[] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("CalibrationGrid_notfound.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("CalibrationGrid_perpendicular.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("CalibrationGrid_inaccurate.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("CalibrationGrid_ok.mim")
   };

// File name of laser line image used to adjust M_MIN_INTENSITY.
static const MIL_TEXT_CHAR* const LASER_LINE_TO_ADJUST_INTENSITY_FILE =
   EXAMPLE_IMAGE_PATH MIL_TEXT("LineExtract.mim");

// File names of laser line images used to calibrate a M_DEPTH_CORRECTION
// 3d reconstruction context.
static const MIL_TEXT_CHAR* const DEPTH_CALIBRATION_LASER_LINE_FILES[] =
   {
   // First M_DEPTH_CORRECTION calibration, with missing data.
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane2_missing.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane3.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane4_missing.mim"),
   M_NULL, // indicates that there are no more planes.

   // Second M_DEPTH_CORRECTION calibration, with inversion.
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane2_curved.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane3.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane4.mim"),
   M_NULL, // indicates that there are no more planes.

   // Third M_DEPTH_CORRECTION calibration, OK.
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane2.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane3.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Depth_CalibPlane4.mim"),
   M_NULL // indicates that there are no more planes.
   };

// File names of laser line images used to calibrate a M_CALIBRATED_CAMERA_LINEAR_MOTION
// 3d reconstruction context.
static const MIL_TEXT_CHAR* const FULL_CALIBRATION_LASER_LINE_FILES[] =
   {
   // First M_CALIBRATED_CAMERA_LINEAR_MOTION calibration, inaccurate (curved plane).
   EXAMPLE_IMAGE_PATH MIL_TEXT("Full_CalibPlane0_curved.mim"),
   M_NULL, // indicates that there are no more planes.

   // Second M_CALIBRATED_CAMERA_LINEAR_MOTION calibration, inaccurate (one tilted plane).
   EXAMPLE_IMAGE_PATH MIL_TEXT("Full_CalibPlane0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Full_CalibPlane1_tilted.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Full_CalibPlane2.mim"),
   M_NULL, // indicates that there are no more planes.

   // Third M_CALIBRATED_CAMERA_LINEAR_MOTION calibration, OK.
   EXAMPLE_IMAGE_PATH MIL_TEXT("Full_CalibPlane0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Full_CalibPlane1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Full_CalibPlane2.mim"),
   M_NULL // indicates that there are no more planes.
   };

// Number of images used to adjust M_MIN_INTENSITY.
static const MIL_INT NB_LASER_LINE_TO_ADJUST_MIN_INTENSITY = 3;

// Status messages used during M_MIN_INTENSITY adjustment.
static const MIL_TEXT_CHAR* const MIN_INTENSITY_STRINGS[NB_LASER_LINE_TO_ADJUST_MIN_INTENSITY] =
   {
   MIL_TEXT("The laser line (in green) is not\n")
   MIL_TEXT("fully extracted; there are columns where the laser line is not detected, as\n")
   MIL_TEXT("shown by the missing annotations. This setting is too high.\n\n"),

   MIL_TEXT("The laser line (in green) is\n")
   MIL_TEXT("detected at unpredictable positions in columns where the laser line is not\n")
   MIL_TEXT("visible. This setting is too low.\n\n"),

   MIL_TEXT("The laser line is fully\n")
   MIL_TEXT("and correctly extracted. This setting is just right.\n\n")
   };

// M_MIN_INTENSITY values to try.
static const MIL_INT MIN_INTENSITY_VALUES[NB_LASER_LINE_TO_ADJUST_MIN_INTENSITY] = {215, 40, 100};

// Number of custom messages printed during camera calibration.
static const MIL_INT NB_CAMERA_CALIBRATION_STRINGS = 2;

// Status messages used during camera calibration.
static const MIL_TEXT_CHAR* const CAMERA_CALIBRATION_STRINGS[NB_CAMERA_CALIBRATION_STRINGS] =
   {
   MIL_TEXT("Red and green marks are too far apart; the camera calibration is inaccurate\n")
   MIL_TEXT("(in this case, the calibration grid is not planar).\n")
   MIL_TEXT("A new calibration must be performed.\n\n"),

   MIL_TEXT("The camera calibration is accurate.\n\n")
   };

// Status messages used during M_DEPTH_CORRECTION 3d reconstruction context calibration,
// after each laser line is extracted.
static const MIL_TEXT_CHAR* const DEPTH_LINE_EXTRACTION_STRINGS[] =
   {
   // First M_DEPTH_CORRECTION calibration, with missing data.
   MIL_TEXT(""),
   MIL_TEXT(""),
   MIL_TEXT("The gap in the laser line will result in more inaccuracies in that region.\n\n"),
   MIL_TEXT(""),
   MIL_TEXT("Since this is the highest reference plane, the gap in the laser line will\n")
   MIL_TEXT("reduce the calibrated region, resulting in more missing data.\n\n"),

   // Second M_DEPTH_CORRECTION calibration, with inversion.
   MIL_TEXT(""),
   MIL_TEXT(""),
   MIL_TEXT("This reference surface is not planar. Moreover, note that in some columns\n")
   MIL_TEXT("the laser line will appear higher than for the next reference plane.\n\n"),
   MIL_TEXT("In some columns the laser line is lower than its position for the previous\n")
   MIL_TEXT("plane: this will result in an inaccurate calibration.\n\n"),
   MIL_TEXT(""),
   MIL_TEXT(""),

   // Third M_DEPTH_CORRECTION calibration, OK.
   MIL_TEXT(""),
   MIL_TEXT(""),
   MIL_TEXT(""),
   MIL_TEXT(""),
   MIL_TEXT("")
   };

// Number of loop iterations to calibrate the M_DEPTH_CORRECTION 3d reconstruction context.
static const MIL_INT NB_DEPTH_LASER_CALIBRATIONS = 3;

// Status messages used during M_DEPTH_CORRECTION 3d reconstruction context calibration,
// indicating if the resulting calibration is accurate or not.
static const MIL_TEXT_CHAR* const DEPTH_LASER_CALIBRATION_STRINGS[NB_DEPTH_LASER_CALIBRATIONS] =
   {
   MIL_TEXT("The calibration of the 3d reconstruction setup is inaccurate because there are\n")
   MIL_TEXT("missing data in planes #2 and #4. A new calibration must be performed.\n\n"),

   MIL_TEXT("The calibration of the 3d reconstruction setup is inaccurate because, for some\n")
   MIL_TEXT("columns, the laser line of plane #2 appears higher than the laser line\n")
   MIL_TEXT("of plane #3. A new calibration must be performed.\n\n"),

   MIL_TEXT("The calibration of the 3d reconstruction setup is accurate.\n\n")
   };

// Status messages used during M_CALIBRATED_CAMERA_LINEAR_MOTION 3d reconstruction context
// calibration, after each laser line is extracted.
static const MIL_TEXT_CHAR* const FULL_LINE_EXTRACTION_STRINGS[] =
   {
   // First M_CALIBRATED_CAMERA_LINEAR_MOTION calibration, inaccurate (curved plane).
   MIL_TEXT("The laser line is extracted accurately, but it will result in an inaccurate\n")
   MIL_TEXT("calibration because the reference surface is not a plane at Z=0 mm.\n\n"),

   // Second M_CALIBRATED_CAMERA_LINEAR_MOTION calibration, inaccurate (one tilted plane).
   MIL_TEXT(""),

   MIL_TEXT("The laser line is extracted accurately, but will result in an inaccurate\n")
   MIL_TEXT("calibration because the reference surface is not a horizontal plane at\n")
   MIL_TEXT("Z=-12 mm.\n\n"),

   MIL_TEXT(""),

   // Third M_CALIBRATED_CAMERA_LINEAR_MOTION calibration, OK.
   MIL_TEXT(""),
   MIL_TEXT(""),
   MIL_TEXT("")
   };

// Number of loop iterations to calibrate the M_CALIBRATED_CAMERA_LINEAR_MOTION 3d reconstruction context.
static const MIL_INT NB_FULL_LASER_CALIBRATIONS = 3;

// Status messages used during M_CALIBRATED_CAMERA_LINEAR_MOTION 3d reconstruction context
// calibration, indicating if the resulting calibration is accurate or not.
static const MIL_TEXT_CHAR* const FULL_LASER_CALIBRATION_STRINGS[] =
   {
   MIL_TEXT("The calibration of the 3d reconstruction setup is inaccurate: the reference\n")
   MIL_TEXT("surface must be a plane at the expected height and in this case, the surface\n")
   MIL_TEXT("is not planar. A new calibration must be performed.\n\n"),

   MIL_TEXT("The calibration of the 3d reconstruction setup is inaccurate: the reference\n")
   MIL_TEXT("surfaces must be planes at expected heights and in this case, the reference\n")
   MIL_TEXT("plane #1 is not parallel to the others. A new calibration must be performed.\n\n"),

   MIL_TEXT("The calibration of the 3d reconstruction setup is accurate.\n\n")
   };

//*****************************************************************************
// Constructor.
//*****************************************************************************
CStandAloneExample::CStandAloneExample()
: CExampleInterface(M_SYSTEM_HOST, M_NULL), // M_NULL => no need for M_GRAB
  m_CalibrationGridCounter           (0),
  m_LaserLineToAdjustIntensityCounter(0),
  m_CalibrationLaserLineCounter      (0),
  m_CameraCalibrationCounter         (0),
  m_LineExtractionCounter            (0),
  m_LaserCalibrationCounter          (0)
   {
   // Get image size from calibration grid image file.
   MIL_INT SizeX = MbufDiskInquire(CALIBRATION_GRID_FILES[0], M_SIZE_X, M_NULL);
   MIL_INT SizeY = MbufDiskInquire(CALIBRATION_GRID_FILES[0], M_SIZE_Y, M_NULL);
   SetImageSize(SizeX, SizeY);

   MosPrintf(MIL_TEXT("This example shows each step of a 3d reconstruction setup calibration.\n")
             MIL_TEXT("For each step, examples of failed and inaccurate calibrations will be shown,\n")
             MIL_TEXT("and the diagnosis steps will be explained.\n\n"));
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CStandAloneExample::~CStandAloneExample()
   {

   }

//*****************************************************************************
// Indicates if all MIL objects are initialized correctly.
//*****************************************************************************
bool CStandAloneExample::IsValid() const
   {
   return IsValidBase();
   }

//*****************************************************************************
// Used to pause the execution in the stand-alone version.
//*****************************************************************************
void CStandAloneExample::PauseInStandAloneMode() const
   {
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//*****************************************************************************
// Explain how to adjust M_MIN_INTENSITY value.
//*****************************************************************************
void CStandAloneExample::PrintExplanationForMinIntensity() const
   {
   MosPrintf(MIL_TEXT("The minimum intensity value must be adjusted to extract the laser line\n")
             MIL_TEXT("correctly.\n\n"));
   }

//*****************************************************************************
// Choose a value of M_MIN_INTENSITY and explain the consequences (according to
// iteration). The function returns true at the last iteration.
//*****************************************************************************
bool CStandAloneExample::AskMinIntensityAdjust(MIL_INT* pMinIntensity)
   {
   *pMinIntensity = MIN_INTENSITY_VALUES[m_LaserLineToAdjustIntensityCounter];
   MosPrintf(MIL_TEXT("In this image, M_MIN_INTENSITY is set to %d. "), *pMinIntensity);
   MosPrintf(MIN_INTENSITY_STRINGS[m_LaserLineToAdjustIntensityCounter]);
   bool AskQuit = (m_LaserLineToAdjustIntensityCounter == NB_LASER_LINE_TO_ADJUST_MIN_INTENSITY-1);
   ++m_LaserLineToAdjustIntensityCounter;
   return AskQuit;
   }

//*****************************************************************************
// Returns true if laser line extraction is assumed to be accurate.
//*****************************************************************************
bool CStandAloneExample::AskIfFeatureExtractionAccurate()
   {
   return true; // Assume laser line extraction is always accurate.
   }

//*****************************************************************************
// Tells if the camera calibration is accurate or not, and why (according to
// iteration). Returns true if it is accurate.
//*****************************************************************************
bool CStandAloneExample::AskIfCameraCalibrationAccurate()
   {
   MosPrintf(CAMERA_CALIBRATION_STRINGS[m_CameraCalibrationCounter]);
   bool Answer = (m_CameraCalibrationCounter == NB_CAMERA_CALIBRATION_STRINGS-1);
   ++m_CameraCalibrationCounter;
   return Answer;
   }

//*****************************************************************************
// Prints informational message on laser line extraction (according to
// iteration). Returns true if laser line extraction is assumed to be
// accurate.
//*****************************************************************************
bool CStandAloneExample::AskIfLineExtractionAccurate()
   {
   if (GetCalibrationMode() == M_DEPTH_CORRECTION)
      MosPrintf(DEPTH_LINE_EXTRACTION_STRINGS[m_LineExtractionCounter]);
   else
      MosPrintf(FULL_LINE_EXTRACTION_STRINGS[m_LineExtractionCounter]);
   ++m_LineExtractionCounter;
   return true; // Assume laser line extraction is always accurate.
   }

//*****************************************************************************
// Tells if the camera calibration is accurate or not, and why (according to
// iteration). Returns true if it is accurate.
//*****************************************************************************
bool CStandAloneExample::AskIfLaserCalibrationAccurate()
   {
   bool Answer;
   if (GetCalibrationMode() == M_DEPTH_CORRECTION)
      {
      MosPrintf(DEPTH_LASER_CALIBRATION_STRINGS[m_LaserCalibrationCounter]);
      Answer = (m_LaserCalibrationCounter == NB_DEPTH_LASER_CALIBRATIONS-1);
      }
   else
      {
      MosPrintf(FULL_LASER_CALIBRATION_STRINGS[m_LaserCalibrationCounter]);
      Answer = (m_LaserCalibrationCounter == NB_FULL_LASER_CALIBRATIONS-1);
      }
   ++m_LaserCalibrationCounter;
   return Answer;
   }

//*****************************************************************************
// Stub function, useful only in interactive mode.
//*****************************************************************************
MIL_ID CStandAloneExample::TryToReloadCameraCalibration(const MIL_TEXT_CHAR* CalibrationFileName) const
   {
   // Never reload a camera calibration in stand-alone mode.
   return M_NULL;
   }

//*****************************************************************************
// Reloads image used for camera calibration (according to iteration).
//*****************************************************************************
void CStandAloneExample::GrabCalibrationGrid()
   {
   MosPrintf(MIL_TEXT("The calibration grid is placed at Z=0 mm.\n\n"));
   MbufLoad(CALIBRATION_GRID_FILES[m_CalibrationGridCounter++], GetMilDisplayImage());
   }

//*****************************************************************************
// Reloads image used to adjust M_MIN_INTENSITY.
//*****************************************************************************
void CStandAloneExample::GrabLaserLineToAdjustIntensity()
   {
   MbufLoad(LASER_LINE_TO_ADJUST_INTENSITY_FILE, GetMilDisplayImage());
   }

//*****************************************************************************
// Reloads image used for 3d reconstruction context calibration, according to
// iteration. If there are no more plane, the function returns true.
//*****************************************************************************
bool CStandAloneExample::GrabCalibrationLaserLine(MIL_INT ReferencePlaneIndex, MIL_DOUBLE CalibrationDepth, bool ShouldAskIfFinished)
   {
   const MIL_TEXT_CHAR* CalibrationLaserLineFile;
   if (GetCalibrationMode() == M_DEPTH_CORRECTION)
      CalibrationLaserLineFile = DEPTH_CALIBRATION_LASER_LINE_FILES[m_CalibrationLaserLineCounter];
   else
      CalibrationLaserLineFile = FULL_CALIBRATION_LASER_LINE_FILES[m_CalibrationLaserLineCounter];
   ++m_CalibrationLaserLineCounter;

   bool ReadyToCalibrate;
   if (CalibrationLaserLineFile != M_NULL)
      {
      MosPrintf(MIL_TEXT("Reference plane #%d is expected to be at Z=%.3g mm.\n\n"), ReferencePlaneIndex, CalibrationDepth);
      MbufLoad(CalibrationLaserLineFile, GetMilDisplayImage());
      ReadyToCalibrate = false;
      }
   else
      {
      ReadyToCalibrate = true;
      }

   return ReadyToCalibrate;
   }
