/*************************************************************************************/
/*
 * File name: MseqProcess.cpp
 *
 * Synopsis:  This program shows the use of the MseqProcess() and MseqFeed() functions 
 *            to perform real-time encoding of a sequence of captured images.
 *
 *            The user's preprocessing and compression code is written in a hook 
 *            function that will be called by MdigProcess() for each frame grabbed 
 *            (see ProcessingFunction()). The queueing for encoding of the next 
 *            frame is also made in that hook function to allow fully parallel 
 *            execution of the capture and the encoding.
 *
 *      Note: The average encoding time must be shorter than the grab time or
 *            some frames will be missed. To miss frames is very frequent when 
 *            the encoding is done by software. Also, if the captured images 
 *            are not displayed and the frame count is not drawn or printed 
 *            in the hook function, the CPU usage is reduced significantly.
 *            
 *            When encoding a 1080p source it is recommended to have your 
 *            MIL Non-Paged Memory set to at least 64MB.
 */
#include <mil.h>

/* Number of images in the buffering grab queue.
   Generally, increasing this number gives better real-time grab.
 */
#define BUFFERING_SIZE_MAX 20

/* Target sequence file name and location. The temporary directory location can 
   be reached with %temp% under Windows.  
 */
#define SEQUENCE_FILE M_TEMP_DIR MIL_TEXT("SeqProcess.mp4")

/* Remote target sequence file name and location if Distributed MIL is used. */
#define REMOTE_SEQUENCE_FILE MIL_TEXT("remote:///") SEQUENCE_FILE

enum ProcessingHookOperation {
   DISPLAY,
   ENCODE,
   };

/* User's processing function prototype. */
MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

/* User's processing function hook data structure. */
typedef struct
   {
   MIL_ID  MilDigitizer;
   MIL_ID  MilImageDisp;
   MIL_ID  MilSeqContext;
   MIL_INT ProcessedImageCount;
   ProcessingHookOperation ProcessingOperation;
   } ProcessingHookDataStruct;

/* Optional encoding end function prototype */
MIL_INT MFTYPE FrameEncodingEndFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

/* Optional encoding end function prototype */
MIL_INT MFTYPE FrameDecodingEndFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

/* Optional encoding end function hook data structure. */
typedef struct
   {
   MIL_INT EncodedImageCount;
   MIL_ID DecompressContextID;
   } EncodingFrameEndHookDataStruct;

/* Optional decoding end function hook data structure. */
typedef struct
   {
   MIL_INT DecodedImageCount;
   MIL_ID MilImageDisp;
   } DecodingFrameEndHookDataStruct;

/* Main function. */
/* ---------------*/

int MosMain(void)
{
   MIL_ID MilApplication;
   MIL_ID MilRemoteApplication;
   MIL_ID MilSystem     ;
   MIL_ID MilDigitizer  ;
   MIL_ID MilDisplay    ;
   MIL_ID MilImageDisp  ;
   MIL_ID MilGrabBufferList[BUFFERING_SIZE_MAX] = { 0 };
   MIL_ID MilCompressContext;
   MIL_ID MilDecompressContext;
   MIL_INT LicenseModules = 0;
   MIL_INT MilSystemLocation;
   MIL_INT MilGrabBufferListSize;
   MIL_INT ProcessFrameCount  = 0;
   MIL_INT NbFrames           = 0, n=0;
   MIL_DOUBLE EncodingDesiredFrameRate = 0.0;
   MIL_DOUBLE ProcessFrameRate=0.0;
   MIL_INT SeqProcessFilePathSize;
   MIL_TEXT_PTR SeqProcessFilePath = M_NULL;
   ProcessingHookDataStruct ProcessingUserHookData;
   EncodingFrameEndHookDataStruct EncodingFrameEndUserHookData;
   DecodingFrameEndHookDataStruct DecodingFrameEndUserHookData;
   MIL_INT SeqSystemType = M_NULL;

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay,
                                                &MilDigitizer, &MilImageDisp);

   MsysInquire(MilSystem, M_OWNER_APPLICATION, &MilRemoteApplication);
   MilSystemLocation = MsysInquire(MilSystem, M_LOCATION, M_NULL);
   // Check if the system is running under Windows
   if(MappInquire(MilRemoteApplication, M_PLATFORM_OS_TYPE, M_NULL) != M_OS_WINDOWS)
      {
      if(MilSystemLocation == M_REMOTE)
         MosPrintf(MIL_TEXT("The Distributed MIL server must run on a Windows system.\n"));
      else
         MosPrintf(MIL_TEXT("This example only works with a Windows system.\n"));

      MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
      MosGetch();

      MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
      return 0;

      }


   /* Inquire MIL licenses. */
   
   MappInquire(MilRemoteApplication, M_LICENSE_MODULES, &LicenseModules);

   if (!(LicenseModules & (M_LICENSE_JPEGSTD)))
      {
      MosPrintf(MIL_TEXT("Need a Compression/Decompression license to run this example.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
      MosGetch();

      MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
      return 0;
      }

     
   /* Allocate the grab buffers and clear them. */
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   for(MilGrabBufferListSize = 0;
       MilGrabBufferListSize < BUFFERING_SIZE_MAX;
       MilGrabBufferListSize++
      )
      {
      MbufAllocColor(MilSystem, MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL),
                     MdigInquire(MilDigitizer, M_SIZE_X, M_NULL),
                     MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL),
                     8+M_UNSIGNED, M_IMAGE+M_GRAB+M_PROC,
                     &MilGrabBufferList[MilGrabBufferListSize]
                    );

      if (MilGrabBufferList[MilGrabBufferListSize])
         {
         MbufClear(MilGrabBufferList[MilGrabBufferListSize], 0xFF);
         }
      else
         break;
      }
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   /* Free buffers to leave space for possible temporary buffers. */
   for (n=0; n<2 && MilGrabBufferListSize; n++)
      {
      MilGrabBufferListSize--;
      MbufFree(MilGrabBufferList[MilGrabBufferListSize]);
      }

   if(MilGrabBufferListSize == 0)
      {
      MosPrintf(MIL_TEXT("!!! No grab buffers have been allocated. Need to set more Non-Paged Memory. !!!\n"));

      MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
      MosGetch();
      return 1;
      }

   /* Initialize the User's processing function data structure only for Display. */
   ProcessingUserHookData.MilDigitizer        = MilDigitizer;
   ProcessingUserHookData.MilSeqContext       = M_NULL;
   ProcessingUserHookData.MilImageDisp        = MilImageDisp;
   ProcessingUserHookData.ProcessedImageCount = 0;
   ProcessingUserHookData.ProcessingOperation = DISPLAY;

   /* Start a digProcess to show the live camera output. */
   MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, M_START, M_DEFAULT, ProcessingFunction, &ProcessingUserHookData);

   /* Print a message. */
   MosPrintf(MIL_TEXT("\nH.264 IMAGE SEQUENCE COMPRESSION.\n"));
   MosPrintf(MIL_TEXT("---------------------------------\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to start compression.\r"));
   MosGetch();

   /* Stop digProcess. */
   MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, M_STOP, M_DEFAULT, ProcessingFunction, &ProcessingUserHookData);

   /* Inquire the dig process frame rate */
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_RATE,   &EncodingDesiredFrameRate);
   MosPrintf(MIL_TEXT("Grabbing frames at  %.2f frames/sec.\n"), EncodingDesiredFrameRate);

   /* Creates a context for the H.264 compression engine. Compression will be done
      using hardware or software depending on the system hardware configuration. 
    */
   MseqAlloc(MilSystem, M_DEFAULT, M_SEQ_COMPRESS, M_DEFAULT, 
                        M_DEFAULT, &MilCompressContext);


   /* Specify the destination of the compressed file and the target container type.
      The last argument specifies to generate an MP4 file.                          
   */
   MseqDefine( MilCompressContext, M_SEQ_OUTPUT(0) + M_SEQ_DEST(0), M_FILE, 
               (MilSystemLocation != M_REMOTE ? SEQUENCE_FILE : REMOTE_SEQUENCE_FILE),
               M_FILE_FORMAT_MP4);


   /* Set the compression parameters. */
   /* Sets the compression parameters valid for any resolution under 1920X1080.
      Any resolution higher than that will generate an warning that can be disabled using
      MseqControl with M_SETTING_AUTO_ADJUSTMENT. See documentation for more details.
    */
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_BIT_RATE_MODE,   M_VARIABLE);      // M_VARIABLE or M_CONSTANT
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_BIT_RATE,        5000);            // 5 Mbps bit rate
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_BIT_RATE_MAX,    5000);            // 5 Mbps bit rate
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_FRAME_RATE,      EncodingDesiredFrameRate);
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_FRAME_RATE_MODE, M_VARIABLE);      // Attempts to update the file header with the encoding frame rate
                                                                                          // if lower than the specified frame rate.
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_QUALITY,         100);             // 1=best speed, 100=best quality
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_PROFILE,         M_PROFILE_HIGH);  // M_PROFILE_BASELINE, M_PROFILE_MAIN, M_PROFILE_HIGH
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_LEVEL,           M_LEVEL_4_2);     // M_LEVEL_1, M_LEVEL_1B, M_LEVEL_1_1, M_LEVEL_1_2, M_LEVEL_1_3,
                                                                                          // M_LEVEL_2, M_LEVEL_2_1, M_LEVEL_2_2,
                                                                                          // M_LEVEL_3, M_LEVEL_3_1, M_LEVEL_3_2,
                                                                                          // M_LEVEL_4, M_LEVEL_4_1, M_LEVEL_4_2,
                                                                                          // M_LEVEL_5, M_LEVEL_5_1
   MseqControl(MilCompressContext, M_CONTEXT, M_STREAM_GROUP_OF_PICTURE_SIZE, 30);        // Interval between I-Frame

   /* Initialize the optional encoding end function data structure. */
   EncodingFrameEndUserHookData.EncodedImageCount = 0;

   /* Register the encoding end function to the sequence context. */
   MseqHookFunction(MilCompressContext, M_FRAME_END, FrameEncodingEndFunction, 
                                                     &EncodingFrameEndUserHookData);

   /* Provide a sample image to initialize the encoding engine accordingly. */
   MseqControl(MilCompressContext, M_CONTEXT, M_BUFFER_SAMPLE, MilGrabBufferList[0]);
   
   /* Start the encoding process, waits for buffer to be fed for encoding. */
   MseqProcess(MilCompressContext, M_START, M_ASYNCHRONOUS);

   /* Display the type of compression used. */
   MosPrintf(MIL_TEXT("Live image capture and compression to file using "));
   MseqInquire(MilCompressContext, M_CONTEXT, M_CODEC_TYPE, &SeqSystemType);
   if (SeqSystemType == M_HARDWARE+M_QSV)
      MosPrintf(MIL_TEXT("Hardware acceleration.\n") );
   else // M_SOFTWARE + M_QSV
      MosPrintf(MIL_TEXT("Software implementation.\n") );

   /* Set the sequence context id in the user hook data structure to start
      feeding buffers for encoding in ProcessingFunction.
   */
   ProcessingUserHookData.MilSeqContext = MilCompressContext;
   ProcessingUserHookData.ProcessedImageCount = 0;
   ProcessingUserHookData.ProcessingOperation = ENCODE;

   MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, M_START, M_DEFAULT, ProcessingFunction, &ProcessingUserHookData);

   /* NOTE: Now the main() is free to perform other tasks while the compression is executing. */
   /* --------------------------------------------------------------------------------------- */
   
   /* Print a message and wait for a key press after a minimum number of frames. */
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));
   MosGetch();

   /* Stop the processing. */
   MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize,
               M_STOP+M_WAIT, M_DEFAULT, ProcessingFunction, &ProcessingUserHookData);

   /* Stop the encoding process */
   MseqProcess(MilCompressContext, M_STOP, M_WAIT);

   /* Print statistics. */
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_COUNT,  &ProcessFrameCount);
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_RATE,   &ProcessFrameRate);
   MosPrintf(MIL_TEXT("%ld frames encoded at %.2f frames/sec (%.1f ms/frame).\n\n"),
                        ProcessFrameCount, ProcessFrameRate, 1000.0/ProcessFrameRate);
   MseqInquire(MilCompressContext, M_SEQ_OUTPUT(0) + M_SEQ_DEST(0), M_STREAM_FILE_NAME_SIZE, &SeqProcessFilePathSize );
   SeqProcessFilePath = new MIL_TEXT_CHAR[SeqProcessFilePathSize];
   MseqInquire(MilCompressContext, M_SEQ_OUTPUT(0) + M_SEQ_DEST(0), M_STREAM_FILE_NAME, SeqProcessFilePath );
   MosPrintf(MIL_TEXT("The video sequence file was written to:\n%s.\n\n"), SeqProcessFilePath);
   MosPrintf(MIL_TEXT("It can be played back using any compatible video player.\n")); 

   /* Free the grab buffers and sequence context. */
   while(MilGrabBufferListSize > 0)
      {
      MbufFree(MilGrabBufferList[--MilGrabBufferListSize]);
      MilGrabBufferList[MilGrabBufferListSize] = M_NULL;
      }
   if(SeqProcessFilePath)
      {
      delete [] SeqProcessFilePath;
      SeqProcessFilePath = M_NULL;
      }

   MseqFree(MilCompressContext);

   /* Wait for a key to start the replay. */
   MosPrintf(MIL_TEXT("Press <Enter> to replay encoded sequence.\n"));
   MosGetch();
   MseqAlloc(MilSystem, M_DEFAULT, M_SEQ_DECOMPRESS, M_DEFAULT, 
                        M_DEFAULT, &MilDecompressContext);

   /* Specify the destination of the compressed file and the target container type. 
      The last argument specifies to generate an MP4 file. 
   */
   MseqDefine( MilDecompressContext, M_SEQ_INPUT(0), M_FILE, 
               (MilSystemLocation != M_REMOTE ? SEQUENCE_FILE : REMOTE_SEQUENCE_FILE),
               M_FILE_FORMAT_MP4);

   MIL_DOUBLE outputFrameRate = 0.0;
   MseqInquire(MilDecompressContext, M_SEQ_INPUT(0), M_STREAM_FRAME_RATE, &outputFrameRate);
   MosPrintf(MIL_TEXT("\nReplaying file at %.2f frames/second.\n"), outputFrameRate);

   /* Initialize the optional decoding end function data structure. */
   DecodingFrameEndUserHookData.DecodedImageCount = 0;
   DecodingFrameEndUserHookData.MilImageDisp = MilImageDisp;

   /* Register the decoding end function to the sequence context. */
   MseqHookFunction(MilDecompressContext, M_FRAME_END, FrameDecodingEndFunction,
                                                       &DecodingFrameEndUserHookData);

   /* Start the decoding process, waits for buffer to be fed for encoding. */
   MseqProcess(MilDecompressContext, M_START, M_ASYNCHRONOUS);

   /* Print a message and wait for a key press after a minimum number of frames. */
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));
   MosGetch();

   /* Stop the play back. */
   MseqProcess(MilDecompressContext, M_STOP, M_NULL);
   MseqFree(MilDecompressContext);

   /* Wait for a key to end. */
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   /* Release defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);

   return 0;
}


/* User's processing function called every time a grab buffer is modified. */
/* ------------------------------------------------------------------------*/

/* Local defines. */
#define STRING_LENGTH_MAX  20
#define STRING_POS_X       20
#define STRING_POS_Y       20

MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
   {
   ProcessingHookDataStruct *UserHookDataPtr = (ProcessingHookDataStruct *)HookDataPtr;
   MIL_ID ModifiedBufferId;
   MIL_TEXT_CHAR Text[STRING_LENGTH_MAX]= {MIL_TEXT('\0'),};

   /* Retrieve the MIL_ID of the grabbed buffer. */
   MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &ModifiedBufferId);

   switch (UserHookDataPtr->ProcessingOperation)
      {
      case DISPLAY:
         /* Update the display with the last captured image. */
         MbufCopy(ModifiedBufferId, UserHookDataPtr->MilImageDisp);
         break;

      case ENCODE:
         /* Increase the compressed images count. */
         UserHookDataPtr->ProcessedImageCount++;

         /* Print and draw the frame count (comment to reduce CPU usage). */
         MosPrintf(MIL_TEXT("Processing frame #%d.\r"), UserHookDataPtr->ProcessedImageCount);
         MosSprintf(Text, STRING_LENGTH_MAX, MIL_TEXT("%ld"), UserHookDataPtr->ProcessedImageCount);
         MgraText(M_DEFAULT, ModifiedBufferId, STRING_POS_X, STRING_POS_Y, Text);

         /* Enqueue the grabbed buffer for parallel encoding. */
         MseqFeed(UserHookDataPtr->MilSeqContext, ModifiedBufferId, M_DEFAULT);

         /* Update the display with the last captured image. */
         MbufCopy(ModifiedBufferId, UserHookDataPtr->MilImageDisp);
         break;
      }
   return 0;
   }


/* Optional encoding end function called every time a buffer is finished being compressed. */
/* ----------------------------------------------------------------------------------------*/
MIL_INT MFTYPE FrameEncodingEndFunction(  MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
   {
   EncodingFrameEndHookDataStruct * UserHookDataPtr = (EncodingFrameEndHookDataStruct *)HookDataPtr;

   /* Frame end hook post processing. */
   if(HookType == M_FRAME_END)
      {
      MIL_ID CompressedBufferId;
      void* CompressedDataPtr = M_NULL;
      MIL_INT CompressedDataSize = 0;

      /* Increment a encoded frame counter */
      UserHookDataPtr->EncodedImageCount++;

      /* Retrieve the MIL_ID of the encoded buffer. */
      MseqGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &CompressedBufferId );

      /* Retrieves the address of the encoded data. */
      MbufInquire(CompressedBufferId, M_HOST_ADDRESS, &CompressedDataPtr);

      /* Retrieves the size in bytes of the encoded data. */
      MbufInquire(CompressedBufferId, M_SIZE_BYTE, &CompressedDataSize);

      /* -----------------------------------------------------------------------------------------------*/
      /* Here you can do any action with the encoded data such as send buffer through a network stream. */
      /* If the processing done on the compressed data is long, it is recommended to copy the           */
      /* buffer and to process it in a separate thread to avoid blocking the compressions flow.         */
      /* -----------------------------------------------------------------------------------------------*/

      }

   return 0;
   }

/* Optional decoding end function called every time a buffer is finished being decompressed. */
/* ----------------------------------------------------------------------------------------*/
MIL_INT MFTYPE FrameDecodingEndFunction(  MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
   {
   DecodingFrameEndHookDataStruct * UserHookDataPtr = (DecodingFrameEndHookDataStruct *)HookDataPtr;

   /* Frame end hook post processing. */
   if(HookType == M_FRAME_END)
      {
      MIL_ID DecompressedBufferId;

      /* Increment a encoded frame counter */
      UserHookDataPtr->DecodedImageCount++;

      /* Retrieve the MIL_ID of the encoded buffer. */
      MseqGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &DecompressedBufferId );

      /* -----------------------------------------------------------------------------------------------*/
      /* Here you can do any action with the decoded buffer.                                            */
      /* -----------------------------------------------------------------------------------------------*/

      MbufCopy(DecompressedBufferId, UserHookDataPtr->MilImageDisp);
      }

   return 0;
   }
