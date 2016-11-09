//***************************************************************************************/
// 
// File name: RegisteredGoldenTemplate.cpp  
//
// Synopsis:  This program contains an example of difference analysis between
//            a target image made of tiles and a golden template that uses the Registration
//            and Model Finder module.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("RegisteredGoldenTemplate\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example performs defect detection analysis by comparing a target image \n"));
   MosPrintf(MIL_TEXT("made of tiles and with a golden template. In the first step, a flat field   \n"));
   MosPrintf(MIL_TEXT("correction is applied to the target image to flatten the image. In the      \n"));
   MosPrintf(MIL_TEXT("second step, the registration module is used to align each tile with the    \n"));
   MosPrintf(MIL_TEXT("golden template, and a map of the differences between the aligned tile and  \n"));
   MosPrintf(MIL_TEXT("the golden template is calculated. Finally, the map of differences is       \n"));
   MosPrintf(MIL_TEXT("analyzed to determine the presence of defects.                              \n\n"));
   MosPrintf(MIL_TEXT("Two methods to get the map of differences are shown                         \n\n"));
   MosPrintf(MIL_TEXT("   1- Grayscale approach:The absolute value of differences combined with an \n"));
   MosPrintf(MIL_TEXT("      an hysteresis threshold.\n"));
   MosPrintf(MIL_TEXT("   2- Binary approach:An XOR between the binary template and target.        \n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: Application, system, graphic, model finder, registration, blob\n"));
   MosPrintf(MIL_TEXT("              image processing                                              \n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//****************************************************************************
// Constants.
//****************************************************************************

// Filenames
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("RegisteredGoldenTemplate/")

static const MIL_INT NB_TILES = 3;

static MIL_CONST_TEXT_PTR TEMPLATE_FILE = EXAMPLE_IMAGE_PATH MIL_TEXT("Template.bmp");

static MIL_CONST_TEXT_PTR TILES_FILES = EXAMPLE_IMAGE_PATH MIL_TEXT("Tile%d.mim");

static const MIL_CONST_TEXT_PTR FLAT_FIELD_IMAGE = EXAMPLE_IMAGE_PATH MIL_TEXT("FlatFieldImage.mim");
static const MIL_CONST_TEXT_PTR DARK_IMAGE = EXAMPLE_IMAGE_PATH MIL_TEXT("DarkImage.mim");
static const MIL_CONST_TEXT_PTR OFFSET_IMAGE = EXAMPLE_IMAGE_PATH MIL_TEXT("OffsetImage.mim");

static const MIL_DOUBLE MODEL_OFFSET_X = 1352;
static const MIL_DOUBLE MODEL_OFFSET_Y = 300;
static const MIL_DOUBLE MODEL_SIZE_X = 256;
static const MIL_DOUBLE MODEL_SIZE_Y = 256;

static const MIL_INT TILES_EXPECTED_OVERLAP = 354;

static const MIL_INT LOCATION_DELTA = 5;

static const MIL_DOUBLE BIN_HIGH_THRESHOLD = 60.0;
static const MIL_DOUBLE BIN_LOW_THRESHOLD = 30.0;

static const MIL_DOUBLE BIN_THRESHOLD = 230.0;

static const MIL_DOUBLE MIN_DEFECT_AREA = 70;

static const MIL_INT FLAT_FIELD_GAIN = 255;

static const MIL_INT EDGE_MASK_THICKNESS = 2;

static const MIL_INT DISPLAY_SEPARATOR_SIZE = 50;

static const MIL_INT IMAGE_TITLE_COLOR = M_COLOR_RED;

// Functions declarations. 
template <bool IsBinaryProc>
void FindDefect(MIL_ID MilSystem, MIL_ID MilTemplateWorkImage, MIL_ID MilTemplateBinMask, MIL_ID MilWarpedTarget, MIL_ID MilDiffImage, MIL_ID MilBinImage, MIL_ID MilFlatFieldContext, MIL_ID MilDisplay, MIL_ID MilGraList);

template <bool Clear>
void DrawText(MIL_ID MilGraList, MIL_CONST_TEXT_PTR, MIL_INT Color, MIL_DOUBLE PosX, MIL_DOUBLE PosY);

// Macro definitions.

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate MIL objects. 
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_WINDOWED, M_NULL);
   MIL_ID MilGraList = MgraAllocList(MilSystem, M_DEFAULT, M_NULL);
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraList);
   MdispControl(MilDisplay, M_SCALE_DISPLAY, M_ENABLE);

   MosPrintf(MIL_TEXT("REGISTERED GOLDEN TEMPLATE:\n")
             MIL_TEXT("----------------------------------------------------------------------------\n\n"));

   // Main application code here. 
   PrintHeader();    
   
   // Print a message indicating that we are in the setup phase.
   MosPrintf(MIL_TEXT("1- SETTING UP THE DIFFERENT TOOL\n")
             MIL_TEXT("----------------------------------------------------------------------------\n\n"));   

   // Allocate the flat field correction context 
   MIL_ID MilFlatFieldContext = MimAlloc(MilSystem, M_FLAT_FIELD_CONTEXT, M_DEFAULT, M_NULL);

   // Set the flat field context 
   MIL_ID MilFlatCalImage = MbufRestore(DARK_IMAGE, MilSystem, M_NULL);
   MdispSelect(MilDisplay, MilFlatCalImage);
   DrawText<true>(MilGraList, MIL_TEXT("Dark Image"), IMAGE_TITLE_COLOR, 0, 0);   
   MimControl(MilFlatFieldContext, M_DARK_IMAGE, MilFlatCalImage);
      
   MosPrintf(MIL_TEXT("The dark image for the flat field correction is shown in the display.\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MbufLoad(FLAT_FIELD_IMAGE, MilFlatCalImage);
   DrawText<true>(MilGraList, MIL_TEXT("Flat Image"), IMAGE_TITLE_COLOR, 0, 0);
   MimControl(MilFlatFieldContext, M_FLAT_IMAGE, MilFlatCalImage);

   MosPrintf(MIL_TEXT("The flat field image for the flat field correction is shown in the display.\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MbufLoad(OFFSET_IMAGE, MilFlatCalImage);
   DrawText<true>(MilGraList, MIL_TEXT("Offset Image"), IMAGE_TITLE_COLOR, 0, 0);
   MimControl(MilFlatFieldContext, M_OFFSET_IMAGE, MilFlatCalImage);

   MosPrintf(MIL_TEXT("The offset image for the flat field correction is shown in the display.\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MimControl(MilFlatFieldContext, M_GAIN_CONST, FLAT_FIELD_GAIN);
   MbufFree(MilFlatCalImage);
   MimFlatField(MilFlatFieldContext, M_NULL, M_NULL, M_PREPROCESS);

   // Restore the template image.
   MIL_ID MilTemplateImage = MbufRestore(TEMPLATE_FILE, MilSystem, M_NULL);
   MIL_INT TemplateSizeX = MbufInquire(MilTemplateImage, M_SIZE_X, M_NULL);
   MIL_INT TemplateSizeY = MbufInquire(MilTemplateImage, M_SIZE_Y, M_NULL);
   MIL_INT TemplateType = MbufInquire(MilTemplateImage, M_TYPE, M_NULL);
   MIL_INT TemplateSizeBand = MbufInquire(MilTemplateImage, M_SIZE_BAND, M_NULL);

   // Display the template image and print intermediate message.
   MdispSelect(MilDisplay, MilTemplateImage);
   DrawText<true>(MilGraList, MIL_TEXT("Template Image"), IMAGE_TITLE_COLOR, 0, 0);
   MosPrintf(MIL_TEXT("The golden template image is shown in the display.\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Allocate a target image in which the tiles will be warped.
   MIL_ID MilWarpedTarget = MbufAllocColor(MilSystem, TemplateSizeBand, TemplateSizeX, TemplateSizeY, TemplateType, M_IMAGE+M_PROC+M_DISP, M_NULL);
   
   // Allocate a difference image.
   MIL_ID MilDiffImage = MbufAllocColor(MilSystem, TemplateSizeBand, TemplateSizeX, TemplateSizeY, TemplateType, M_IMAGE+M_PROC+M_DISP, M_NULL);

   // Allocate some binary image for the example.
   MIL_ID MilTemplateBinImage = MbufAllocColor(MilSystem, TemplateSizeBand, TemplateSizeX, TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MIL_ID MilTemplateBinMask = MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MIL_ID MilBinImage = MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);

   // Transform the template into a binary image and get the edge mask.
   MimBinarize(MilTemplateImage, MilTemplateBinImage, M_GREATER, BIN_THRESHOLD, M_DEFAULT);
   MimRank(MilTemplateBinImage, MilTemplateBinImage, M_3X3_RECT, M_MEDIAN, M_BINARY);
   MimDilate(MilTemplateBinImage, MilTemplateBinMask, EDGE_MASK_THICKNESS, M_BINARY);
   MimArith(MilTemplateBinMask, MilTemplateBinImage, MilTemplateBinMask, M_SUB);
   MimErode(MilTemplateBinImage, MilBinImage, EDGE_MASK_THICKNESS, M_BINARY);
   MimArith(MilTemplateBinImage, MilBinImage, MilBinImage, M_SUB);
   MimArith(MilTemplateBinMask, MilBinImage, MilTemplateBinMask, M_OR);
   MimArith(MilTemplateBinMask, M_NULL, MilTemplateBinMask, M_NOT);

   // Calibrate the template.
   McalUniform(MilTemplateImage, 0.0, 0.0, 1.0, 1.0, 0.0, M_DEFAULT);
   McalUniform(MilTemplateBinImage, 0.0, 0.0, 1.0, 1.0, 0.0, M_DEFAULT);

   // Show the binary image as well as the edge mask.
   MdispSelect(MilDisplay, MilTemplateBinImage);
   DrawText<true>(MilGraList, MIL_TEXT("Template Binary Image"), IMAGE_TITLE_COLOR, 0, 0);
   MosPrintf(MIL_TEXT("A binary version of the golden template, that could be also interpreted as a\n") 
             MIL_TEXT("CAD model, was created.\n")       
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MdispSelect(MilDisplay, MilTemplateBinMask);
   DrawText<true>(MilGraList, MIL_TEXT("Template Binary Mask"), IMAGE_TITLE_COLOR, 0, 0);
   MosPrintf(MIL_TEXT("An edge mask was created from the binary golden template image.\n") 
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Find the defects in the gray scale image.
   MosPrintf(MIL_TEXT("2- DEFECT DETECTION USING GRAYSCALE IMAGES\n") 
             MIL_TEXT("---------------------------------------\n\n"));
   FindDefect<false>(MilSystem, MilTemplateImage, MilTemplateBinMask, MilWarpedTarget, MilDiffImage, MilBinImage, MilFlatFieldContext, MilDisplay, MilGraList);

   // Break between the methods.
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Find the defects in the binary image.
   MosPrintf(MIL_TEXT("2- DEFECT DETECTION USING BINARY IMAGES\n") 
             MIL_TEXT("------------------------------------\n\n"));
   FindDefect<true>(MilSystem, MilTemplateBinImage, MilTemplateBinMask, MilWarpedTarget, MilDiffImage, MilBinImage, MilFlatFieldContext, MilDisplay, MilGraList);

   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   // Free MIL objects. 
   MbufFree(MilTemplateImage);
   MbufFree(MilTemplateBinImage);
   MbufFree(MilTemplateBinMask);
   MbufFree(MilWarpedTarget);
   MbufFree(MilDiffImage);
   MbufFree(MilBinImage);  

   MimFree(MilFlatFieldContext);

   MgraFree(MilGraList);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   
   MappFree(MilApplication);

   return 0;
   }


//*****************************************************************************
// Defect detection function.
//*****************************************************************************
template <bool IsBinaryProc>
void FindDefect(MIL_ID MilSystem, MIL_ID MilTemplateWorkImage, MIL_ID MilTemplateBinMask, MIL_ID MilWarpedTarget, MIL_ID MilDiffImage, MIL_ID MilBinImage, MIL_ID MilFlatFieldContext, MIL_ID MilDisplay, MIL_ID MilGraList)
   {
   // Allocate the registration context and result.
   MIL_ID MilRegContext = MregAlloc(MilSystem, M_CORRELATION, M_DEFAULT, M_NULL);
   MIL_ID MilRegResult = MregAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Setup the registration context.
   MregControl(MilRegContext, M_CONTEXT, M_LOCATION_DELTA, LOCATION_DELTA);
   MregControl(MilRegContext, M_CONTEXT, M_TRANSFORMATION_TYPE, M_PERSPECTIVE);
   MregControl(MilRegResult, M_GENERAL, M_MOSAIC_STATIC_INDEX, 0);
   MregControl(MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_X, 0);
   MregControl(MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_Y, 0);
   MregControl(MilRegResult, M_GENERAL, M_MOSAIC_COMPOSITION, IsBinaryProc ? M_LAST_IMAGE : M_FUSION_IMAGE);

   // Allocate the Model Finder context.
   MIL_ID MilModContext = MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, M_NULL);
   MIL_ID MilModResult = MmodAllocResult(MilSystem, M_DEFAULT, M_NULL);

   // Allocate blob and select some features.
   MIL_ID MilBlobResult = MblobAllocResult(MilSystem, M_NULL);
   MIL_ID MilBlobFeatureList = MblobAllocFeatureList(MilSystem, M_NULL);
   MblobSelectFeature(MilBlobFeatureList, M_AREA);
   MblobSelectFeature(MilBlobFeatureList, M_MAX_PIXEL);
   MblobSelectFeature(MilBlobFeatureList, M_BOX);

   // Define the model and put its reference in the top left corner.
   MmodDefine(MilModContext, M_IMAGE, MilTemplateWorkImage, MODEL_OFFSET_X, MODEL_OFFSET_Y, MODEL_SIZE_X, MODEL_SIZE_Y);
   MmodControl(MilModContext, 0, M_REFERENCE_X, 0);
   MmodControl(MilModContext, 0, M_REFERENCE_Y, 0);
   MmodControl(MilModContext, M_CONTEXT, M_SPEED, M_VERY_HIGH);
   MmodControl(MilModContext, 0, M_ASSOCIATED_CALIBRATION, McalInquire(MilTemplateWorkImage, M_ASSOCIATED_CALIBRATION, M_NULL));
   MmodPreprocess(MilModContext, M_DEFAULT);

   // Create a fixturing offset.
   MIL_ID MilModelFixturingOffset = McalAlloc(MilSystem, M_FIXTURING_OFFSET, M_DEFAULT, M_NULL);
   McalFixture(M_NULL, MilModelFixturingOffset, M_LEARN_OFFSET, M_MODEL_MOD, MilModContext, 0, M_DEFAULT, M_DEFAULT, M_DEFAULT);  

   // Display the model position in the template image and print intermediate message.
   MdispSelect(MilDisplay, MilTemplateWorkImage);
   DrawText<true>(MilGraList, MIL_TEXT("Template Image"), IMAGE_TITLE_COLOR, 0, 0);
   
   MgraControl(M_DEFAULT, M_DRAW_OFFSET_X, -MODEL_OFFSET_X);
   MgraControl(M_DEFAULT, M_DRAW_OFFSET_Y, -MODEL_OFFSET_Y);
   MgraColor(M_DEFAULT, M_COLOR_RED);
   MmodDraw(M_DEFAULT, MilModContext, MilGraList, M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   MmodDraw(M_DEFAULT, MilModContext, MilGraList, M_DRAW_BOX + M_DRAW_POSITION, M_DEFAULT, M_DEFAULT);
   MgraControl(M_DEFAULT, M_DRAW_OFFSET_X, 0.0);
   MgraControl(M_DEFAULT, M_DRAW_OFFSET_Y, 0.0);
   
   MgraColor(M_DEFAULT, M_COLOR_YELLOW);
   McalDraw(M_DEFAULT, MilModelFixturingOffset, MilGraList, M_DRAW_FIXTURING_OFFSET, M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("The model used to find the rough locations of the tiles is shown in the     \n")
             MIL_TEXT("display over the template image.\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Restore the tiles and calibrate the images.
   // The calibration is placed so that the position of the model is at the same place
   // in the roughly aligned image as in the template image.
   MIL_ID MilTilesImages[NB_TILES];
   MIL_ID MilRegImageArray[NB_TILES+1];
   MIL_ID MilTileDispImage;
   MIL_TEXT_CHAR FileName[512];
   MIL_INT CurrentOffset = 0;
   MIL_INT NbFound = 0;
   MIL_DOUBLE ExpectedWorldPosX, ExpectedWorldPosY, ExpectedAngle;
   MIL_INT TilesSizeX, TilesSizeY, DispImageSizeX;
   for(MIL_INT TileIdx = 0; TileIdx < NB_TILES; TileIdx++)
      {
      // Restore the tile.
      MosSprintf(FileName, 512, TILES_FILES, TileIdx);
      MbufRestore(FileName, MilSystem, &MilTilesImages[TileIdx]);
      
      // Disable the display updates.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

      // Allocate the display image if on the first iteration.
      if(TileIdx == 0)
         {
         MbufInquire(MilTilesImages[TileIdx], M_SIZE_X, &TilesSizeX);
         MbufInquire(MilTilesImages[TileIdx], M_SIZE_Y, &TilesSizeY);

         DispImageSizeX = IsBinaryProc ? TilesSizeX*3 + DISPLAY_SEPARATOR_SIZE * 2 : TilesSizeX*2 + DISPLAY_SEPARATOR_SIZE;

         MbufAlloc2d(MilSystem, DispImageSizeX, TilesSizeY, 8+M_UNSIGNED, M_IMAGE + M_DISP, &MilTileDispImage);
         MdispSelect(MilDisplay, MilTileDispImage);
         }
      
      MgraClear(M_DEFAULT, MilGraList);
      DrawText<true>(MilGraList, MIL_TEXT("Original Tile Image"), IMAGE_TITLE_COLOR, 0, 0);
      DrawText<false>(MilGraList, MIL_TEXT("Flat Field Corrected Tile Image"), IMAGE_TITLE_COLOR, (MIL_DOUBLE)(TilesSizeX + DISPLAY_SEPARATOR_SIZE), 0);        
      if(IsBinaryProc)
         DrawText<false>(MilGraList, MIL_TEXT("Binary Image"), IMAGE_TITLE_COLOR, (MIL_DOUBLE)((TilesSizeX + DISPLAY_SEPARATOR_SIZE))*2, 0);

      // Set the tile in the registration image array.
      MilRegImageArray[TileIdx+1] = MilTilesImages[TileIdx];

      // Copy the tile on the display.
      MbufCopyColor2d(MilTilesImages[TileIdx], MilTileDispImage, M_ALL_BANDS, 0, 0, M_ALL_BANDS, 0, 0, TilesSizeX, TilesSizeY);

      // Do the flat field correction.
      MimFlatField(MilFlatFieldContext, MilTilesImages[TileIdx], MilTilesImages[TileIdx], M_DEFAULT);
      
      // Copy the flat field corrected tile in the display.
      MbufCopyColor2d(MilTilesImages[TileIdx], MilTileDispImage, M_ALL_BANDS, 0, 0, M_ALL_BANDS, TilesSizeX + DISPLAY_SEPARATOR_SIZE, 0, TilesSizeX, TilesSizeY);

      // Create the binary version of the tile.
      if(IsBinaryProc)
         {
         MimBinarize(MilTilesImages[TileIdx], MilTilesImages[TileIdx], M_GREATER, BIN_THRESHOLD, M_DEFAULT);
         MimRank(MilTilesImages[TileIdx], MilTilesImages[TileIdx], M_3X3_RECT, M_MEDIAN, M_BINARY);
         MbufCopyColor2d(MilTilesImages[TileIdx], MilTileDispImage, M_ALL_BANDS, 0, 0, M_ALL_BANDS, (TilesSizeX + DISPLAY_SEPARATOR_SIZE)*2, 0, TilesSizeX, TilesSizeY);
         }

      // Enable the display updates.
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

      // Print intermediate message.
      MosPrintf(MIL_TEXT("Tile %i is shown in the display.\n"), TileIdx);
      
      // Associate a uniform calibration on the image based on the expected position of the tile in the world.
      McalFixture(MilTilesImages[TileIdx], M_NULL, M_MOVE_RELATIVE, M_POINT_AND_ANGLE, MilTemplateWorkImage, -CurrentOffset, 0.0, 0.0, M_DEFAULT);
      CurrentOffset += MbufInquire(MilTilesImages[TileIdx], M_SIZE_X, M_NULL) - TILES_EXPECTED_OVERLAP;

      if(!NbFound)
         {
         // Try to find the model in the tile.
         MmodFind(MilModContext, MilTilesImages[TileIdx], MilModResult);

         // Check if the model is found.      
         MmodGetResult(MilModResult, M_GENERAL, M_NUMBER + M_TYPE_MIL_INT, &NbFound);
         if(NbFound)
            {
            // Get the expected world position and angle of the model.
            MmodGetResult(MilModResult, 0, M_POSITION_X, &ExpectedWorldPosX);
            MmodGetResult(MilModResult, 0, M_POSITION_Y, &ExpectedWorldPosY);
            MmodGetResult(MilModResult, 0, M_ANGLE, &ExpectedAngle);

            // Draw the model in the graphic list of the display.
            MgraControl(M_DEFAULT, M_DRAW_OFFSET_X, -(MIL_DOUBLE)(DispImageSizeX - TilesSizeX));
            MgraColor(M_DEFAULT, M_COLOR_RED);
            MmodDraw(M_DEFAULT, MilModResult, MilGraList, M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);
            MgraColor(M_DEFAULT, M_COLOR_GREEN);
            MmodDraw(M_DEFAULT, MilModResult, MilGraList, M_DRAW_BOX + M_DRAW_POSITION, M_DEFAULT, M_DEFAULT);
            MgraControl(M_DEFAULT, M_DRAW_OFFSET_X, 0.0);

            // Print intermediate message.
            MosPrintf(MIL_TEXT("The model was found in the tile and its position is drawn over the\n")
                      MIL_TEXT("tile image.\n"));
            }
         }   

      // Pause to show the tile and (if it was found) the model.
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();
      }

   // Free the tile disp image.
   MbufFree(MilTileDispImage);

   // Set the location of the tiles according to the Model Finder position found.
   for(MIL_INT TileIdx = 0; TileIdx < NB_TILES; TileIdx++)
      {
      // Move the calibration of the tile.
      McalFixture(MilTilesImages[TileIdx], MilModelFixturingOffset, M_MOVE_RELATIVE, M_POINT_AND_ANGLE, M_DEFAULT, ExpectedWorldPosX, ExpectedWorldPosY, ExpectedAngle, M_DEFAULT);

      // Get the world position of the top left corner.
      MIL_DOUBLE RealWorldRoughPosX, RealWorldRoughPosY;
      McalTransformCoordinate(MilTilesImages[TileIdx], M_PIXEL_TO_WORLD, 0, 0, &RealWorldRoughPosX, &RealWorldRoughPosY);

      // Set the rough location of the tile according to the template image.
      MregSetLocation(MilRegContext, TileIdx + 1, 0, M_POSITION_XY_ANGLE, RealWorldRoughPosX, RealWorldRoughPosY, -ExpectedAngle, M_DEFAULT, M_DEFAULT);
      }
  
   // Registers all the images with the template.
   MilRegImageArray[0] = MilTemplateWorkImage;
   MregCalculate(MilRegContext, MilRegImageArray, MilRegResult, NB_TILES+1, M_DEFAULT);

   // Get the status of the registration.
   MIL_INT Status;
   MregGetResult(MilRegResult, M_GENERAL, M_RESULT+M_TYPE_MIL_INT, &Status);

   if(Status == M_SUCCESS)
      {
      // Create the mosaic image. Copy the template first so that the pixel that were not present in the 
      // tiles do not appear as defects.
      MbufCopy(MilTemplateWorkImage, MilWarpedTarget);

      MilRegImageArray[0] = M_NULL;
      MregTransformImage(MilRegResult, MilRegImageArray, MilWarpedTarget, NB_TILES+1, (IsBinaryProc? M_NEAREST_NEIGHBOR : M_BILINEAR)+M_OVERSCAN_DISABLE, M_DEFAULT);

      // Display the warped result.
      MdispSelect(MilDisplay, MilWarpedTarget);
      MgraClear(M_DEFAULT, MilGraList);
      DrawText<true>(MilGraList, MIL_TEXT("Mosaic Image"), IMAGE_TITLE_COLOR, 0, 0);
      for(MIL_INT TileIdx = 0; TileIdx < NB_TILES; TileIdx++)
         {
         MgraColor(M_DEFAULT, M_COLOR_DARK_GREEN);
         MregDraw(M_DEFAULT, MilRegResult, MilGraList, M_DRAW_BOX, TileIdx+1, M_DEFAULT);
         
         // Get the world position of the top left corner.
         MIL_DOUBLE RealWorldRoughPosX, RealWorldRoughPosY;
         McalTransformCoordinate(MilTilesImages[TileIdx], M_PIXEL_TO_WORLD, 0, 0, &RealWorldRoughPosX, &RealWorldRoughPosY);
         
         // Draw the rectangel in the image.
         MgraColor(M_DEFAULT, M_COLOR_DARK_RED);
         MgraRectAngle(M_DEFAULT,
                       MilGraList,
                       RealWorldRoughPosX,
                       RealWorldRoughPosY,
                       MbufInquire(MilTilesImages[TileIdx], M_SIZE_X, M_NULL),
                       MbufInquire(MilTilesImages[TileIdx], M_SIZE_Y, M_NULL),
                       -ExpectedAngle,
                       M_DEFAULT);
         }
      MosPrintf(MIL_TEXT("The mosaic image is shown in the display.\n")
                MIL_TEXT("The tiles bounding boxes are drawn over the mosaic image.\n")
                MIL_TEXT("The red rectangles show the rough locations while the green\n")
                MIL_TEXT("rectangle show the registered locations.\n")
                MIL_TEXT("Press <Enter> to continue.\n\n"));         
      MosGetch();
      MgraClear(M_DEFAULT, MilGraList);
      
      // Get the difference map.
      if(IsBinaryProc)
         {
         // Get the XOR between the mosaic and the template.
         MimArith(MilTemplateWorkImage, MilWarpedTarget, MilBinImage, M_XOR);
         MdispSelect(MilDisplay, MilBinImage);
         }
      else
         {
         // Get the absolute value of difference between the warped target and the template image.
         MimArith(MilTemplateWorkImage, MilWarpedTarget, MilDiffImage, M_SUB_ABS);
         MdispSelect(MilDisplay, MilDiffImage);
         }
      DrawText<true>(MilGraList, MIL_TEXT("Difference Image"), IMAGE_TITLE_COLOR, 0, 0);
                        
      // Show the difference image.      
      MosPrintf(MIL_TEXT("The differences between the template and the mosaic  is shown in the     \n")
                MIL_TEXT("display.\n")
                MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      if(!IsBinaryProc)
         {
         // Binarize the difference image.
         MimBinarize(MilDiffImage, MilBinImage, M_GREATER, BIN_LOW_THRESHOLD, M_DEFAULT);

         // Show the binarize differences.
         MdispSelect(MilDisplay, MilBinImage);
         MosPrintf(MIL_TEXT("The binarized differences are shown in the display.\n")
                   MIL_TEXT("Press <Enter> to continue.\n\n"));
         MosGetch();
         }

      // Mask the edges.
      MimArith(MilBinImage, MilTemplateBinMask, MilBinImage, M_AND);

      // Show the masked edges in the display.
      MosPrintf(MIL_TEXT("The edges were masked in the binary image.\n")
                MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();      

      // Disable the display updates.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);
      
      // Calculate the blobs and select only the ones over a certain area and
      // and with a maximum pixel over a certain value(Threshold hysteresis).
      MblobCalculate(MilBinImage, IsBinaryProc? M_NULL : MilDiffImage, MilBlobFeatureList, MilBlobResult);
      MblobSelect(MilBlobResult, M_DELETE, M_AREA, M_LESS, MIN_DEFECT_AREA, M_NULL);
      
      if(!IsBinaryProc)
         MblobSelect(MilBlobResult, M_DELETE, M_MAX_PIXEL, M_LESS, BIN_HIGH_THRESHOLD, M_NULL);

      // If some blobs were found draw them in the graphic list.
      MIL_INT NbBlobs;
      if(MblobGetNumber(MilBlobResult, &NbBlobs))
         {
         MgraColor(M_DEFAULT, M_COLOR_BLUE);
         MblobDraw(M_DEFAULT, MilBlobResult, MilGraList, M_DRAW_BLOBS, M_DEFAULT, M_DEFAULT);
         MgraColor(M_DEFAULT, M_COLOR_RED);
         MblobDraw(M_DEFAULT, MilBlobResult, MilGraList, M_DRAW_BOX, M_DEFAULT, M_DEFAULT);
         }

      // Show the extracted defect.
      MdispSelect(MilDisplay, MilWarpedTarget);
      MosPrintf(MIL_TEXT("The defects are shown in the mosaic image.\n"));
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
      }
   else
      {
      MosPrintf(MIL_TEXT("Registration unsuccessfull!\n"));
      }

   MblobFree(MilBlobResult);
   MblobFree(MilBlobFeatureList);

   for(MIL_INT TileIdx = 0; TileIdx < NB_TILES; TileIdx++)
      MbufFree(MilTilesImages[TileIdx]);

   McalFree(MilModelFixturingOffset);

   MmodFree(MilModContext);
   MmodFree(MilModResult);

   MregFree(MilRegResult);
   MregFree(MilRegContext);
   }

template <bool Clear>
void DrawText(MIL_ID MilGraList, MIL_CONST_TEXT_PTR Text, MIL_INT Color, MIL_DOUBLE PosX, MIL_DOUBLE PosY)
   {
   if(Clear)
      MgraClear(M_DEFAULT, MilGraList);
   MgraColor(M_DEFAULT, (MIL_DOUBLE)Color);
   MgraText(M_DEFAULT, MilGraList, PosX, PosY, Text);
   }