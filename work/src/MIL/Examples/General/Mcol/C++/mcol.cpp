/***************************************************************************************/
/* 
* File name: MCol.cpp  
*
* Synopsis:  This program contains 3 examples of the color module usage:
*
*            The first example performs color segmentation of an image
*            by classifying each pixel with one out of 7 color samples.
*            The ratio of each color in the image is then calculated.
*
*            The second example performs color matching of circular regions
*            in objects located with model finder.
*
*            The third example performs color separation in order to
*            separate 2 types of ink on a piece of paper.
*/
#include <mil.h>

/* Display image margin */
#define DISPLAY_CENTER_MARGIN_X  5

/* Color patch sizes */
#define COLOR_PATCH_SIZEX        30
#define COLOR_PATCH_SIZEY        40

/* Example functions declarations. */  
void ColorSegmentationExample(MIL_ID MilSystem, MIL_ID MilDisplay);
void ColorMatchingExample(MIL_ID MilSystem, MIL_ID MilDisplay);
void ColorSeparationExample(MIL_ID MilSystem, MIL_ID MilDisplay);

/* Utility function */
void DrawSampleColors(MIL_ID DestImage, 
                      const MIL_INT pSamplesColors[][3], 
                      MIL_CONST_TEXT_PTR *pSampleNames, 
                      MIL_INT NumSamples, 
                      MIL_INT XSpacing, 
                      MIL_INT YOffset);

/*****************************************************************************
 Main.
*****************************************************************************/
int MosMain(void)
   {
   MIL_ID MilApplication,     /* Application identifier. */
          MilSystem,          /* System identifier.      */
          MilDisplay;         /* Display identifier.     */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Run the color segmentation example. */
   ColorSegmentationExample(MilSystem, MilDisplay);

   /* Run the color matching example. */
   ColorMatchingExample(MilSystem, MilDisplay);

   /* Run the color projection example. */
   ColorSeparationExample(MilSystem, MilDisplay);

   /* Free defaults. */    
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
   }
/****************************************************************************
 Color Segmentation using color samples. 
****************************************************************************/

/* Image filenames */
#define CANDY_SAMPLE_IMAGE_FILE   M_IMAGE_PATH MIL_TEXT("CandySamples.mim")
#define CANDY_TARGET_IMAGE_FILE   M_IMAGE_PATH MIL_TEXT("Candy.mim")

/* Number of samples */
#define NUM_SAMPLES               6

/* Draw spacing and offset */
#define CANDY_SAMPLES_XSPACING    35
#define CANDY_SAMPLES_YOFFSET     145

/* Match parameters */
#define MATCH_MODE                M_MIN_DIST_VOTE /* Minimal distance vote mode.        */
#define DISTANCE_TYPE             M_MAHALANOBIS   /* Mahalanobis distance.              */
#define TOLERANCE_MODE            M_SAMPLE_STDDEV /* Standard deviation tolerance mode. */
#define TOLERANCE_VALUE           6.0             /* Mahalanobis tolerance value.       */
#define RED_TOLERANCE_VALUE       6.0
#define YELLOW_TOLERANCE_VALUE    12.0
#define PINK_TOLERANCE_VALUE      5.0

void ColorSegmentationExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID     SourceChild        = M_NULL,   /* Source image buffer identifier.    */
              DestChild          = M_NULL,   /* Dest image buffer identifier.      */
              MatchContext       = M_NULL,   /* Color matching context identifier. */
              MatchResult        = M_NULL,   /* Color matching result identifier.  */
              DisplayImage       = M_NULL;   /* Display image buffer identifier.   */

   MIL_INT    SourceSizeX, SourceSizeY,      /* Source image sizes */
              SampleIndex, SpacesIndex;      /* Indices */

   MIL_DOUBLE MatchScore;                    /* Color matching score. */

   /* Blank spaces to align the samples names evenly. */
   MIL_CONST_TEXT_PTR Spaces[4] = { MIL_TEXT(""), 
                                    MIL_TEXT(" "),
                                    MIL_TEXT("  "),
                                    MIL_TEXT("   ") };

   /* Color samples names. */
   MIL_CONST_TEXT_PTR SampleNames[NUM_SAMPLES] = { MIL_TEXT("Green"),
                                                   MIL_TEXT("Red"),
                                                   MIL_TEXT("Yellow"),
                                                   MIL_TEXT("Purple"),
                                                   MIL_TEXT("Blue"),
                                                   MIL_TEXT("Pink") };

   /* Color samples position: {OffsetX, OffsetY}  */
   const MIL_DOUBLE SamplesROI[NUM_SAMPLES][2] = { { 58, 143},
                                                   {136, 148},
                                                   {217, 144},
                                                   {295, 142},
                                                   {367, 143},
                                                   {442, 147} };

   /* Color samples size. */
   const MIL_DOUBLE SampleSizeX = 36, SampleSizeY = 32;

   /* Array for match sample colors. */
   MIL_INT SampleMatchColor[NUM_SAMPLES][3];

   MosPrintf(MIL_TEXT("\nCOLOR SEGMENTATION:\n"));
   MosPrintf(  MIL_TEXT("-------------------\n"));

   /* Allocate the parent display image. */  
   MbufDiskInquire(CANDY_SAMPLE_IMAGE_FILE, M_SIZE_X, &SourceSizeX);
   MbufDiskInquire(CANDY_SAMPLE_IMAGE_FILE, M_SIZE_Y, &SourceSizeY);
   MbufAllocColor(MilSystem, 3, 2*SourceSizeX + DISPLAY_CENTER_MARGIN_X, SourceSizeY, 
                                8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &DisplayImage);
   MbufClear(DisplayImage, M_COLOR_BLACK);

   /* Create a source and dest child in the display image. */
   MbufChild2d(DisplayImage, 0, 0, SourceSizeX, SourceSizeY, &SourceChild);
   MbufChild2d(DisplayImage, SourceSizeX + DISPLAY_CENTER_MARGIN_X, 0, \
                                                 SourceSizeX, SourceSizeY, &DestChild);

   /* Load the source image into the source child. */
   MbufLoad(CANDY_SAMPLE_IMAGE_FILE, SourceChild);
  
   /* Allocate a color matching context. */
   McolAlloc(MilSystem, M_COLOR_MATCHING, M_RGB, M_DEFAULT, M_DEFAULT, &MatchContext);

   /* Define each color sample in the context. */
   for(MIL_INT i=0; i<NUM_SAMPLES; i++)
      {
      McolDefine(MatchContext, SourceChild, M_SAMPLE_LABEL(i+1), M_IMAGE, 
                     SamplesROI[i][0], SamplesROI[i][1], SampleSizeX, SampleSizeY);
      }   

   /* Set the color matching parameters. */
   McolSetMethod(MatchContext, MATCH_MODE, DISTANCE_TYPE, M_DEFAULT, M_DEFAULT);
   McolControl(MatchContext, M_CONTEXT, M_DISTANCE_TOLERANCE_MODE, TOLERANCE_MODE);
   McolControl(MatchContext, M_ALL, M_DISTANCE_TOLERANCE, TOLERANCE_VALUE);

   /* Adjust tolerances for the red, yellow and pink samples. */
   McolControl(MatchContext, M_SAMPLE_INDEX(1), M_DISTANCE_TOLERANCE, 
                                                RED_TOLERANCE_VALUE);
   McolControl(MatchContext, M_SAMPLE_INDEX(2), M_DISTANCE_TOLERANCE,
                                                YELLOW_TOLERANCE_VALUE);
   McolControl(MatchContext, M_SAMPLE_INDEX(5), M_DISTANCE_TOLERANCE,
                                                PINK_TOLERANCE_VALUE);

   /* Preprocess the context. */
   McolPreprocess(MatchContext, M_DEFAULT); 

   /* Fill the samples colors array. */
   for(MIL_INT i=0; i<NUM_SAMPLES; i++)
      {
      McolInquire(MatchContext, M_SAMPLE_LABEL(i+1), 
                  M_MATCH_SAMPLE_COLOR_BAND_0 + M_TYPE_MIL_INT, &SampleMatchColor[i][0]);
      McolInquire(MatchContext, M_SAMPLE_LABEL(i+1), 
                  M_MATCH_SAMPLE_COLOR_BAND_1 + M_TYPE_MIL_INT, &SampleMatchColor[i][1]);
      McolInquire(MatchContext, M_SAMPLE_LABEL(i+1), 
                  M_MATCH_SAMPLE_COLOR_BAND_2 + M_TYPE_MIL_INT, &SampleMatchColor[i][2]);
      }

   /* Draw the samples. */
   DrawSampleColors(DestChild, SampleMatchColor, SampleNames, NUM_SAMPLES, 
                                          CANDY_SAMPLES_XSPACING, CANDY_SAMPLES_YOFFSET);

   /* Select the image buffer for display. */
   MdispSelect(MilDisplay, DisplayImage);

   /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("Color samples are defined for each possible candy color.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to do color matching.\n\n"));
   MosGetch();

   /* Load the target image.*/
   MbufClear(DisplayImage, M_COLOR_BLACK);
   MbufLoad(CANDY_TARGET_IMAGE_FILE, SourceChild);

   /* Allocate a color matching result buffer. */
   McolAllocResult(MilSystem, M_COLOR_MATCHING_RESULT, M_DEFAULT, &MatchResult);

   /* Enable controls to draw the labeled color image. */
   McolControl(MatchContext, M_CONTEXT, M_GENERATE_PIXEL_MATCH, M_ENABLE);
   McolControl(MatchContext, M_CONTEXT, M_GENERATE_SAMPLE_COLOR_LUT, M_ENABLE);

   /* Match with target image. */
   McolMatch(MatchContext, SourceChild, M_DEFAULT, M_NULL, MatchResult, M_DEFAULT);

   /* Retrieve and display results. */   
   MosPrintf(MIL_TEXT("Each pixel of the mixture is matched ")
             MIL_TEXT("with one of the color samples.\n"));
   MosPrintf(MIL_TEXT("\nColor segmentation results:\n"));
   MosPrintf(MIL_TEXT("---------------------------\n"));

   for(SampleIndex=0; SampleIndex<NUM_SAMPLES; SampleIndex++)
      {
      McolGetResult(MatchResult, M_DEFAULT, M_SAMPLE_INDEX(SampleIndex), 
                                                                 M_SCORE, &MatchScore);
      SpacesIndex = 6 - MosStrlen(SampleNames[SampleIndex]);
      MosPrintf(MIL_TEXT("Ratio of %s%s sample = %5.2f%%\n"), SampleNames[SampleIndex], 
                                                      Spaces[SpacesIndex], MatchScore);
      }
   MosPrintf(MIL_TEXT("\nResults reveal the low proportion of Blue candy.\n"));

   /* Draw the colored label image in the destination child. */
   McolDraw(M_DEFAULT, MatchResult, DestChild, M_DRAW_PIXEL_MATCH_USING_COLOR,
                                                            M_ALL, M_ALL, M_DEFAULT);

   /* Pause to show the result image. */
   MosPrintf(MIL_TEXT("\nPress <Enter> to end.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(DestChild);
   MbufFree(SourceChild);
   MbufFree(DisplayImage);
   McolFree(MatchResult);
   McolFree(MatchContext);
   }

/*****************************************************************************
 Color matching in labeled regions.
*****************************************************************************/
/* Image filenames */
#define FUSE_SAMPLES_IMAGE       M_IMAGE_PATH MIL_TEXT("FuseSamples.mim")
#define FUSE_TARGET_IMAGE        M_IMAGE_PATH MIL_TEXT("Fuse.mim")

/* Model Finder context filename */
#define FINDER_CONTEXT           M_IMAGE_PATH MIL_TEXT("FuseModel.mmf")

/* Number of fuse sample objects */
#define NUM_FUSES                4

/* Draw spacing and offset */
#define FUSE_SAMPLES_XSPACING    40
#define FUSE_SAMPLES_YOFFSET     145

void ColorMatchingExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID  DisplayImage            = M_NULL,  /* Display image buffer identifier.   */
           SourceChild             = M_NULL,  /* Source image buffer identifier.    */
           DestChild               = M_NULL,  /* Dest image buffer identifier.      */
           ColMatchContext         = M_NULL,  /* Color matching context identifier. */
           ColMatchResult          = M_NULL,  /* Color matching result identifier.  */
           ModelImage              = M_NULL,  /* Model image buffer identifier.     */
           AreaImage               = M_NULL,  /* Area  image buffer identifier.     */
           OverlayID               = M_NULL,  /* Overlay image buffer identifier.   */
           OverlaySourceChild      = M_NULL,  /* Overlay source child identifier.   */
           OverlayDestChild        = M_NULL,  /* Overlay dest child identifier.     */
           FuseFinderCtx           = M_NULL,  /* Model finder context identifier.   */
           FuseFinderRes           = M_NULL;  /* Model finder result identifier.    */

   /* Image sizes */
   MIL_INT SizeX, SizeY;

   /* Color sample names */
   MIL_CONST_TEXT_PTR SampleNames[NUM_FUSES] = 
      {MIL_TEXT("Green"), MIL_TEXT(" Blue"), MIL_TEXT(" Red"), MIL_TEXT("Yellow")};

   /* Sample ROIs coordinates: OffsetX, OffsetY, SizeX, SizeY */
   const MIL_INT SampleROIs[NUM_FUSES][4]   = {{ 54, 139, 28, 14},
                                               {172, 137, 30, 23},
                                               {296, 135, 31, 23},
                                               {417, 134, 27, 22}};

   /* Array of match sample colors. */
   MIL_INT SampleMatchColor[NUM_FUSES][3];

   MosPrintf(MIL_TEXT("\nCOLOR IDENTIFICATION:\n"));
   MosPrintf(  MIL_TEXT("---------------------\n"));

   /* Allocate the parent display image. */
   MbufDiskInquire(FUSE_TARGET_IMAGE, M_SIZE_X, &SizeX);
   MbufDiskInquire(FUSE_TARGET_IMAGE, M_SIZE_Y, &SizeY);   
   MbufAllocColor(MilSystem,
                  3, 
                  2*SizeX + DISPLAY_CENTER_MARGIN_X, 
                  SizeY, 8+M_UNSIGNED,
                  M_IMAGE+M_DISP+M_PROC, 
                  &DisplayImage);
   MbufClear(DisplayImage, M_COLOR_BLACK);

   /* Allocate the model, area and label images. */
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &ModelImage);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &AreaImage);

   /* Create a source and destination child in the display image. */
   MbufChild2d(DisplayImage, 0, 0, SizeX, SizeY, &SourceChild);
   MbufChild2d(DisplayImage, SizeX + DISPLAY_CENTER_MARGIN_X, 0, SizeX, SizeY, &DestChild);

   /* Load the sample source image. */
   MbufLoad(FUSE_SAMPLES_IMAGE, SourceChild);

   /* Display the image buffer. */
   MdispSelect(MilDisplay, DisplayImage);

   /* Prepare the overlay. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &OverlayID);
   MbufChild2d(OverlayID, 0, 0, SizeX, SizeY, &OverlaySourceChild);
   MbufChild2d(OverlayID, SizeX + DISPLAY_CENTER_MARGIN_X, 0, 
                          SizeX, SizeY, &OverlayDestChild);

   /* Prepare the model finder context and result. */
   MmodRestore(FINDER_CONTEXT, MilSystem, M_DEFAULT, &FuseFinderCtx);   
   MmodPreprocess(FuseFinderCtx, M_DEFAULT);
   MmodAllocResult(MilSystem, M_DEFAULT, &FuseFinderRes);

   /* Allocate a color match context and result. */
   McolAlloc(MilSystem, M_COLOR_MATCHING, M_RGB, M_DEFAULT, M_DEFAULT, &ColMatchContext);
   McolAllocResult(MilSystem, M_COLOR_MATCHING_RESULT, M_DEFAULT, &ColMatchResult);

   /* Define the color samples in the context. */
   for(MIL_INT i = 0; i < NUM_FUSES; i++)
      {
      McolDefine(ColMatchContext, SourceChild, M_SAMPLE_LABEL(i+1), M_IMAGE, 
                (MIL_DOUBLE)SampleROIs[i][0], 
                (MIL_DOUBLE)SampleROIs[i][1], 
                (MIL_DOUBLE)SampleROIs[i][2],
                (MIL_DOUBLE)SampleROIs[i][3]);
      }

   /* Preprocess the context. */
   McolPreprocess(ColMatchContext, M_DEFAULT);
   
   /* Fill the samples colors array. */
   for(MIL_INT i=0; i<NUM_FUSES; i++)
      {
      McolInquire(ColMatchContext, M_SAMPLE_LABEL(i+1), 
                     M_MATCH_SAMPLE_COLOR_BAND_0 + M_TYPE_MIL_INT, &SampleMatchColor[i][0]);
      McolInquire(ColMatchContext, M_SAMPLE_LABEL(i+1), 
                     M_MATCH_SAMPLE_COLOR_BAND_1 + M_TYPE_MIL_INT, &SampleMatchColor[i][1]);
      McolInquire(ColMatchContext, M_SAMPLE_LABEL(i+1), 
                     M_MATCH_SAMPLE_COLOR_BAND_2 + M_TYPE_MIL_INT, &SampleMatchColor[i][2]);
      }

   /* Draw the color samples. */
   DrawSampleColors(DestChild, SampleMatchColor, SampleNames, 
                    NUM_FUSES, FUSE_SAMPLES_XSPACING, FUSE_SAMPLES_YOFFSET);
  
   /* Draw the sample ROIs in the source image overlay. */
   MgraColor(M_DEFAULT, M_COLOR_RED);
   for(MIL_INT SampleIndex = 0; SampleIndex < NUM_FUSES; SampleIndex++)
      {
      MIL_INT XEnd = SampleROIs[SampleIndex][0] + SampleROIs[SampleIndex][2] - 1;
      MIL_INT YEnd = SampleROIs[SampleIndex][1] + SampleROIs[SampleIndex][3] - 1;
      MgraRect(M_DEFAULT, OverlaySourceChild, SampleROIs[SampleIndex][0], 
                                              SampleROIs[SampleIndex][1], 
                                              XEnd, YEnd);
      }

   /* Pause to show the source image. */
   MosPrintf(MIL_TEXT("Colors are defined using one color sample region per fuse.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to process the target image.\n"));
   MosGetch();

   /* Clear the overlay. */
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   /* Load the target image into the source child. */
   MbufLoad(FUSE_TARGET_IMAGE, SourceChild);

   /* Get the grayscale model image and copy it into the display dest child. */
   MimConvert(SourceChild, ModelImage, M_RGB_TO_L);
   MbufCopy(ModelImage, DestChild);

   /* Find the Model. */
   MmodFind(FuseFinderCtx, ModelImage, FuseFinderRes);   

   /* Draw the blob image: labeled circular areas centered at each found fuse occurrence. */
   MIL_INT Number=0;
   MmodGetResult(FuseFinderRes, M_DEFAULT, M_NUMBER+M_TYPE_MIL_INT, &Number);
   MbufClear(AreaImage, 0);
   for(MIL_INT ii=0; ii<Number; ii++)
      {
      MIL_DOUBLE X, Y;
      /* Get the position */
      MmodGetResult(FuseFinderRes, ii, M_POSITION_X, &X);
      MmodGetResult(FuseFinderRes, ii, M_POSITION_Y, &Y);
      /* Set the label color */
      MgraColor(M_DEFAULT, (MIL_DOUBLE) ii+1);
      /* Draw the filled circle */
      MgraArcFill(M_DEFAULT, AreaImage, X, Y, 20, 20, 0, 360);
      }

   /* Enable controls to draw the labeled color image. */
   McolControl(ColMatchContext, M_CONTEXT, M_SAVE_AREA_IMAGE, M_ENABLE);
   McolControl(ColMatchContext, M_CONTEXT, M_GENERATE_SAMPLE_COLOR_LUT, M_ENABLE);

   /* Perform the color matching. */
   McolMatch(ColMatchContext, SourceChild, M_DEFAULT, AreaImage, ColMatchResult, M_DEFAULT);

   /* Draw the label image into the overlay child. */
   McolDraw(M_DEFAULT, ColMatchResult, OverlayDestChild, 
                   M_DRAW_AREA_MATCH_USING_COLOR, M_ALL, M_ALL, M_DEFAULT);

   /* Draw the model position over the colored areas. */
   MgraColor(M_DEFAULT, M_COLOR_BLUE);
   MmodDraw(M_DEFAULT, FuseFinderRes, OverlayDestChild, M_DRAW_BOX+M_DRAW_POSITION, 
            M_ALL, M_DEFAULT);

   /* Enable the display update. */
   MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
   
   /* Pause to show the resulting image. */
   MosPrintf(MIL_TEXT("\nFuses are located using the Model Finder tool.\n"));
   MosPrintf(MIL_TEXT("The color of each target area is identified.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   /* Free all allocations. */
   MmodFree(FuseFinderRes);
   MmodFree(FuseFinderCtx);
   MbufFree(AreaImage);
   MbufFree(ModelImage);
   MbufFree(SourceChild);
   MbufFree(DestChild);
   MbufFree(OverlaySourceChild);
   MbufFree(OverlayDestChild);
   MbufFree(DisplayImage);   
   McolFree(ColMatchContext);
   McolFree(ColMatchResult);
   }
/*****************************************************************************
 Perform color separation of colored inks on a piece of paper.
*****************************************************************************/
 /* Source image */
#define WRITING_IMAGE_FILE    M_IMAGE_PATH MIL_TEXT("stamp.mim")

/* Color triplets */
#define BACKGROUND_COLOR   {245, 234, 206}         
#define WRITING_COLOR      {141, 174, 174}         
#define STAMP_COLOR        {226, 150, 118}         

 /* Drawing spacing */
#define PATCHES_XSPACING    70

void ColorSeparationExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID DisplayImage     = M_NULL,        /* Display image buffer identifier.     */
          SourceChild      = M_NULL,        /* Source image buffer identifier.      */
          DestChild        = M_NULL,        /* Destination image buffer identifier. */
          Child            = M_NULL,        /* Child buffer identifier.             */
          ColorsArray      = M_NULL;        /* Array buffer identifier.             */

   /* Source image sizes. */
   MIL_INT SourceSizeX, SourceSizeY;

   /* Color samples' names */
   MIL_CONST_TEXT_PTR ColorNames[3] =
      { MIL_TEXT("BACKGROUND"), MIL_TEXT("WRITING"), MIL_TEXT("STAMP") };

   /* Array with color patches to draw. */
   MIL_INT Colors[3][3] = { BACKGROUND_COLOR, WRITING_COLOR, STAMP_COLOR };

   /* Samples' color coordinates */
   MIL_UINT8 BackgroundColor[3] = BACKGROUND_COLOR;
   MIL_UINT8 SelectedColor[3]   = WRITING_COLOR;
   MIL_UINT8 RejectedColor[3]   = STAMP_COLOR;

   MosPrintf(MIL_TEXT("\nCOLOR SEPARATION:\n"));
   MosPrintf(  MIL_TEXT("-----------------\n"));

   /* Allocate an array buffer and fill it with the color coordinates. */
   MbufAlloc2d(MilSystem, 3, 3, 8+M_UNSIGNED, M_ARRAY, &ColorsArray);
   MbufPut2d(ColorsArray, 0, 0, 3, 1, BackgroundColor);
   MbufPut2d(ColorsArray, 0, 1, 3, 1, SelectedColor);
   MbufPut2d(ColorsArray, 0, 2, 3, 1, RejectedColor);

   /* Allocate the parent display image. */   
   MbufDiskInquire(WRITING_IMAGE_FILE, M_SIZE_X, &SourceSizeX);
   MbufDiskInquire(WRITING_IMAGE_FILE, M_SIZE_Y, &SourceSizeY);
   MbufAllocColor(MilSystem, 
                  3, 
                  2*SourceSizeX + DISPLAY_CENTER_MARGIN_X, 
                  SourceSizeY, 
                  8+M_UNSIGNED, 
                  M_IMAGE+M_DISP+M_PROC, 
                  &DisplayImage);
   MbufClear(DisplayImage, M_COLOR_BLACK);

   /* Clear the overlay. */
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);   

   /* Create a source and dest child in the display image */
   MbufChild2d(DisplayImage, 0, 0, SourceSizeX, SourceSizeY, &SourceChild);
   MbufChild2d(DisplayImage, SourceSizeX + DISPLAY_CENTER_MARGIN_X, 
                                    0, SourceSizeX, SourceSizeY, &DestChild);

   /* Load the source image into the display image source child. */
   MbufLoad(WRITING_IMAGE_FILE, SourceChild);
     
   /* Draw the color patches. */
   DrawSampleColors(DestChild, Colors, ColorNames, 3, PATCHES_XSPACING, -1);

   /* Display the image. */
   MdispSelect(MilDisplay, DisplayImage);

   /* Pause to show the source image and color patches. */
   MosPrintf(MIL_TEXT("The writing will be separated from the ")
                                     MIL_TEXT("stamp using the following triplets:\n"));
   MosPrintf(MIL_TEXT("the background color: beige [%d, %d, %d],\n"),   
                               BackgroundColor[0], BackgroundColor[1], BackgroundColor[2]);
   MosPrintf(MIL_TEXT("the writing color   : green [%d, %d, %d],\n"),   
                               SelectedColor[0],   SelectedColor[1],   SelectedColor[2]);
   MosPrintf(MIL_TEXT("the stamp color     : red   [%d, %d, %d].\n\n"), 
                               RejectedColor[0],   RejectedColor[1],   RejectedColor[2]);
   MosPrintf(MIL_TEXT("Press <Enter> to extract the writing.\n\n"));
   MosGetch();

   /* Perform the color projection. */
   McolProject(SourceChild, ColorsArray, DestChild, M_NULL, 
                                                M_COLOR_SEPARATION, M_DEFAULT, M_NULL);

   /* Wait for a key. */
   MosPrintf(MIL_TEXT("Press <Enter> to extract the stamp.\n\n"));
   MosGetch();

   /* Switch the order of the selected vs rejected colors in the color array. */
   MbufPut2d(ColorsArray, 0, 2, 3, 1, SelectedColor);
   MbufPut2d(ColorsArray, 0, 1, 3, 1, RejectedColor);   

   /* Perform the color projection. */
   McolProject(SourceChild, ColorsArray, DestChild, M_NULL, 
                                                M_COLOR_SEPARATION, M_DEFAULT, M_NULL);

   /* Wait for a key. */
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(ColorsArray);
   MbufFree(SourceChild);
   MbufFree(DestChild);
   MbufFree(DisplayImage);   
   };
/*****************************************************************************/
/* Draw the samples as color patches. */
void DrawSampleColors(MIL_ID DestImage, 
                      const MIL_INT pSamplesColors[][3], 
                      MIL_CONST_TEXT_PTR *pSampleNames, 
                      MIL_INT NumSamples, 
                      MIL_INT XSpacing, 
                      MIL_INT YOffset)
   {
   MIL_INT DestSizeX = MbufInquire(DestImage, M_SIZE_X, M_NULL);
   MIL_INT DestSizeY = MbufInquire(DestImage, M_SIZE_Y, M_NULL);
   MIL_DOUBLE OffsetX = (DestSizeX - (NumSamples * COLOR_PATCH_SIZEX) - 
                        ((NumSamples - 1) * XSpacing)) /2.0;
   MIL_DOUBLE OffsetY =  YOffset > 0 ? YOffset : (DestSizeY - COLOR_PATCH_SIZEY)/2.0;
   MIL_DOUBLE TextOffsetX;
   MgraFont(M_DEFAULT, M_FONT_DEFAULT_SMALL);

   for(MIL_INT SampleIndex = 0; SampleIndex < NumSamples; SampleIndex++)
      {
      MgraColor(M_DEFAULT, M_RGB888(pSamplesColors[SampleIndex][0], 
                pSamplesColors[SampleIndex][1], pSamplesColors[SampleIndex][2]));
      MgraRectFill(M_DEFAULT, DestImage, OffsetX, OffsetY, OffsetX + COLOR_PATCH_SIZEX, 
                                                           OffsetY + COLOR_PATCH_SIZEY);
      MgraColor(M_DEFAULT, M_COLOR_YELLOW);
      TextOffsetX = OffsetX + COLOR_PATCH_SIZEX / 2.0 - 4.0 * 
                                               MosStrlen(pSampleNames[SampleIndex]) + 0.5;
      MgraText(M_DEFAULT, DestImage, TextOffsetX, OffsetY-20, pSampleNames[SampleIndex]);
      OffsetX += (COLOR_PATCH_SIZEX + XSpacing);
      }
   }
