/*******************************************************************************/
/*
* File name: MReg.cpp
*
* Synopsis:  This program uses the registration module to form a mosaic of 
*            three images taken from a camera at unknown translation.
*/ 
#include <mil.h>  
#include <stdlib.h>  

/* Number of images to register. */
#define NUM_IMAGES_TO_REGISTER    3

/* MIL image file specifications. */  
#define IMAGE_FILES_SOURCE M_IMAGE_PATH MIL_TEXT("CircuitBoardPart%d.mim")

int MosMain(void)
   {
   MIL_ID      MilApplication,                 /* Application identifier.          */
               MilSystem,                      /* System identifier.               */
               MilDisplay,                     /* Display identifier.              */
               MilGraphicList,                 /* Graphic list identifier.         */
               MilSourceImages
                     [NUM_IMAGES_TO_REGISTER], /* Source images buffer identifiers.*/
               MilMosaicImage = M_NULL,        /* Mosaic image buffer identifier.  */
               MilRegContext,                  /* Registration context identifier. */
               MilRegResult;                   /* Registration result identifier.  */
   MIL_INT     i;                              /* Iterator.                        */
   MIL_INT     Result;                         /* Result of the registration.      */
   MIL_INT     MosaicSizeX,                    /* Size of the mosaic.              */
               MosaicSizeY;
   MIL_INT     MosaicSizeBand,                 /* Characteristics of mosaic image. */
               MosaicType;
   MIL_TEXT_CHAR ImageFilesSource[NUM_IMAGES_TO_REGISTER][260];
   
   /* Allocate defaults */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Load source image names. */
   for (i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
      {
      MosSprintf(ImageFilesSource[i], 260, IMAGE_FILES_SOURCE, i);
      }

   /* Print module name. */
   MosPrintf(MIL_TEXT("\nREGISTRATION MODULE:\n"));
   MosPrintf(MIL_TEXT("---------------------\n\n"));
 
   /* Print comment. */ 
   MosPrintf(MIL_TEXT("This program will make a mosaic from many source images.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Restore the source images. */ 
   for (i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
      MbufRestore(ImageFilesSource[i], MilSystem, &MilSourceImages[i]);

   /* Display the source images. */
   for (i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
      {
      MdispSelect(MilDisplay, MilSourceImages[i]);

      /* Pause to show each image. */
      MosPrintf(MIL_TEXT("image %ld.\n"), i);
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();
      }

   /* Allocate a graphic list to hold the subpixel annotations to draw. */
   MgraAllocList(MilSystem, M_DEFAULT, &MilGraphicList);
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList);

   /* Allocate a new empty registration context. */
   MregAlloc(MilSystem, M_CORRELATION, M_DEFAULT, &MilRegContext);

   /* Allocate a new empty registration result buffer. */
   MregAllocResult(MilSystem, M_DEFAULT, &MilRegResult);

   /* Set the transformation type to translation. */
   MregControl(MilRegContext, M_CONTEXT, M_TRANSFORMATION_TYPE,  M_TRANSLATION);

   /* By default, each image will be registered with the previous in the list
      No need to set other location parameters. */

   /* Set range to 100% in order to search all possible translations. */
   MregControl(MilRegContext, M_CONTEXT, M_LOCATION_DELTA, 100);

   /* Calculate the registration on the images. */
   MregCalculate(MilRegContext, MilSourceImages, MilRegResult, 
                                          NUM_IMAGES_TO_REGISTER, M_DEFAULT);

   /* Verify if registration is successful. */
   MregGetResult(MilRegResult, M_GENERAL, M_RESULT + M_TYPE_MIL_INT, &Result);
   if( Result == M_SUCCESS )
      {
      /* Get the size of the required mosaic buffer. */
      MregGetResult(MilRegResult, M_GENERAL, M_MOSAIC_SIZE_X + M_TYPE_MIL_INT, 
                                             &MosaicSizeX);
      MregGetResult(MilRegResult, M_GENERAL, M_MOSAIC_SIZE_Y + M_TYPE_MIL_INT, 
                                             &MosaicSizeY);

      /* The mosaic type will be the same as the source images. */
      MbufInquire(MilSourceImages[0], M_SIZE_BAND, &MosaicSizeBand);
      MbufInquire(MilSourceImages[0], M_TYPE     , &MosaicType    );

      /* Allocate mosaic image. */
      MbufAllocColor(MilSystem, MosaicSizeBand, MosaicSizeX, MosaicSizeY, MosaicType, 
                                              M_IMAGE+M_PROC+M_DISP, &MilMosaicImage);

      /* Compose the mosaic from the source images. */
      MregTransformImage(MilRegResult, MilSourceImages, MilMosaicImage, 
                         NUM_IMAGES_TO_REGISTER, M_BILINEAR+M_OVERSCAN_CLEAR, M_DEFAULT);

      /* Display the mosaic image and prepare for overlay annotations. */
      MdispSelect(MilDisplay, MilMosaicImage);
      MgraColor(M_DEFAULT, M_RGB888(0, 0xFF, 0));

      /* Pause to show the mosaic. */
      MosPrintf(MIL_TEXT("mosaic image.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      /* Draw the box of all source images in the mosaic. */
      MregDraw(M_DEFAULT, MilRegResult, MilGraphicList, M_DRAW_BOX, M_ALL, M_DEFAULT);

      /* Draw a cross at the center of each image in the mosaic. */
      for (i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
         {
         MIL_DOUBLE  SourcePosX,
                     SourcePosY,
                     MosaicPosX,
                     MosaicPosY;
         MIL_INT     MosaicPosXMilInt,
                     MosaicPosYMilInt;

         /* Coordinates of the center of the source image. */
         SourcePosX = 0.5 * (MIL_DOUBLE)MbufInquire(MilSourceImages[i], M_SIZE_X, NULL);
         SourcePosY = 0.5 * (MIL_DOUBLE)MbufInquire(MilSourceImages[i], M_SIZE_Y, NULL);

         /* Transform the coordinates to the mosaic. */
         MregTransformCoordinate(MilRegResult, i, M_MOSAIC, SourcePosX, SourcePosY, 
                                                  &MosaicPosX, &MosaicPosY, M_DEFAULT);
         MosaicPosXMilInt = (MIL_INT)(MosaicPosX + 0.5);
         MosaicPosYMilInt = (MIL_INT)(MosaicPosY + 0.5);

         /* Draw the cross in the mosaic. */
         MgraLine(M_DEFAULT, MilGraphicList, MosaicPosXMilInt - 4, MosaicPosYMilInt    , 
                                             MosaicPosXMilInt + 4, MosaicPosYMilInt    );
         MgraLine(M_DEFAULT, MilGraphicList, MosaicPosXMilInt    , MosaicPosYMilInt - 4, 
                                             MosaicPosXMilInt    , MosaicPosYMilInt + 4);
         }

      MosPrintf(MIL_TEXT("The bounding boxes and the center of all source images\n"));
      MosPrintf(MIL_TEXT("have been drawn in the mosaic.\n"));
      }
   else
      {
      MosPrintf(MIL_TEXT("Error: Registration was not successful.\n"));
      }

   /* Pause to show results. */ 
   MosPrintf(MIL_TEXT("\nPress <Enter> to end.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MgraFree(MilGraphicList);
   if (MilMosaicImage != M_NULL)
      MbufFree(MilMosaicImage);
   MregFree(MilRegContext);
   MregFree(MilRegResult);
   for (i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
      MbufFree(MilSourceImages[i]);

   /* Free defaults. */ 
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
   }
