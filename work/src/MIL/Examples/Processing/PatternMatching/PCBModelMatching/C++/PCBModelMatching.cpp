//*************************************************************************************
//
// File name: PCBModelMatching.cpp
//
// Synopsis:  This program contains an example of PCB pattern matching using
//            some advanced search settings.
//            See the PrintHeader() function below for detailed description.
//

#include <mil.h>

//***************************************************************************
// Example description.
//***************************************************************************
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("PCBModelMatching\n\n"));
   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example uses advanced grayscale pattern matching settings\n"));
   MosPrintf(MIL_TEXT("to improve the search robustness of models with fine structures.\n"));
   MosPrintf(MIL_TEXT("The settings used are pyramid level and angular tolerance, which\n"));
   MosPrintf(MIL_TEXT("are set to advanced automatic modes.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: application, system, display, pattern matching.\n\n"));
 
   // Wait for a key to be pressed.
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

// Functions declarations.
void FindWithAutoFirstLevel(MIL_ID MilSystem, MIL_ID MilDisplay);
void BenchCompareWithAutoFirstLevel(MIL_ID MilSystem, MIL_ID MilDisplay);
void FindWithAutoAngleTolerance(MIL_ID MilSystem, MIL_ID MilDisplay);
void Restore(MIL_CONST_TEXT_PTR ModelSrcFilename, MIL_CONST_TEXT_PTR TargetSrcFilename,
             MIL_ID System, MIL_ID *pParentImage, MIL_ID *pModelChild, MIL_ID *pTargetChild);
void GetOverlayChilds(MIL_ID Display, MIL_ID ParentImage, MIL_ID ModelChild, MIL_ID TargetChild,
                      MIL_ID *pModelOverlay, MIL_ID *pTargetOverlay);

// Macros
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("PCBModelMatching/")

// Constants
static const MIL_INT MAX_CHAR_LENGTH   = 256;

static MIL_CONST_TEXT_PTR SOURCE_FILE    = EXAMPLE_IMAGE_PATH MIL_TEXT("PCBsource.mim");
static MIL_CONST_TEXT_PTR TARGET_FILE    = EXAMPLE_IMAGE_PATH MIL_TEXT("PCBtarget.mim");
static MIL_CONST_TEXT_PTR CO_SOURCE_FILE = EXAMPLE_IMAGE_PATH MIL_TEXT("PCBrotSource.mim");
static MIL_CONST_TEXT_PTR CO_TARGET_FILE = EXAMPLE_IMAGE_PATH MIL_TEXT("PCBrotTarget.mim");

static const MIL_INT MODEL1_OFFSET_X   =  45;
static const MIL_INT MODEL1_OFFSET_Y   =  88;
static const MIL_INT MODEL2_OFFSET_X   = 193;
static const MIL_INT MODEL2_OFFSET_Y   =  22;
static const MIL_INT MODEL_SIZE_X      = 111;
static const MIL_INT MODEL_SIZE_Y      = 100;

static const MIL_INT CO_MODEL_OFFSET_X =  12;
static const MIL_INT CO_MODEL_OFFSET_Y = 111;
static const MIL_INT CO_MODEL_SIZE_X   = 188;
static const MIL_INT CO_MODEL_SIZE_Y   =  36;

static const MIL_INT MARGIN_Y          =  10;

//****************************************************************************
// Main.
//****************************************************************************
int MosMain(void)
   {
   MIL_ID MilApplication,        // Application identifier.
          MilSystem,             // System identifier.
          MilDisplay;            // Display identifier.

   // Allocate MIL objects.
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);

   // Print example description.
   PrintHeader();

   // Run the example with resolution first level adjustment.
   FindWithAutoFirstLevel(MilSystem, MilDisplay);

   // Run the example with benchmark comparison of auto first level.
   BenchCompareWithAutoFirstLevel(MilSystem, MilDisplay);

   // Run the example with angular tolerance adjustment.
   FindWithAutoAngleTolerance(MilSystem, MilDisplay);

   // Free MIL objects.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//***************************************************************************
// Find the model after setting the resolution first level.
//***************************************************************************
void FindWithAutoFirstLevel(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   // Restore the model source image and target image in a parent display image.
   MIL_ID MilSourceImage,
          MilTargetImage,
          MilParentDisplayImage;
   Restore(SOURCE_FILE, TARGET_FILE, MilSystem,
           &MilParentDisplayImage, &MilSourceImage, &MilTargetImage);

   // Define a pattern matching model.
   MIL_ID MilPatModel = MpatAllocModel(MilSystem, MilSourceImage,
                                       MODEL1_OFFSET_X, MODEL1_OFFSET_Y,
                                       MODEL_SIZE_X, MODEL_SIZE_Y,
                                       M_NORMALIZED, M_NULL);

   // Display the model source image and inquire its overlay.
   MIL_ID MilModelOverlay;
   MdispSelect(MilDisplay, MilSourceImage);
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilModelOverlay);

   // Draw the model.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_YELLOW);
   MpatDraw(M_DEFAULT, MilPatModel, MilModelOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_ORIGINAL);

   MosPrintf(MIL_TEXT("ADJUSTING THE PYRAMID'S FIRST LEVEL SETTING:\n"));
   MosPrintf(MIL_TEXT("--------------------------------------------\n\n"));
   MosPrintf(MIL_TEXT("A model with fine structures has been defined in the source image.\n\n"));

   // Wait for a key to be pressed.
   MosPrintf(MIL_TEXT("Press <ENTER> to find the model using default settings in the target image.\n\n"));
   MosGetch();

   // Disable display update.
   MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

   // Display the parent image.
   MdispSelect(MilDisplay, MilParentDisplayImage);

   // Get the overlays of parent, source and target images.
   MIL_ID MilTargetOverlay;
   GetOverlayChilds(MilDisplay, MilParentDisplayImage, MilSourceImage,
                    MilTargetImage, &MilModelOverlay, &MilTargetOverlay);

   // Allocate a pattern matching result.
   MIL_ID MilPatResult = MpatAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Find the model twice with different first level.
   for (MIL_INT i = 0; i < 2; i++)
      {
      if(i == 0)
         {
         // Set the model first level to default strategy.
         MpatSetSearchParameter(MilPatModel, M_FIRST_LEVEL, M_DEFAULT);
         }
      else
         {
         // Set the model first level to automatic content-based strategy.
         MpatSetSearchParameter(MilPatModel, M_FIRST_LEVEL, M_AUTO_CONTENT_BASED);
         MosPrintf(MIL_TEXT("Press <ENTER> to find the model using the content-based first level\n"));
         MosPrintf(MIL_TEXT("pyramid setting.\n\n"));
         MosGetch();
         }

      // Draw the model.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      MgraColor(M_DEFAULT, M_COLOR_YELLOW);
      MpatDraw(M_DEFAULT, MilPatModel, MilModelOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_ORIGINAL);

      // Inquire the first level used.
      MIL_INT FirstLevel;
      MpatInquire(MilPatModel, M_PROC_FIRST_LEVEL + M_TYPE_MIL_INT, &FirstLevel);
      MosPrintf(MIL_TEXT("Model first level used: %d\n"), FirstLevel);

      // Find the model.
      MpatFindModel(MilTargetImage, MilPatModel, MilPatResult);

      // Draw the find results.
      MgraColor(M_DEFAULT, M_COLOR_GREEN);
      MpatDraw(M_DEFAULT, MilPatResult, MilTargetOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_DEFAULT);

      // Print find results.
      if(MpatGetNumber(MilPatResult, M_NULL) == 1)
         {
         MIL_DOUBLE XPos, YPos, Score;

         MpatGetResult(MilPatResult, M_POSITION_X, &XPos);
         MpatGetResult(MilPatResult, M_POSITION_Y, &YPos);
         MpatGetResult(MilPatResult, M_SCORE, &Score);

         MosPrintf(MIL_TEXT("Model found at (%5.2f, %5.2f), Score = %5.2f%%\n\n"), XPos, YPos, Score);
         }
      else
         {
         MosPrintf(MIL_TEXT("Model not found!\n\n"));

         // Print status in display.
         MgraColor(M_DEFAULT, M_COLOR_RED);
         MgraText(M_DEFAULT, MilTargetOverlay, 5, 5, MIL_TEXT("NOT FOUND!"));
         }
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
      }

   // Wait for a key to be pressed.
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Free overlay child.
   MbufFree(MilModelOverlay);
   MbufFree(MilTargetOverlay);

   // Free Pat objects.
   MpatFree(MilPatResult);
   MpatFree(MilPatModel);

   // Free allocated images.
   MbufFree(MilSourceImage);
   MbufFree(MilTargetImage);
   MbufFree(MilParentDisplayImage);
   }

//***************************************************************************
// Compare benchmarks between two models with content-based first level.
//***************************************************************************
void BenchCompareWithAutoFirstLevel(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("-----------------------------------------------------------\n"));
   MosPrintf(MIL_TEXT("A benchmark comparison is performed using the content-based\n"));
   MosPrintf(MIL_TEXT("pyramid level setting:\n\n"));
   MosPrintf(MIL_TEXT("Two models of the same size are defined in the source image.\n"));
   MosPrintf(MIL_TEXT("Model #1 has larger structures than Model #0, allowing it to use\n"));
   MosPrintf(MIL_TEXT("a greater pyramid first level to speed up the search.\n\n"));

   // Restore the model source image and target image in a parent display image.
   MIL_ID MilSourceImage,
          MilTargetImage,
          MilParentDisplayImage;
   Restore(SOURCE_FILE, TARGET_FILE, MilSystem,
           &MilParentDisplayImage, &MilSourceImage, &MilTargetImage);

   MIL_DOUBLE FindTime[2];
   MIL_ID MilPatModelArray[2];

   // Define a first pattern matching model.
   MpatAllocModel(MilSystem, MilSourceImage, MODEL1_OFFSET_X, MODEL1_OFFSET_Y,
                  MODEL_SIZE_X, MODEL_SIZE_Y, M_NORMALIZED, &MilPatModelArray[0]);

   // Define a second pattern matching model.
   MpatAllocModel(MilSystem, MilSourceImage, MODEL2_OFFSET_X, MODEL2_OFFSET_Y,
                  MODEL_SIZE_X, MODEL_SIZE_Y, M_NORMALIZED, &MilPatModelArray[1]);

   // Display the model source image and inquire its overlay.
   MIL_ID MilModelOverlay;
   MdispSelect(MilDisplay, MilSourceImage);
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilModelOverlay);

   // Draw the models.
   MdispControl(MilDisplay, M_UPDATE, M_DISABLE);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_YELLOW);
   MpatDraw(M_DEFAULT, MilPatModelArray[0], MilModelOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_ORIGINAL);
   MpatDraw(M_DEFAULT, MilPatModelArray[1], MilModelOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_ORIGINAL);
   MgraText(M_DEFAULT, MilModelOverlay, MODEL1_OFFSET_X, MODEL1_OFFSET_Y-5, MIL_TEXT("#0"));
   MgraText(M_DEFAULT, MilModelOverlay, MODEL2_OFFSET_X, MODEL2_OFFSET_Y-5, MIL_TEXT("#1"));
   MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

   // Display the parent image.
   MdispSelect(MilDisplay, MilParentDisplayImage);

   // Get the overlays of parent, source and target images.
   MIL_ID MilTargetOverlay;
   GetOverlayChilds(MilDisplay, MilParentDisplayImage, MilSourceImage,
                    MilTargetImage, &MilModelOverlay, &MilTargetOverlay);

   // Allocate a pattern matching result.
   MIL_ID MilPatResult = MpatAllocResult(MilSystem, M_DEFAULT, &MilPatResult);

   MdispControl(MilDisplay, M_UPDATE, M_DISABLE);
   for (MIL_INT i = 0; i < 2; i++)
      {
      // Set the model first level to automatic content-based strategy.
      MpatSetSearchParameter(MilPatModelArray[i], M_FIRST_LEVEL, M_AUTO_CONTENT_BASED);

      // Inquire the first level used.
      MIL_INT FirstLevel;
      MpatInquire(MilPatModelArray[i], M_PROC_FIRST_LEVEL + M_TYPE_MIL_INT, &FirstLevel);
      MosPrintf(MIL_TEXT("Model #%d first level: %d\n"), i, FirstLevel);

      // Find the model.
      MappTimer(M_DEFAULT, M_TIMER_RESET, NULL);
      MpatFindModel(MilTargetImage, MilPatModelArray[i], MilPatResult);
      MappTimer(M_DEFAULT, M_TIMER_READ, &FindTime[i]);

      if(MpatGetNumber(MilPatResult, M_NULL) == 1)
         {
         // Get result and center positions.
         MIL_DOUBLE PosX, PosY, CenterX, CenterY;
         MpatGetResult(MilPatResult, M_POSITION_X, &PosX);
         MpatGetResult(MilPatResult, M_POSITION_Y, &PosY);
         MpatInquire(MilPatModelArray[i], M_CENTER_X, &CenterX);
         MpatInquire(MilPatModelArray[i], M_CENTER_Y, &CenterY);

         // Draw the found occurrence.
         MgraColor(M_DEFAULT, M_COLOR_GREEN);
         MpatDraw(M_DEFAULT, MilPatResult, MilTargetOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_DEFAULT);
         MIL_TEXT_CHAR Label[MAX_CHAR_LENGTH];
         MosSprintf(Label, MAX_CHAR_LENGTH, MIL_TEXT("#%d"), i);
         MgraText(M_DEFAULT, MilTargetOverlay, PosX-CenterX, PosY-CenterY-5, Label);
         }

      MosPrintf(MIL_TEXT("Model #%d find time: %6.3f ms\n\n"), i, FindTime[i] * 1000.0);
      }
   MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

   // Wait for a key to be pressed.
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Clear the overlay.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   // Free overlay child.
   MbufFree(MilModelOverlay);
   MbufFree(MilTargetOverlay);

   // Free Pat objects.
   MpatFree(MilPatResult);
   MpatFree(MilPatModelArray[0]);
   MpatFree(MilPatModelArray[1]);

   // Free allocated images.
   MbufFree(MilSourceImage);
   MbufFree(MilTargetImage);
   MbufFree(MilParentDisplayImage);
   }

//***************************************************************************
// Find the model after setting the angular tolerance.
//***************************************************************************
void FindWithAutoAngleTolerance(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID MilSourceImage,
          MilRotatedTargetImage,
          MilParentDisplayImage;

   // Restore the model source image and target image in a parent display image.
   Restore(CO_SOURCE_FILE, CO_TARGET_FILE, MilSystem,
           &MilParentDisplayImage, &MilSourceImage, &MilRotatedTargetImage);

   // Define a Circular Overscan Model.
   MIL_ID MilPatCOModel = MpatAllocModel(MilSystem, MilSourceImage, CO_MODEL_OFFSET_X, CO_MODEL_OFFSET_Y,
                                         CO_MODEL_SIZE_X, CO_MODEL_SIZE_Y, M_NORMALIZED + M_CIRCULAR_OVERSCAN, M_NULL);

   // Enable angular search.
   MpatSetAngle(MilPatCOModel, M_SEARCH_ANGLE_MODE, M_ENABLE);
   MpatSetAngle(MilPatCOModel, M_SEARCH_ANGLE_DELTA_NEG, 180);
   MpatSetAngle(MilPatCOModel, M_SEARCH_ANGLE_DELTA_POS, 180);
   MpatSetAngle(MilPatCOModel, M_SEARCH_ANGLE_INTERPOLATION_MODE, M_BILINEAR);

   // Display the model source image and get its overlay.
   MIL_ID MilModelOverlay;
   MdispSelect(MilDisplay, MilSourceImage);
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilModelOverlay);

   // Draw the model.
   MgraColor(M_DEFAULT, M_COLOR_YELLOW);
   MpatDraw(M_DEFAULT, MilPatCOModel, MilModelOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_ORIGINAL);

   MosPrintf(MIL_TEXT("ADJUSTING THE ANGULAR TOLERANCE SETTING:\n"));
   MosPrintf(MIL_TEXT("----------------------------------------\n\n"));
   MosPrintf(MIL_TEXT("An elongated model with fine structures has been defined in the source image.\n"));
   MosPrintf(MIL_TEXT("Such a model is very sensitive to rotation: a small angular variation results\n"));
   MosPrintf(MIL_TEXT("in a significant drop in correlation score.\n\n"));

   // Wait for a key to be pressed.
   MosPrintf(MIL_TEXT("Press <ENTER> to find the model using default settings in the target image.\n\n"));
   MosGetch();

   // Disable display update.
   MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

   // Display the parent image.
   MdispSelect(MilDisplay, MilParentDisplayImage);

   // Get the overlays of parent, source and target images.
   MIL_ID MilTargetOverlay;
   GetOverlayChilds(MilDisplay, MilParentDisplayImage, MilSourceImage,
                    MilRotatedTargetImage, &MilModelOverlay, &MilTargetOverlay);

   // Allocate a pattern matching result.
   MIL_ID MilPatResult = MpatAllocResult(MilSystem, M_DEFAULT, &MilPatResult);

   // Find the model twice with different angular tolerance.
   for (MIL_INT i = 0; i < 2; i++)
      {
      if(i == 0)
         {
         // Set the model angular tolerance to default value.
         MpatSetAngle(MilPatCOModel, M_SEARCH_ANGLE_TOLERANCE, M_DEFAULT);
         }
      else
         {
         // Set the model angular tolerance to automatic value.
         MpatSetAngle(MilPatCOModel, M_SEARCH_ANGLE_TOLERANCE, M_AUTO);
         MosPrintf(MIL_TEXT("Press <ENTER> to find the model using the automatic angular tolerance setting.\n\n"));
         MosGetch();
         }

      // Draw the model.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      MgraColor(M_DEFAULT, M_COLOR_YELLOW);
      MpatDraw(M_DEFAULT, MilPatCOModel, MilModelOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_ORIGINAL);

      // Preprocess the model.
      MpatPreprocModel(M_NULL, MilPatCOModel, M_DEFAULT);

      // Find the model.
      MpatFindModel(MilRotatedTargetImage, MilPatCOModel, MilPatResult);

      // Draw the find results.
      MgraColor(M_DEFAULT, M_COLOR_GREEN);
      MpatDraw(M_DEFAULT, MilPatResult, MilTargetOverlay, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_DEFAULT);

      // Print find results.
      if(MpatGetNumber(MilPatResult, M_NULL) == 1)
         {
         MIL_DOUBLE XPos, YPos, Score, Angle;

         MpatGetResult(MilPatResult, M_POSITION_X, &XPos);
         MpatGetResult(MilPatResult, M_POSITION_Y, &YPos);
         MpatGetResult(MilPatResult, M_SCORE, &Score);
         MpatGetResult(MilPatResult, M_ANGLE, &Angle);

         MosPrintf(MIL_TEXT("Model found at (%5.2f, %5.2f), Angle = %5.2f, Score = %5.2f%%\n\n"), XPos, YPos, Angle, Score);
         }
      else
         {
         MosPrintf(MIL_TEXT("Model not found!\n\n"));

         // Print status in display.
         MgraColor(M_DEFAULT, M_COLOR_RED);
         MgraText(M_DEFAULT, MilTargetOverlay, 5, 5, MIL_TEXT("NOT FOUND!"));
         }

      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
      }

   // Wait for a key to be pressed.
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   // Free overlay child.
   MbufFree(MilModelOverlay);
   MbufFree(MilTargetOverlay);

   // Free Pat objects.
   MpatFree(MilPatResult);
   MpatFree(MilPatCOModel);

   // Free allocated images.
   MbufFree(MilSourceImage);
   MbufFree(MilRotatedTargetImage);
   MbufFree(MilParentDisplayImage);
   }

//***************************************************************************
// Load model and target buffers in a parent display image.
//***************************************************************************
void Restore(MIL_CONST_TEXT_PTR ModelSrcFilename, MIL_CONST_TEXT_PTR TargetSrcFilename,
             MIL_ID System, MIL_ID *pParentImage, MIL_ID *pModelChild, MIL_ID *pTargetChild)
   {
   MIL_INT ModelSizeX = MbufDiskInquire(ModelSrcFilename, M_SIZE_X, M_NULL);
   MIL_INT ModelSizeY = MbufDiskInquire(ModelSrcFilename, M_SIZE_Y, M_NULL);
   MIL_INT TargetSizeX = MbufDiskInquire(TargetSrcFilename, M_SIZE_X, M_NULL);
   MIL_INT TargetSizeY = MbufDiskInquire(TargetSrcFilename, M_SIZE_Y, M_NULL);

   // Create parent image.
   MIL_INT ParentSizeX = TargetSizeX > ModelSizeX ? TargetSizeX : ModelSizeX;
   MIL_INT ParentSizeY = ModelSizeY + MARGIN_Y + TargetSizeY;

   MbufAlloc2d(System, ParentSizeX, ParentSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, pParentImage);

   // Clear the parent.
   MbufClear(*pParentImage, 0);

   // Define the child regions.
   MbufChild2d(*pParentImage, 0, 0, ModelSizeX, ModelSizeY, pModelChild);
   MbufChild2d(*pParentImage, 0, ModelSizeY + MARGIN_Y, TargetSizeX, TargetSizeY, pTargetChild);

   // Load the images.
   MbufLoad(ModelSrcFilename,  *pModelChild);
   MbufLoad(TargetSrcFilename, *pTargetChild);
   }

//***************************************************************************
// Get the overlay of target and model child images.
//***************************************************************************
void GetOverlayChilds(MIL_ID Display, MIL_ID ParentImage, MIL_ID ModelChild, MIL_ID TargetChild,
                      MIL_ID *pModelOverlay, MIL_ID *pTargetOverlay)
   {
   MIL_ID ParentOverlay;
   MdispControl(Display, M_OVERLAY, M_ENABLE);
   MdispInquire(Display, M_OVERLAY_ID, &ParentOverlay);

   // Define the overlay child buffers.
   MIL_INT TargetSizeX = MbufInquire(TargetChild, M_SIZE_X, M_NULL);
   MIL_INT TargetSizeY = MbufInquire(TargetChild, M_SIZE_Y, M_NULL);
   MIL_INT SourceSizeX = MbufInquire(ModelChild, M_SIZE_X, M_NULL);
   MIL_INT SourceSizeY = MbufInquire(ModelChild, M_SIZE_Y, M_NULL);
   MbufChild2d(ParentOverlay, 0, 0, SourceSizeX, SourceSizeY, pModelOverlay);
   MbufChild2d(ParentOverlay, 0, SourceSizeY + MARGIN_Y, TargetSizeX, TargetSizeY, pTargetOverlay);
   }
