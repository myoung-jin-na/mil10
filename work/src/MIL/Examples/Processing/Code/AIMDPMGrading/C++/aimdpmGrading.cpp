//***************************************************************************************/
// 
// File name: aimdpmGrading.cpp  
//
// Synopsis:  This program contains an example of calibration and code grading  
//            for DataMatrix based on AIM DPM-1-2006 specifications.
//            See the PrintHeader() function below for a detailed description.
//
//
#include <mil.h>

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("aimdpmGrading\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This program is an example of verifying a 2D data code symbol. \n")
             MIL_TEXT("The example follows AIM DPM-1-2006 Quality Guidelines. The example \n")
             MIL_TEXT("also shows steps for reflectance calibration (required).\n\n")
  
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: Application, system, display, buffer, \n")
             MIL_TEXT("graphic, code\n\n")
  
             MIL_TEXT("Press <Enter> to continue.\n"));
   MosGetch();
   }

static const MIL_INT CodeType = M_DATAMATRIX;
static const MIL_INT CodeCalibrationType = M_UPC_A;

static const MIL_INT MinimumGrayScaleValue = 0;
static const MIL_INT MaximumGrayScaleValue = 255;

static const MIL_INT ApertureMode = M_RELATIVE;
static const double ApertureSize = M_AUTO;

static const double MinimumMeanLight = 0.7;
static const double MaximumMeanLight = 0.86;

static const MIL_DOUBLE SRexposure = 60.0;
static const MIL_DOUBLE SRgain = 1.25;

const MIL_INT CodeReflectanceCalibrationNumber = 7;

static const MIL_TEXT_CHAR* CodeReflectanceCalibrationFileName[CodeReflectanceCalibrationNumber] = 
   {
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/ReflectanceCalibration1.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/ReflectanceCalibration2.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/ReflectanceCalibration3.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/ReflectanceCalibration4.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/ReflectanceCalibration5.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/ReflectanceCalibration6.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/ReflectanceCalibration7.mim")
   };

const MIL_INT CodeSourceImageNumber = 5;

static const MIL_TEXT_CHAR* CodeSourceImageFileName[CodeSourceImageNumber] = 
   {
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/Image1.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/Image2.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/Image3.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/Image4.mim"),
   M_IMAGE_PATH MIL_TEXT("AIMDPMGrading/Image5.mim")
   };


//************************************
// functions declarations
//************************************
void ReflectanceCalibration(const MIL_TEXT_CHAR* SrcFilename[], MIL_INT NbFilename, MIL_ID MilSystem, MIL_ID MilDisplay, 
                            MIL_ID MilCodeContext, MIL_DOUBLE &SRcal);

void AIMDPMVerification(const MIL_TEXT_CHAR* SrcFilename[], MIL_INT NbFilename, MIL_ID MilSystem, MIL_ID MilDisplay, 
                        MIL_ID MilCodeContext, const MIL_DOUBLE SRcal);


//************************************
// Utility sub-functions declarations
//************************************

void AllocDisplayImage(MIL_ID MilSystem, 
                       MIL_ID MilSrcImage, 
                       MIL_ID MilDisplay, 
                       MIL_ID& MilDispProcImage, 
                       MIL_ID& MilOverlayImage);

void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, double  Grade);
void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, MIL_INT Grade);
void PrintGrade(MIL_CONST_TEXT_PTR Text, double  Grade);
void PrintGrade(MIL_CONST_TEXT_PTR Text, MIL_INT Grade);


//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate the MIL objects.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_WINDOWED, M_NULL);
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);
   // Print Header.
   PrintHeader();
   
   //***************************
   // Reflectance Calibration    
   //***************************
   MIL_DOUBLE SRcal = 0.0;

   ReflectanceCalibration(CodeReflectanceCalibrationFileName, CodeReflectanceCalibrationNumber,
                          MilSystem, MilDisplay, MilCodeContext, SRcal);

   //**************************************
   // Target calibration and verification    
   //**************************************
   AIMDPMVerification(CodeSourceImageFileName, CodeSourceImageNumber,
                      MilSystem, MilDisplay, MilCodeContext, SRcal);

   // Free MIL objects. 
   McodeFree(MilCodeContext);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//****************************************************************************
// ReflectanceCalibration    
//****************************************************************************
void ReflectanceCalibration(const MIL_TEXT_CHAR* SrcFilename[], MIL_INT NbFilename,
                            MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilCodeTargetContext, MIL_DOUBLE &SRcal)
   {
   MIL_INT ii;
   MIL_DOUBLE MLcal = 0.0;
   MIL_DOUBLE Rcal = 255;

   // Allocate a code context.
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, CodeCalibrationType, M_NULL, M_DEFAULT, M_NULL);

   // Allocate a code result.
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Set the aperture mode and size (optional); by default it's M_RELATIVE
   McodeControl(MilCodeContext, M_APERTURE_MODE, ApertureMode);

   if(ApertureMode == M_RELATIVE || ApertureMode == M_DEFAULT)
      McodeControl(MilCodeContext, M_RELATIVE_APERTURE_FACTOR, ApertureSize);
   else if(ApertureMode == M_ABSOLUTE)
      McodeControl(MilCodeContext, M_ABSOLUTE_APERTURE_SIZE, ApertureSize);

   // Set the calibrated reflectance values (optional); the default values are [0, 255]
   McodeControl(MilCodeContext, M_MINIMUM_CALIBRATED_REFLECTANCE, MinimumGrayScaleValue);
   McodeControl(MilCodeContext, M_MAXIMUM_CALIBRATED_REFLECTANCE, MaximumGrayScaleValue);

   bool EndCalibration = false;

   for(ii = 0; ii < NbFilename && !EndCalibration; ii++)
      {
      // Restore source image into an automatically allocated image buffer.
      MIL_ID MilSrcImage;
      MbufRestore(SrcFilename[ii], MilSystem, &MilSrcImage);

      // Allocate a display image.
      MIL_ID MilDispProcImage,         // Display and destination buffer.
             MilOverlayImage;          // Overlay buffer.
      AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

      // Read the code and display the result.
      McodeVerify(MilCodeContext, MilDispProcImage, M_NULL, MilCodeResult);

      // Get decoding status.
      MIL_INT ReadStatus;
      McodeGetResult(MilCodeResult, M_STATUS+M_TYPE_MIL_INT, &ReadStatus);

      if(ReadStatus == M_STATUS_VERIFY_OK)
         {
         // Retrieve MLcal : mean of the light lobe from a histogram of the calibrated standard
         McodeGetResult(MilCodeResult, M_MEAN_LIGHT_CALIBRATION+M_TYPE_DOUBLE, &MLcal);
         
         // Retrieve Rcal : reported reflectance value, Rmax, from a calibration standard  
         McodeGetResult(MilCodeResult, M_REFLECTANCE_CALIBRATION+M_TYPE_DOUBLE, &Rcal);

         double RatioMLcal = MLcal / (double)MaximumGrayScaleValue;

         if((RatioMLcal >= MinimumMeanLight) && (RatioMLcal <= MaximumMeanLight))
            {
            // Record SRcal : value of system response parameters (such as exposure and/or gain) used to 
            //                create an image of the calibration standard 
            SRcal = SRexposure * SRgain;
            EndCalibration = true;
            MosPrintf(MIL_TEXT(" Calibration finished successfully:\n"));
            MosPrintf(MIL_TEXT(" - Mean light lobe:     %7.3f\n"), RatioMLcal);
            MosPrintf(MIL_TEXT(" - Highest reflectance: %7.3f\n"), Rcal);
            MosPrintf(MIL_TEXT(" - System Response:     %7.3f\n\n"), SRcal);

            // Retrieve calibration reflectance from the result and pass the information to target context
            McodeControl(MilCodeTargetContext, M_AIMDPM_CALIBRATION_RESULTS, MilCodeResult);
            }
         else if(RatioMLcal < MinimumMeanLight)
            {
            MosPrintf(MIL_TEXT("Mean light lobe is %7.3f. This is too low. Increase \n"), RatioMLcal);
            MosPrintf(MIL_TEXT("exposure or gain and continue to calibrate\n\n"));
            }
         else if(RatioMLcal > MaximumMeanLight)
            {
            MosPrintf(MIL_TEXT("Mean light lobe is %7.3f. This is too high. Decrease \n"), RatioMLcal);
            MosPrintf(MIL_TEXT("exposure or gain and continue to calibrate\n\n"));
            }
         }
      else
         {
         MosPrintf(MIL_TEXT("Verification operation failed. Verify your setting and continue to calibrate.\n\n"));
         }

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Free source image.
      MbufFree(MilSrcImage);

      // Free Display image
      MbufFree(MilDispProcImage);
      }

   // Free Context and result objects
   McodeFree(MilCodeContext);
   McodeFree(MilCodeResult);
   }



//****************************************************************************
// AIMDPMVerification
//****************************************************************************
void AIMDPMVerification(const MIL_TEXT_CHAR* SrcFilename[], MIL_INT NbFilename,
                        MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilCodeContext, const MIL_DOUBLE SRcal)
   {
   MIL_INT ii;

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, CodeType, M_NULL, M_DEFAULT, M_NULL);

   // Allocate a code result.
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Set verification standard 
   McodeControl(MilCodeContext, M_VERIFICATION_STANDARD, M_AIMDPM_VERIFICATION);

   // Set the aperture mode and size (optional); by default it's M_RELATIVE 
   McodeControl(MilCodeContext, M_APERTURE_MODE, ApertureMode);

   if(ApertureMode == M_RELATIVE || ApertureMode == M_DEFAULT)
      McodeControl(MilCodeContext, M_RELATIVE_APERTURE_FACTOR, ApertureSize);
   else if(ApertureMode == M_ABSOLUTE)
      McodeControl(MilCodeContext, M_ABSOLUTE_APERTURE_SIZE, ApertureSize);

   // Set the calibrated reflectance values (optional); the default values are [0, 255]
   McodeControl(MilCodeContext, M_MINIMUM_CALIBRATED_REFLECTANCE, MinimumGrayScaleValue);
   McodeControl(MilCodeContext, M_MAXIMUM_CALIBRATED_REFLECTANCE, MaximumGrayScaleValue);

   // Set SRcal : value of system response parameters (such as exposure and/or gain) used to 
   //             create an image of the calibration standard
   McodeControl(MilCodeContext, M_SYSTEM_RESPONSE_CALIBRATION, SRcal);

   // Set foreground color
   McodeControl(MilCodeContext, M_FOREGROUND_VALUE, M_FOREGROUND_WHITE);

   // Set Distortion
   McodeControl(MilCodeContext, M_DISTORTION, M_UNEVEN_GRID_STEP);

   // Enable Presearch
   McodeControl(MilCodeContext, M_USE_PRESEARCH, M_ENABLE);


   bool EndAdjust = false;
   MIL_DOUBLE SRtarget = SRcal; 
   MIL_DOUBLE MLtarget;

   for(ii = 0; ii < NbFilename && !EndAdjust; ii++)
      {
      // Restore source image into an automatically allocated image buffer.
      MIL_ID MilSrcImage;
      MbufRestore(SrcFilename[ii], MilSystem, &MilSrcImage);

      // Allocate a display image.
      MIL_ID MilDispProcImage,         // Display and destination buffer.
             MilOverlayImage;          // Overlay buffer.
      AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

      // Set SRcal : value of system response parameters (such as exposure and/or gain) used to 
      //             create an image of the calibration standard
      McodeControl(MilCodeContext, M_SYSTEM_RESPONSE_TARGET, SRtarget);

      // Read the code and display the result.
      McodeVerify(MilCodeContext, MilDispProcImage, M_NULL, MilCodeResult);

      // Get decoding status.
      MIL_INT ReadStatus;
      McodeGetResult(MilCodeResult, M_STATUS+M_TYPE_MIL_INT, &ReadStatus);

      if(ReadStatus == M_STATUS_VERIFY_OK)
         {
         // Retrieve Mean of light elements
         McodeGetResult(MilCodeResult, M_MEAN_LIGHT_TARGET, &MLtarget);

         double RatioMLtarget = MLtarget / (double)MaximumGrayScaleValue;

         if((RatioMLtarget >= MinimumMeanLight) && (RatioMLtarget <= MaximumMeanLight))
            {
            // Record the system response (such as exposure or gain)
            SRtarget = SRexposure * SRgain;
            EndAdjust = true;
            MosPrintf(MIL_TEXT(" Verification finished successfully:\n"));
            MosPrintf(MIL_TEXT(" - Mean light lobe: %7.3f\n"), RatioMLtarget);
            MosPrintf(MIL_TEXT(" - System Response: %7.3f\n\n"), SRtarget);
            }
         else if(RatioMLtarget < MinimumMeanLight)
            {
            MosPrintf(MIL_TEXT("Mean light lobe is %7.3f. This is too low. Increase \n"), RatioMLtarget);
            MosPrintf(MIL_TEXT("exposure or gain and continue to adjust\n\n"));
            }
         else if(RatioMLtarget > MaximumMeanLight)
            {
            MosPrintf(MIL_TEXT("Mean light lobe is %7.3f. This is too high. Decrease \n"), RatioMLtarget);
            MosPrintf(MIL_TEXT("exposure or gain and continue to adjust\n\n"));
            }
         }
      else
         {
         MosPrintf(MIL_TEXT("Verification operation failed. Verify your setting and continue to calibrate.\n\n"));
         }

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Free source image.
      MbufFree(MilSrcImage);

      // Free Display image
      MbufFree(MilDispProcImage);
      }

   // Retrieve and print grading result 
   MIL_DOUBLE OverallGrade                = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE DecodeGrade                 = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE CellContrast                = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE CellContrastGrade           = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE CellModulationGrade         = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE MinimumReflectance          = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE MinimumReflectanceGrade     = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE UnusedErrorCorrection       = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE UnusedErrorCorrectionGrade  = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE AxialNonUniformity          = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE AxialNonUniformityGrade     = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE GridNonUniformity           = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE GridNonUniformityGrade      = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE FixedPatternDamageGrade     = M_CODE_GRADE_NOT_AVAILABLE;
   MIL_DOUBLE PrintGrowth                 = M_CODE_GRADE_NOT_AVAILABLE;

   // Retrieve Results 
   McodeGetResult(MilCodeResult, M_OVERALL_SYMBOL_GRADE,             &OverallGrade);
   McodeGetResult(MilCodeResult, M_DECODE_GRADE,                     &DecodeGrade);
   McodeGetResult(MilCodeResult, M_CELL_CONTRAST,                    &CellContrast);
   McodeGetResult(MilCodeResult, M_CELL_CONTRAST_GRADE,              &CellContrastGrade);
   McodeGetResult(MilCodeResult, M_CELL_MODULATION_GRADE,            &CellModulationGrade);
   McodeGetResult(MilCodeResult, M_MINIMUM_REFLECTANCE,              &MinimumReflectance);
   McodeGetResult(MilCodeResult, M_MINIMUM_REFLECTANCE_GRADE,        &MinimumReflectanceGrade);
   McodeGetResult(MilCodeResult, M_UNUSED_ERROR_CORRECTION,          &UnusedErrorCorrection);
   McodeGetResult(MilCodeResult, M_UNUSED_ERROR_CORRECTION_GRADE,    &UnusedErrorCorrectionGrade);
   McodeGetResult(MilCodeResult, M_AXIAL_NONUNIFORMITY,              &AxialNonUniformity);
   McodeGetResult(MilCodeResult, M_AXIAL_NONUNIFORMITY_GRADE,        &AxialNonUniformityGrade);
   McodeGetResult(MilCodeResult, M_GRID_NONUNIFORMITY,               &GridNonUniformity);
   McodeGetResult(MilCodeResult, M_GRID_NONUNIFORMITY_GRADE,         &GridNonUniformityGrade);
   McodeGetResult(MilCodeResult, M_FIXED_PATTERN_DAMAGE_GRADE,       &FixedPatternDamageGrade);
   McodeGetResult(MilCodeResult, M_PRINT_GROWTH,                     &PrintGrowth);

   // Print Results
   PrintGrade(MIL_TEXT(" - Overall Grade:                "), OverallGrade);
   PrintGrade(MIL_TEXT(" - Decode Grade:                 "), DecodeGrade);
   PrintValueAndGrade(MIL_TEXT(" - Cell Contrast:              "), CellContrast, CellContrastGrade);
   PrintGrade(MIL_TEXT(" - Cell Modulation Grade:        "), CellModulationGrade);
   PrintValueAndGrade(MIL_TEXT(" - Minimum Reflectance:        "), MinimumReflectance, MinimumReflectanceGrade);
   PrintValueAndGrade(MIL_TEXT(" - Unused Error Correction:    "), UnusedErrorCorrection, UnusedErrorCorrectionGrade);
   PrintValueAndGrade(MIL_TEXT(" - Axial Non-Uniformity:       "), AxialNonUniformity, AxialNonUniformityGrade);
   PrintValueAndGrade(MIL_TEXT(" - Grid Non-Uniformity:        "), GridNonUniformity, GridNonUniformityGrade);
   PrintGrade(MIL_TEXT(" - Fixed Pattern Damage Grade:   "), FixedPatternDamageGrade);
   MosPrintf(MIL_TEXT(" - Print Growth:               %7.3f\n\n"), PrintGrowth);

   // Free Context and result objects
   McodeFree(MilCodeResult);
   }


/************************************************************************
GetGradeString
************************************************************************/
MIL_CONST_TEXT_PTR GetGradeString(double Grade)
   {
   switch(static_cast<MIL_INT>(Grade + 0.5))
      {
      case M_CODE_GRADE_A:             return MIL_TEXT("A");  break;
      case M_CODE_GRADE_B:             return MIL_TEXT("B");   break;
      case M_CODE_GRADE_C:             return MIL_TEXT("C");   break;
      case M_CODE_GRADE_D:             return MIL_TEXT("D");   break;
      case M_CODE_GRADE_F:             return MIL_TEXT("F");   break;
      case M_CODE_GRADE_NOT_AVAILABLE: return MIL_TEXT("N/A"); break;
      }
   return MIL_TEXT("N/A");
   }

/************************************************************************
PrintGrade
************************************************************************/
void PrintGrade(MIL_CONST_TEXT_PTR Text, double Grade)
   {
   if (Grade != M_CODE_GRADE_NOT_AVAILABLE)
      MosPrintf(MIL_TEXT("%s%-1.3f  (%s)\n"), Text, Grade, GetGradeString(Grade));
   }

void PrintGrade(MIL_CONST_TEXT_PTR Text, MIL_INT Grade)
   {
   PrintGrade(Text, static_cast<double>(Grade));
   }

/************************************************************************
PrintValueAndGrade
************************************************************************/
void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, double Grade)
   {
   if (Grade != M_CODE_GRADE_NOT_AVAILABLE)
      MosPrintf(MIL_TEXT("%s%7.3f  (%s)\n"), Text, Value, GetGradeString(Grade));
   }

void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, MIL_INT Grade)
   {
   PrintValueAndGrade(Text, Value, static_cast<double>(Grade));
   }


/************************************************************************
AllocDisplayImage
************************************************************************/
void AllocDisplayImage(MIL_ID MilSystem, 
                       MIL_ID MilSrcImage, 
                       MIL_ID MilDisplay, 
                       MIL_ID& MilDispProcImage, 
                       MIL_ID& MilOverlayImage)
   {
   // Retrieve the source image size.
   MIL_INT SrcSizeX, SrcSizeY;
   MbufInquire(MilSrcImage, M_SIZE_X, &SrcSizeX);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SrcSizeY);

   // Allocate the display image.
   MbufAlloc2d(MilSystem, 
      SrcSizeX,
      SrcSizeY,
      8L+M_UNSIGNED,
      M_IMAGE+M_PROC+M_DISP, 
      &MilDispProcImage);

   MbufCopy(MilSrcImage, MilDispProcImage);

   // Display the image buffer.
   MdispSelect(MilDisplay, MilDispProcImage);

   // Prepare for overlay annotations.
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   }
