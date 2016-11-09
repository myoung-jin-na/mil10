/*****************************************************************************/
/* 
 * File name: MdigAutoFocus.cpp
 *
 * Synopsis:  This program performs an autofocus operation using the 
 *            MdigFocus() function. Since the way to move a motorized
 *            camera lens is device-specific, we will not include real
 *            lens movement control and image grab but will simulate 
 *            the lens focus with a smooth operation. 
 *
 *     Note : Under MIL-Lite, the out of focus lens simulation is not supported.
 */
#include <mil.h>

/* Source MIL image file specification. */ 
#define IMAGE_FILE                     M_IMAGE_PATH MIL_TEXT("BaboonMono.mim")

/* Lens mechanical characteristics. */
#define FOCUS_MAX_NB_POSITIONS         100
#define FOCUS_MIN_POSITION             0
#define FOCUS_MAX_POSITION             (FOCUS_MAX_NB_POSITIONS - 1)
#define FOCUS_START_POSITION           10

/* Autofocus search properties. */
#define FOCUS_MAX_POSITION_VARIATION   M_DEFAULT
#define FOCUS_MODE                     M_SMART_SCAN
#define FOCUS_SENSITIVITY              1

/* User Data structure definition. */
typedef struct 
{  MIL_ID      SourceImage;
   MIL_ID      FocusImage;
   MIL_ID      Display;
   long        Iteration;
}  DigHookUserData;

/* Autofocus callback function responsible for moving the lens. */
MIL_INT MFTYPE MoveLensHookFunction(MIL_INT HookType,
                                    MIL_INT Position,
                                    void*   UserDataHookPtr);

/* Simulate a grab from a camera at different lens positions. */
void SimulateGrabFromCamera(MIL_ID SourceImage, 
                            MIL_ID FocusImage, 
                            MIL_INT Iteration, 
                            MIL_ID AnnotationDisplay);

/* Draw position of the lens. */
void DrawCursor(MIL_ID AnnotationImage, MIL_INT Position);

/*****************************************************************************/
/*  Main application function.                                               */
int MosMain(void)
{
   MIL_ID  MilApplication,                  /* Application identifier.       */
           MilSystem,                       /* System identifier.            */
           MilDisplay,                      /* Display identifier.           */
           MilSource,                       /* Source image                  */
           MilCameraFocus;                  /* Focus simulated image         */
   MIL_INT FocusPos;                        /* Best focus position           */
   DigHookUserData UserData;                /* User data passed to the hook  */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Load the source image. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilSource);
   MbufRestore(IMAGE_FILE, MilSystem, &MilCameraFocus);
   MbufClear(MilCameraFocus, 0);

   /* Select image on the display. */ 
   MdispSelect(MilDisplay, MilCameraFocus);

   /* Simulate the first image grab. */
   SimulateGrabFromCamera(MilSource, MilCameraFocus, FOCUS_START_POSITION, MilDisplay);

   /* Initialize user data needed within the hook function. */
   UserData.SourceImage = MilSource;
   UserData.FocusImage  = MilCameraFocus;
   UserData.Iteration   = 0L;
   UserData.Display     = MilDisplay;

   /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("\nAUTOFOCUS:\n"));
   MosPrintf(MIL_TEXT("----------\n\n"));
   MosPrintf(MIL_TEXT("Automatic focusing operation will be done on this image.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));  
   MosGetch();
   MosPrintf(MIL_TEXT("Autofocusing...\n\n"));  
  
   /* Perform Autofocus. 
      Since lens movement is hardware specific, no digitizer is used here.
      We simulate the lens movement with by smoothing the image data in 
      the hook function instead.
   */ 
   MdigFocus(M_NULL,
             MilCameraFocus,
             M_DEFAULT,
             MoveLensHookFunction,
             &UserData,
             FOCUS_MIN_POSITION,
             FOCUS_START_POSITION,
             FOCUS_MAX_POSITION,
             FOCUS_MAX_POSITION_VARIATION,
             FOCUS_MODE + FOCUS_SENSITIVITY,
             &FocusPos); 

   /* Print the best focus position and number of iterations. */
   MosPrintf(MIL_TEXT("The best focus position is %d.\n"), FocusPos);
   MosPrintf(MIL_TEXT("The best focus position found in %d iterations.\n\n"), 
                                                         UserData.Iteration);
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));  
   MosGetch();

  /* Free all allocations. */
   MbufFree(MilSource);
   MbufFree(MilCameraFocus);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}

/********************************************************************************/
/* Autofocus hook function responsible to move the lens.                        */

MIL_INT MFTYPE MoveLensHookFunction(MIL_INT HookType,
                                    MIL_INT Position,
                                    void*   UserDataHookPtr)
   {
   DigHookUserData *UserData = (DigHookUserData *)UserDataHookPtr;

   /* Here, the lens position must be changed according to the Position parameter.
      In that case, we simulate the lens position change followed by a grab.
   */
   if(HookType == M_CHANGE || HookType == M_ON_FOCUS)
      {
      SimulateGrabFromCamera( UserData->SourceImage, 
                              UserData->FocusImage, 
                              Position,
                              UserData->Display 
                              );
      UserData->Iteration++;
      }

   return 0;
   }

/***********************************************************************************/
/* Utility function to simulate a grab from a camera at different lens position    */
/* by smoothing the original image. It should be replaced with a true camera grab. */
/*                                                                                 */
/* Note that this lens simulation will not work under MIL-lite because it uses     */
/* MimConvolve().                                                                  */

/* Lens simulation characteristics. */
#define FOCUS_BEST_POSITION            (FOCUS_MAX_NB_POSITIONS/2)

void SimulateGrabFromCamera(MIL_ID SourceImage, MIL_ID FocusImage, 
                            MIL_INT Iteration, MIL_ID AnnotationDisplay)
{
   MIL_INT NbSmoothNeeded;   /* Number of smooths needed          */
   MIL_INT BufType;          /* Buffer type                       */
   MIL_INT BufSizeX;         /* Buffer size X                     */
   MIL_INT BufSizeY;         /* Buffer size Y                     */
   MIL_INT Smooth;           /* Smooth index                      */
   MIL_ID TempBuffer;        /* Temporary buffer                  */
   MIL_ID SourceOwnerSystem; /* Owner system of the source buffer */

   /* Throw an error under MIL-lite since lens simulation cannot be used. */
   #if (M_MIL_LITE)
      #error "Replace the SimulateGrabFromCamera()function with a true image grab."
   #endif

   /* Compute number of smooths needed to simulate focus. */
   NbSmoothNeeded = MosAbs(Iteration - FOCUS_BEST_POSITION);

   /* Buffer inquires. */
   BufType        = MbufInquire(FocusImage, M_TYPE,   M_NULL);
   BufSizeX       = MbufInquire(FocusImage, M_SIZE_X, M_NULL);
   BufSizeY       = MbufInquire(FocusImage, M_SIZE_Y, M_NULL);

   if(NbSmoothNeeded == 0)
      {
      /* Directly copy image source to destination. */
      MbufCopy(SourceImage, FocusImage);
      }
   else if(NbSmoothNeeded == 1)
      {
      /* Directly convolve image from source to destination. */
      MimConvolve(SourceImage, FocusImage, M_SMOOTH);
      }
   else 
      {
      SourceOwnerSystem = (MIL_ID)MbufInquire(SourceImage, M_OWNER_SYSTEM, M_NULL);

      /* Allocate temporary buffer. */   
      MbufAlloc2d(SourceOwnerSystem, BufSizeX, BufSizeY, 
                  BufType, M_IMAGE+M_PROC, &TempBuffer);
   
      /* Perform first smooth. */
      MimConvolve(SourceImage, TempBuffer, M_SMOOTH);
         
      /* Perform smooths. */
      for(Smooth=1;Smooth<NbSmoothNeeded-1;Smooth++)
         {
         MimConvolve(TempBuffer, TempBuffer, M_SMOOTH);
         }

      /* Perform last smooth. */
      MimConvolve(TempBuffer, FocusImage, M_SMOOTH);

      /* Free temporary buffer. */
      MbufFree(TempBuffer);
      }

   /* Draw position cursor. */
   DrawCursor(AnnotationDisplay, Iteration);
}

/****************************************************************/
/* Draw position of the focus lens.                             */

/* Cursor specifications. */
#define CURSOR_POSITION                ((BufSizeY*7)/8)
#define CURSOR_SIZE                    14
#define CURSOR_COLOR                   M_COLOR_GREEN

void DrawCursor(MIL_ID AnnotationDisplay, MIL_INT Position)
{
   MIL_ID AnnotationImage;
   MIL_INT BufSizeX, BufSizeY, n;
   MIL_DOUBLE CursorColor;

   /* Prepare for overlay annotations. */
   MdispControl(AnnotationDisplay, M_OVERLAY, M_ENABLE);
   MdispControl(AnnotationDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MdispInquire(AnnotationDisplay, M_OVERLAY_ID, &AnnotationImage);
   MbufInquire(AnnotationImage, M_SIZE_X, &BufSizeX);
   MbufInquire(AnnotationImage, M_SIZE_Y, &BufSizeY);
   CursorColor = CURSOR_COLOR;
   MgraColor(M_DEFAULT, CursorColor);

   /* Write annotations. */
   n = (BufSizeX/FOCUS_MAX_NB_POSITIONS);
   MgraLine(M_DEFAULT, AnnotationImage, 0, CURSOR_POSITION+CURSOR_SIZE, 
                                        BufSizeX-1, CURSOR_POSITION+CURSOR_SIZE);
   MgraLine(M_DEFAULT, AnnotationImage, Position*n, CURSOR_POSITION+CURSOR_SIZE,
                                        Position*n-CURSOR_SIZE, CURSOR_POSITION);
   MgraLine(M_DEFAULT, AnnotationImage, Position*n, CURSOR_POSITION+CURSOR_SIZE, 
                                        Position*n+CURSOR_SIZE, CURSOR_POSITION);
   MgraLine(M_DEFAULT, AnnotationImage, Position*n-CURSOR_SIZE, CURSOR_POSITION, 
                                        Position*n+CURSOR_SIZE, CURSOR_POSITION);
}
