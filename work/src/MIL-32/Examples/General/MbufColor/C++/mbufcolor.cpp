/**********************************************************************************/
/* 
 * File name: MBufColor.cpp
 *
 * Synopsis:  This program demonstrates color buffer manipulations. It allocates 
 *            a displayable color image buffer, displays it, and loads a color   
 *            image into the left part. After that, color annotations are done 
 *            in each band of the color buffer. Finally, to increase the image
 *            luminance, the image is converted to Hue, Saturation and Luminance
 *            (HSL), a certain offset is added to the luminance component and 
 *            the image is converted back to Red, Green, Blue (RGB) into the 
 *            right part to display the result. 
 *
 *            The example also demonstrates how to display multiple images 
 *            in a single display using child buffers.
 */
#include <mil.h> 

/* Source MIL image file specifications. */ 
#define IMAGE_FILE              M_IMAGE_PATH MIL_TEXT("Bird.mim")

/* Luminance offset to add to the image. */
#define IMAGE_LUMINANCE_OFFSET  40L

/* Main function. */
int MosMain(void)
{ 
   MIL_ID  MilApplication,       /* Application identifier.  */
           MilSystem,            /* System identifier.       */
           MilDisplay,           /* Display identifier.      */
           MilImage,             /* Image buffer identifier. */
           MilLeftSubImage,      /* Sub-image buffer identifier for original image. */
           MilRightSubImage,     /* Sub-image buffer identifier for processed image.*/
           MilLumSubImage=0,     /* Sub-image buffer identifier for luminance.      */
           MilRedBandSubImage,   /* Sub-image buffer identifier for red component.  */
           MilGreenBandSubImage, /* Sub-image buffer identifier for green component.*/
           MilBlueBandSubImage;  /* Sub-image buffer identifier for blue component. */
   MIL_INT SizeX, SizeY, SizeBand, Type;

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Allocate a color display buffer twice the size of the source image and display it. */
   MbufAllocColor(MilSystem,
                  MbufDiskInquire(IMAGE_FILE, M_SIZE_BAND, &SizeBand),
                  MbufDiskInquire(IMAGE_FILE, M_SIZE_X, &SizeX) * 2,
                  MbufDiskInquire(IMAGE_FILE, M_SIZE_Y, &SizeY),
                  MbufDiskInquire(IMAGE_FILE, M_TYPE,   &Type),
                  M_IMAGE+M_DISP+M_PROC, &MilImage);
   MbufClear(MilImage, 0L);
   MdispSelect(MilDisplay, MilImage);

   /* Define 2 child buffers that maps to the left and right part of the display 
      buffer, to put the source and destination color images.
    */
   MbufChild2d(MilImage, 0L, 0L, SizeX, SizeY, &MilLeftSubImage);
   MbufChild2d(MilImage, SizeX, 0L, SizeX, SizeY, &MilRightSubImage);

   /* Load the color source image on the left. */
   MbufLoad(IMAGE_FILE, MilLeftSubImage);
      
   /* Define child buffers that map to the red, green and blue components
      of the source image.
    */
   MbufChildColor(MilLeftSubImage, M_RED,   &MilRedBandSubImage);
   MbufChildColor(MilLeftSubImage, M_GREEN, &MilGreenBandSubImage);
   MbufChildColor(MilLeftSubImage, M_BLUE,  &MilBlueBandSubImage);

   /* Write color text annotations to show access in each individual band of the image.
   
      Note that this is typically done more simply by using:
      MgraColor(M_DEFAULT, M_RGB888(0xFF,0x90,0x00));
      MgraText(M_DEFAULT, MilLeftSubImage, ...);
    */
   MgraColor(M_DEFAULT, 0xFF);
   MgraText(M_DEFAULT, MilRedBandSubImage,   SizeX/16, SizeY/8, MIL_TEXT(" TOUCAN "));
   MgraColor(M_DEFAULT, 0x90);
   MgraText(M_DEFAULT, MilGreenBandSubImage, SizeX/16, SizeY/8, MIL_TEXT(" TOUCAN "));
   MgraColor(M_DEFAULT, 0x00);
   MgraText(M_DEFAULT, MilBlueBandSubImage,  SizeX/16, SizeY/8, MIL_TEXT(" TOUCAN "));
 
   /* Print a message. */
   MosPrintf(MIL_TEXT("\nCOLOR OPERATIONS:\n"));
   MosPrintf(MIL_TEXT("-----------------\n\n"));
   MosPrintf(MIL_TEXT("A color source image was loaded on the left and color text\n"));
   MosPrintf(MIL_TEXT("annotations were written in it.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Convert image to Hue, Saturation, Luminance color space (HSL). */
   MimConvert(MilLeftSubImage, MilRightSubImage, M_RGB_TO_HSL);
  
   /* Create a child buffer that maps to the luminance component. */
   MbufChildColor(MilRightSubImage, M_LUMINANCE, &MilLumSubImage);
     
   /* Add an offset to the luminance component. */
   MimArith(MilLumSubImage, IMAGE_LUMINANCE_OFFSET, MilLumSubImage, 
                                                      M_ADD_CONST+M_SATURATION);
  
   /* Convert image back to Red, Green, Blue color space (RGB) for display. */
   MimConvert(MilRightSubImage, MilRightSubImage, M_HSL_TO_RGB); 

   /* Print a message. */
   MosPrintf(MIL_TEXT("Luminance was increased using color image processing.\n"));
  
   /* Print a message. */
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   /* Release sub-images and color image buffer. */
   MbufFree(MilLumSubImage);
   MbufFree(MilRedBandSubImage);
   MbufFree(MilGreenBandSubImage);
   MbufFree(MilBlueBandSubImage);
   MbufFree(MilRightSubImage);
   MbufFree(MilLeftSubImage);
   MbufFree(MilImage);

   /* Release defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}

