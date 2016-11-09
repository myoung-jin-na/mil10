/*****************************************************************************/
/*
 * File name: MImWarp.cpp 
 *
 * Synopsis:  : This program performs three types of warp transformations. 
 *              First the image is stretched according to four specified 
 *              reference points. Then it is warped on a sinusoid, and 
 *              finally, the program loops while warping the image on a 
 *              sphere.
 */
#include <mil.h>
#include <math.h>
#include <malloc.h>

/* Target image specifications. */
#define IMAGE_FILE               M_IMAGE_PATH MIL_TEXT("BaboonMono.mim")
#define INTERPOLATION_MODE       M_NEAREST_NEIGHBOR
#if(INTERPOLATION_MODE == M_NEAREST_NEIGHBOR)
   #define FIXED_POINT_PRECISION    M_FIXED_POINT+0L
   #define FLOAT_TO_FIXED_POINT(x)  (1L * (x))
#else
   #define FIXED_POINT_PRECISION    M_FIXED_POINT+6L
   #define FLOAT_TO_FIXED_POINT(x)  (64L * (x))
#endif
#define ROTATION_STEP            2

/* Utility functions. */
void YieldToGUI();
void GenerateSphericLUT(MIL_INT ImageWidth, MIL_INT ImageHeight, 
                        short *MilLutXPtr, short *MilLutYPtr);


/* Main function. */
/* -------------- */
int MosMain(void)
{
   MIL_ID   MilApplication,  /* Application identifier.         */
            MilSystem,       /* System identifier.              */
            MilDisplay,      /* Display identifier.             */
            MilDisplayImage, /* Image buffer identifier.        */
            MilSourceImage,  /* Image buffer identifier.        */
            Mil4CornerArray, /* Coefficients buffer identifier. */
            MilLutX,         /* Lut buffer identifier.          */
            MilLutY,         /* Lut buffer identifier.          */
            ChildWindow;     /* Child Image identifier.         */
   float    FourCornerMatrix[12] = {
            0.0,             /* X coordinate of quadrilateral's 1st corner */
            0.0,             /* Y coordinate of quadrilateral's 1st corner */
            456.0,           /* X coordinate of quadrilateral's 2nd corner */
            62.0,            /* Y coordinate of quadrilateral's 2nd corner */
            333.0,           /* X coordinate of quadrilateral's 3rd corner */
            333.0,           /* Y coordinate of quadrilateral's 3rd corner */
            100.0,           /* X coordinate of quadrilateral's 4th corner */
            500.0,           /* Y coordinate of quadrilateral's 4th corner */
            0.0,           /* X coordinate of rectangle's top-left corner */
            0.0,           /* Y coordinate of rectangle's top-left corner */
            511.0,           /* X coordinate of rectangle's bottom-right corner */
            511.0 };         /* Y coordinate of rectangle's bottom-right corner */
   MIL_INT  Precision     = FIXED_POINT_PRECISION;
   MIL_INT  Interpolation = INTERPOLATION_MODE;
   short    *MilLutXPtr,  *MilLutYPtr;
   MIL_INT  OffsetX       = 0;
   MIL_INT  ImageWidth, ImageHeight, ImageType, i, j;
   MIL_DOUBLE   FramesPerSecond = 0, Time = 0, NbLoop = 0;

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Restore the source image. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilSourceImage);

   /* Allocate a display buffers and show the source image. */
   MbufAlloc2d(MilSystem, MbufInquire(MilSourceImage, M_SIZE_X, &ImageWidth),
                          MbufInquire(MilSourceImage, M_SIZE_Y, &ImageHeight),
                          MbufInquire(MilSourceImage, M_TYPE,   &ImageType),
                          M_IMAGE+M_PROC+M_DISP, &MilDisplayImage);
   MbufCopy(MilSourceImage, MilDisplayImage);
   MdispSelect(MilDisplay, MilDisplayImage);

   /* Print a message. */
   MosPrintf(MIL_TEXT("\nWARPING:\n"));
   MosPrintf(MIL_TEXT("--------\n\n"));
   MosPrintf(MIL_TEXT("This image will be warped using different methods.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   
   /* Four-corner LUT warping */
   /*-------------------------*/
   
   /* Allocate 2 LUT buffers. */
   MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, 16L+M_SIGNED, M_LUT, &MilLutX);
   MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, 16L+M_SIGNED, M_LUT, &MilLutY);

   /* Allocate the coefficient buffer. */
   MbufAlloc2d(MilSystem, 12L, 1L, 32L+M_FLOAT, M_ARRAY, &Mil4CornerArray);

   /* Put warp values into the coefficient buffer. */
   MbufPut1d(Mil4CornerArray, 0L, 12L, FourCornerMatrix);

   /* Generate LUT buffers. */
   MgenWarpParameter(Mil4CornerArray, MilLutX,   MilLutY,  
                     M_WARP_4_CORNER+Precision, M_DEFAULT, 
                     0.0, 0.0);

   /* Clear the destination. */
   MbufClear(MilDisplayImage,0);

   /* Warp the image. */
   MimWarp(MilSourceImage, MilDisplayImage, MilLutX, MilLutY, M_WARP_LUT+Precision,
           Interpolation);

   /* Print a message. */
   MosPrintf(MIL_TEXT("The image was warped from an arbitrary"));
   MosPrintf(MIL_TEXT(" quadrilateral to a square.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   
   /* Sinusoidal LUT warping */
   /*------------------------*/

   /* Allocate user-defined LUTs. */
   MilLutXPtr = (short*)malloc(sizeof(short)*ImageHeight*ImageWidth);
   MilLutYPtr = (short*)malloc(sizeof(short)*ImageHeight*ImageWidth);
  
   /* Fill the LUT with a sinusoidal waveforms with a 6-bit precision.*/
   for (j=0;j<ImageHeight;j++)
      {
      for (i=0;i<ImageWidth;i++)
         {
         MilLutYPtr[i+ (j*ImageWidth)] = 
            (short)FLOAT_TO_FIXED_POINT(((j) + (int)((20*sin(0.03*i)))));
         MilLutXPtr[i+ (j*ImageWidth)] = 
            (short)FLOAT_TO_FIXED_POINT(((i) + (int)((20*sin(0.03*j))))); 
         }
      }
   
   /* Put the values into the LUT buffers.*/
   MbufPut2d(MilLutX, 0L, 0L, ImageWidth, ImageHeight, MilLutXPtr);
   MbufPut2d(MilLutY, 0L, 0L, ImageWidth, ImageHeight, MilLutYPtr);

   /* Clear the destination. */
   MbufClear(MilDisplayImage,0);

   /* Warp the image. */
   MimWarp(MilSourceImage,MilDisplayImage,MilLutX,MilLutY,M_WARP_LUT +Precision,
           Interpolation);
   
   /* wait for a key */
   MosPrintf(MIL_TEXT("The image was warped on two sinusoidal waveforms.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Continuous spherical LUT warping */
   /*--------------------------------*/

   /* Allocate temporary buffer. */
   MbufFree(MilSourceImage);
   MbufAlloc2d(MilSystem, ImageWidth*2, ImageHeight, ImageType, 
                                        M_IMAGE+M_PROC, &MilSourceImage);
                            
   /* Reload the image. */
   MbufLoad(IMAGE_FILE, MilSourceImage);
   
   /* Fill the LUTs with a sphere pattern with a 6-bit precision.*/
   GenerateSphericLUT(ImageWidth, ImageHeight, MilLutXPtr,MilLutYPtr);
   MbufPut2d(MilLutX, 0L, 0L, ImageWidth, ImageHeight, MilLutXPtr);
   MbufPut2d(MilLutY, 0L, 0L, ImageWidth, ImageHeight, MilLutYPtr);

   /* Duplicate the buffer to allow wrap around in the warping. */
   MbufCopy(MilSourceImage, MilDisplayImage);
   MbufChild2d(MilSourceImage, ImageWidth, 0, ImageWidth, ImageHeight, &ChildWindow);
   MbufCopy(MilDisplayImage, ChildWindow);
   MbufFree(ChildWindow);
  
   /* Clear the destination. */
   MbufClear(MilDisplayImage,0);

   /* Print a message and start the timer. */
   MosPrintf(MIL_TEXT("The image is continuously warped on a sphere.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));
   MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);

   /* Create a child in the buffer containing the two images. */
   MbufChild2d(MilSourceImage, OffsetX, 0, ImageWidth, ImageHeight, &ChildWindow);

   /* Warp the image continuously. */
   while (!MosKbhit())
      {
      /* Move the child to the new position */
      MbufChildMove(ChildWindow, OffsetX, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
      
      /* Warp the child in the window. */
      MimWarp(ChildWindow, MilDisplayImage, MilLutX, MilLutY, 
                           M_WARP_LUT+Precision, Interpolation);

      /* Update the offset (shift the window to the right). */
      OffsetX += ROTATION_STEP;
      
      /* Reset the offset if the child is outside the buffer. */
      if (OffsetX>ImageWidth-1)
         OffsetX = 0;

      NbLoop++;

      /* Calculate and print the number of frames per second processed. */
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &Time);
      FramesPerSecond = NbLoop/Time;
      MosPrintf(MIL_TEXT("Processing speed: %.0f Images/Sec.\r"), FramesPerSecond);
      YieldToGUI();
      }
   MosGetch();
   MosPrintf(MIL_TEXT("\n"));

   /* Free objects. */
   free(MilLutXPtr);
   free(MilLutYPtr);

   MbufFree(ChildWindow);
   MbufFree(MilLutX);
   MbufFree(MilLutY);
   MbufFree(Mil4CornerArray);
   MbufFree(MilSourceImage);
   MbufFree(MilDisplayImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}


/* Generate two custom LUTs used to map the image on a sphere. */
/* ----------------------------------------------------------- */
void GenerateSphericLUT(MIL_INT ImageWidth, MIL_INT ImageHeight, 
                        short *MilLutXPtr, short *MilLutYPtr)
{
   MIL_INT      i, j, k;
   MIL_DOUBLE       utmp, vtmp, tmp;
   short        v;
     
   /* Set the radius of the sphere */
   MIL_DOUBLE       Radius = 200.0;         

   /* Generate the X and Y buffers */
   for (j=0; j < ImageHeight; j++ )
      {
      k = j*ImageWidth;

      /* Check that still in the sphere (in the Y axis). */
      if (fabs( vtmp = ((MIL_DOUBLE)(j - (ImageHeight/2)) / Radius) ) < 1.0)
      {
      /* We scan from top to bottom, so reverse the value obtained above
         and obtain the angle. */
      vtmp = acos( -vtmp );
      if(vtmp == 0.0)
         vtmp=0.0000001;

   
      /* Compute the position to fetch in the source. */
      v = (short)((vtmp/3.1415926) * (MIL_DOUBLE)(ImageHeight - 1) + 0.5);

      /* Compute the Y coordinate of the sphere. */
      tmp = Radius*sin(vtmp);

      for (i=0; i < ImageWidth; i++ )
         {
         /* Check that still in the sphere. */
         if ( fabs(utmp = ((MIL_DOUBLE)(i - (ImageWidth/2)) / tmp)) < 1.0 )
            {
            utmp = acos( -utmp);
         
            /* Compute the position to fetch (fold the image in four). */ 
            MilLutXPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(((utmp/3.1415926) * 
                     (MIL_DOUBLE)((ImageWidth/2) - 1) + 0.5));
            MilLutYPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(v);
            }  
         else
            {
            /* Default position (fetch outside the buffer to 
               activate the clear overscan). */
            MilLutXPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageWidth);
            MilLutYPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageHeight);
            }  
         }   
      }
        else
      {
      for (i=0; i < ImageWidth ;i++ )
         {
         /* Default position (fetch outside the buffer for clear overscan). */
         MilLutXPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageWidth);
         MilLutYPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageHeight);
         } 
      }
   }  
}  

/* Windows CE GUI Scheduling Adjustment Handling */
/* --------------------------------------------- */
/* NOTE: Under Windows CE, YieldToGUI funtion improves system responsiveness 
 *       in case a normal Windows CE console application thread is processing 
 *       in a while loop.
 */
void YieldToGUI()
{
   #if M_MIL_USE_CE
   MosSleep(0);
   #endif
}
   
