/************************************************************************************/
/*
 * File name: MImFFT.cpp 
 *
 * Synopsis:  This program uses the Fast Fourier Transform to filter an image.
 */
#include <mil.h>
#include <math.h>

/* Target image specifications. */
#define NOISY_IMAGE                    M_IMAGE_PATH MIL_TEXT("noise.mim")
#define IMAGE_WIDTH                    256
#define IMAGE_HEIGHT                   256
#define X_NEGATIVE_FREQUENCY_POSITION   63
#define X_POSITIVE_FREQUENCY_POSITION  191
#define Y_FREQUENCY_POSITION           127
#define CIRCLE_WIDTH                     9

int MosMain(void)
{
   MIL_ID MilApplication,   /* Application identifier.                  */
          MilSystem,        /* System identifier.                       */
          MilDisplay,       /* Display identifier.                      */
          MilImage,         /* Image buffer identifier.                 */
          MilOverlayImage,  /* Overlay image buffer identifier.         */
          MilSubImage00,    /* Child buffer identifier.                 */
          MilSubImage01,    /* Child buffer identifier.                 */
          MilSubImage10,    /* Child buffer identifier.                 */
          MilSubImage11,    /* Child buffer identifier.                 */
          MilTransformReal, /* Real part of the transformed image.      */
          MilTransformIm;   /* Imaginary part of the transformed image. */

   float  ZeroVal = 0.0;

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay,M_NULL, M_NULL);

   /* Allocate a display buffer and clear it. */
   MbufAlloc2d(MilSystem, IMAGE_WIDTH*2, IMAGE_HEIGHT*2,
               8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &MilImage);
   MbufClear(MilImage, 0L);

   /* Display the image buffer and prepare for overlay annotations. */
   MdispSelect(MilDisplay, MilImage);
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   /* Allocate child buffers in the 4 quadrants of the display image. */
   MbufChild2d(MilImage, 0L, 0L, IMAGE_WIDTH, IMAGE_HEIGHT, &MilSubImage00);
   MbufChild2d(MilImage, IMAGE_WIDTH, 0L,IMAGE_WIDTH, IMAGE_HEIGHT, &MilSubImage01);
   MbufChild2d(MilImage, 0L, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, &MilSubImage10);
   MbufChild2d(MilImage, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, 
                                                                 &MilSubImage11);

   /* Allocate processing buffers. */
   MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 32+M_FLOAT, 
                                          M_IMAGE+M_PROC, &MilTransformReal);
   MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 32+M_FLOAT, 
                                          M_IMAGE+M_PROC, &MilTransformIm);

   /* Load a noisy image. */
   MbufLoad(NOISY_IMAGE, MilSubImage00);

   /* Print a message on the screen. */
   MosPrintf(MIL_TEXT("\nFFT:\n"));
   MosPrintf(MIL_TEXT("----\n\n"));
   MosPrintf(MIL_TEXT("The frequency spectrum of a noisy image will be computed ")
                                       MIL_TEXT("to remove the periodic noise.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* The image is Fourier transformed to obtain the magnitude of the
      spectrum. This result will be used to design the filter.   */
   MimTransform(MilSubImage00, M_NULL, MilTransformReal, MilTransformIm,
                M_FFT, M_FORWARD+M_CENTER+M_MAGNITUDE+M_LOG_SCALE);
   MbufCopy(MilTransformReal, MilSubImage10);

   /* Draw circles in the overlay around the points of interest. */
   MbufCopy(MilTransformReal, MilSubImage11);
   MgraColor(M_DEFAULT, M_COLOR_YELLOW);
   MgraArc(M_DEFAULT, MilOverlayImage, X_NEGATIVE_FREQUENCY_POSITION,
           Y_FREQUENCY_POSITION+IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);
   MgraArc(M_DEFAULT, MilOverlayImage, X_POSITIVE_FREQUENCY_POSITION,
           Y_FREQUENCY_POSITION+IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);
   MgraArc(M_DEFAULT, MilOverlayImage, X_NEGATIVE_FREQUENCY_POSITION+IMAGE_WIDTH,
           Y_FREQUENCY_POSITION+IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);
   MgraArc(M_DEFAULT, MilOverlayImage, X_POSITIVE_FREQUENCY_POSITION+IMAGE_WIDTH,
           Y_FREQUENCY_POSITION+IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);

   /* Put zero in the spectrum where the noise is located. */
   MbufPut2d(MilSubImage11,   X_NEGATIVE_FREQUENCY_POSITION,
                              Y_FREQUENCY_POSITION, 1, 1, &ZeroVal);
   MbufPut2d(MilSubImage11,   X_POSITIVE_FREQUENCY_POSITION,
                              Y_FREQUENCY_POSITION, 1, 1, &ZeroVal);

   /* Compute the Fast Fourier Transform of the image. */
   MimTransform(MilSubImage00, M_NULL, MilTransformReal,
                MilTransformIm, M_FFT, M_FORWARD+M_CENTER);

   /* Filter the image in the frequency domain. */
   MbufPut2d(MilTransformReal, X_NEGATIVE_FREQUENCY_POSITION,
                               Y_FREQUENCY_POSITION, 1, 1, &ZeroVal);
   MbufPut2d(MilTransformReal, X_POSITIVE_FREQUENCY_POSITION,
                               Y_FREQUENCY_POSITION, 1, 1, &ZeroVal);
   MbufPut2d(MilTransformIm, X_NEGATIVE_FREQUENCY_POSITION,
                             Y_FREQUENCY_POSITION, 1, 1, &ZeroVal);
   MbufPut2d(MilTransformIm, X_POSITIVE_FREQUENCY_POSITION,
                             Y_FREQUENCY_POSITION, 1, 1, &ZeroVal);

   /* Recover the image in the spatial domain. */
   MimTransform(MilTransformReal, MilTransformIm,
                MilSubImage01, M_NULL, M_FFT, M_REVERSE+M_CENTER+M_SATURATION);

   /* Print a message. */
   MosPrintf(MIL_TEXT("The frequency components of the noise are located ")
                                MIL_TEXT("in the center of the circles.\n"));
   MosPrintf(MIL_TEXT("The noise was removed by setting these frequency ")
                                         MIL_TEXT("components to zero.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free buffers. */
   MbufFree(MilSubImage00);
   MbufFree(MilSubImage01);
   MbufFree(MilSubImage10);
   MbufFree(MilSubImage11);
   MbufFree(MilImage);
   MbufFree(MilTransformReal);
   MbufFree(MilTransformIm);

   /* Free defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}
