/*****************************************************************************/
/*
 * File name: MImDeinterlace.cpp 
 *
 * Synopsis:  : This program performs deinterlacing on an image. 
 *
 *              First, a static image is deinterlaced using the Discard
 *              algorithm. Then, if a digitizer is present, the grabbed 
 *              images are deinterlaced live using the same algorithms.
 */
#include <mil.h>
/* Target image specifications. */
#define IMAGE_FILE               M_IMAGE_PATH MIL_TEXT("/Preprocessing/InterlacedBird.mim")

#define MOTION_DETECT_NUM_FRAMES 3
#define MOTION_DETECT_THRESHOLD  20

#define NUM_GRAB_IMAGES          (MOTION_DETECT_NUM_FRAMES+1)

#define GRAB_DEINTERLACE_METHOD  M_DISCARD

void GrabDeinterlace(MIL_ID MilSystem);

/* Main function. */
/* -------------- */
int MosMain(void)
{
   MIL_ID   MilApplication,        /* Application identifier.         */
            MilSystem,             /* System identifier.              */
            MilDisplay,            /* Display identifier.             */
            MilDisplayImage,       /* Image buffer identifier.        */
            MilSourceImage,        /* Image buffer identifier.        */
            MilDeinterlaceContext; /* Deinterlace context identifier. */
   MIL_INT  ImageSizeBand,
            ImageWidth,
            ImageHeight, 
            ImageType;

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Restore the source image. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilSourceImage);   

   /* Allocate a display buffers and show the source image. */
   MbufAllocColor(MilSystem, MbufInquire(MilSourceImage, M_SIZE_BAND, &ImageSizeBand),
                             MbufInquire(MilSourceImage, M_SIZE_X   , &ImageWidth),
                             MbufInquire(MilSourceImage, M_SIZE_Y   , &ImageHeight),
                             MbufInquire(MilSourceImage, M_TYPE     , &ImageType),
                             M_IMAGE+M_PROC+M_DISP, &MilDisplayImage);
   MbufCopy(MilSourceImage, MilDisplayImage);
   MdispSelect(MilDisplay, MilDisplayImage);

   /* Allocate a deinterlacing context. */
   MimAlloc(MilSystem, M_DEINTERLACE_CONTEXT, M_DEFAULT, &MilDeinterlaceContext);

   /* Print a message. */
   MosPrintf(MIL_TEXT("\nDEINTERLACING:\n"));
   MosPrintf(MIL_TEXT("----------------\n\n"));
   MosPrintf(MIL_TEXT("This image has been grabbed using an interlaced camera.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Deinterlace using M_DISCARD. */
   MimControl(MilDeinterlaceContext, M_DEINTERLACE_TYPE, M_DISCARD);
   MimDeinterlace(MilDeinterlaceContext, &MilSourceImage, &MilDisplayImage, 1, 1, M_DEFAULT);

   /* Display message. */
   MosPrintf(MIL_TEXT("The image has been deinterlaced using the DISCARD algorithm.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Free objects. */
   MbufFree(MilSourceImage);
   MbufFree(MilDisplayImage);
   MdispFree(MilDisplay);
   MimFree(MilDeinterlaceContext);

   /* If the system has a digitizer, use it to do live deinterlacing. */
   if (MsysInquire(MilSystem, M_DIGITIZER_NUM, M_NULL))
      {
      GrabDeinterlace(MilSystem);
      }

   /* Free objects. */
   MappFreeDefault(MilApplication, MilSystem, M_NULL, M_NULL, M_NULL);

   return 0;
}

   
void GrabDeinterlace(MIL_ID MilSystem)
   {
   MIL_ID   MilDigitizer,          /* Digitizer identifier.           */
            MilDisplay,            /* Display identifier.             */
            MilDisplayImage,       /* Image buffer identifier.        */
            MilDeinterlaceContext, /* Deinterlace context identifier. */
            MilGrabImages[NUM_GRAB_IMAGES],
            MilPreviousImages[MOTION_DETECT_NUM_FRAMES];

   MIL_INT  ImageSizeBand,
            ImageWidth,
            ImageHeight;
   MIL_INT  i,
            CurrentGrabIndex,
            NextGrabIndex;

   /* Try to allocate a digitizer. */
   MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
   if (MilDigitizer == M_NULL)
      return;

   /* Inquire characteristics of the digitizer. */
   MdigInquire(MilDigitizer, M_SIZE_BAND, &ImageSizeBand);
   MdigInquire(MilDigitizer, M_SIZE_X   , &ImageWidth   );
   MdigInquire(MilDigitizer, M_SIZE_Y   , &ImageHeight  );

   /* Allocate grab images. */
   for (i = 0; i < NUM_GRAB_IMAGES; i++)
      {
      MbufAllocColor(MilSystem, 
                     ImageSizeBand, 
                     ImageWidth, 
                     ImageHeight, 
                     8+M_UNSIGNED, 
                     M_IMAGE+M_GRAB+M_PROC, 
                     &MilGrabImages[i]);
      }

   /* Allocate a deinterlacing context. */
   MimAlloc(MilSystem, M_DEINTERLACE_CONTEXT, M_DEFAULT, &MilDeinterlaceContext);

   /* Allocate new display and display image. */
   MbufAllocColor(MilSystem, 
                  ImageSizeBand, 
                  ImageWidth, 
                  ImageHeight, 
                  8+M_UNSIGNED, 
                  M_IMAGE+M_GRAB+M_DISP+M_PROC, 
                  &MilDisplayImage);
   MdispAlloc(MilSystem, 
              M_DEFAULT, 
              MIL_TEXT("M_DEFAULT"),
              M_DEFAULT, 
              &MilDisplay);
   MbufClear(MilDisplayImage, 0);
   MdispSelect(MilDisplay, MilDisplayImage);

   /* Print a message. */
   MosPrintf(MIL_TEXT("Normal live grab is displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));

   /* Grab continuously on display and wait for a key press. */
   MdigGrabContinuous(MilDigitizer, MilDisplayImage);
   MosGetch();

   /* Halt continuous grab. */
   MdigHalt(MilDigitizer);

   /* Configure the Deinterlace context for the appropriate live deinterlacing method. */
   MimControl(MilDeinterlaceContext, M_DEINTERLACE_TYPE, GRAB_DEINTERLACE_METHOD);
   if (GRAB_DEINTERLACE_METHOD == M_ADAPTIVE_DISCARD || 
       GRAB_DEINTERLACE_METHOD == M_ADAPTIVE_AVERAGE)
      {
      MimControl(MilDeinterlaceContext, M_MOTION_DETECT_NUM_FRAMES, MOTION_DETECT_NUM_FRAMES);
      MimControl(MilDeinterlaceContext, M_MOTION_DETECT_THRESHOLD , MOTION_DETECT_THRESHOLD );
      }

   /* Set the digitizer to asynchronous grab. */
   MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);

   /* Send the first grab to the digitizer. */
   MdigGrab(MilDigitizer, MilGrabImages[0]);
   CurrentGrabIndex = 0;
   NextGrabIndex    = 1;

   /* Print a message. */
   MosPrintf(MIL_TEXT("Deinterlaced live grab is displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));

   /* Grab and process. */
   while (!MosKbhit())
      {
      /* Send the next grab command to the digitizer. 
         This will block until the current grab is finished. */
      MdigGrab(MilDigitizer, MilGrabImages[NextGrabIndex]);

      if ((GRAB_DEINTERLACE_METHOD != M_ADAPTIVE_DISCARD) && 
          (GRAB_DEINTERLACE_METHOD != M_ADAPTIVE_AVERAGE))
         {
         /* Deinterlace the current grabbed image. */
         MimDeinterlace(MilDeinterlaceContext, &MilGrabImages[CurrentGrabIndex], 
                                                   &MilDisplayImage, 1, 1, M_DEFAULT);
         }
      else
         {
         /* Build the table of the last MOTION_DETECT_NUM_FRAMES grabbed frames. */
         for (i = 0; i < MOTION_DETECT_NUM_FRAMES; i++)
            {
            MIL_INT  PreviousGrabIndex = NextGrabIndex - MOTION_DETECT_NUM_FRAMES + i;
            if (PreviousGrabIndex < 0)
               PreviousGrabIndex += NUM_GRAB_IMAGES;
            MilPreviousImages[i] = MilGrabImages[PreviousGrabIndex];
            }

         /* Deinterlace the previous grabbed images. */
         MimDeinterlace(MilDeinterlaceContext, MilPreviousImages, &MilDisplayImage, 
                                             MOTION_DETECT_NUM_FRAMES, 1, M_DEFAULT);
         }

      /* Increment grab index. */
      CurrentGrabIndex = NextGrabIndex;
      NextGrabIndex = (NextGrabIndex+1) % NUM_GRAB_IMAGES;
      }
   MosGetch();

   /* Display the last grabbed image. */
   MosPrintf(MIL_TEXT("Last deinterlaced image displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free objects. */
   MimFree(MilDeinterlaceContext);
   MbufFree(MilDisplayImage);
   MdispFree(MilDisplay);
   for (i = 0; i < NUM_GRAB_IMAGES; i++)
      MbufFree(MilGrabImages[i]);
   MdigFree(MilDigitizer);
   }
