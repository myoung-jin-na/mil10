/*****************************************************************************/
/*
 * File name: MImSegment.cpp
 *
 * Synopsis:  This program uses the watershed and the edge detection functions 
 *            to remove the background of an image with a non-linear lighting.
 *            Then, the watershed and the distance functions are used to separate
 *            the touching objects.
 */
#include <mil.h>

/* Source image specifications. */ 
#define IMAGE_FILE         M_IMAGE_PATH MIL_TEXT("pills.mim")

/* Minimal distance and gradient variations for the watershed calculation. */
#define WATERSHED_MINIMAL_GRADIENT_VARIATION       45
#define WATERSHED_MINIMAL_DISTANCE_VARIATION        2

/* Position used to fetch the label of the background. */
#define PIXEL_FETCH_POSITION_X                2
#define PIXEL_FETCH_POSITION_Y                2

/*  Main function. */
int MosMain(void)
{
   MIL_ID    MilApplication,    /* Application identifier.       */
             MilSystem,         /* System identifier.            */
             MilDisplay,        /* Display identifier.           */
             MilImage,          /* Image buffer identifier.      */
             MilImageWatershed; /* Image buffer identifier.      */
   MIL_INT32 lFetchedValue = 0; /* Label of the background.      */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem,
                             &MilDisplay, M_NULL, M_NULL);

   /* Restore the source image into two automatically allocated
    * image buffers and select one of them to the display.
    */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImageWatershed);
   MbufRestore(IMAGE_FILE, MilSystem, &MilImage);
   MdispSelect(MilDisplay, MilImage);

   /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("\nSEGMENTATION:\n"));
   MosPrintf(MIL_TEXT("-------------\n\n"));
   MosPrintf(MIL_TEXT("An edge detection followed by a watershed will be used to remove\n"));
   MosPrintf(MIL_TEXT("the background.\nPress <Enter> to continue.\n\n"));
   MosGetch();

   /* Perform a edge detection on the original image. */
   MimEdgeDetect(MilImageWatershed, MilImageWatershed, M_NULL, M_SOBEL,
                 M_REGULAR_EDGE_DETECT, M_NULL);

   /* Find the basins of the edge detected image that have a minimal gray scale
    * variation of WATERSHED_MINIMAL_GRADIENT_VARIATION.
    */
   MimWatershed(MilImageWatershed, M_NULL, MilImageWatershed, 
                WATERSHED_MINIMAL_GRADIENT_VARIATION, M_MINIMA_FILL+M_BASIN);

   /* Fetch the label of the background, clip it to zero and clip the other labels to 
    * the maximum value of the buffer.
    */
   MbufGet2d(MilImageWatershed, PIXEL_FETCH_POSITION_X, PIXEL_FETCH_POSITION_Y, 
                                                        1, 1, &lFetchedValue);
   MimClip(MilImageWatershed, MilImageWatershed, M_EQUAL, lFetchedValue, 0, 0, 0);
   MimClip(MilImageWatershed, MilImage, M_NOT_EQUAL, 0, 0, 0xFF, 0);

   /* Pause to show the binarized image. */
   MosPrintf(MIL_TEXT("A distance transformation followed by a watershed will be used \n"));
   MosPrintf(MIL_TEXT("to separate the touching pills.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Perform a distance transformation on the binarized image. */
   MimDistance(MilImage, MilImageWatershed, M_CHAMFER_3_4);

   /* Find the watersheds of the image. */
   MimWatershed(MilImageWatershed, M_NULL, MilImageWatershed,
                WATERSHED_MINIMAL_DISTANCE_VARIATION,
                M_STRAIGHT_WATERSHED+M_MAXIMA_FILL+M_SKIP_LAST_LEVEL);
    
   /* AND the watershed image with the binarized image to separate the touching pills. */
   MimArith(MilImageWatershed, MilImage, MilImage, M_AND);

   /* Pause to show the segmented image. */
   MosPrintf(MIL_TEXT("Here are the segmented pills.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(MilImageWatershed);
   MbufFree(MilImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}
