/*************************************************************************************/
/* 
 * File name: MMod.cpp
 * 
 * Synopsis:  This program uses the Geometric Model Finder module to define geometric 
 *            models and searches for these models in a target image. A simple single 
 *            model example (1 model, 1 occurrence, good search conditions) is  
 *            presented first, followed by a more complex example (multiple models, 
 *            multiple occurrences in a complex scene with bad search conditions).
 */
#include <mil.h>

/* Example functions declarations. */  
void SingleModelExample(MIL_ID MilSystem, MIL_ID MilDisplay);
void MultipleModelsExample(MIL_ID MilSystem, MIL_ID MilDisplay);

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

   /* Run single model example. */
   SingleModelExample(MilSystem, MilDisplay);

   /* Run multiple model example. */
   MultipleModelsExample(MilSystem, MilDisplay);

   /* Free defaults. */    
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
   }


/*****************************************************************************/
/* Single model example. */

/* Source MIL image file specifications. */
#define SINGLE_MODEL_IMAGE          M_IMAGE_PATH MIL_TEXT("SingleModel.mim")

/* Target MIL image file specifications. */
#define SINGLE_MODEL_TARGET_IMAGE   M_IMAGE_PATH MIL_TEXT("SingleTarget.mim")

/* Search speed: M_VERY_HIGH for faster search, M_MEDIUM for precision and robustness. */
#define SINGLE_MODEL_SEARCH_SPEED   M_VERY_HIGH 

/* Model specifications. */
#define MODEL_OFFSETX               176L
#define MODEL_OFFSETY               136L
#define MODEL_SIZEX                 128L
#define MODEL_SIZEY                 128L
#define MODEL_MAX_OCCURRENCES       16L


void SingleModelExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID      MilImage,                         /* Image buffer identifier.*/
               GraphicList;                      /* Graphic list identifier.  */
   MIL_ID      MilSearchContext,                 /* Search context          */
               MilResult;                        /* Result identifier.      */
   MIL_DOUBLE  ModelDrawColor = M_COLOR_RED;     /* Model draw color.       */
   MIL_INT     Model[MODEL_MAX_OCCURRENCES],     /* Model index.            */
               NumResults  = 0L;                 /* Number of results found.*/
   MIL_DOUBLE  Score[MODEL_MAX_OCCURRENCES],     /* Model correlation score.*/
               XPosition[MODEL_MAX_OCCURRENCES], /* Model X position.       */
               YPosition[MODEL_MAX_OCCURRENCES], /* Model Y position.       */
               Angle[MODEL_MAX_OCCURRENCES],     /* Model occurrence angle. */
               Scale[MODEL_MAX_OCCURRENCES],     /* Model occurrence scale. */
               Time = 0.0;                       /* Bench variable.         */
   int         i;                                /* Loop variable.          */


   /* Restore the model image and display it */
   MbufRestore(SINGLE_MODEL_IMAGE, MilSystem, &MilImage);
   MdispSelect(MilDisplay, MilImage);

   /* Allocate a graphic list to hold the subpixel annotations to draw. */
   MgraAllocList(MilSystem, M_DEFAULT, &GraphicList);

   /* Associate the graphic list to the display for annotations. */
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);
   
   /* Allocate a Geometric Model Finder context. */
   MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &MilSearchContext);

   /* Allocate a result buffer. */
   MmodAllocResult(MilSystem, M_DEFAULT, &MilResult);

   /* Define the model. */
   MmodDefine(MilSearchContext, M_IMAGE, MilImage,
              MODEL_OFFSETX, MODEL_OFFSETY, MODEL_SIZEX, MODEL_SIZEY);

   /* Set the search speed. */
   MmodControl(MilSearchContext, M_CONTEXT, M_SPEED, SINGLE_MODEL_SEARCH_SPEED);

   /* Preprocess the search context. */
   MmodPreprocess(MilSearchContext, M_DEFAULT);
   
   /* Draw box and position it in the source image to show the model. */
   MgraColor(M_DEFAULT, ModelDrawColor);
   MmodDraw(M_DEFAULT, MilSearchContext, GraphicList,
            M_DRAW_BOX+M_DRAW_POSITION, 0, M_ORIGINAL);

   /* Pause to show the model. */
   MosPrintf(MIL_TEXT("\nGEOMETRIC MODEL FINDER:\n"));
   MosPrintf(MIL_TEXT("-----------------------\n\n"));
   MosPrintf(MIL_TEXT("A model context was defined with "));
   MosPrintf(MIL_TEXT("the model in the displayed image.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   
   /* Clear annotations. */
   MgraClear(M_DEFAULT, GraphicList);

   /* Load the single model target image. */
   MbufLoad(SINGLE_MODEL_TARGET_IMAGE, MilImage);   

   /* Dummy first call for bench measure purpose only (bench stabilization, 
      cache effect, etc...). This first call is NOT required by the application. */
   MmodFind(MilSearchContext, MilImage, MilResult);

   /* Reset the timer. */
   MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
   
   /* Find the model. */
   MmodFind(MilSearchContext, MilImage, MilResult);

   /* Read the find time. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &Time);

   /* Get the number of models found. */
   MmodGetResult(MilResult, M_DEFAULT, M_NUMBER+M_TYPE_MIL_INT, &NumResults);

   /* If a model was found above the acceptance threshold. */
   if ( (NumResults >= 1) && (NumResults <= MODEL_MAX_OCCURRENCES) )
      {
      /* Get the results of the single model. */
      MmodGetResult(MilResult, M_DEFAULT, M_INDEX+M_TYPE_MIL_INT, Model);
      MmodGetResult(MilResult, M_DEFAULT, M_POSITION_X, XPosition);
      MmodGetResult(MilResult, M_DEFAULT, M_POSITION_Y, YPosition);
      MmodGetResult(MilResult, M_DEFAULT, M_ANGLE, Angle);
      MmodGetResult(MilResult, M_DEFAULT, M_SCALE, Scale);
      MmodGetResult(MilResult, M_DEFAULT, M_SCORE, Score);

      /* Print the results for each model found. */
      MosPrintf(MIL_TEXT("The model was found in the target image:\n\n"));
      MosPrintf(MIL_TEXT("Result   Model   X Position   Y Position   ")
                                  MIL_TEXT("Angle   Scale   Score\n\n"));      
      for (i=0; i<NumResults; i++)         
         {
         MosPrintf(MIL_TEXT("%-9d%-8d%-13.2f%-13.2f%-8.2f%-8.2f%-5.2f%%\n"), 
               i, Model[i], XPosition[i], YPosition[i], 
               Angle[i], Scale[i], Score[i]);                 
         }
      MosPrintf(MIL_TEXT("\nThe search time is %.1f ms\n\n"), Time*1000.0);
      
      /* Draw edges, position and box over the occurrences that were found. */
      for (i=0; i<NumResults; i++)
         {         
         MgraColor(M_DEFAULT, ModelDrawColor);
         MmodDraw(M_DEFAULT,  MilResult, GraphicList,
                  M_DRAW_EDGES+M_DRAW_BOX+M_DRAW_POSITION, i, M_DEFAULT);
         }
      }
   else
      {
      MosPrintf(MIL_TEXT("The model was not found or the number of models ")
                                         MIL_TEXT("found is greater than\n"));
      MosPrintf(MIL_TEXT("the specified maximum number of occurrence !\n\n"));
      }

   /* Wait for a key to be pressed. */ 
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Free MIL objects. */
   MgraFree(GraphicList);
   MbufFree(MilImage);
   MmodFree(MilSearchContext);
   MmodFree(MilResult);
   }


/*****************************************************************************
/* Multiple models example. */

/* Source MIL image file specifications. */
#define MULTI_MODELS_IMAGE          M_IMAGE_PATH MIL_TEXT("MultipleModel.mim")

/* Target MIL image file specifications. */
#define MULTI_MODELS_TARGET_IMAGE   M_IMAGE_PATH MIL_TEXT("MultipleTarget.mim")

/* Search speed: M_VERY_HIGH for faster search, M_MEDIUM for precision and robustness.*/
#define MULTI_MODELS_SEARCH_SPEED   M_VERY_HIGH 

/* Number of models. */
#define NUMBER_OF_MODELS            3L
#define MODELS_MAX_OCCURRENCES     16L

/* Model 1 specifications. */
#define MODEL0_OFFSETX              34L
#define MODEL0_OFFSETY              93L
#define MODEL0_SIZEX                214L
#define MODEL0_SIZEY                76L
#define MODEL0_DRAW_COLOR           M_COLOR_RED

/* Model 2 specifications. */
#define MODEL1_OFFSETX              73L
#define MODEL1_OFFSETY              232L
#define MODEL1_SIZEX                150L
#define MODEL1_SIZEY                154L
#define MODEL1_REFERENCEX           23L
#define MODEL1_REFERENCEY           127L
#define MODEL1_DRAW_COLOR           M_COLOR_GREEN

/* Model 3 specifications. */
#define MODEL2_OFFSETX              308L
#define MODEL2_OFFSETY              39L
#define MODEL2_SIZEX                175L
#define MODEL2_SIZEY                357L
#define MODEL2_REFERENCEX           62L
#define MODEL2_REFERENCEY           150L
#define MODEL2_DRAW_COLOR           M_COLOR_BLUE

/* Models array specifications. */
#define MODELS_ARRAY_SIZE      3L
#define MODELS_OFFSETX         {MODEL0_OFFSETX, MODEL1_OFFSETX, MODEL2_OFFSETX}
#define MODELS_OFFSETY         {MODEL0_OFFSETY, MODEL1_OFFSETY, MODEL2_OFFSETY}
#define MODELS_SIZEX           {MODEL0_SIZEX, MODEL1_SIZEX, MODEL2_SIZEX}
#define MODELS_SIZEY           {MODEL0_SIZEY, MODEL1_SIZEY, MODEL2_SIZEY}
#define MODELS_DRAW_COLOR      {MODEL0_DRAW_COLOR, MODEL1_DRAW_COLOR, MODEL2_DRAW_COLOR}


void MultipleModelsExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
MIL_ID     MilImage,                                         /* Image buffer identifier. */
           GraphicList;                                      /* Graphic list identifier. */
MIL_ID     MilSearchContext,                                 /* Search context           */
           MilResult;                                        /* Result identifier.       */
MIL_INT    Models[MODELS_MAX_OCCURRENCES],                   /* Model indices.           */
           ModelsOffsetX[MODELS_ARRAY_SIZE] = MODELS_OFFSETX,/* Model X offsets array.   */
           ModelsOffsetY[MODELS_ARRAY_SIZE] = MODELS_OFFSETY,/* Model Y offsets array.   */
           ModelsSizeX[MODELS_ARRAY_SIZE]   = MODELS_SIZEX,  /* Model X sizes array.     */
           ModelsSizeY[MODELS_ARRAY_SIZE]   = MODELS_SIZEY;  /* Model Y sizes array.     */
MIL_DOUBLE ModelsDrawColor[MODELS_ARRAY_SIZE]=MODELS_DRAW_COLOR; /* Model drawing colors.*/
MIL_INT    NumResults  = 0L;                                 /* Number of results found. */
MIL_DOUBLE Score[MODELS_MAX_OCCURRENCES],                    /* Model correlation scores.*/
           XPosition[MODELS_MAX_OCCURRENCES],                /* Model X positions.       */
           YPosition[MODELS_MAX_OCCURRENCES],                /* Model Y positions.       */
           Angle[MODELS_MAX_OCCURRENCES],                    /* Model occurrence angles. */
           Scale[MODELS_MAX_OCCURRENCES],                    /* Model occurrence scales. */
           Time = 0.0;                                       /* Time variable.           */
int        i;                                                /* Loop variable            */


   /* Restore the model image and display it. */
   MbufRestore(MULTI_MODELS_IMAGE, MilSystem, &MilImage);
   MdispSelect(MilDisplay, MilImage);

   /* Allocate a graphic list to hold the subpixel annotations to draw. */
   MgraAllocList(MilSystem, M_DEFAULT, &GraphicList);

   /* Associate the graphic list to the display for annotations. */
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

   /* Allocate a geometric model finder. */
   MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &MilSearchContext);

   /* Allocate a result buffer. */
   MmodAllocResult(MilSystem, M_DEFAULT, &MilResult);

   /* Define the models. */
   for (i=0; i<NUMBER_OF_MODELS; i++)
      {
      MmodDefine(MilSearchContext, M_IMAGE, MilImage,
                 (MIL_DOUBLE)ModelsOffsetX[i], (MIL_DOUBLE)ModelsOffsetY[i], 
                 (MIL_DOUBLE)ModelsSizeX[i], (MIL_DOUBLE)ModelsSizeY[i]);
      }

   /* Set the desired search speed. */
   MmodControl(MilSearchContext, M_CONTEXT, M_SPEED, MULTI_MODELS_SEARCH_SPEED);

   /* Increase the smoothness for the edge extraction in the search context. */
   MmodControl(MilSearchContext, M_CONTEXT, M_SMOOTHNESS, 75);

   /* Modify the acceptance and the certainty for all the models that were defined. */
   MmodControl(MilSearchContext, M_DEFAULT, M_ACCEPTANCE, 40);
   MmodControl(MilSearchContext, M_DEFAULT, M_CERTAINTY,  60);

   /* Set the number of occurrences to 2 for all the models that were defined. */
   MmodControl(MilSearchContext, M_DEFAULT, M_NUMBER, 2);

   #if (NUMBER_OF_MODELS>1)
   /* Change the reference point of the second model. */
   MmodControl(MilSearchContext, 1, M_REFERENCE_X, MODEL1_REFERENCEX);
   MmodControl(MilSearchContext, 1, M_REFERENCE_Y, MODEL1_REFERENCEY);

   #if (NUMBER_OF_MODELS>2)
   /* Change the reference point of the third model. */
   MmodControl(MilSearchContext, 2, M_REFERENCE_X, MODEL2_REFERENCEX);
   MmodControl(MilSearchContext, 2, M_REFERENCE_Y, MODEL2_REFERENCEY);
   #endif
   #endif

   /* Preprocess the search context. */
   MmodPreprocess(MilSearchContext, M_DEFAULT);   
   
   /* Draw boxes and positions in the source image to identify the models. */
   for (i=0; i<NUMBER_OF_MODELS; i++)
      {      
      MgraColor(M_DEFAULT, ModelsDrawColor[i]);
      MmodDraw( M_DEFAULT, MilSearchContext, GraphicList,
                M_DRAW_BOX+M_DRAW_POSITION, i, M_ORIGINAL);
      }

   /* Pause to show the models. */
   MosPrintf(MIL_TEXT("A model context was defined with the ")
                  MIL_TEXT("models in the displayed image.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

    /* Clear annotations. */
   MgraClear(M_DEFAULT, GraphicList);


   /* Load the complex target image. */
   MbufLoad(MULTI_MODELS_TARGET_IMAGE, MilImage);  

   /* Dummy first call for bench measure purpose only (bench stabilization, 
    cache effect, etc...). This first call is NOT required by the application. */
   MmodFind(MilSearchContext, MilImage, MilResult);

   /* Reset the timer. */
   MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
   
   /* Find the models. */   
   MmodFind(MilSearchContext, MilImage, MilResult);

   /* Read the find time. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &Time);

   /* Get the number of models found. */
   MmodGetResult(MilResult, M_DEFAULT, M_NUMBER+M_TYPE_MIL_INT, &NumResults);

   /* If the models were found above the acceptance threshold. */
   if( (NumResults >= 1) && (NumResults <= MODELS_MAX_OCCURRENCES) ) 
      {
      /* Get the results for each model. */
      MmodGetResult(MilResult, M_DEFAULT, M_INDEX+M_TYPE_MIL_INT, Models);
      MmodGetResult(MilResult, M_DEFAULT, M_POSITION_X, XPosition);
      MmodGetResult(MilResult, M_DEFAULT, M_POSITION_Y, YPosition);
      MmodGetResult(MilResult, M_DEFAULT, M_ANGLE, Angle);
      MmodGetResult(MilResult, M_DEFAULT, M_SCALE, Scale);
      MmodGetResult(MilResult, M_DEFAULT, M_SCORE, Score);

      /* Print information about the target image. */
      MosPrintf(MIL_TEXT("The models were found in the target "));
      MosPrintf(MIL_TEXT("image although there is:\n   "));
      MosPrintf(MIL_TEXT("Full rotation\n   Small scale change\n   "));
      MosPrintf(MIL_TEXT("Contrast variation\n   Specular reflection\n   "));
      MosPrintf(MIL_TEXT("Occlusion\n   Multiple models\n"));
      MosPrintf(MIL_TEXT("   Multiple occurrences\n\n"));

      /* Print the results for the found models. */
      MosPrintf(MIL_TEXT("Result   Model   X Position   Y Position   ")
                MIL_TEXT("Angle   Scale   Score\n\n"));      
      for (i=0; i<NumResults; i++)         
         {
         MosPrintf(MIL_TEXT("%-9d%-8d%-13.2f%-13.2f%-8.2f%-8.2f%-5.2f%%\n"), 
               i, Models[i], XPosition[i], YPosition[i], 
               Angle[i], Scale[i], Score[i]);                 
         }
      MosPrintf(MIL_TEXT("\nThe search time is %.1f ms\n\n"), Time*1000.0);
      
      /* Draw edges and positions over the occurrences that were found. */
      for (i=0; i < NumResults; i++)
         {
         MgraColor(M_DEFAULT, ModelsDrawColor[Models[i]]);
         MmodDraw(M_DEFAULT, MilResult,GraphicList,
                  M_DRAW_EDGES+M_DRAW_POSITION, i, M_DEFAULT);
         }
      }
   else
      {
      MosPrintf(MIL_TEXT("The models were not found or the number of ")
                             MIL_TEXT("models found is greater than\n"));
      MosPrintf(MIL_TEXT("the defined value of maximum occurrences !\n\n"));
      }

   /* Wait for a key to be pressed. */ 
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free MIL objects. */
   MgraFree(GraphicList);
   MbufFree(MilImage);
   MmodFree(MilSearchContext);
   MmodFree(MilResult);
   }
