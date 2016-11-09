/*********************************************************************************/
/* 
* File name: MModelTracking.cpp
*
* Synopsis:  This program shows how to track a unique object using 
*            pattern recognition. It allocates a model in the field of 
*            view of the camera and finds it in a loop. It also prints 
*            the coordinates of the found model and draws a box around it.
*            It searches using 2 methods, the normalized grayscale 
*            correlation (Mpat), which is very fast and with the Geometric 
*            Model Finder (Mmod), which is independent of the model rotation 
*            and scale but slower.
*
*            Note:  Display update and annotations drawing can require
*                   significant CPU usage.
*/
#include <mil.h>

/* Model specification. */
#define MODEL_WIDTH                   128L
#define MODEL_HEIGHT                  128L
#define MODEL_POS_X_INIT(TargetImage) (MbufInquire(TargetImage, M_SIZE_X, M_NULL)/2)
#define MODEL_POS_Y_INIT(TargetImage) (MbufInquire(TargetImage, M_SIZE_Y, M_NULL)/2)

/* Minimum score to consider the object found (in percent). */
#define MODEL_MIN_MATCH_SCORE       50.0

/* Drawing color */
#define DRAW_COLOR                  0xFF /* White */

/* Example selection. */
#define RUN_PAT_TRACKING_EXAMPLE    M_YES
#define RUN_MOD_TRACKING_EXAMPLE    M_YES 

/* Example functions declarations. */  
void MpatTrackingExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer,
                         MIL_ID MilDisplayImage, MIL_ID MilModelImage);
void MmodTrackingExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, 
                         MIL_ID MilDisplayImage, MIL_ID MilModelImage);
void GetModelImage(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, 
                   MIL_ID MilDisplayImage, MIL_ID MilModelImage);


/*****************************************************************************
Main.
*****************************************************************************/
int MosMain(void)
   {
   MIL_ID MilApplication,   /* Application identifier.   */
          MilSystem,        /* System identifier.        */
          MilDisplay,       /* Display identifier.       */
          MilDigitizer,     /* Digitizer identifier.     */
          MilDisplayImage,  /* Display image identifier. */
          MilModelImage;    /* Model image identifier.   */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, 
                    &MilDigitizer, &MilDisplayImage);

   /* Allocate a model image buffer. */
   MbufAlloc2d(MilSystem, 
               MbufInquire(MilDisplayImage, M_SIZE_X, M_NULL), 
               MbufInquire(MilDisplayImage, M_SIZE_Y, M_NULL), 
               8, M_IMAGE+M_PROC, &MilModelImage);

   MosPrintf(MIL_TEXT("\nMODEL TRACKING:\n"));
   MosPrintf(MIL_TEXT("---------------\n\n"));

   /* Get the model image. */
   GetModelImage(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage);

#if (RUN_PAT_TRACKING_EXAMPLE)
   /* Finds the model using pattern matching. */
   MpatTrackingExample(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage);
#endif

#if (RUN_MOD_TRACKING_EXAMPLE)
   /* Finds the model using geometric model finder. */
   MmodTrackingExample(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage);
#endif

   /* Free allocated buffers. */
   MbufFree(MilModelImage);

   /* Free defaults. */    
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilDisplayImage);

   return 0;
   }


/*****************************************************************************
Get Model Image Function.
*****************************************************************************/

void GetModelImage(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, 
                   MIL_ID MilDisplayImage, MIL_ID MilModelImage)
   {
   MIL_ID     MilOverlayImage;         /* Overlay image.      */
   MIL_DOUBLE DrawColor = DRAW_COLOR;  /* Drawing color.      */

   /* Prepare for overlay annotations. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   /* Draw the position of the model to define in the overlay. */
   MgraColor(M_DEFAULT, DrawColor);
   MgraRect(M_DEFAULT, MilOverlayImage,
            MODEL_POS_X_INIT(MilOverlayImage) - (MODEL_WIDTH/2),
            MODEL_POS_Y_INIT(MilOverlayImage) - (MODEL_HEIGHT/2),
            MODEL_POS_X_INIT(MilOverlayImage) + (MODEL_WIDTH/2), 
            MODEL_POS_Y_INIT(MilOverlayImage) + (MODEL_HEIGHT/2));

   /* Grab continuously. */
   MosPrintf(MIL_TEXT("Model definition:\n\n"));
   MosPrintf(MIL_TEXT("Place a unique model to find in the marked rectangle.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));

   /* Grab a reference model image. */
   MdigGrabContinuous(MilDigitizer, MilDisplayImage);
   MosGetch();
   MdigHalt(MilDigitizer);

   /* Copy the grabbed image to the Model image to keep it. */
   MbufCopy(MilDisplayImage, MilModelImage);

   /* Clear and disable the overlay. */
   MdispControl(MilDisplay, M_OVERLAY, M_DISABLE);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   }


/*****************************************************************************
Tracking object with pattern matching module.
*****************************************************************************/

void MpatTrackingExample(MIL_ID MilSystem,    MIL_ID MilDisplay, 
                         MIL_ID MilDigitizer, MIL_ID MilDisplayImage, MIL_ID MilModelImage)
   {
   MIL_ID      MilImage[2],             /* Processing image buffer identifiers. */
               Model,                   /* Model identifier.                    */
               Result;                  /* Result identifier.                   */
   MIL_DOUBLE  DrawColor = DRAW_COLOR;  /* Model drawing color.                 */
   MIL_INT     Found;                   /* Number of found models.              */
   MIL_INT     NbFindDone = 0;          /* Number of loops to find model done.  */
   MIL_DOUBLE  OrgX = 0.0, OrgY = 0.0;    /* Original center of model.            */
   MIL_DOUBLE  x, y, Score = 0.0;         /* Result variables.                    */
   MIL_DOUBLE  Time = 0.0;                /* Timer.                               */ 

   /* Print a start message. */
   MosPrintf(MIL_TEXT("\nGRAYSCALE PATTERN MATCHING:\n"));
   MosPrintf(MIL_TEXT("---------------------------\n\n"));

   /* Display the model image. */
   MbufCopy(MilModelImage, MilDisplayImage);

   /* Allocate normalized grayscale type model. */
   MpatAllocModel(MilSystem, MilModelImage,
                  MODEL_POS_X_INIT(MilModelImage) - (MODEL_WIDTH/2),
                  MODEL_POS_Y_INIT(MilModelImage) - (MODEL_HEIGHT/2),
                  MODEL_WIDTH, MODEL_HEIGHT, M_NORMALIZED, &Model);

   /* Allocate result. */
   MpatAllocResult(MilSystem, 1L, &Result);

   /* Draw box around the model. */
   MgraColor(M_DEFAULT, DrawColor);
   MpatDraw(M_DEFAULT, Model, MilDisplayImage, M_DRAW_BOX, M_DEFAULT, M_ORIGINAL);

   /* Set minimum acceptance for search. */
   MpatSetAcceptance(Model,MODEL_MIN_MATCH_SCORE);

   /* Set speed. */
   MpatSetSpeed(Model,M_HIGH);

   /* Set accuracy. */
   MpatSetAccuracy(Model,M_LOW);

   /* Preprocess model. */
   MpatPreprocModel(MilModelImage, Model, M_DEFAULT);

   /* Inquire about center of model. */
   MpatInquire(Model, M_ORIGINAL_X, &OrgX);
   MpatInquire(Model, M_ORIGINAL_Y, &OrgY);

   /* Print the original position. */
   MosPrintf(MIL_TEXT("A Grayscale Model was defined.\n"));
   MosPrintf(MIL_TEXT("Model dimensions:  %ld x %ld.\n"), MODEL_WIDTH, MODEL_HEIGHT);
   MosPrintf(MIL_TEXT("Model center:      X=%.2f, Y=%.2f.\n"), OrgX, OrgY);
   MosPrintf(MIL_TEXT("Model is scale and rotation dependant.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Allocate 2 grab buffers. */
   MbufAlloc2d(MilSystem, 
               MbufInquire(MilModelImage, M_SIZE_X, M_NULL), 
               MbufInquire(MilModelImage, M_SIZE_Y, M_NULL), 
               8, 
               M_IMAGE+M_GRAB+M_PROC,
               &MilImage[0]);
   MbufAlloc2d(MilSystem, 
               MbufInquire(MilModelImage, M_SIZE_X, M_NULL), 
               MbufInquire(MilModelImage, M_SIZE_Y, M_NULL), 
               8, 
               M_IMAGE+M_GRAB+M_PROC,
               &MilImage[1]);

   /* Grab continuously and perform the find operation using double buffering. */
   MosPrintf(MIL_TEXT("\nContinuously finding the Grayscale model.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));

   /* Grab a first target image into first buffer (done twice for timer reset accuracy). */
   MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);
   MdigGrab(MilDigitizer, MilImage[NbFindDone%2]);
   MdigGrab(MilDigitizer, MilImage[NbFindDone%2]);
   MappTimer(M_DEFAULT, M_TIMER_RESET, &Time);

   /* Loop, processing one buffer while grabbing the other. */
   do
      {
      /* Grab a target image into the other buffer. */
      MdigGrab(MilDigitizer, MilImage[(NbFindDone+1)%2]);

      /* Read the time. */
      MappTimer(M_DEFAULT, M_TIMER_READ, &Time);

      /* Find model. */
      MpatFindModel(MilImage[NbFindDone%2], Model, Result);

      /* Get results. */
      MpatGetNumber(Result, &Found);
      MpatGetResult(Result, M_POSITION_X, &x);
      MpatGetResult(Result, M_POSITION_Y, &y);
      MpatGetResult(Result, M_SCORE, &Score);

      /* Print a message based upon the score. */
      if (Found)
         {
         /* Draw a box around the model and print the results. */
         MpatDraw(M_DEFAULT, Result, MilImage[NbFindDone%2], 
                  M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_DEFAULT);
         MosPrintf(MIL_TEXT("Found: X=%7.2f, Y=%7.2f, Score=%5.1f%% (%.1f fps).    \r"),
                   x, y, Score, (NbFindDone+1)/Time);
         }
      else
         {
         /* Print the "not found" message. */
         MosPrintf(MIL_TEXT("Not found ! (score<%5.1f%%)                (%.1f fps).     \r"), 
                   MODEL_MIN_MATCH_SCORE, (NbFindDone+1)/Time);
         }

      /* Copy target image to the display. */              
      MbufCopy(MilImage[NbFindDone%2], MilDisplayImage);

      /* Increment find count */
      NbFindDone++;
      } 
   while (!MosKbhit());

   MosGetch();
   MosPrintf(MIL_TEXT("\n\n"));
   
   /* Wait for end of last grab. */
   MdigGrabWait(MilDigitizer, M_GRAB_END);

   /* Free all allocated objects. */
   MpatFree(Result);
   MpatFree(Model);
   MbufFree(MilImage[1]);
   MbufFree(MilImage[0]);   
   }


/*****************************************************************************
Tracking object with Geometric Model Finder module
*****************************************************************************/

#define MODEL_MAX_OCCURRENCES       16L

void MmodTrackingExample(MIL_ID MilSystem,    MIL_ID MilDisplay, 
                         MIL_ID MilDigitizer, MIL_ID MilDisplayImage, MIL_ID MilModelImage)
   {
   MIL_ID     MilImage[2],                  /* Processing image buffer identifiers. */
              SearchContext,                /* Search context identifier.           */
              Result;                       /* Result identifier.                   */
   MIL_DOUBLE DrawColor = DRAW_COLOR;       /* Model drawing color.                 */
   MIL_INT    Found;                        /* Number of models found.              */
   MIL_INT    NbFindDone=0;                 /* Number of loops to find model done.  */
   MIL_DOUBLE OrgX = 0.0, OrgY = 0.0;       /* Original center of model.            */
   MIL_DOUBLE Score[MODEL_MAX_OCCURRENCES], /* Model correlation score.             */
              x[MODEL_MAX_OCCURRENCES],     /* Model X position.                    */
              y[MODEL_MAX_OCCURRENCES],     /* Model Y position.                    */
              Angle[MODEL_MAX_OCCURRENCES], /* Model occurrence angle.              */
              Scale[MODEL_MAX_OCCURRENCES]; /* Model occurrence scale.              */
   MIL_DOUBLE Time = 0.0;                   /* Timer.                               */ 

   /* Print a start message. */
   MosPrintf(MIL_TEXT("\nGEOMETRIC MODEL FINDER (scale and rotation independent):\n"));
   MosPrintf(MIL_TEXT("--------------------------------------------------------\n\n"));

   /* Display model image. */
   MbufCopy(MilModelImage, MilDisplayImage);

   /* Allocate a context and define a geometric model. */
   MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &SearchContext);
   MmodDefine(SearchContext, M_IMAGE, MilModelImage, 
              (MIL_DOUBLE)MODEL_POS_X_INIT(MilModelImage) - (MODEL_WIDTH/2),
              (MIL_DOUBLE)MODEL_POS_Y_INIT(MilModelImage) - (MODEL_HEIGHT/2),
              MODEL_WIDTH, MODEL_HEIGHT);

   /* Allocate result. */
   MmodAllocResult(MilSystem, M_DEFAULT, &Result);

   /* Draw a box around the model. */
   MgraColor(M_DEFAULT, DrawColor);
   MmodDraw(M_DEFAULT, SearchContext, MilDisplayImage, M_DRAW_BOX, M_DEFAULT, M_ORIGINAL);

   /* Set speed to VERY HIGH for fast but less precise search. */
   MmodControl(SearchContext, M_CONTEXT, M_SPEED, M_VERY_HIGH);

   /* Set minimum acceptance for the search. */
   MmodControl(SearchContext, M_DEFAULT, M_ACCEPTANCE, MODEL_MIN_MATCH_SCORE);

   /* Preprocess model. */
   MmodPreprocess(SearchContext, M_DEFAULT);

   /* Inquire about center of model. */
   MmodInquire(SearchContext, M_DEFAULT, M_ORIGINAL_X, &OrgX);
   MmodInquire(SearchContext, M_DEFAULT, M_ORIGINAL_Y, &OrgY);

   /* Print the original position. */
   MosPrintf(MIL_TEXT("The Geometric target model was defined.\n"));
   MosPrintf(MIL_TEXT("Model dimensions: %ld x %ld.\n"), MODEL_WIDTH, MODEL_HEIGHT);
   MosPrintf(MIL_TEXT("Model center:     X=%.2f, Y=%.2f.\n"), OrgX, OrgY);
   MosPrintf(MIL_TEXT("Model is scale and rotation independent.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Allocate 2 grab buffers. */
   MbufAlloc2d(MilSystem, 
               MbufInquire(MilModelImage, M_SIZE_X, M_NULL), 
               MbufInquire(MilModelImage, M_SIZE_Y, M_NULL), 
               8, 
               M_IMAGE+M_GRAB+M_PROC,
               &MilImage[0]);
   MbufAlloc2d(MilSystem, 
               MbufInquire(MilModelImage, M_SIZE_X, M_NULL), 
               MbufInquire(MilModelImage, M_SIZE_Y, M_NULL), 
               8, 
               M_IMAGE+M_GRAB+M_PROC,
               &MilImage[1]);

   /* Grab continuously grab and perform the find operation using double buffering. */
   MosPrintf(MIL_TEXT("\nContinuously finding the Geometric Model.\n"));
   MosPrintf(MIL_TEXT("Press a <Enter> to stop.\n\n"));

   /* Grab a first target image into first buffer (done twice for timer reset accuracy). */
   MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);
   MdigGrab(MilDigitizer, MilImage[NbFindDone%2]);
   MdigGrab(MilDigitizer, MilImage[NbFindDone%2]);
   MappTimer(M_DEFAULT, M_TIMER_RESET, &Time);

   /* Loop, processing one buffer while grabbing the other. */
   do
      {
      /* Grab a target image into the other buffer. */
      MdigGrab(MilDigitizer, MilImage[(NbFindDone+1)%2]);

      /* Read the time. */
      MappTimer(M_DEFAULT, M_TIMER_READ, &Time);

      /* Find model. */
      MmodFind(SearchContext, MilImage[NbFindDone%2], Result);

      /* Get the number of occurrences found. */
      MmodGetResult(Result, M_DEFAULT, M_NUMBER+M_TYPE_MIL_INT, &Found);

      /* Print a message based on the score. */
      if ( (Found >= 1) && (Found < MODEL_MAX_OCCURRENCES) )
         {
         /* Get results. */
         MmodGetResult(Result, M_DEFAULT, M_POSITION_X, x);
         MmodGetResult(Result, M_DEFAULT, M_POSITION_Y, y);
         MmodGetResult(Result, M_DEFAULT, M_SCALE, Scale);
         MmodGetResult(Result, M_DEFAULT, M_ANGLE, Angle);
         MmodGetResult(Result, M_DEFAULT, M_SCORE, Score);

         /* Draw a box and a cross where the model was found and print the results. */
         MmodDraw(M_DEFAULT, Result, MilImage[NbFindDone%2], 
                  M_DRAW_BOX+M_DRAW_POSITION+M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);
         MosPrintf(MIL_TEXT("Found: X=%6.1f, Y=%6.1f, Angle=%6.1f, Scale=%5.2f, ")
                   MIL_TEXT("Score=%5.1f%% (%5.1f fps).\r"),
                   x[0], y[0], Angle[0], Scale[0], Score[0], (NbFindDone+1)/Time);
         }
      else
         {
         /* Print the "not found" message. */
         MosPrintf(MIL_TEXT("Not found! (score<%5.1f%%)                          ")
                   MIL_TEXT("(%5.1f fps).\r"),MODEL_MIN_MATCH_SCORE, (NbFindDone+1)/Time);
         }

      /* Copy target image to the display. */
      MbufCopy(MilImage[NbFindDone%2], MilDisplayImage);

      /* Increment the counter. */
      NbFindDone++;         
      } 
   while (!MosKbhit());

   MosGetch();
   MosPrintf(MIL_TEXT("\n\n"));

   /* Wait for the end of last grab. */
   MdigGrabWait(MilDigitizer, M_GRAB_END);

   /* Free all allocations. */
   MmodFree(Result);
   MmodFree(SearchContext);
   MbufFree(MilImage[1]);
   MbufFree(MilImage[0]);   
   }
