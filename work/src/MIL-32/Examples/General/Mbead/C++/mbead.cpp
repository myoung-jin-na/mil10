/*****************************************************************************/
/* 
* File name: Mbead.cpp
*
* Synopsis:  This program uses the Bead module to train a bead template
*            and then to inspect a defective bead in a target image.
*/

#include <mil.h>

/* Example functions declarations. */  
void FixturedBeadExample(MIL_ID MilSystem, MIL_ID MilDisplay);
void PredefinedBeadExample(MIL_ID MilSystem, MIL_ID MilDisplay);

/* Utility definitions. */
#define USER_POSITION_COLOR        M_COLOR_RED
#define USER_TEMPLATE_COLOR        M_COLOR_CYAN
#define TRAINED_BEAD_WIDTH_COLOR   M_RGB888(255, 128,   0)
#define MODEL_FINDER_COLOR         M_COLOR_GREEN
#define COORDINATE_SYSTEM_COLOR    M_RGB888(164, 164,   0)
#define RESULT_SEARCH_BOX_COLOR    M_COLOR_CYAN
#define PASS_BEAD_WIDTH_COLOR      M_COLOR_GREEN
#define PASS_BEAD_POSITION_COLOR   M_COLOR_GREEN
#define FAIL_NOT_FOUND_COLOR       M_COLOR_RED
#define FAIL_SMALL_WIDTH_COLOR     M_RGB888(255,   128, 0)
#define FAIL_EDGE_OFFSET_COLOR     M_COLOR_RED

/*****************************************************************************/
/* Main. 
******************************************************************************/
int MosMain(void)
   {
   MIL_ID MilApplication,     /* Application identifier. */
      MilSystem,          /* System Identifier.      */
      MilDisplay;         /* Display identifier.     */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Run fixtured bead example. */
   FixturedBeadExample(MilSystem, MilDisplay);

   /* Run predefined bead example. */
   PredefinedBeadExample(MilSystem, MilDisplay);

   /* Free defaults. */    
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
   }

/*****************************************************************************/
/* Fixtured 'stripe' bead example. */

/* Target MIL image specifications. */
#define IMAGE_FILE_TRAINING       M_IMAGE_PATH MIL_TEXT("BeadTraining.mim")
#define IMAGE_FILE_TARGET         M_IMAGE_PATH MIL_TEXT("BeadTarget.mim")

/* Bead stripe training data definition. */
#define NUMBER_OF_TRAINING_POINTS 13

static MIL_DOUBLE  TrainingPointsX[NUMBER_OF_TRAINING_POINTS] =
   {180, 280, 400, 430, 455, 415, 370, 275, 185, 125, 105, 130, 180};
static MIL_DOUBLE  TrainingPointsY[NUMBER_OF_TRAINING_POINTS] =
   {190, 215, 190, 200, 260, 330, 345, 310, 340, 305, 265, 200, 190};

/* Max angle correction. */
#define MAX_ANGLE_CORRECTION_VALUE 20.0

/* Max offset deviation. */
#define MAX_DEVIATION_OFFSET_VALUE 25.0

/* Maximum negative width variation. */
#define WIDTH_DELTA_NEG_VALUE       2.0

/* Model region  definition. */
#define MODEL_ORIGIN_X              145
#define MODEL_ORIGIN_Y              115
#define MODEL_SIZE_X                275
#define MODEL_SIZE_Y                 60

void FixturedBeadExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID MilGraList,            /* Graphic list identifier.         */
      MilImageTraining,      /* Image buffer identifier.         */
      MilImageTarget,        /* Image buffer identifier.         */
      MilBeadContext,        /* Bead context identifier.         */
      MilBeadResult,         /* Bead result identifier.          */
      MilModelFinderContext, /* Model finder context identifier. */
      MilModelFinderResult,  /* Model finder result identifier.  */
      MilFixturingOffset;    /* Fixturing offset identifier.     */

   MIL_DOUBLE NominalWidth,      /* Nominal width result value. */
      AvWidth,           /* Average width result value. */
      GapCov,            /* Gap coverage result value.  */
      MaxGap,            /* Maximum gap result value.   */
      Score;             /* Bead score result value.    */

   /* Restore source images into automatically allocated image buffers. */
   MbufRestore(IMAGE_FILE_TRAINING, MilSystem, &MilImageTraining);
   MbufRestore(IMAGE_FILE_TARGET,   MilSystem, &MilImageTarget);

   /* Display the training image buffer. */
   MdispSelect(MilDisplay, MilImageTraining);

   /* Allocate a graphic list to hold the subpixel annotations to draw. */
   MgraAllocList(MilSystem, M_DEFAULT, &MilGraList);

   /* Associate the graphic list to the display for annotations. */
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraList);

   /* Original template image. */
   MosPrintf(MIL_TEXT("\nFIXTURED BEAD INSPECTION:\n"));
   MosPrintf(MIL_TEXT("-------------------------\n\n"));
   MosPrintf(MIL_TEXT("This program performs a bead inspection on a mechanical part.\n"));
   MosPrintf(MIL_TEXT("In the first step, a bead template context is trained using an "));
   MosPrintf(MIL_TEXT("image.\nIn the second step, a mechanical part, at an arbitrary "));
   MosPrintf(MIL_TEXT("angle and with\na defective bead, is inspected.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Allocate a MIL bead context. */
   MbeadAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &MilBeadContext);

   /* Allocate a MIL bead result. */
   MbeadAllocResult(MilSystem, M_DEFAULT, &MilBeadResult);

   /* Add a bead template. */
   MbeadTemplate(MilBeadContext, M_ADD, M_DEFAULT, M_TEMPLATE_LABEL(1), 
      NUMBER_OF_TRAINING_POINTS, TrainingPointsX, TrainingPointsY, M_NULL, M_DEFAULT);

   /* Set template input units to world units. */   
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_TEMPLATE_INPUT_UNITS, M_WORLD);

   /* Set the bead 'edge type' search properties. */
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_ANGLE_ACCURACY_MAX_DEVIATION, 
      MAX_ANGLE_CORRECTION_VALUE);

   /* Set the maximum valid bead deformation. */
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_OFFSET_MAX, 
      MAX_DEVIATION_OFFSET_VALUE);

   /* Set the valid bead minimum width criterion. */
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_WIDTH_DELTA_NEG,
      WIDTH_DELTA_NEG_VALUE);

   /* Display the bead polyline. */
   MgraColor(M_DEFAULT, USER_TEMPLATE_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadContext, MilGraList, M_DRAW_POSITION_POLYLINE, 
      M_USER, M_ALL, M_ALL, M_DEFAULT);

   /* Display the bead training points. */
   MgraColor(M_DEFAULT, USER_POSITION_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadContext, MilGraList, M_DRAW_POSITION, 
      M_USER, M_ALL, M_ALL, M_DEFAULT);

   /* Pause to show the template image and user points. */
   MosPrintf(MIL_TEXT("The initial points specified by the user (in red) are\n"));
   MosPrintf(MIL_TEXT("used to train the bead information from an image.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Set a 1:1 calibration to the training image. */
   McalUniform(MilImageTraining, 0, 0, 1, 1, 0, M_DEFAULT);

   /* Train the bead context. */
   MbeadTrain(MilBeadContext, MilImageTraining, M_DEFAULT);

   /* Display the trained bead. */   
   MgraColor(M_DEFAULT, TRAINED_BEAD_WIDTH_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadContext, MilGraList, M_DRAW_WIDTH, 
      M_TRAINED, M_TEMPLATE_LABEL(1), M_ALL, M_DEFAULT);

   /* Retrieve the trained nominal width. */
   MbeadInquire(MilBeadContext,M_TEMPLATE_LABEL(1), M_TRAINED_WIDTH_NOMINAL, 
      &NominalWidth);

   MosPrintf(MIL_TEXT("The template has been trained and is displayed in orange.\n"));
   MosPrintf(MIL_TEXT("Its nominal trained width is %.2f pixels.\n\n"), NominalWidth);

   /* Define model to further fixture the bead template. */
   MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &MilModelFinderContext);

   MmodAllocResult(MilSystem, M_DEFAULT, &MilModelFinderResult);

   MmodDefine(MilModelFinderContext, M_IMAGE, MilImageTraining, 
      MODEL_ORIGIN_X, MODEL_ORIGIN_Y, MODEL_SIZE_X, MODEL_SIZE_Y);

   /* Preprocess the model. */
   MmodPreprocess(MilModelFinderContext, M_DEFAULT);   

   /* Allocate a fixture object. */
   McalAlloc(MilSystem, M_FIXTURING_OFFSET, M_DEFAULT, &MilFixturingOffset);

   /* Learn the relative offset of the model. */
   McalFixture(M_NULL, MilFixturingOffset, M_LEARN_OFFSET, M_MODEL_MOD, 
      MilModelFinderContext, 0, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   /* Display the model. */
   MgraColor(M_DEFAULT, MODEL_FINDER_COLOR);
   MmodDraw(M_DEFAULT, MilModelFinderContext, MilGraList, M_DRAW_BOX+M_DRAW_POSITION, 
      M_DEFAULT, M_ORIGINAL); 

   MosPrintf(MIL_TEXT("A Model Finder model (in green) is also defined to\n"));
   MosPrintf(MIL_TEXT("further fixture the bead verification operation.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Clear the overlay annotation. */
   MgraClear(M_DEFAULT, MilGraList);

   /* Display the target image buffer. */
   MdispSelect(MilDisplay, MilImageTarget);

   /* Find the location of the fixture using Model Finder. */
   MmodFind(MilModelFinderContext, MilImageTarget, MilModelFinderResult);

   /* Display the found model occurrence. */
   MgraColor(M_DEFAULT, MODEL_FINDER_COLOR);
   MmodDraw(M_DEFAULT, MilModelFinderResult, MilGraList, M_DRAW_BOX+M_DRAW_POSITION, 
      M_DEFAULT, M_DEFAULT);

   /* Apply fixture offset to the target image. */
   McalFixture(MilImageTarget, MilFixturingOffset, M_MOVE_RELATIVE, M_RESULT_MOD, 
      MilModelFinderResult, 0, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   /* Display the relative coordinate system. */
   MgraColor(M_DEFAULT, COORDINATE_SYSTEM_COLOR);
   McalDraw(M_DEFAULT, M_NULL, MilGraList, M_DRAW_RELATIVE_COORDINATE_SYSTEM, 
      M_DEFAULT, M_DEFAULT);

   /* Perform the inspection of the bead in the fixtured target image. */
   MbeadVerify(MilBeadContext, MilImageTarget, MilBeadResult, M_DEFAULT);

   /* Display the result search boxes. */
   MgraColor(M_DEFAULT, RESULT_SEARCH_BOX_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadResult, MilGraList, M_DRAW_SEARCH_BOX, M_ALL, 
      M_ALL, M_ALL, M_DEFAULT);

   MosPrintf(MIL_TEXT("The mechanical part's position and angle (in green) were "));
   MosPrintf(MIL_TEXT("located\nusing Model Finder, and the bead's search boxes "));
   MosPrintf(MIL_TEXT("(in cyan) were\npositioned accordingly.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Clear the overlay annotation. */
   MgraClear(M_DEFAULT, MilGraList);

   /* Display the moved relative coordinate system. */
   MgraColor(M_DEFAULT, COORDINATE_SYSTEM_COLOR);
   McalDraw(M_DEFAULT, M_NULL, MilGraList, M_DRAW_RELATIVE_COORDINATE_SYSTEM, 
      M_DEFAULT, M_DEFAULT);

   /* Display the pass bead sections. */
   MgraColor(M_DEFAULT, PASS_BEAD_WIDTH_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadResult, MilGraList, M_DRAW_WIDTH, M_PASS, 
      M_TEMPLATE_LABEL(1), M_ALL, M_DEFAULT);

   /* Display the missing bead sections. */
   MgraColor(M_DEFAULT, FAIL_NOT_FOUND_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadResult, MilGraList, M_DRAW_SEARCH_BOX, 
      M_FAIL_NOT_FOUND, M_ALL, M_ALL, M_DEFAULT);

   /* Display bead sections which do not meet the minimum width criteria. */
   MgraColor(M_DEFAULT, FAIL_SMALL_WIDTH_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadResult, MilGraList, M_DRAW_SEARCH_BOX, 
      M_FAIL_WIDTH_MIN, M_TEMPLATE_LABEL(1), M_ALL, M_DEFAULT);

   /* Retrieve and display general bead results. */
   MbeadGetResult(MilBeadResult, M_TEMPLATE_LABEL(1), M_GENERAL, M_SCORE, &Score);
   MbeadGetResult(MilBeadResult, M_TEMPLATE_LABEL(1), M_GENERAL, M_GAP_COVERAGE, &GapCov);
   MbeadGetResult(MilBeadResult, M_TEMPLATE_LABEL(1), M_GENERAL, M_WIDTH_AVERAGE, &AvWidth);
   MbeadGetResult(MilBeadResult, M_TEMPLATE_LABEL(1), M_GENERAL, M_GAP_MAX_LENGTH, &MaxGap);

   MosPrintf(MIL_TEXT("The bead has been inspected:\n"));
   MosPrintf(MIL_TEXT(" -Passing bead sections (green) cover %.2f%% of the bead\n"), Score);
   MosPrintf(MIL_TEXT(" -Missing bead sections (red) cover %.2f%% of the bead\n"), GapCov);
   MosPrintf(MIL_TEXT(" -Sections outside the specified tolerances are drawn in orange\n"));
   MosPrintf(MIL_TEXT(" -The bead's average width is %.2f pixels\n"), AvWidth);
   MosPrintf(MIL_TEXT(" -The bead's longest gap section is %.2f pixels\n"), MaxGap);

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n"));
   MosGetch();

   /* Free all allocations. */
   MmodFree(MilModelFinderContext);
   MmodFree(MilModelFinderResult);
   MbeadFree(MilBeadContext);
   MbeadFree(MilBeadResult);
   McalFree(MilFixturingOffset);
   MbufFree(MilImageTraining);
   MbufFree(MilImageTarget);
   MgraFree(MilGraList);
   }


/*****************************************************************************/
/* Predefined 'edge' bead example. */

/* Target MIL image specifications. */
#define CAP_FILE_TARGET         M_IMAGE_PATH MIL_TEXT("Cap.mim")

/* Template attributes definition. */
#define CIRCLE_CENTER_X                330.0
#define CIRCLE_CENTER_Y                230.0
#define CIRCLE_RADIUS                  120.0

/* Edge threshold value. */
#define EDGE_THRESHOLD_VALUE            25.0

/* Max offset found and deviation tolerance. */
#define MAX_CONTOUR_DEVIATION_OFFSET     5.0
#define MAX_CONTOUR_FOUND_OFFSET        25.0

void PredefinedBeadExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID MilOverlayImage,       /* Overlay buffer identifier.   */
      MilImageTarget,        /* Image buffer identifier.     */
      MilBeadContext,        /* Bead context identifier.     */
      MilBeadResult;         /* Bead result identifier.      */

   MIL_DOUBLE MaximumOffset;     /* Maximum offset result value. */

   /* Restore target image into an automatically allocated image buffers. */
   MbufRestore(CAP_FILE_TARGET, MilSystem, &MilImageTarget);

   /* Display the training image buffer. */
   MdispSelect(MilDisplay, MilImageTarget);

   /* Prepare the overlay for annotations. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_TRANSPARENT_COLOR);

   /* Original template image. */
   MosPrintf(MIL_TEXT("\nPREDEFINED BEAD INSPECTION:\n"));
   MosPrintf(MIL_TEXT("---------------------------\n\n"));
   MosPrintf(MIL_TEXT("This program performs a bead inspection of a bottle\n"));
   MosPrintf(MIL_TEXT("cap's contour using a predefined circular bead.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Allocate a MIL bead context. */
   MbeadAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &MilBeadContext);

   /* Allocate a MIL bead result. */
   MbeadAllocResult(MilSystem, M_DEFAULT, &MilBeadResult);

   /* Add the bead templates. */
   MbeadTemplate(MilBeadContext, M_ADD, M_BEAD_EDGE, M_TEMPLATE_LABEL(1), 
      0, M_NULL, M_NULL, M_NULL, M_DEFAULT);

   /* Set the bead shape properties. */
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_TRAINING_PATH, M_CIRCLE);

   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_TEMPLATE_CIRCLE_CENTER_X,
      CIRCLE_CENTER_X);
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_TEMPLATE_CIRCLE_CENTER_Y,
      CIRCLE_CENTER_Y);
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_TEMPLATE_CIRCLE_RADIUS,
      CIRCLE_RADIUS);

   /* Set the edge threshold value to extract the object shape. */
   MbeadControl(MilBeadContext, M_TEMPLATE_LABEL(1), M_THRESHOLD_VALUE,
      EDGE_THRESHOLD_VALUE);

   /* Using the default fixed user defined nominal edge width. */
   MbeadControl(MilBeadContext, M_ALL, M_WIDTH_NOMINAL_MODE, M_USER_DEFINED);

   /* Set the maximal expected contour deformation. */
   MbeadControl(MilBeadContext, M_ALL, M_FOUND_OFFSET_MAX, MAX_CONTOUR_FOUND_OFFSET);

   /* Set the maximum valid bead deformation. */
   MbeadControl(MilBeadContext, M_ALL, M_OFFSET_MAX, MAX_CONTOUR_DEVIATION_OFFSET);      

   /* Display the bead in the overlay image. */
   MgraColor(M_DEFAULT, USER_TEMPLATE_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadContext, MilOverlayImage, M_DRAW_POSITION, 
      M_USER, M_ALL, M_ALL, M_DEFAULT);

   /* The bead template is entirely defined and is trained without sample image. */
   MbeadTrain(MilBeadContext, M_NULL, M_DEFAULT);

   /* Display the trained bead. */
   MgraColor(M_DEFAULT, TRAINED_BEAD_WIDTH_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadContext, MilOverlayImage, M_DRAW_SEARCH_BOX, 
      M_TRAINED, M_ALL, M_ALL, M_DEFAULT);

   /* Pause to show the template image and user points. */
   MosPrintf(MIL_TEXT("A circular template that was parametrically defined by the\n"));
   MosPrintf(MIL_TEXT("user is displayed (in cyan). The template has been trained\n")); 
   MosPrintf(MIL_TEXT("and the resulting search is displayed (in orange).\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Perform the inspection of the bead in the fixtured target image. */
   MbeadVerify(MilBeadContext, MilImageTarget, MilBeadResult, M_DEFAULT);

   /* Clear the overlay annotation. */
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_TRANSPARENT_COLOR);

   /* Display the pass bead sections. */
   MgraColor(M_DEFAULT, PASS_BEAD_POSITION_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadResult, MilOverlayImage, M_DRAW_POSITION, M_PASS, 
      M_ALL, M_ALL, M_DEFAULT);

   /* Display the offset bead sections. */
   MgraColor(M_DEFAULT, FAIL_EDGE_OFFSET_COLOR);
   MbeadDraw(M_DEFAULT, MilBeadResult, MilOverlayImage, M_DRAW_POSITION, M_FAIL_OFFSET, 
      M_ALL, M_ALL, M_DEFAULT);

   /* Retrieve and display general bead results. */
   MbeadGetResult(MilBeadResult, M_TEMPLATE_LABEL(1), M_GENERAL, M_OFFSET_MAX,
      &MaximumOffset);

   MosPrintf(MIL_TEXT("The bottle cap shape has been inspected:\n"));
   MosPrintf(MIL_TEXT(" -Sections outside the specified offset"));
   MosPrintf(MIL_TEXT(" tolerance are drawn in red\n"));
   MosPrintf(MIL_TEXT(" -The maximum offset value is %0.2f pixels.\n\n"), MaximumOffset);

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Press <Enter> to terminate.\n"));
   MosGetch();

   /* Free all allocations. */
   MbeadFree(MilBeadContext);
   MbeadFree(MilBeadResult);
   MbufFree(MilImageTarget);
   }