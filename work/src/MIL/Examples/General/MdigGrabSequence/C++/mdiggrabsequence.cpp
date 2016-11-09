/********************************************************************************/
/*
 * File name: MDigGrabSequence.cpp
 *
 * Synopsis:  This example shows how to grab a sequence, archive it, and play 
 *            it back in real time from an AVI file. 
 *
 * NOTE:      This example assumes that the hard disk is sufficiently fast 
 *            to keep up with the grab. Also, removing the sequence display or 
 *            the text annotation while grabbing will reduce the CPU usage and
 *            might help if some frames are missed during acquisition. 
 *            If the disk or system are not fast enough, set GRAB_SCALE to 0.5, 
 *            FRAME_NUMBER_ANNOTATION to M_NO or SAVE_SEQUENCE_TO_DISK to M_NO.
 */
#include <mil.h>

/* Sequence file name.*/
#define SEQUENCE_FILE M_TEMP_DIR MIL_TEXT("MilSequence.avi")

/* Image acquisition scale. */
#define GRAB_SCALE   1.0

/* Quantization factor to use during the compression.
   Valid values are 1 to 99 (higher to lower quality). 
*/
#define COMPRESSION_Q_FACTOR   50

/* Annotation flag. Set to M_YES to draw the frame number in the saved image. */
#define FRAME_NUMBER_ANNOTATION  M_YES

/* Archive flag. Set to M_NO to disable AVI Import/Export to disk. */
#define SAVE_SEQUENCE_TO_DISK  M_YES

/* Maximum number of images for the multiple buffering grab. */
#define NB_GRAB_IMAGE_MAX  22

/* User's archive hook function prototype (called for every grabbed frame). */
MIL_INT MFTYPE ArchiveFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

/* User's archive function hook data structure. */
typedef struct
   {
   MIL_ID MilSystem;
   MIL_ID MilDisplay;
   MIL_ID MilImageDisp;
   MIL_ID MilCompressedImage;
   MIL_INT NbGrabbedFrames;
   MIL_INT NbArchivedFrames;
   MIL_INT SaveSequenceToDisk;
   } HookDataStruct;


/* Main function. */
/* -------------- */
int MosMain(void)
{    
   MIL_ID     MilApplication, MilRemoteApplication, MilSystem, MilDigitizer, MilDisplay, MilImageDisp;
   MIL_ID     MilGrabImages[NB_GRAB_IMAGE_MAX];
   MIL_ID     MilCompressedImage = M_NULL;
   MIL_INT    CompressAttribute=0;
   MIL_INT    NbFrames=0,   Selection=1,   LicenseModules=0,   n=0;
   MIL_INT    FrameCount=0, FrameMissed=0, NbFramesReplayed=0, Exit=0;
   MIL_DOUBLE FrameRate=0,  TimeWait=0,    TotalReplay=0;
   MIL_DOUBLE GrabScale=GRAB_SCALE;
   MIL_INT    SaveSequenceToDisk = SAVE_SEQUENCE_TO_DISK;
   HookDataStruct UserHookData;   

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay,
                                              &MilDigitizer, M_NULL);

   /* Allocate an image and display it. */
   MbufAllocColor(MilSystem,
                  MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL),
                  (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_X, M_NULL)*GrabScale),
                  (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL)*GrabScale),
                  8L+M_UNSIGNED,
                  M_IMAGE+M_GRAB+M_DISP, &MilImageDisp);
   MbufClear(MilImageDisp, 0x0);
   MdispSelect(MilDisplay, MilImageDisp);

   /* Grab continuously on display at the specified scale. */
   MdigControl(MilDigitizer, M_GRAB_SCALE, GrabScale);
   MdigGrabContinuous(MilDigitizer, MilImageDisp);

   /* Print a message */
   MosPrintf(MIL_TEXT("\nSEQUENCE ACQUISITION:\n"));
   MosPrintf(MIL_TEXT("--------------------\n\n"));

   /* Inquire MIL licenses. */
   MsysInquire(MilSystem, M_OWNER_APPLICATION, &MilRemoteApplication);
   MappInquire(MilRemoteApplication, M_LICENSE_MODULES, &LicenseModules);

   /* If sequence is saved to disk, select between grabbing an 
      uncompressed, JPEG or JPEG2000 sequence. 
   */
   if (SaveSequenceToDisk && (LicenseModules & (M_LICENSE_JPEGSTD | M_LICENSE_JPEG2000)))
      {
      MosPrintf(MIL_TEXT("Choose the sequence format:\n"));
      MosPrintf(MIL_TEXT("1) Uncompressed images.\n") );
      if(LicenseModules & M_LICENSE_JPEGSTD)
         MosPrintf(MIL_TEXT("2) Compressed images with a lossy JPEG algorithm.\n"));
      if(LicenseModules & M_LICENSE_JPEG2000)
         MosPrintf(MIL_TEXT("3) Compressed images with a lossy JPEG 2000 algorithm.\n"));
      Selection = MosGetch();
      }
   else
      {
      MosPrintf(MIL_TEXT("Press <Enter> to record images.\n"));
      Selection = '1';
      MosGetch();
      }


   /* Set the buffer attribute. */
   switch(Selection)
      {
      case '1':
      case '\r':
         MosPrintf(MIL_TEXT("\nRecording uncompressed images...\n\n"));
         CompressAttribute = M_NULL;
         break;

      case '2':
         MosPrintf(MIL_TEXT("\nRecording JPEG images...\n\n"));
         CompressAttribute = M_COMPRESS + M_JPEG_LOSSY;
         break;

      case '3':
         MosPrintf(MIL_TEXT("\nRecording JPEG 2000 images...\n\n"));
         CompressAttribute = M_COMPRESS + M_JPEG2000_LOSSY;
         break;

      default:
         MosPrintf(MIL_TEXT("\nInvalid selection !.\n\nUsing uncompressed images.\n\n"));
         CompressAttribute = M_NULL;
         while (MosKbhit()) MosGetch();
         break;
      }

   /* Allocate compressed buffer if required. */
   if (CompressAttribute)
      {
      MbufAllocColor(MilSystem,
                     MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL),
                     (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_X, M_NULL)*GrabScale),
                     (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL)*GrabScale),
                     8L+M_UNSIGNED, M_IMAGE+CompressAttribute, &MilCompressedImage);
      MbufControl(MilCompressedImage, M_Q_FACTOR, COMPRESSION_Q_FACTOR);
      }

   /* Allocate the grab buffers to hold the sequence buffering. */
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   for (NbFrames=0, n=0; n<NB_GRAB_IMAGE_MAX; n++)
      {
      MbufAllocColor(MilSystem,
                     MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL),
                     (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_X, M_NULL)*GrabScale),
                     (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL)*GrabScale),
                     8L+M_UNSIGNED, M_IMAGE+M_GRAB, &MilGrabImages[n]);
      if (MilGrabImages[n])
        {
         NbFrames++;
         MbufClear(MilGrabImages[n], 0xFF);
        }
      else
         break;
      }
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   /* Free buffers to leave space for possible temporary buffers. */
   for (n=0; n<2 && NbFrames; n++)
      {
      NbFrames--;
      MbufFree(MilGrabImages[NbFrames]);
      }

   /* Halt continuous grab. */
   MdigHalt(MilDigitizer);

   /* Open the AVI file if required. */
   if (SaveSequenceToDisk)
      {
      MosPrintf(MIL_TEXT("Saving the sequence to an AVI file...\n\n"));
      MbufExportSequence(SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_OPEN);
      }

   /* Initialize User's archiving function hook data structure. */
   UserHookData.MilSystem           = MilSystem;
   UserHookData.MilDisplay          = MilDisplay;
   UserHookData.MilImageDisp        = MilImageDisp;
   UserHookData.MilCompressedImage  = MilCompressedImage;
   UserHookData.SaveSequenceToDisk  = SaveSequenceToDisk;
   UserHookData.NbGrabbedFrames     = 0;
   UserHookData.NbArchivedFrames    = 0;

   /* Acquire the sequence. The processing hook function will
      be called for each image grabbed to archive and display it. 
      If sequence is not saved to disk, stop after NbFrames.
   */
   MdigProcess(MilDigitizer, MilGrabImages, NbFrames, 
               SaveSequenceToDisk ? M_START : M_SEQUENCE, 
               M_DEFAULT, ArchiveFunction, &UserHookData);

   /* Wait for a key press. */
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Stop the sequence acquisition. */
   MdigProcess(MilDigitizer, MilGrabImages, NbFrames, M_STOP, 
                             M_DEFAULT, ArchiveFunction, &UserHookData);

   /* Read and print final statistics. */
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_COUNT,  &FrameCount);
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_RATE,   &FrameRate);
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_MISSED, &FrameMissed);
   MosPrintf(MIL_TEXT("\n\n%ld frames archived (%ld missed), at %.1f frames/sec ")
             MIL_TEXT("(%.1f ms/frame).\n"),UserHookData.NbArchivedFrames, 
                                           FrameMissed, FrameRate, 1000.0/FrameRate);

   /* Sequence file closing if required. */
   if (SaveSequenceToDisk)
       MbufExportSequence(SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, FrameRate, M_CLOSE);

   /* Playback the sequence until a key is pressed. */
   if (UserHookData.NbArchivedFrames > 0)
      {
      MIL_INT KeyPressed = 0;
      do
         {
         /* If sequence must be loaded. */
         if (SaveSequenceToDisk)
            {
            /* Inquire information about the sequence. */
            MosPrintf(MIL_TEXT("\nPlaying back sequence from the AVI file...\n"));
            MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
            MbufDiskInquire(SEQUENCE_FILE, M_NUMBER_OF_IMAGES, &FrameCount);
            MbufDiskInquire(SEQUENCE_FILE, M_FRAME_RATE, &FrameRate);
            MbufDiskInquire(SEQUENCE_FILE, M_COMPRESSION_TYPE, &CompressAttribute);

            /* Open the sequence file. */
            MbufImportSequence(SEQUENCE_FILE, M_DEFAULT, M_NULL, 
               M_NULL, M_NULL, M_NULL, M_NULL, M_OPEN);
            }
      
         /* Copy the images to the screen respecting the sequence frame rate. */
         TotalReplay = 0.0;
         NbFramesReplayed = 0;
         for (n=0; n<FrameCount; n++)
            {
            /* Reset the time. */
            MappTimer(M_DEFAULT, M_TIMER_RESET, M_NULL);

            /* If image was saved to disk. */
            if (SaveSequenceToDisk)
               {
               /* Load image directly to the display. */
               MbufImportSequence(SEQUENCE_FILE, M_DEFAULT, M_LOAD, 
                  M_NULL, &MilImageDisp, n, 1, M_READ);
               NbFramesReplayed++;
               MosPrintf(MIL_TEXT("Frame #%d             \r"), NbFramesReplayed);
               }
            else 
               {
               /* Copy the grabbed image to the display. */
               MbufCopy(MilGrabImages[n], MilImageDisp);
               NbFramesReplayed++;
               MosPrintf(MIL_TEXT("Frame #%d             \r"), NbFramesReplayed);
               }

            /* Check for a pressed key to exit. */
            if (MosKbhit() && (n>=(NB_GRAB_IMAGE_MAX-1)))
               {
               MosGetch();
               break;
               }

            /* Wait to have a proper frame rate. */
            MappTimer(M_DEFAULT, M_TIMER_READ, &TimeWait);
            TotalReplay += TimeWait;
            TimeWait = (1/FrameRate) - TimeWait;
            MappTimer(M_DEFAULT, M_TIMER_WAIT, &TimeWait);
            TotalReplay += (TimeWait > 0) ? TimeWait: 0.0;
            }

         /* Close the sequence file. */
         if (SaveSequenceToDisk)
            MbufImportSequence(SEQUENCE_FILE, M_DEFAULT, M_NULL, 
               M_NULL, M_NULL, M_NULL, M_NULL, M_CLOSE);

         /* Print statistics. */
         MosPrintf(MIL_TEXT("\n\n%ld frames replayed, at a frame rate of %.1f frames/sec ")
            MIL_TEXT("(%.1f ms/frame).\n\n"), NbFramesReplayed, n/TotalReplay, 
            1000.0*TotalReplay/n);
         MosPrintf(MIL_TEXT("Press <Enter> to end (or any other key to playback again).\n"));
         KeyPressed = MosGetch();
         }
      while((KeyPressed != '\r') && (KeyPressed != '\n'));
      }

   /* Free all allocated buffers. */
   MbufFree(MilImageDisp);
   for (n=0; n<NbFrames; n++)
       MbufFree(MilGrabImages[n]);
   if (MilCompressedImage)
      MbufFree(MilCompressedImage);

   /* Free defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, M_NULL);

   return 0;
}


/* User's archive function called each time a new buffer is grabbed. */
/* -------------------------------------------------------------------*/

/* Local defines for the annotations. */
#define STRING_LENGTH_MAX  20
#define STRING_POS_X       20
#define STRING_POS_Y       20

MIL_INT MFTYPE ArchiveFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
   {
   HookDataStruct *UserHookDataPtr = (HookDataStruct *)HookDataPtr;
   MIL_ID ModifiedImage = 0;
   MIL_INT n = 0;
   MIL_TEXT_CHAR Text[STRING_LENGTH_MAX]= {MIL_TEXT('\0'),};

   /* Retrieve the MIL_ID of the grabbed buffer. */
   MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &ModifiedImage);

   /* Increment the frame count. */
   UserHookDataPtr->NbGrabbedFrames++;

   /* Draw the frame count in the image if enabled. */
   if ((FRAME_NUMBER_ANNOTATION == M_YES))
     {
     MosSprintf(Text, STRING_LENGTH_MAX, MIL_TEXT(" %ld "), 
                                          UserHookDataPtr->NbGrabbedFrames);
     MgraText(M_DEFAULT, ModifiedImage,   STRING_POS_X, STRING_POS_Y, Text);
     }

   /* Compress the new image. */
   if (UserHookDataPtr->MilCompressedImage)
      {
      MbufCopy(ModifiedImage, UserHookDataPtr->MilCompressedImage);
      }

   /* Archive the new image. */
   if (UserHookDataPtr->SaveSequenceToDisk)
      {
      MbufExportSequence(SEQUENCE_FILE, M_DEFAULT, UserHookDataPtr->MilCompressedImage? 
                         &UserHookDataPtr->MilCompressedImage: &ModifiedImage,
                         1, M_DEFAULT, M_WRITE);

      UserHookDataPtr->NbArchivedFrames++;
      MosPrintf(MIL_TEXT("Frame #%d               \r"), UserHookDataPtr->NbArchivedFrames);
      }

   /* Copy the new grabbed image to the display. */
   MbufCopy(ModifiedImage, UserHookDataPtr->MilImageDisp);

   return 0;
   }
