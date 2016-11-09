/***************************************************************************************/
/* 
* File name: CodeGrading.cpp  
*
* Synopsis:  This program contains an example of code grading for different type of 
*            linear Code, Composite code and Cross-Row using the Code Reader module.
*            See the PrintHeader() function below for detailed description.
*
*/
#include <mil.h>

#if   M_MIL_USE_LINUX
#define SAVE_PATH MIL_TEXT("")
#elif M_MIL_USE_CE
#define SAVE_PATH MIL_TEXT("\\Userdisk\\")
#else
#define SAVE_PATH MIL_TEXT("")
#endif

/****************************************************************************
Example description.
****************************************************************************/
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("CodeGrading\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This program verifies different types of Linear codes, Composite codes,\n"));
   MosPrintf(MIL_TEXT("Cross-row codes and 2D matrix codes.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: application, system, display, buffer, graphic, code.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

/* Number of Linear code */
static const MIL_INT NUMBER_OF_IMAGES               = 14L;

/* Height of Buffer used for reflectance profile */
static const MIL_INT REFLECTANCE_PROFILE_HEIGHT     = 256L;


/* Linear Code images */
static const MIL_TEXT_CHAR* CodeFileName[NUMBER_OF_IMAGES] =
   {
   M_IMAGE_PATH MIL_TEXT("CodeGrading/UPCA_Decode.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/UPCA_SC.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/UPCE_Rmin.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/EAN14_ECmin.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/Code39_Modulation.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/UPCA_Defects.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/UPCE_Decodability.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/Code93_QuietZone.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/PDF417_UEC.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/TruncatedPDF417_2DDecodability.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/PDF417_2DDefects.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/Composite_Defects.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/DataMatrix.mim"),
   M_IMAGE_PATH MIL_TEXT("CodeGrading/QrCode.mim")
   };


static const MIL_INT CodeType[NUMBER_OF_IMAGES] = 
   {
   M_UPC_A,
   M_UPC_A,
   M_UPC_E,
   M_EAN14,
   M_CODE39,
   M_UPC_A,
   M_UPC_E,
   M_CODE93,
   M_PDF417,
   M_TRUNCATED_PDF417,
   M_PDF417,
   M_COMPOSITECODE,
   M_DATAMATRIX,
   M_QRCODE
   };

/* Result Per Scan Reflectance profile */
struct SCAN_REFLECTANCE_PROFILE_RESULT_ST
   {
   
   MIL_INT ScanReflectanceProfileGrade;
   MIL_INT DecodeGrade;
   MIL_INT SymbolContrastGrade;     
   MIL_INT ReflectanceMinimumGrade;
   MIL_INT EdgeContrastMinimumGrade;
   MIL_INT ModulationGrade;
   MIL_INT DefectsGrade;
   MIL_INT DecodabilityGrade;
   MIL_INT QuietZoneGrade;
   MIL_INT GuardPatternGrade;
   MIL_INT WideToNarrowRatioGrade;
   MIL_INT InterCharacterGapGrade;

   double  SymbolContrast;     
   double  ReflectanceMinimum;
   double  EdgeContrastMinimum;
   double  Modulation;
   double  Defects;
   double  Decodability;
   double  QuietZone;
   double  GuardPattern;
   double  WideToNarrowRatio;
   double  InterCharacterGap;
   };

struct ROW_RESULT_ST
   {
   double RowOverallGrade;
   SCAN_REFLECTANCE_PROFILE_RESULT_ST *ScanResults;
   };

struct MATRIX_RESULT_ST
   {

   MIL_INT DecodeGrade;
   MIL_INT UnusedErrorCorrectionGrade;
   MIL_INT AxialNonUniformityGrade;
   MIL_INT GridNonUniformityGrade;
   MIL_INT FixedPatternDamageGrade;
   MIL_INT FormatInformationGrade;
   MIL_INT VersionInformationGrade;
   MIL_INT SymbolContrastGrade;
   MIL_INT ModulationGrade;
   MIL_INT CellModulationGrade;
   MIL_INT CellContrastGrade;
   MIL_INT MinimumReflectanceGrade;
   
   double UnusedErrorCorrection;
   double PrintGrowth;
   double AxialNonUniformity;
   double GridNonUniformity;
   double SymbolContrast;
   double CellContrast;
   double MinimumReflectance;
   };

/* Container of dynamic 2D array */
class VERIFY_ARRAY
   {
   public:
      VERIFY_ARRAY();
      ~VERIFY_ARRAY();

      void AllocateRows(MIL_INT NumberOfRows);
      void AllocateScans(MIL_INT RowIndex, MIL_INT NumberOfScans);
      SCAN_REFLECTANCE_PROFILE_RESULT_ST *GetScanResultPtr(MIL_INT RowIndex, MIL_INT ScanIndex);
      const SCAN_REFLECTANCE_PROFILE_RESULT_ST &GetScanResult(MIL_INT RowIndex, MIL_INT ScanIndex) const;
      ROW_RESULT_ST *GetRowResultPtr(MIL_INT RowIndex);
      void Free();   

   private:
      ROW_RESULT_ST *m_RowResults;
      MIL_INT m_NumberOfRows;
   };

/* Verify result structure */
struct VERIFY_RESULT_ST
   {
   MIL_INT CodeType;
   MIL_INT Encoding;

   double OverallGrade;

   /* Worst grade */
   MIL_INT WorstScanIndex;
   MIL_INT WorstRowIndex;
   MIL_INT WorstScanIndexOffset;
   MIL_INT WorstGrade;

   /* Result Per Row */
   VERIFY_ARRAY RowResults;

   /* Results specific to Cross-row component */
   double  StartStopGrade;
   MIL_INT CodewordYieldGrade;  
   MIL_INT ModulationGrade;
   MIL_INT DecodabilityGrade;
   MIL_INT DefectsGrade;
   MIL_INT UnusedErrorCorrectionGrade;
   double  CodewordYield;
   double  UnusedErrorCorrection;

   /* Results specific to 2D component */
   MATRIX_RESULT_ST MatrixResults;

   void InitializeWorstGrade();
   void UpdateWorstGrade(MIL_INT RowIndex, MIL_INT ScanIndex, MIL_INT ScanIndexOffset, MIL_INT Grade);
   MIL_INT GetWorstCrossRowGrade();
   };

enum CodeTypeCategory
   {
   IS_1D_CODE,
   IS_CROSS_ROW_CODE,
   IS_COMPOSITE_CODE,
   IS_2D_MATRIX_CODE,
   IS_NOT_SUPPORTED_BY_THIS_EXAMPLE
   };

#define min(a,b)    (((a) < (b)) ? (a) : (b))

/* Functions declarations. */

/* General grading function. */
void CodeGrading(MIL_ID MilSystem, MIL_ID MilSrcImage, MIL_ID MilDisplay, MIL_INT CodeType);

/* Sub-functions that extract the grading results from the MIL code result object. */
void GetAndDisplayResultsFor1DCode(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage, MIL_INT Accessor);
void GetAndDisplayResultsForCrossRowCode(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage, MIL_INT Accessor);
void GetAndDisplayResultsFor2DMatrixCode(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage);

void GetResultForRow(MIL_ID MilCodeResult, MIL_INT RowIndex, MIL_INT ScanIndexOffset, MIL_INT NumberOfScans, VERIFY_RESULT_ST* GradingResult);

/* Sub-functions that display and print results. */
void DisplayScanReflectanceProfileOfWorstGrade(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage, MIL_INT Accessor, const VERIFY_RESULT_ST& GradingResult);
void PrintScanWorstGrade(const VERIFY_RESULT_ST& GradingResult);
void PrintCrossRowGrade(const VERIFY_RESULT_ST& GradingResult, MIL_INT WorstGrade);
void Print2DMatrixGrade(const VERIFY_RESULT_ST& GradingResult);

/* Utility sub-functions. */
CodeTypeCategory GetCodeTypeCategory(MIL_INT CodeType);
MIL_INT Get1DCodeTypeOfCompositeCode(MIL_INT Encoding);
MIL_CONST_TEXT_PTR GetCodeTypeString(const VERIFY_RESULT_ST& GradingResult);
MIL_CONST_TEXT_PTR GetGradeString(double Grade, MIL_TEXT_CHAR* GradeString);

void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, double  Grade);
void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, MIL_INT Grade);
void PrintGrade(MIL_CONST_TEXT_PTR Text, double  Grade);
void PrintGrade(MIL_CONST_TEXT_PTR Text, MIL_INT Grade);
void PrintValue(MIL_CONST_TEXT_PTR Text, double  Grade);



/*****************************************************************************
Main.
*****************************************************************************/
int MosMain(void)
   {
   MIL_ID      MilApplication,        /* Application identifier.         */
               MilSystem,             /* System identifier.              */
               MilDisplay;            /* Display identifier.             */

   /* Allocate MIL objects. */
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);

   /* Print Header. */
   PrintHeader();

   /********************/
   /* BARCODE GRADING: */
   /********************/
   for(MIL_INT ii = 0; ii < NUMBER_OF_IMAGES; ii++)
      {
      /* Restore source image into an automatically allocated image buffer. */
      MIL_ID MilSrcImage;
      MbufRestore(CodeFileName[ii], MilSystem, &MilSrcImage);

      CodeGrading(MilSystem, MilSrcImage, MilDisplay, CodeType[ii]);

      /* Free source image. */
      MbufFree(MilSrcImage);
      }

   /* Free other allocations. */
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);


   return 0;
   }

/************************************************************************
  CodeGrading

  Calculate the grading results of a code using MIL, display and
  print the results.
************************************************************************/
void CodeGrading(MIL_ID MilSystem, MIL_ID MilSrcImage, MIL_ID MilDisplay, MIL_INT CodeType)
   {
   /* Allocate a display buffers and show the source image. */
   MIL_INT SrcImageWidth, SrcImageHeight, SrcImageType;
   MbufInquire(MilSrcImage, M_SIZE_X, &SrcImageWidth);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SrcImageHeight);
   MbufInquire(MilSrcImage, M_TYPE,   &SrcImageType);

   MIL_ID MilDisplayImage;
   MbufAlloc2d(MilSystem, 
               SrcImageWidth,
               SrcImageHeight + REFLECTANCE_PROFILE_HEIGHT,
               SrcImageType,
               M_IMAGE+M_PROC+M_DISP, 
               &MilDisplayImage);

   MbufClear(MilDisplayImage, 0L);
   MbufCopy(MilSrcImage, MilDisplayImage);

   /* Display the image buffer. */
   MdispSelect(MilDisplay, MilDisplayImage);

   /* Prepare for overlay annotations. */
   MIL_ID MilOverlayImage;
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   /* Determine the type categories of the code */
   CodeTypeCategory ComponentType = GetCodeTypeCategory(CodeType);
   if(ComponentType == IS_NOT_SUPPORTED_BY_THIS_EXAMPLE)
      {
      /* Free source image. */
      MbufFree(MilDisplayImage);
      MbufFree(MilSrcImage);
      }

   /* Allocate CODE objects. */
   MIL_ID MilCodeContext;
   McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &MilCodeContext);
   McodeControl(MilCodeContext, M_TIMEOUT, 10000.0);
   McodeModel(MilCodeContext, M_ADD, CodeType, M_NULL, M_DEFAULT, M_NULL);

   /* Prepare bar code results buffer */
   MIL_ID MilCodeResult;
   McodeAllocResult(MilSystem, M_DEFAULT, &MilCodeResult);

   /* Verify codes from image. */
   McodeVerify(MilCodeContext, MilSrcImage, M_NULL, MilCodeResult);

   /* Get verification status. */
   MIL_INT Status;
   McodeGetResult(MilCodeResult, M_STATUS+M_TYPE_MIL_INT, &Status);

   /* Check if the verification operation was successful. */
   if (Status == M_STATUS_VERIFY_OK)
      {
      MgraColor(M_DEFAULT, M_COLOR_GREEN);
      McodeDraw(M_DEFAULT, MilCodeResult, MilOverlayImage, M_DRAW_SCAN_PROFILES, M_DEFAULT, M_DEFAULT);

      switch(ComponentType)
         {
         case IS_1D_CODE:
            GetAndDisplayResultsFor1DCode(MilSystem, MilCodeResult, MilOverlayImage, 0);
            break;
         case IS_CROSS_ROW_CODE:
            GetAndDisplayResultsForCrossRowCode(MilSystem, MilCodeResult, MilOverlayImage, 0);
            break;
         case IS_COMPOSITE_CODE:
            GetAndDisplayResultsFor1DCode(MilSystem, MilCodeResult, MilOverlayImage, M_LINEAR_COMPONENT);
            MosPrintf(MIL_TEXT("Press <Enter> to get the 2D part.\n\n"));
            MosGetch();
            GetAndDisplayResultsForCrossRowCode(MilSystem, MilCodeResult, MilOverlayImage, M_2D_COMPONENT);
            break;

         case IS_2D_MATRIX_CODE:
            GetAndDisplayResultsFor2DMatrixCode(MilSystem, MilCodeResult, MilOverlayImage);
            break;
         }

      /* Saves a report containing most of the results from a verify operation as a flat text file */
      MIL_TEXT_PTR pOutFilename = const_cast<MIL_TEXT_PTR>(SAVE_PATH MIL_TEXT("ReportFile.txt"));
      McodeStream(pOutFilename, M_NULL, M_SAVE_REPORT, M_FILE, M_DEFAULT, M_DEFAULT, &MilCodeResult, M_NULL);
      MosPrintf(MIL_TEXT("A verification report was saved in ReportFile.txt\n\n"));
      }
   else
      {
      MosPrintf(MIL_TEXT("Code verify operation failed.\n\n"));
      }

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Free Display image. */
   MbufFree(MilDisplayImage);

   /* Free objects. */
   McodeFree(MilCodeResult);
   McodeFree(MilCodeContext);
   }


/************************************************************************
  GetAndDisplayResultsFor1DCode

  Extracts the grading results for a 1D code from a MIL code result
  and display them.
************************************************************************/
void GetAndDisplayResultsFor1DCode(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage, MIL_INT Accessor)
   {
   MIL_INT ScanIndexOffset = 0;
   VERIFY_RESULT_ST GradingResult;
   
   /* Initialize the worst grade. */
   GradingResult.InitializeWorstGrade();

   /* Get the code type. */
   McodeGetResultSingle(MilCodeResult, 0, M_CODE_TYPE + M_TYPE_MIL_INT, &(GradingResult.CodeType));
   McodeGetResultSingle(MilCodeResult, 0, M_ENCODING  + M_TYPE_MIL_INT, &(GradingResult.Encoding));

   if(GradingResult.CodeType == M_COMPOSITECODE)
      {
      GradingResult.CodeType = Get1DCodeTypeOfCompositeCode(GradingResult.Encoding);
      }

   /* Get the overall grade. */
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_OVERALL_SYMBOL_GRADE + Accessor, &(GradingResult.OverallGrade));

   /* Get number of Rows */
   /* N.B: Accessor is used for composite code to identify LINEAR part (if part of a composite code) */
   MIL_INT NumberOfRows;
   McodeGetResultSingle(MilCodeResult, 0, M_NUMBER_OF_ROWS + Accessor + M_TYPE_MIL_INT, &NumberOfRows);
   
   /* Allocate memory for each row */
   GradingResult.RowResults.AllocateRows(NumberOfRows);

   /* Get results for each row. */
   for(MIL_INT RowIndex = 0; RowIndex < NumberOfRows; RowIndex++)
      {
      /* Get number of scan per row */
      MIL_INT NumberOfScans;
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(RowIndex), M_NUMBER_OF_SCANS_PER_ROW + Accessor + M_TYPE_MIL_INT, &NumberOfScans);

      /* Allocate memory to hold results of scan */
      GradingResult.RowResults.AllocateScans(RowIndex, NumberOfScans);

      /* Get grading of the current row */
      GetResultForRow(MilCodeResult, RowIndex, ScanIndexOffset, Accessor, &GradingResult);

      /* Increment the scan profile index */
      ScanIndexOffset += NumberOfScans;
      }

   /* Display scan reflectance profile */
   DisplayScanReflectanceProfileOfWorstGrade(MilSystem, MilCodeResult, MilOverlayImage, Accessor, GradingResult);

   /* Print worst grade */
   PrintScanWorstGrade(GradingResult);

   /* Free allocated memory */
   GradingResult.RowResults.Free();
   }

/************************************************************************
  GetResultForRow

  Extracts the grading results for a row of a 1D code from a 
  MIL code result.
************************************************************************/
void GetResultForRow(MIL_ID MilCodeResult, MIL_INT RowIndex, MIL_INT ScanIndexOffset, MIL_INT Accessor, VERIFY_RESULT_ST* GradingResult)
   {
   /* Get number of scan per row */
   MIL_INT NumberOfScans;
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(RowIndex), M_NUMBER_OF_SCANS_PER_ROW + Accessor + M_TYPE_MIL_INT, &NumberOfScans);
  
   ROW_RESULT_ST *CurrentRowResult = GradingResult->RowResults.GetRowResultPtr(RowIndex);

   /* Save the Row Overall Grade */
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(RowIndex), M_ROW_OVERALL_GRADE                + Accessor, &(CurrentRowResult->RowOverallGrade));

   /* Get results for each scan in this row. */
   for(MIL_INT ScanIndex = 0; ScanIndex < NumberOfScans; ScanIndex++)
      {
      SCAN_REFLECTANCE_PROFILE_RESULT_ST *CurrentScanResult = GradingResult->RowResults.GetScanResultPtr(RowIndex, ScanIndex);

      /* Get some values and grading */
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_REFLECTANCE_PROFILE_GRADE   + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->ScanReflectanceProfileGrade));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_DECODE_GRADE                + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->DecodeGrade                ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_SYMBOL_CONTRAST_GRADE       + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->SymbolContrastGrade        ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_REFLECTANCE_MINIMUM_GRADE   + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->ReflectanceMinimumGrade    ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_EDGE_CONTRAST_MINIMUM_GRADE + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->EdgeContrastMinimumGrade   ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_MODULATION_GRADE            + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->ModulationGrade            ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_DEFECTS_GRADE               + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->DefectsGrade               ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_DECODABILITY_GRADE          + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->DecodabilityGrade          ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_QUIET_ZONE_GRADE            + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->QuietZoneGrade             ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_GUARD_PATTERN_GRADE         + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->GuardPatternGrade          ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_WIDE_TO_NARROW_RATIO_GRADE  + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->WideToNarrowRatioGrade     ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_INTERCHARACTER_GAP_GRADE    + Accessor + M_TYPE_MIL_INT, &(CurrentScanResult->InterCharacterGapGrade     ));

      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_SYMBOL_CONTRAST             + Accessor, &(CurrentScanResult->SymbolContrast          ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_REFLECTANCE_MINIMUM         + Accessor, &(CurrentScanResult->ReflectanceMinimum      ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_EDGE_CONTRAST_MINIMUM       + Accessor, &(CurrentScanResult->EdgeContrastMinimum     ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_MODULATION                  + Accessor, &(CurrentScanResult->Modulation              ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_DEFECTS                     + Accessor, &(CurrentScanResult->Defects                 ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_DECODABILITY                + Accessor, &(CurrentScanResult->Decodability            ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_GUARD_PATTERN               + Accessor, &(CurrentScanResult->GuardPattern            ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_QUIET_ZONE                  + Accessor, &(CurrentScanResult->QuietZone               ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_WIDE_TO_NARROW_RATIO        + Accessor, &(CurrentScanResult->WideToNarrowRatio       ));
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(ScanIndex + ScanIndexOffset), M_SCAN_INTERCHARACTER_GAP          + Accessor, &(CurrentScanResult->InterCharacterGap       ));

      /* Remember which scan has the worst grade in the whole code. */
      GradingResult->UpdateWorstGrade(RowIndex, ScanIndex, ScanIndexOffset, CurrentScanResult->ScanReflectanceProfileGrade);
      }
   }

/************************************************************************
  DisplayScanReflectanceProfileOfWorstGrade

  Display the scan reflectance profile of the scan that obtained the
  worst grade in the code.
************************************************************************/
void DisplayScanReflectanceProfileOfWorstGrade(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage, MIL_INT Accessor, const VERIFY_RESULT_ST& GradingResult)
   {
   MIL_INT WorstScanIndex = GradingResult.WorstScanIndex + GradingResult.WorstScanIndexOffset;

   /* Draw the position of the scan that had the worst grade. */
   MgraColor(M_DEFAULT, M_COLOR_RED);
   McodeDraw(M_DEFAULT, MilCodeResult, MilOverlayImage, M_DRAW_SCAN_PROFILES, M_VERIFY_RESULT_INDEX(WorstScanIndex), M_DEFAULT);
   MIL_INT SizeX = 0;                             /* Size X */
   MIL_INT SizeY = REFLECTANCE_PROFILE_HEIGHT;    /* Size Y */

   /* Allocate buffer to hold the draw of the scan reflectance profile.
      It will be allocated as a child in the bottom of the displayed
      image. */
   MIL_INT OverlayHeight, OverlayWidth;
   MbufInquire(MilOverlayImage, M_SIZE_Y, &OverlayHeight);
   MbufInquire(MilOverlayImage, M_SIZE_X, &OverlayWidth);

   MIL_ID MilOverlayImageChild;
   MbufChild2d(MilOverlayImage, 0, OverlayHeight - REFLECTANCE_PROFILE_HEIGHT, OverlayWidth, REFLECTANCE_PROFILE_HEIGHT, &MilOverlayImageChild);

   /* Draw the scan reflectance profile. */
   MbufClear(MilOverlayImageChild, 255);
   MgraColor(M_DEFAULT, 0);
   McodeDraw(M_DEFAULT, MilCodeResult, MilOverlayImageChild, M_DRAW_REFLECTANCE_PROFILE + Accessor, M_VERIFY_RESULT_INDEX(WorstScanIndex), M_DEFAULT);
   MbufFree(MilOverlayImageChild);
   }


/************************************************************************
  PrintScanWorstGrade

  Print the grading results of the reflectance profile of the scan 
  that obtained the worst grade in the code.
************************************************************************/
void PrintScanWorstGrade(const VERIFY_RESULT_ST& GradingResult)
   {
   MosPrintf(MIL_TEXT("Code Type:               %s\n"), GetCodeTypeString(GradingResult));
   PrintGrade(MIL_TEXT("Overall Symbol Grade:    "), GradingResult.OverallGrade);
   MosPrintf(MIL_TEXT("Defect Row: #%d Scan reflectance profile: #%d\n\n"), GradingResult.WorstRowIndex, GradingResult.WorstScanIndex);

   const SCAN_REFLECTANCE_PROFILE_RESULT_ST &CurrentScanResult = GradingResult.RowResults.GetScanResult(GradingResult.WorstRowIndex, GradingResult.WorstScanIndex);

   PrintGrade(MIL_TEXT("  Scan reflectance profile grade: "), CurrentScanResult.ScanReflectanceProfileGrade);
   MosPrintf(MIL_TEXT("  ----------------------------------------------\n"));
   MosPrintf(MIL_TEXT("  Parameter                         Value      Grade\n"));
   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n"));

   PrintGrade(MIL_TEXT("    Decode                                     "), CurrentScanResult.DecodeGrade);
   PrintValueAndGrade(MIL_TEXT("    Symbol Contrast (SC)            "), CurrentScanResult.SymbolContrast     , CurrentScanResult.SymbolContrastGrade     );
   PrintValueAndGrade(MIL_TEXT("    Minimum reflectance (Rmin)      "), CurrentScanResult.ReflectanceMinimum , CurrentScanResult.ReflectanceMinimumGrade );
   PrintValueAndGrade(MIL_TEXT("    Edge contrast minimum (ECmin)   "), CurrentScanResult.EdgeContrastMinimum, CurrentScanResult.EdgeContrastMinimumGrade);
   PrintValueAndGrade(MIL_TEXT("    Modulation (MOD)                "), CurrentScanResult.Modulation         , CurrentScanResult.ModulationGrade         );
   PrintValueAndGrade(MIL_TEXT("    Defects                         "), CurrentScanResult.Defects            , CurrentScanResult.DefectsGrade            );
   PrintValueAndGrade(MIL_TEXT("    Decodability (V)                "), CurrentScanResult.Decodability       , CurrentScanResult.DecodabilityGrade       );
   PrintValueAndGrade(MIL_TEXT("    Quiet Zone                      "), CurrentScanResult.QuietZone          , CurrentScanResult.QuietZoneGrade          );
   PrintValueAndGrade(MIL_TEXT("    Interior guard [in Z]           "), CurrentScanResult.GuardPattern       , CurrentScanResult.GuardPatternGrade       );
   PrintValueAndGrade(MIL_TEXT("    Wide/Narrow ratio [in Z]        "), CurrentScanResult.WideToNarrowRatio  , CurrentScanResult.WideToNarrowRatioGrade  );
   PrintValueAndGrade(MIL_TEXT("    Intercharacter gap [in Z]       "), CurrentScanResult.InterCharacterGap  , CurrentScanResult.InterCharacterGapGrade  );

   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n\n"));

   if(GradingResult.WorstGrade == M_CODE_GRADE_A)
      return;

   if(CurrentScanResult.SymbolContrastGrade == GradingResult.WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the symbol contrast grade,\n")); 
      MosPrintf(MIL_TEXT("the difference between the highest and the lowest intensity\n"));
      MosPrintf(MIL_TEXT("values (=Rmax-Rmin), in at least one scan line (displayed in red),\n"));
      MosPrintf(MIL_TEXT("is too small.\n\n"));
      }
   else if(CurrentScanResult.ReflectanceMinimumGrade == GradingResult.WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the minimum reflectance grade,\n"));
      MosPrintf(MIL_TEXT("the lowest intensity value (Rmin) should not be more than\n"));
      MosPrintf(MIL_TEXT("0.5x the maximum intensity value (Rmax) in at least one scan\n")); 
      MosPrintf(MIL_TEXT("profile (displayed in red).\n\n"));

      }
   else if(CurrentScanResult.EdgeContrastMinimumGrade == GradingResult.WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the Edge contrast minimum grade,\n")); 
      MosPrintf(MIL_TEXT("the smallest intensity difference (=Rs-Rb) of adjoining\n")); 
      MosPrintf(MIL_TEXT("elements of a scan profile (displayed in red) is too small.\n\n"));
      }
   else if(CurrentScanResult.ModulationGrade == GradingResult.WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the modulation grade (=ECmin/SC),\n")); 
      MosPrintf(MIL_TEXT("the ratio between the minimum edge contrast and the symbol\n")); 
      MosPrintf(MIL_TEXT("contrast is too small in at least one scan profile (displayed in red)\n\n"));
      }
   else if(CurrentScanResult.DefectsGrade == GradingResult.WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the defects grade (=ERNmax/SC),\n")); 
      MosPrintf(MIL_TEXT("intensity irregularities found in the barcode elements, including its\n")); 
      MosPrintf(MIL_TEXT("quiet zone, are too important in at least one scan profile (displayed in red).\n\n"));
      }
   else if(CurrentScanResult.DecodabilityGrade == GradingResult.WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the decodabilty grade measures.\n"));
      MosPrintf(MIL_TEXT("  V = absolute value of ( (RT - M) / (RT - A) ).\n"));
      MosPrintf(MIL_TEXT("where: (RT - M) represents the remaining margin not used by the printing\n"));
      MosPrintf(MIL_TEXT("                variation,\n"));
      MosPrintf(MIL_TEXT("   and (RT - A) represents the total theoretical margin based on the\n"));
      MosPrintf(MIL_TEXT("                ideal measurement of the element(s).\n\n"));
      }
   else if((CurrentScanResult.QuietZoneGrade == GradingResult.WorstGrade) || 
            (CurrentScanResult.GuardPatternGrade == GradingResult.WorstGrade))
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the quiet zone grade, the required blank\n")); 
      MosPrintf(MIL_TEXT("space before and after the bar code is not respected in at least one scan\n")); 
      MosPrintf(MIL_TEXT("profile (displayed in red).\n\n"));
      }
   else if(CurrentScanResult.DecodeGrade == GradingResult.WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: the code cannot be decoded. One of the following criteria\n"));
      MosPrintf(MIL_TEXT("is probably not respected: character encodation, start/stop patterns,\n"));
      MosPrintf(MIL_TEXT("check digits, quiet zones or inter-character gaps. In this image, some bars,\n")); 
      MosPrintf(MIL_TEXT("along at least one scan profile (displayed in red), are too thin.\n\n"));
      }

   }

/************************************************************************
  GetAndDisplayResultsForCrossRowCode

  Extracts the grading results for a cross row code from a MIL code result
  and display them.
************************************************************************/
void GetAndDisplayResultsForCrossRowCode(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage, MIL_INT Accessor)
   {
   MIL_INT ScanIndexOffset = 0;
   VERIFY_RESULT_ST GradingResult;
   
   /* Initialize the worst grade. */
   GradingResult.InitializeWorstGrade();

   /* Get the code type. */
   McodeGetResultSingle(MilCodeResult, 0, M_CODE_TYPE + M_TYPE_MIL_INT, &(GradingResult.CodeType));
   McodeGetResultSingle(MilCodeResult, 0, M_ENCODING  + M_TYPE_MIL_INT, &(GradingResult.Encoding));

   if(GradingResult.CodeType == M_COMPOSITECODE)
      {
      if(GradingResult.Encoding == M_ENC_GS1_128_PDF417)
         GradingResult.CodeType = M_PDF417;
      else
         GradingResult.CodeType = M_MICROPDF417;
      }

   /* Get the global code grading results. */
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_OVERALL_SYMBOL_GRADE          + Accessor,                  &(GradingResult.OverallGrade              ));

   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_START_STOP_PATTERN_GRADE      + Accessor,                  &(GradingResult.StartStopGrade            ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_CODEWORD_YIELD_GRADE          + Accessor + M_TYPE_MIL_INT, &(GradingResult.CodewordYieldGrade        ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_MODULATION_GRADE              + Accessor + M_TYPE_MIL_INT, &(GradingResult.ModulationGrade           ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_DECODABILITY_GRADE            + Accessor + M_TYPE_MIL_INT, &(GradingResult.DecodabilityGrade         ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_DEFECTS_GRADE                 + Accessor + M_TYPE_MIL_INT, &(GradingResult.DefectsGrade              ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_UNUSED_ERROR_CORRECTION_GRADE + Accessor + M_TYPE_MIL_INT, &(GradingResult.UnusedErrorCorrectionGrade));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_CODEWORD_YIELD                + Accessor,                  &(GradingResult.CodewordYield             ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_UNUSED_ERROR_CORRECTION       + Accessor,                  &(GradingResult.UnusedErrorCorrection     ));

   /* Get number of Rows*/
   /* N.B: Accessor is used for composite code to identify 2D part */

   MIL_INT NumberOfRows;
   McodeGetResultSingle(MilCodeResult, 0, M_NUMBER_OF_ROWS + Accessor + M_TYPE_MIL_INT, &NumberOfRows);

   /* Allocate memory for each row */
   GradingResult.RowResults.AllocateRows(NumberOfRows);

   for(MIL_INT RowIndex = 0; RowIndex < NumberOfRows; RowIndex++)
      {
      /* Get number of scan per row */
      MIL_INT NumberOfScans;
      McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(RowIndex), M_NUMBER_OF_SCANS_PER_ROW + Accessor + M_TYPE_MIL_INT, &NumberOfScans);

      /* Allocate memory to hold results of scan */
      GradingResult.RowResults.AllocateScans(RowIndex, NumberOfScans);

      /* Get grading of the current row */
      GetResultForRow(MilCodeResult, RowIndex, ScanIndexOffset, Accessor, &GradingResult);

      /* Increment the scan profile index */
      ScanIndexOffset += NumberOfScans;
      }

   /* Display scan reflectance profile */
   DisplayScanReflectanceProfileOfWorstGrade(MilSystem, MilCodeResult, MilOverlayImage, Accessor, GradingResult);

   /* Print worst scan result */
   PrintScanWorstGrade(GradingResult);

   /* Print worst Cross-Row result */
   MIL_INT WorstGrade = GradingResult.GetWorstCrossRowGrade();
   PrintCrossRowGrade(GradingResult, WorstGrade);

   /* Free allocated memory */
   GradingResult.RowResults.Free();
   }


/************************************************************************
  PrintCrossRowGrade

  Print Cross-Row code verification result and the grading results for
  the scan that obtained the worst grade.
************************************************************************/
void PrintCrossRowGrade(const VERIFY_RESULT_ST& GradingResult, MIL_INT WorstGrade)
   {
   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n"));
   MosPrintf(MIL_TEXT("  Parameter                         Value      Grade\n"));
   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n"));

   PrintGrade(MIL_TEXT("    Start/Stop Pattern                         "), GradingResult.StartStopGrade);
   PrintValueAndGrade(MIL_TEXT("    Codeword Yield                  "), GradingResult.CodewordYield, GradingResult.CodewordYieldGrade);
   PrintGrade(MIL_TEXT("    Modulation                                 "), GradingResult.ModulationGrade);
   PrintGrade(MIL_TEXT("    Decodability                               "), GradingResult.DecodabilityGrade);
   PrintGrade(MIL_TEXT("    Defects                                    "), GradingResult.DefectsGrade);
   PrintValueAndGrade(MIL_TEXT("    Unused Error Correction         "), GradingResult.UnusedErrorCorrection, GradingResult.UnusedErrorCorrectionGrade);
   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n\n"));

   if(WorstGrade == M_CODE_GRADE_A)
      return;

   if(GradingResult.CodewordYieldGrade == WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure caused by the codeword yield. The codeword yield\n"));
      MosPrintf(MIL_TEXT("result determines how well the code can be read at an angle relative\n"));
      MosPrintf(MIL_TEXT("to the horizontal and vertical axis of code. When all other results\n"));
      MosPrintf(MIL_TEXT("are good, a poor codeword yield result can indicate a problem along\n"));
      MosPrintf(MIL_TEXT("the Y-axis of the code.\n\n"));
      }
   else if(GradingResult.ModulationGrade == WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure caused by the modulation of each codeword.\n"));
      MosPrintf(MIL_TEXT("Modulation is the ratio of the minimum edge contrast to symbol\n"));
      MosPrintf(MIL_TEXT("contrast within the code.\n\n")); 
      }
   else if(GradingResult.DecodabilityGrade == WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the codeword decodability grade,\n")); 
      MosPrintf(MIL_TEXT("the print quality of each codeword relative is too poor.\n")); 
      MosPrintf(MIL_TEXT("Note that this grade does not take into account any start/stop patterns.\n\n")); 
      }
   else if(GradingResult.DefectsGrade == WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the codeword defects grade,\n")); 
      MosPrintf(MIL_TEXT("the deviation relative to in the expected signal that denotes a\n")); 
      MosPrintf(MIL_TEXT("codeword in the code is too large. The larger the result, the\n")); 
      MosPrintf(MIL_TEXT("greater the defect in the codeword, and the less likely that the\n")); 
      MosPrintf(MIL_TEXT("codeword can be decoded without error.\n\n"));
      }
   else if(GradingResult.UnusedErrorCorrectionGrade == WorstGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the unused error correction grade, the ratio\n"));
      MosPrintf(MIL_TEXT("between the unused error correction and the total number of error correction\n"));
      MosPrintf(MIL_TEXT("available in the code if too low. Damages in the code have eroded the \n"));
      MosPrintf(MIL_TEXT("\"reading safety margin\" that the error correction provides.\n\n"));
      }

   }


/************************************************************************
  GetAndDisplayResultsFor2DMatrixCode

  Extracts the grading results for a 2D Matrix code from a MIL code result
  and display them.
************************************************************************/
void GetAndDisplayResultsFor2DMatrixCode(MIL_ID MilSystem, MIL_ID MilCodeResult, MIL_ID MilOverlayImage)
   {
   MIL_INT ScanIndexOffset = 0;
   VERIFY_RESULT_ST GradingResult;
   
   /* Initialize the worst grade. */
   GradingResult.InitializeWorstGrade();

   /* Get the code type. */
   McodeGetResultSingle(MilCodeResult, 0, M_CODE_TYPE + M_TYPE_MIL_INT, &(GradingResult.CodeType));
   McodeGetResultSingle(MilCodeResult, 0, M_ENCODING  + M_TYPE_MIL_INT, &(GradingResult.Encoding));

   /* Get the global code grading results. */
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_OVERALL_SYMBOL_GRADE                          ,  &(GradingResult.OverallGrade              ));

   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_DECODE_GRADE                  + M_TYPE_MIL_INT,  &(GradingResult.MatrixResults.DecodeGrade                ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_PRINT_GROWTH                                  ,  &(GradingResult.MatrixResults.PrintGrowth                ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_UNUSED_ERROR_CORRECTION_GRADE + M_TYPE_MIL_INT,  &(GradingResult.MatrixResults.UnusedErrorCorrectionGrade ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_UNUSED_ERROR_CORRECTION                       ,  &(GradingResult.MatrixResults.UnusedErrorCorrection      ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_AXIAL_NONUNIFORMITY_GRADE     + M_TYPE_MIL_INT,  &(GradingResult.MatrixResults.AxialNonUniformityGrade    ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_AXIAL_NONUNIFORMITY                           ,  &(GradingResult.MatrixResults.AxialNonUniformity         ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_GRID_NONUNIFORMITY_GRADE      + M_TYPE_MIL_INT,  &(GradingResult.MatrixResults.GridNonUniformityGrade     ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_GRID_NONUNIFORMITY                            ,  &(GradingResult.MatrixResults.GridNonUniformity          ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_FIXED_PATTERN_DAMAGE_GRADE    + M_TYPE_MIL_INT,  &(GradingResult.MatrixResults.FixedPatternDamageGrade    ));
   
   /* Available only for ISO/IEC:15415 verification standard. */
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_SYMBOL_CONTRAST_GRADE         + M_TYPE_MIL_INT, &(GradingResult.MatrixResults.SymbolContrastGrade         ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_SYMBOL_CONTRAST                               , &(GradingResult.MatrixResults.SymbolContrast              ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_MODULATION_GRADE              + M_TYPE_MIL_INT, &(GradingResult.MatrixResults.ModulationGrade             ));

   /* Available only for Qr Code. */
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_FORMAT_INFORMATION_GRADE    + M_TYPE_MIL_INT,  &(GradingResult.MatrixResults.FormatInformationGrade       ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_VERSION_INFORMATION_GRADE   + M_TYPE_MIL_INT,  &(GradingResult.MatrixResults.VersionInformationGrade      ));

   /* Available only for AIM DPM verification standard. */
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_CELL_MODULATION_GRADE         + M_TYPE_MIL_INT, &(GradingResult.MatrixResults.CellModulationGrade         ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_CELL_CONTRAST_GRADE           + M_TYPE_MIL_INT, &(GradingResult.MatrixResults.CellContrastGrade           ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_CELL_CONTRAST                                 , &(GradingResult.MatrixResults.CellContrast                ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_MINIMUM_REFLECTANCE_GRADE     + M_TYPE_MIL_INT, &(GradingResult.MatrixResults.MinimumReflectanceGrade     ));
   McodeGetResultSingle(MilCodeResult, M_VERIFY_RESULT_INDEX(M_ALL), M_MINIMUM_REFLECTANCE                           , &(GradingResult.MatrixResults.MinimumReflectance          ));

   /* Print worst 2D Matrix result */
   Print2DMatrixGrade(GradingResult);
   }

/************************************************************************
  Print2DMatrixGrade

  Print 2D Matrix code verification result and the grading results for
  the scan that obtained the worst grade.
************************************************************************/
void Print2DMatrixGrade(const VERIFY_RESULT_ST& GradingResult)
   {
   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n"));
   MosPrintf(MIL_TEXT("  Parameter                         Value      Grade\n"));
   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n"));

   PrintGrade(MIL_TEXT("    Decode Grade                               "), GradingResult.MatrixResults.DecodeGrade);
   PrintValueAndGrade(MIL_TEXT("    Unused Error Correction         "), GradingResult.MatrixResults.UnusedErrorCorrection, GradingResult.MatrixResults.UnusedErrorCorrectionGrade);
   PrintValue(MIL_TEXT("    Print Growth                    "), GradingResult.MatrixResults.PrintGrowth);
   PrintValueAndGrade(MIL_TEXT("    Axial Non-Uniformity            "), GradingResult.MatrixResults.AxialNonUniformity, GradingResult.MatrixResults.AxialNonUniformityGrade);
   PrintValueAndGrade(MIL_TEXT("    Grid Non-Uniformity             "), GradingResult.MatrixResults.GridNonUniformity, GradingResult.MatrixResults.GridNonUniformityGrade);
   PrintGrade(MIL_TEXT("    Fixed Pattern Damage                       "), GradingResult.MatrixResults.FixedPatternDamageGrade);
   PrintGrade(MIL_TEXT("    Format Information                         "), GradingResult.MatrixResults.FormatInformationGrade);
   PrintGrade(MIL_TEXT("    Version Information                        "), GradingResult.MatrixResults.VersionInformationGrade);
   PrintValueAndGrade(MIL_TEXT("    Symbol Contrast                 "), GradingResult.MatrixResults.SymbolContrast, GradingResult.MatrixResults.SymbolContrastGrade);
   PrintGrade(MIL_TEXT("    Modulation Grade                           "), GradingResult.MatrixResults.ModulationGrade);
   PrintValueAndGrade(MIL_TEXT("    Cell Contrast                   "), GradingResult.MatrixResults.CellContrast, GradingResult.MatrixResults.CellContrastGrade);
   PrintValueAndGrade(MIL_TEXT("    Minimum Reflectance             "), GradingResult.MatrixResults.MinimumReflectance, GradingResult.MatrixResults.MinimumReflectanceGrade);
   PrintGrade(MIL_TEXT("    Cell Modulation Grade                      "), GradingResult.MatrixResults.CellModulationGrade);

   MosPrintf(MIL_TEXT("  ----------------------------------------------------\n\n"));

   if(GradingResult.OverallGrade == M_CODE_GRADE_A)
      return;

   if(GradingResult.MatrixResults.DecodeGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the decode grade. The print quality\n")); 
      MosPrintf(MIL_TEXT("of the symbol is too poor to be readable.\n")); 
      }
   else if(GradingResult.MatrixResults.UnusedErrorCorrectionGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the unused error correction grade. The ratio\n"));
      MosPrintf(MIL_TEXT("between the unused error correction and the total number of error corrections\n"));
      MosPrintf(MIL_TEXT("available in the code is too low. Damages in the code have eroded the \n"));
      MosPrintf(MIL_TEXT("\"reading safety margin\" that the error correction provides.\n\n"));
      }
   else if(GradingResult.MatrixResults.SymbolContrastGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the symbol contrast grade.\n")); 
      MosPrintf(MIL_TEXT("The difference between the highest and the lowest intensity\n"));
      MosPrintf(MIL_TEXT("values (=Rmax-Rmin) is too small.\n\n"));
      }
   else if(GradingResult.MatrixResults.ModulationGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure caused by the modulation of each codeword.\n"));
      MosPrintf(MIL_TEXT("Modulation is a measure of the uniformity of reflectance of\n"));
      MosPrintf(MIL_TEXT("the dark and light modules, respectively.\n\n"));
      }
   else if(GradingResult.MatrixResults.CellContrastGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the cell contrast grade measures.\n")); 
      MosPrintf(MIL_TEXT("CC = (MLtarget – MDtarget) / MLtarget.\n"));
      MosPrintf(MIL_TEXT("Where: MLtarget represents the mean of the light cell from the\n"));
      MosPrintf(MIL_TEXT("                final grid-point histogram of the symbol,\n"));
      MosPrintf(MIL_TEXT("  and  MDtarget represents the mean of the dark cell from the\n"));
      MosPrintf(MIL_TEXT("                final grid-point histogram of the symbol.\n\n"));
      }
   else if(GradingResult.MatrixResults.MinimumReflectanceGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the minimum reflectance grade measures.\n")); 
      MosPrintf(MIL_TEXT("Rtarget = Rcal x (SRcal/SRtarget) x (MLtarget/MLcal).\n"));
      MosPrintf(MIL_TEXT("Where: MLtarget represents the mean of the light cell from the\n"));
      MosPrintf(MIL_TEXT("                final grid-point histogram of the symbol.\n"));
      MosPrintf(MIL_TEXT("       SRtarget represents the value of System Response parameters\n"));
      MosPrintf(MIL_TEXT("                used to create an image of the symbol.\n"));
      MosPrintf(MIL_TEXT("       MLcal    represents the mean of the light cell from a\n"));
      MosPrintf(MIL_TEXT("                histogram of the calibrated standard.\n"));
      MosPrintf(MIL_TEXT("       SRcal    represents the value of System Response parameters\n"));
      MosPrintf(MIL_TEXT("                used to create an image of the calibrated standard.\n"));
      MosPrintf(MIL_TEXT("       Rcal     represents the reflectance value Rmax from a\n"));
      MosPrintf(MIL_TEXT("                calibrated standard.\n\n"));
      }
   else if(GradingResult.MatrixResults.CellModulationGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure caused by the cell modulation of each codeword.\n"));
      MosPrintf(MIL_TEXT("Modulation is a measure of the uniformity of reflectance of\n"));
      MosPrintf(MIL_TEXT("the dark and light cells, respectively.\n\n"));
      }
   else if(GradingResult.MatrixResults.AxialNonUniformityGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the Axial Non-Uniformity grade measures.\n"));
      MosPrintf(MIL_TEXT("Axial Non-Uniformity is a measure of how much the sampling point spacing\n"));
      MosPrintf(MIL_TEXT("differs from one axis to another, namely:\n"));
      MosPrintf(MIL_TEXT("      AN = abs(XAVG - YAVG) / ((XAVG + YAVG) / 2)\n"));
      MosPrintf(MIL_TEXT("where: XAVG and YAVG are average spacing along each axis,\n"));
      MosPrintf(MIL_TEXT("   and abs() yields the absolute value.\n\n"));
      }
   else if(GradingResult.MatrixResults.GridNonUniformityGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the Grid Non-Uniformity grade measures.\n"));
      MosPrintf(MIL_TEXT("Grid Non-Uniformity is a measure of the largest vector deviation of the\n"));
      MosPrintf(MIL_TEXT("grid intersections, determined by the reference decode algorithm, from\n"));
      MosPrintf(MIL_TEXT("an ideal theoretical position.\n\n"));
      }
   else if(GradingResult.MatrixResults.FixedPatternDamageGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the Fixed Pattern Damage grade.\n"));
      MosPrintf(MIL_TEXT("The number of module errors (modules that appear as the inverse\n"));
      MosPrintf(MIL_TEXT("of the intended colour) in the finder pattern, quiet zone, timing,\n")); 
      MosPrintf(MIL_TEXT("and other fixed patterns, is too high.\n\n"));
      }
   else if(GradingResult.MatrixResults.FormatInformationGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the Format Information grade.\n"));
      MosPrintf(MIL_TEXT("The number of module errors (modules that appear as the inverse\n"));
      MosPrintf(MIL_TEXT("of the intended colour) in each block of format information is\n"));
      MosPrintf(MIL_TEXT("too high.\n\n"));
      }
   else if(GradingResult.MatrixResults.VersionInformationGrade == GradingResult.OverallGrade)
      {
      MosPrintf(MIL_TEXT("Grading failure: as reported by the Version Information grade.\n"));
      MosPrintf(MIL_TEXT("The number of module errors (modules that appear as the inverse\n"));
      MosPrintf(MIL_TEXT("of the intended colour) in each block of version information is\n"));
      MosPrintf(MIL_TEXT("too high.\n\n"));
      }

   }

/************************************************************************
  GetCodeTypeCategory
************************************************************************/
CodeTypeCategory GetCodeTypeCategory(MIL_INT CodeType)
   {  
   CodeTypeCategory ComponentType = IS_NOT_SUPPORTED_BY_THIS_EXAMPLE;

   switch(CodeType)
      {
      case M_COMPOSITECODE:
         ComponentType = IS_COMPOSITE_CODE;

         MosPrintf(MIL_TEXT("COMPOSITE BAR CODE GRADING:\n"));
         MosPrintf(MIL_TEXT("===========================\n"));
         break;

      case M_CODE39:
      case M_EAN13:        
      case M_INTERLEAVED25:
      case M_CODE128:
      case M_GS1_128:
      case M_EAN14:
      case M_BC412:
      case M_CODABAR:
      case M_UPC_A:
      case M_UPC_E:
      case M_GS1_DATABAR:
      case M_EAN8:
      case M_CODE93:
         ComponentType = IS_1D_CODE;

         MosPrintf(MIL_TEXT("LINEAR BAR CODE GRADING:\n"));
         MosPrintf(MIL_TEXT("========================\n"));
         break;

      case M_PDF417:
      case M_MICROPDF417:
      case M_TRUNCATED_PDF417:
         ComponentType = IS_CROSS_ROW_CODE;

         MosPrintf(MIL_TEXT("CROSS-ROW BAR CODE GRADING:\n"));
         MosPrintf(MIL_TEXT("===========================\n"));
         break;

      case M_DATAMATRIX:
      case M_QRCODE:
         ComponentType = IS_2D_MATRIX_CODE;

         MosPrintf(MIL_TEXT("2D MATRIX CODE GRADING:\n"));
         MosPrintf(MIL_TEXT("=======================\n"));
         break;
      
      case M_MAXICODE:
      case M_MICROQRCODE:
         MosPrintf(MIL_TEXT("Not all supported yet.\n"));
         MosGetch();
         break;
      
      default:
         MosPrintf(MIL_TEXT("Symbology does not support verification.\n"));
         MosGetch();
         break;
      }

   return ComponentType;
   }

/************************************************************************
  Get1DCodeTypeOfCompositeCode
************************************************************************/
MIL_INT Get1DCodeTypeOfCompositeCode(MIL_INT Encoding)
   {
   MIL_INT CodeType = 0;
   switch(Encoding)
      {
      case M_ENC_EAN13:                 CodeType = M_EAN13;   break;
      case M_ENC_EAN8:                  CodeType = M_EAN8;    break;
      case M_ENC_UPCA:                  CodeType = M_UPC_A;   break;
      case M_ENC_UPCE:                  CodeType = M_UPC_E;   break;

      case M_ENC_GS1_128_PDF417:      
      case M_ENC_GS1_128_MICROPDF417:   CodeType = M_GS1_128; break;

      case M_ENC_GS1_DATABAR_OMNI:                 
      case M_ENC_GS1_DATABAR_TRUNCATED:       
      case M_ENC_GS1_DATABAR_LIMITED:           
      case M_ENC_GS1_DATABAR_EXPANDED:          
      case M_ENC_GS1_DATABAR_STACKED:         
      case M_ENC_GS1_DATABAR_STACKED_OMNI:    
      case M_ENC_GS1_DATABAR_EXPANDED_STACKED:  CodeType = M_GS1_DATABAR; break;         
      }
   return CodeType;
   }

/************************************************************************
  GetCodeTypeString
************************************************************************/
MIL_CONST_TEXT_PTR GetCodeTypeString(const VERIFY_RESULT_ST& GradingResult)
   {
   switch(GradingResult.CodeType)
      {
      case M_CODE39:                return MIL_TEXT("Code 39");                         break;
      case M_DATAMATRIX:            return MIL_TEXT("DataMatrix");                      break;
      case M_EAN13:                 return MIL_TEXT("EAN13");                           break;
      case M_MAXICODE:              return MIL_TEXT("Maxicode");                        break;
      case M_INTERLEAVED25:         return MIL_TEXT("Interleaved 2 of 5");              break;
      case M_BC412:                 return MIL_TEXT("BC412");                           break;
      case M_CODABAR:               return MIL_TEXT("Codabar");                         break;
      case M_PDF417:                return MIL_TEXT("PDF417");                          break;
      case M_POSTNET:               return MIL_TEXT("Postnet");                         break;
      case M_PLANET:                return MIL_TEXT("Planet");                          break;
      case M_4_STATE:               return MIL_TEXT("4-State");                         break;
      case M_UPC_A:                 return MIL_TEXT("UPC-A");                           break;
      case M_UPC_E:                 return MIL_TEXT("UPC-E");                           break;
      case M_PHARMACODE:            return MIL_TEXT("Pharmacode");                      break;
      case M_EAN8:                  return MIL_TEXT("EAN8");                            break;
      case M_MICROPDF417:           return MIL_TEXT("MicroPDF417");                     break;
      case M_COMPOSITECODE:         return MIL_TEXT("Composite code");                  break;
      case M_GS1_128:               return MIL_TEXT("UCC/EAN/GS1-128");                 break;
      case M_QRCODE:                return MIL_TEXT("QRcode");                          break;
      case M_CODE93:                return MIL_TEXT("Code 93");                         break;
      case M_TRUNCATED_PDF417:      return MIL_TEXT("Truncated PDF417");                break;
      case M_EAN14:                 return MIL_TEXT("EAN14");                           break;

      case M_CODE128:
         if((GradingResult.Encoding == M_ENC_GS1_128_PDF417) ||
            (GradingResult.Encoding == M_ENC_GS1_128_MICROPDF417))
            return MIL_TEXT("UCC/EAN/GS1-128");
         else
            return MIL_TEXT("Code 128");                        
         break;

      case M_GS1_DATABAR:
         switch(GradingResult.Encoding)
            {
            case M_ENC_GS1_DATABAR_OMNI:                 return MIL_TEXT("GS1-DATABAR");                          break;
            case M_ENC_GS1_DATABAR_TRUNCATED:            return MIL_TEXT("GS1-DATABAR Truncated");                break;
            case M_ENC_GS1_DATABAR_LIMITED:              return MIL_TEXT("GS1-DATABAR Limited");                  break;
            case M_ENC_GS1_DATABAR_EXPANDED:             return MIL_TEXT("GS1-DATABAR Expanded");                 break;
            case M_ENC_GS1_DATABAR_STACKED:              return MIL_TEXT("GS1-DATABAR Stacked");                  break;
            case M_ENC_GS1_DATABAR_STACKED_OMNI:         return MIL_TEXT("GS1-DATABAR Stacked Omnidirectional");  break;
            case M_ENC_GS1_DATABAR_EXPANDED_STACKED:     return MIL_TEXT("GS1-DATABAR Expanded Stacked");         break;
            default:                                     return MIL_TEXT("GS1-DATABAR code");                     break;
            }
         break;

      default:                      
         return MIL_TEXT("Unavailable");                     break;
      }
   return MIL_TEXT("Unavailable");
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
      MosPrintf(MIL_TEXT("%s%-1.1f (%s)\n"), Text, Grade, GetGradeString(Grade));
   }

void PrintGrade(MIL_CONST_TEXT_PTR Text, MIL_INT Grade)
   {
   PrintGrade(Text, static_cast<double>(Grade));
   }

/************************************************************************
  PrintValue
************************************************************************/
void PrintValue(MIL_CONST_TEXT_PTR Text, double Value)
   {
   MosPrintf(MIL_TEXT("%s%7.4f\n"), Text, Value);
   }

/************************************************************************
  PrintValueAndGrade
************************************************************************/
void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, double Grade)
   {
   if (Grade != M_CODE_GRADE_NOT_AVAILABLE)
      MosPrintf(MIL_TEXT("%s%7.4f    %-1.1f (%s)\n"), Text, Value, Grade, GetGradeString(Grade));
   }

void PrintValueAndGrade(MIL_CONST_TEXT_PTR Text, double Value, MIL_INT Grade)
   {
   PrintValueAndGrade(Text, Value, static_cast<double>(Grade));
   }


/************************************************************************
  VERIFY_ARRAY member functions
************************************************************************/
VERIFY_ARRAY::VERIFY_ARRAY()
   {
   m_RowResults = NULL;
   m_NumberOfRows = 0;
   }

VERIFY_ARRAY::~VERIFY_ARRAY()
   {
   Free();
   }

void VERIFY_ARRAY::AllocateRows(MIL_INT NumberOfRows)
   {
   /* Allocate memory for rows. */
   Free();
   m_RowResults = new ROW_RESULT_ST [NumberOfRows];
   m_NumberOfRows = NumberOfRows;

   for(int ii = 0; ii < m_NumberOfRows; ii++)
      {
      m_RowResults[ii].RowOverallGrade = M_CODE_GRADE_NOT_AVAILABLE;
      m_RowResults[ii].ScanResults = NULL;
      }
   }


void VERIFY_ARRAY::AllocateScans(MIL_INT RowIndex, MIL_INT NumberOfScans)
   {
   /* Allocate memory for scans of a given row. */
   m_RowResults[RowIndex].ScanResults = new SCAN_REFLECTANCE_PROFILE_RESULT_ST[NumberOfScans];
   }
   

void VERIFY_ARRAY::Free()
   {
   /* Free all allocated memory */
   for(int ii = 0; ii < m_NumberOfRows; ii++ )
      delete [] m_RowResults[ii].ScanResults;

   delete [] m_RowResults;

   m_RowResults = NULL;
   m_NumberOfRows = 0;
   }

SCAN_REFLECTANCE_PROFILE_RESULT_ST *VERIFY_ARRAY::GetScanResultPtr(MIL_INT RowIndex, MIL_INT ScanIndex)
   {
   return &(m_RowResults[RowIndex].ScanResults[ScanIndex]);
   }

const SCAN_REFLECTANCE_PROFILE_RESULT_ST &VERIFY_ARRAY::GetScanResult(MIL_INT RowIndex, MIL_INT ScanIndex) const
   {
   return m_RowResults[RowIndex].ScanResults[ScanIndex];
   }

ROW_RESULT_ST *VERIFY_ARRAY::GetRowResultPtr(MIL_INT RowIndex)
   {
   return &(m_RowResults[RowIndex]);
   }
/************************************************************************
  VERIFY_RESULT_ST member functions
************************************************************************/
void VERIFY_RESULT_ST::InitializeWorstGrade()
   {
   WorstGrade           = M_CODE_GRADE_NOT_AVAILABLE;
   WorstScanIndex       = 0;
   WorstScanIndexOffset = 0;
   WorstRowIndex        = 0;
   }

void VERIFY_RESULT_ST::UpdateWorstGrade(MIL_INT RowIndex, MIL_INT ScanIndex, MIL_INT ScanIndexOffset, MIL_INT Grade)
   {
   if((Grade < WorstGrade) || (Grade == M_CODE_GRADE_NOT_AVAILABLE))
      {
      WorstGrade           = Grade;
      WorstScanIndex       = ScanIndex;
      WorstRowIndex        = RowIndex;
      WorstScanIndexOffset = ScanIndexOffset;
      }
   }

MIL_INT VERIFY_RESULT_ST::GetWorstCrossRowGrade()
   {
   MIL_INT WorstGrade = CodewordYieldGrade;
   MIL_INT CurrentWorstGrade;

   CurrentWorstGrade = min(ModulationGrade, DecodabilityGrade);
   WorstGrade = min(CurrentWorstGrade, WorstGrade);
   CurrentWorstGrade = min(DefectsGrade, UnusedErrorCorrectionGrade);
   WorstGrade = min( CurrentWorstGrade, WorstGrade);

   return WorstGrade;
   }
