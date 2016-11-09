/***************************************************************************************/
/* 
* File name: VariousCodeReadings.cpp  
*
* Synopsis:  This program contains examples of code reading operations for different 
*            types of codes under various conditions.
*            See the PrintHeader() function below for detailed description.
*
*/
#include <mil.h>


///***************************************************************************
// Example description.
///***************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("VariousCodeReadings\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This program reads different types of codes,\n")
             MIL_TEXT("under various conditions.\n\n")

             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, buffer,\n")
             MIL_TEXT("graphic, calibration, code.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//**************************************
// CODE FOREGROUND COLOR declarations
//**************************************

static const MIL_TEXT_CHAR* CodeForegroundColorFilename = M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/BlackAndWhiteDatamatrix.mim");

void CodeForegroundColor(const MIL_TEXT_CHAR* SrcFilename, 
                         MIL_ID MilSystem, 
                         MIL_ID MilDisplay);

//******************************
// CODE ROTATION declarations
//******************************

static const MIL_TEXT_CHAR* CodeRotationFilename = M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/GS1Databar.mim");

void CodeRotation(const MIL_TEXT_CHAR* SrcFilename, 
                  MIL_ID MilSystem, 
                  MIL_ID MilDisplay);

//*********************************
// CODE DEFORMATION declarations
//*********************************

const MIL_INT NUMBER_GRID_ROWS    = 19;
const MIL_INT NUMBER_GRID_COLUMNS = 19;

static const MIL_TEXT_CHAR* CalDeformationFilename  = M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/CalibrationQRCode.mim");
static const MIL_TEXT_CHAR* CodeDeformationFilename = M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/DeformedQRCode.mim");

void CodeDeformation(const MIL_TEXT_CHAR* SrcFilename, 
                     const MIL_TEXT_CHAR* GridFilename, 
                     MIL_INT RowNumber, 
                     MIL_INT ColumNumber, 
                     MIL_ID MilSystem, 
                     MIL_ID MilDisplay);

//*********************************
// CODE UNEVEN GRID declarations
//*********************************

const MIL_INT CodeUnevenGridNumber = 5;

static const MIL_TEXT_CHAR* CodeUnevenGridFilename[CodeUnevenGridNumber] = 
   {
   M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/UnevenGridDatamatrix1.mim"),
   M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/UnevenGridDatamatrix2.mim"),
   M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/UnevenGridDatamatrix3.mim"),
   M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/UnevenGridDatamatrix4.mim"),
   M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/UnevenGridDatamatrix5.mim")
   };

void CodeUnevenGrid(const MIL_TEXT_CHAR* SrcFilename, 
                    MIL_ID MilSystem, 
                    MIL_ID MilDisplay);

//***********************************************
// CODE ASPECT RATIO AND SHEARING declarations
//***********************************************

static const MIL_TEXT_CHAR* CodeAspectRatioAndShearingFilename = M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/SampleQRCode.mim");

void CodeAspectRatioAndShearing(const MIL_TEXT_CHAR* SrcFilename, 
                                MIL_ID MilSystem, 
                                MIL_ID MilDisplay);

//****************************************
//* CODE FLIPPED DATAMATRIX declarations
//****************************************

const MIL_INT CodeFlippedDatamatrixNumber = 2;

static const MIL_TEXT_CHAR* CodeFlippedDatamatrixFilename[CodeFlippedDatamatrixNumber] = 
   {
   M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/FlippedDatamatrix1.mim"),
   M_IMAGE_PATH MIL_TEXT("VariousCodeReadings/FlippedDatamatrix2.mim")
   };

void CodeFlippedDatamatrix(const MIL_TEXT_CHAR* SrcFilename, 
                           MIL_ID MilSystem, 
                           MIL_ID MilDisplay);

//************************************
// Utility sub-functions declarations
//************************************

void AllocDisplayImage(MIL_ID MilSystem, 
                       MIL_ID MilSrcImage, 
                       MIL_ID MilDisplay, 
                       MIL_ID& MilDispProcImage, 
                       MIL_ID& MilOverlayImage);

void RetrieveAndDrawCode(MIL_ID     MilCodeResult, 
                         MIL_ID     MilDisplay, 
                         MIL_ID     MilOverlayImage, 
                         MIL_DOUBLE DrawPosX,
                         MIL_DOUBLE DrawPosY,
                         bool       DrawBox, 
                         bool       DrawCode);

//*****************************************************************************
// Main
//*****************************************************************************
int MosMain(void)
   {
   MIL_INT ii;

   // Allocate the MIL objects.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

   // Print Header.
   PrintHeader();

   //************************
   // CODE FOREGROUND COLOR    
   //************************
   CodeForegroundColor(CodeForegroundColorFilename, MilSystem, MilDisplay);

   //*****************
   // CODE ROTATION 
   //*****************
   CodeRotation(CodeRotationFilename, MilSystem, MilDisplay);

   //*******************
   // CODE DEFORMATION 
   //*******************
   CodeDeformation(CodeDeformationFilename, CalDeformationFilename, 19, 19, MilSystem, MilDisplay);

   //****************************
   // CODE UNVEN GRID DISTORTION
   //****************************
   for(ii=0; ii<CodeUnevenGridNumber; ii++)
      CodeUnevenGrid(CodeUnevenGridFilename[ii], MilSystem, MilDisplay);

   //*********************************
   // CODE ASPECT RATIO AND SHEARING
   //*********************************
   CodeAspectRatioAndShearing(CodeAspectRatioAndShearingFilename, MilSystem, MilDisplay);

   //*************************
   // CODE FLIPPED DATAMTRIX
   //*************************
   for(ii=0; ii<CodeFlippedDatamatrixNumber; ii++)
      CodeFlippedDatamatrix(CodeFlippedDatamatrixFilename[ii], MilSystem, MilDisplay);

   // Free other allocations.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//************************
// CODE FOREGROUND COLOR  
//************************
void CodeForegroundColor(const MIL_TEXT_CHAR* SrcFilename, 
                         MIL_ID MilSystem, 
                         MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[READING BLACK AND WHITE FOREGROUND COLOR CODES]\n\n")

             MIL_TEXT("In this example two codes of opposite color are read\n")
             MIL_TEXT("by setting the foreground color property.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   // Retrieve image info.
   MIL_INT SizeX;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);

   // Allocate a code context.
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Allocate a code result.
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, M_DATAMATRIX, M_NULL, M_DEFAULT, M_NULL);

   // Enable the presearch mode.
   McodeControl(MilCodeModel, M_USE_PRESEARCH, M_ENABLE);

   // Set the foreground color to black.
   McodeControl(MilCodeModel, M_FOREGROUND_VALUE, M_FOREGROUND_BLACK);
   
   // Read the code and display the result.
   McodeRead(MilCodeContext, MilSrcImage, MilCodeResult);
   RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.25*SizeX, 10, true, false);

   // Set the foreground color to white.
   McodeControl(MilCodeModel, M_FOREGROUND_VALUE, M_FOREGROUND_WHITE);

   // Read the code and display the result.
   McodeRead(MilCodeContext, MilSrcImage, MilCodeResult);

   RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.75*SizeX, 10, true, false);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects.
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);
   McodeFree(MilCodeContext);
   McodeFree(MilCodeResult);
   }

//*****************
// CODE ROTATION 
//*****************
void CodeRotation(const MIL_TEXT_CHAR* SrcFilename, 
                  MIL_ID MilSystem, 
                  MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[READING A ROTATED CODE]\n\n")

             MIL_TEXT("In this example a linear code is read at any angle\n")
             MIL_TEXT("by increasing the search angle range settings.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Retrieve image info.
   MIL_INT SizeX;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);

   // Allocate a code context.
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Allocate a code result.
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, M_GS1_DATABAR, M_NULL, M_DEFAULT, M_NULL);

   // Set search angle range.
   McodeControl(MilCodeModel, M_SEARCH_ANGLE_DELTA_NEG, 180);
   McodeControl(MilCodeModel, M_SEARCH_ANGLE_DELTA_POS, 180);

   // Read the code and display the result.
   McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
   RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 10, true, false);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Rotate the image, then read and display the result.
   for(MIL_INT ii=5; ii<=360; ii+=5)
      {
      // Disable display update.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

      // Clear overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      // Rotate the image.
      MimRotate(MilSrcImage, MilDispProcImage, (MIL_DOUBLE)(ii), M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_BILINEAR);

      // Read the code and display the result.
      McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
      RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 10, true, false);

      // Disable display update.
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
      }

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);
   McodeFree(MilCodeContext);
   McodeFree(MilCodeResult);
   };

//*******************
// CODE DEFORMATION 
//*******************
void CodeDeformation(const MIL_TEXT_CHAR* SrcFilename, 
                     const MIL_TEXT_CHAR* GridFilename, 
                     MIL_INT RowNumber, 
                     MIL_INT ColumNumber, 
                     MIL_ID MilSystem, 
                     MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[RECTIFYING AND READING A DISTORTED CODE]\n\n")

             MIL_TEXT("In this example a distorted code printed on a given non planar surface\n")
             MIL_TEXT("is read by calibrating and correcting the image of the printing surface.\n\n"));

   // Restore the grid image.
   MIL_ID MilSrcImage = MbufRestore(GridFilename, MilSystem, M_NULL);

   // Allocate a calibration.
   MIL_ID MilCalContext = McalAlloc(MilSystem, M_LINEAR_INTERPOLATION, M_DEFAULT, M_NULL);

   // Calibrate from the grid image.
   McalGrid(MilCalContext, MilSrcImage, 0, 0, 0, NUMBER_GRID_ROWS, NUMBER_GRID_COLUMNS, 1, 1, M_DEFAULT, M_CHESSBOARD_GRID);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   // Display the calibration result.
   MgraColor(M_DEFAULT, M_COLOR_BLUE);
   McalDraw(M_DEFAULT, MilSrcImage, MilOverlayImage, M_DRAW_RELATIVE_COORDINATE_SYSTEM, M_DEFAULT, M_DEFAULT);
   
   MosPrintf(MIL_TEXT("The image of the surface is calibrated using a chessboard grid.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Free the calibration image and the display image.
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);

   // Restore the image.
   MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Associate the calibration.
   McalAssociate(MilCalContext, MilSrcImage, M_DEFAULT);

   // Allocate a display image.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   // Retrieve image info.
   MIL_INT SizeX, SizeY;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SizeY);

   // Display the calibration result.
   MgraColor(M_DEFAULT, M_COLOR_BLUE);
   McalDraw(M_DEFAULT, MilSrcImage, MilOverlayImage, M_DRAW_RELATIVE_COORDINATE_SYSTEM, M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("The image of the distorted code is displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Clear the overlay image.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   // Transform the image.
   McalTransformImage(MilSrcImage, MilDispProcImage, MilCalContext, M_BILINEAR, M_DEFAULT, M_WARP_IMAGE + M_CLIP);

   // Display the calibration result.
   McalDraw(M_DEFAULT, MilDispProcImage, MilOverlayImage, M_DRAW_RELATIVE_COORDINATE_SYSTEM, M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("The transformed image of the code is displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Allocate a code context 
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Allocate a code result
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, M_QRCODE, M_NULL, M_DEFAULT, M_NULL);

   // Read the code and display the result.
   McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
   RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 0.5*SizeY, true, true);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects.
   McalFree(MilCalContext);
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);
   McodeFree(MilCodeContext);
   McodeFree(MilCodeResult);
   };

//****************************
// CODE UNVEN GRID DISTORTION
//****************************
void CodeUnevenGrid(const MIL_TEXT_CHAR* SrcFilename, 
                    MIL_ID MilSystem, 
                    MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[READING UNEVEN GRID STEP DISTORTED DATAMATRIX]\n\n")

             MIL_TEXT("In this example an uneven grid step distorted datamatrix is\n")
             MIL_TEXT("read by enabling the reading with distortion capability.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Retrieve image info.
   MIL_INT SizeX;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);

   // Allocate a code context.
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Allocate a code result.
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, M_DATAMATRIX, M_NULL, M_DEFAULT, M_NULL);

   // Set the foreground color to white.
   McodeControl(MilCodeModel, M_FOREGROUND_VALUE, M_FOREGROUND_WHITE);
   
   // Enable the presearch mode.
   McodeControl(MilCodeModel, M_USE_PRESEARCH, M_ENABLE);

   // Enable the uneven grid step distortion mode.
   McodeControl(MilCodeModel, M_DISTORTION, M_UNEVEN_GRID_STEP);

   // Read the code and display the result.
   McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
   RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 10, true, true);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);
   McodeFree(MilCodeContext);
   McodeFree(MilCodeResult);
   };

//*********************************
// CODE ASPECT RATIO AND SHEARING
//*********************************
void CodeAspectRatioAndShearing(const MIL_TEXT_CHAR* SrcFilename, 
                                MIL_ID MilSystem, 
                                MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[READING DATAMATRIX DISTORTED BY ASPECT RATIO AND SHEARING]\n\n")

             MIL_TEXT("In this example a datamatrix is read even if it has an aspect ratio\n")
             MIL_TEXT("different than 1 or if it has shearing.\n\n"));

   MIL_INT ii;

   const MIL_DOUBLE StepValue  = 0.01;
   const MIL_INT    Iterations = 10;

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Retrieve image info.
   MIL_INT SizeX;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);

   // Allocate a code context.
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Allocate a code result.
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, M_QRCODE, M_NULL, M_DEFAULT, M_NULL);

   // Read the code and display the result.
   McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
   RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 10, true, true);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Allocate a warp matrix and initialize it to identity.
   MIL_ID MilWarpMatrix = MbufAlloc2d(MilSystem, 3, 3, 32+M_FLOAT, M_ARRAY, M_NULL);
   MgenWarpParameter(M_NULL, MilWarpMatrix, M_NULL, M_WARP_POLYNOMIAL, M_TRANSLATE, 0, 0);

   for(ii=0; ii<Iterations; ii++)
      {
      // Increase aspect ratio
      MgenWarpParameter(MilWarpMatrix, MilWarpMatrix, M_NULL, M_WARP_POLYNOMIAL, M_SCALE, 1.0, 1.0+StepValue);

      // Disable display update.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

      // Clear the overlay image.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      // Apply the transformation.
      MimWarp(MilSrcImage, MilDispProcImage, MilWarpMatrix, M_NULL, M_WARP_POLYNOMIAL, M_BILINEAR+M_OVERSCAN_CLEAR);

      // Read the code and display the result.
      McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
      RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 10, true, true);

      // Enable display update.
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
      }

   for(ii=0; ii<Iterations; ii++)
      {
      // Increase the shearing in X.
      MgenWarpParameter(MilWarpMatrix, MilWarpMatrix, M_NULL, M_WARP_POLYNOMIAL, M_SHEAR_X, StepValue, M_DEFAULT);
 
      // Disable display update.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

      // Clear the overlay image.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      // Apply the transformation.
      MimWarp(MilSrcImage, MilDispProcImage, MilWarpMatrix, M_NULL, M_WARP_POLYNOMIAL, M_BILINEAR+M_OVERSCAN_CLEAR);

      // Read the code and display the result.
      McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
      RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 10, true, true);

      // Enable display update.
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
      }

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects
   MbufFree(MilWarpMatrix);
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);
   McodeFree(MilCodeContext);
   McodeFree(MilCodeResult);
   };

//*************************
// CODE FLIPPED DATAMTRIX
//*************************
void CodeFlippedDatamatrix(const MIL_TEXT_CHAR* SrcFilename, 
                           MIL_ID MilSystem, 
                           MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[READING FLIPPED DATAMATRIX]\n\n")

             MIL_TEXT("In this example a flipped datamatrix is\n")
             MIL_TEXT("read by enabling the flip capability.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Retrieve image info.
   MIL_INT SizeX;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);

   // Allocate a code context.
   MIL_ID MilCodeContext = McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Allocate a code result.
   MIL_ID MilCodeResult = McodeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Add a code model.
   MIL_ID MilCodeModel = McodeModel(MilCodeContext, M_ADD, M_DATAMATRIX, M_NULL, M_DEFAULT, M_NULL);

   // Enable the presearch mode.
   McodeControl(MilCodeModel, M_USE_PRESEARCH, M_ENABLE);

   // Enable the uneven grid step distortion mode.
   McodeControl(MilCodeModel, M_DISTORTION, M_UNEVEN_GRID_STEP);

   // Enable the uneven grid step distortion mode.
   McodeControl(MilCodeModel, M_CODE_FLIP, M_ANY);

   // Read the code and display the result.
   McodeRead(MilCodeContext, MilDispProcImage, MilCodeResult);
   RetrieveAndDrawCode(MilCodeResult, MilDisplay, MilOverlayImage, 0.5*SizeX, 10, true, true);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects.
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);
   McodeFree(MilCodeContext);
   McodeFree(MilCodeResult);
   };

//************************************
// Utility sub-functions definitions
//************************************

void RetrieveAndDrawCode(MIL_ID     MilCodeResult, 
                         MIL_ID     MilDisplay, 
                         MIL_ID     MilOverlayImage, 
                         MIL_DOUBLE DrawPosX,
                         MIL_DOUBLE DrawPosY,
                         bool       DrawBox, 
                         bool       DrawCode)
   {
   const MIL_INT DispOffsetY = 30;

   // Get decoding status.
   MIL_INT ReadStatus;
   McodeGetResult(MilCodeResult, M_STATUS+M_TYPE_MIL_INT, &ReadStatus);

   // Check if the decode operation was successful.
   if (ReadStatus == M_STATUS_READ_OK)
      {
      // Get decoded string.
      MIL_INT ResultStringSize;
      McodeGetResult(MilCodeResult, M_STRING_SIZE+M_TYPE_MIL_INT, &ResultStringSize);
      MIL_TEXT_CHAR* ResultString = new MIL_TEXT_CHAR[ResultStringSize+1];
      McodeGetResult(MilCodeResult, M_STRING, ResultString);

      // Replace non printable characters with space.
      MIL_INT ii;
      for(ii=0; ResultString[ii] != MIL_TEXT('\0'); ii++) 
         {
         if ((ResultString[ii] < MIL_TEXT('0')) || (ResultString[ii] > MIL_TEXT('Z')))
            ResultString[ii] = MIL_TEXT(' ');
         }

      // Add prefix to the string.
      const MIL_TEXT_CHAR PrefixString[] = MIL_TEXT("Read code: ");
      MIL_INT OutputStringSize = ResultStringSize + MosStrlen(PrefixString) + 1;
      MIL_TEXT_CHAR* OutputString = new MIL_TEXT_CHAR[OutputStringSize]; // Array of characters to draw.
      MosSprintf(OutputString, OutputStringSize, MIL_TEXT("%s%s"), PrefixString, ResultString);

      // Draw read string.
      MgraColor(M_DEFAULT, M_COLOR_CYAN); 
      MgraBackColor(M_DEFAULT, M_COLOR_GRAY);
      MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
      MgraText(M_DEFAULT, MilOverlayImage, DrawPosX, DrawPosY, OutputString);

      // Draw a box around the code.
      if(DrawBox)
         {
         MgraColor(M_DEFAULT, M_COLOR_GREEN); 
         McodeDraw(M_DEFAULT, MilCodeResult, MilOverlayImage, M_DRAW_BOX, 0, M_DEFAULT);
         }

      if(DrawCode)
         {
         MgraColor(M_DEFAULT, M_COLOR_RED); 
         McodeDraw(M_DEFAULT, MilCodeResult, MilOverlayImage, M_DRAW_CODE, 0, M_DEFAULT);
         }

      // Retrieve basic results.
      MIL_DOUBLE PositionX, PositionY, SizeX, SizeY;
      McodeControl(MilCodeResult, M_RESULT_OUTPUT_UNITS, M_PIXEL);
      McodeGetResult(MilCodeResult, M_POSITION_X, &PositionX);
      McodeGetResult(MilCodeResult, M_POSITION_X, &PositionY);
      McodeGetResult(MilCodeResult, M_SIZE_X,     &SizeX);
      McodeGetResult(MilCodeResult, M_SIZE_Y,     &SizeY);
      
      MosPrintf(MIL_TEXT("Reading was successful.\n\n"));
      MosPrintf(MIL_TEXT(" - %s\n"), OutputString);
      MosPrintf(MIL_TEXT(" - Position: (%.2f, %.2f)\n"), PositionX, PositionY);
      MosPrintf(MIL_TEXT(" - Dimensions: (%.2f x %.2f)\n\n"), SizeX, SizeY);
	  
	  delete [] ResultString;
	  delete [] OutputString;
      }
   else
      {
      MosPrintf(MIL_TEXT("Code read operation failed.\n\n"));
      }
   }

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