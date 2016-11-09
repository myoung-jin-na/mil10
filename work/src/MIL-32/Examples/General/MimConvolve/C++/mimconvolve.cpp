/*****************************************************************************/
/* 
 * File name: MImConvolve.cpp
 *
 * Synopsis:  This program performs a 3x3 convolution using a custom kernel
 *            and calculates the convolution time.
 */
#include <mil.h>

/* Target MIL image specifications. */
#define IMAGE_FILE       M_IMAGE_PATH MIL_TEXT("BaboonMono.mim")
#define ZOOM_VALUE       2

/* Kernel data definition. */
#define KERNEL_WIDTH     3L
#define KERNEL_HEIGHT    3L
#define KERNEL_DEPTH     8L
unsigned char  KernelData[KERNEL_HEIGHT][KERNEL_WIDTH] =
               { {1, 2, 1},
                 {2, 4, 2},
                 {1, 2, 1}
               };

/* Timing loop iterations. */
#define NB_LOOP 100

int MosMain(void)
{
   MIL_ID MilApplication,  /* Application identifier.     */
          MilSystem,       /* System identifier.          */
          MilDisplay,      /* Display identifier.         */
          MilDisplayImage, /* Image buffer identifier.    */
          MilImage,        /* Image buffer identifier.    */
          MilKernel;       /* Custom kernel identifier.   */
   long   n;
   MIL_DOUBLE Time;   

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem,
                    &MilDisplay, M_NULL, M_NULL);

   /* Restore source image into an automatically allocated image buffers. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImage);
   MbufRestore(IMAGE_FILE, MilSystem, &MilDisplayImage);

   /* Zoom display to see the result of image processing better. */
   MdispZoom(MilDisplay, ZOOM_VALUE, ZOOM_VALUE);

   /* Display the image buffer. */
   MdispSelect(MilDisplay, MilDisplayImage);
   
   /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("\nIMAGE PROCESSING:\n"));
   MosPrintf(MIL_TEXT("-----------------\n\n"));
   MosPrintf(MIL_TEXT("This program performs a convolution on the displayed image.\n"));
   MosPrintf(MIL_TEXT("It uses a custom smoothing kernel.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Allocate a MIL kernel. */
   MbufAlloc2d(MilSystem, KERNEL_WIDTH, KERNEL_HEIGHT,
               KERNEL_DEPTH+M_UNSIGNED, M_KERNEL, &MilKernel);

   /* Put the custom data in it. */
   MbufPut(MilKernel,KernelData);

   /* Set a normalization (divide) factor to have a kernel with
    * a sum equal to one.
    */
   MbufControlNeighborhood(MilKernel,M_NORMALIZATION_FACTOR,16L);

   /* Convolve the image using the kernel. */
   MimConvolve(MilImage, MilDisplayImage, MilKernel);

   /* Now time the convolution (MimConvolve()):
      Overscan calculation is disabled and a destination image that
      is not displayed is used to have the real convolution time. Also the 
      function must be called once before the timing loop for more accurate 
      time (dll load, ...).
   */
   MbufControlNeighborhood(MilKernel, M_OVERSCAN, M_DISABLE);
   MimConvolve(MilDisplayImage, MilImage, MilKernel);  
   MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
   for (n= 0; n < NB_LOOP; n++)
         MimConvolve(MilDisplayImage, MilImage, MilKernel);  
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &Time);

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Convolve time: %.3f ms.\n"), Time*1000/NB_LOOP);
   MosPrintf(MIL_TEXT("Press <Enter> to terminate.\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(MilKernel);
   MbufFree(MilImage);
   MbufFree(MilDisplayImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);
   
   return 0;
}
