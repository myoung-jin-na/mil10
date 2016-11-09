/*****************************************************************************************/
/*
 * File name: MFunc.cpp 
 *
 * Synopsis:  This example shows the use of the MIL Function Developer Tool Kit and how 
 *            MIL and custom code can be mixed to create a custom MIL function that 
 *            accesses the data pointer of a MIL buffer directly in order to process it.
 *
 *            The example creates a Master MIL function that registers all the parameters 
 *            to MIL and calls the Slave function. The Slave function retrieves all the 
 *            parameters, gets the pointers to the MIL image buffers, uses them to access 
 *            the data directly and adds a constant.
 *             
 *            Note: The images must be 8-bit unsigned and have a valid Host pointer.
 */
#include <mil.h>

/* MIL function specifications. */
#define FUNCTION_NB_PARAM                 4
#define FUNCTION_OPCODE_ADD_CONSTANT      1 
#define FUNCTION_PARAMETER_ERROR_CODE     1
#define FUNCTION_SUPPORTED_IMAGE_TYPE     (8+M_UNSIGNED)

/* Target image file name. */
#define IMAGE_FILE M_IMAGE_PATH MIL_TEXT("BoltsNutsWashers.mim")

/* Master and Slave MIL functions declarations. */
MIL_INT AddConstant(MIL_ID SrcImageId, MIL_ID DstImageId, MIL_INT ConstantToAdd);
void MFTYPE SlaveAddConstant(MIL_ID Func);


/* Master MIL Function definition. */
/* ------------------------------- */

MIL_INT AddConstant(MIL_ID SrcImageId, MIL_ID DstImageId, MIL_INT ConstantToAdd)
{
   MIL_ID   Func;
   MIL_INT  SlaveReturnValue = 0;

   /* Allocate a MIL function context that will be used to call a target 
      Slave function locally on the Host to do the processing.
   */
   MfuncAlloc(MIL_TEXT("AddConstant"), 
              FUNCTION_NB_PARAM,
              SlaveAddConstant, M_NULL, M_NULL, 
              M_USER_MODULE_1+FUNCTION_OPCODE_ADD_CONSTANT, 
              M_LOCAL+M_SYNCHRONOUS_FUNCTION, 
              &Func
             );

   /* Register the parameters. */
   MfuncParamMilId( Func, 1, SrcImageId, M_IMAGE, M_IN);
   MfuncParamMilId( Func, 2, DstImageId, M_IMAGE, M_OUT);
   MfuncParamMilInt(Func, 3, ConstantToAdd);
   MfuncParamDataPointer(Func, 4, &SlaveReturnValue, sizeof(MIL_INT), M_OUT);

   /* Call the target Slave function. */
   MfuncCall(Func);

   /* Free the MIL function context. */
   MfuncFree(Func);
   
   /* Return the value recieved from the Slave function. */
   return SlaveReturnValue;
}


/* MIL Slave function definition. */
/* ------------------------------ */

void MFTYPE SlaveAddConstant(MIL_ID Func)
{
  MIL_ID    SrcImageId, DstImageId;
  MIL_INT   ConstantToAdd, TempValue;
  unsigned  char *SrcImageDataPtr, *DstImageDataPtr;
  MIL_INT   SrcImageSizeX, SrcImageSizeY, SrcImageType, SrcImagePitchByte;
  MIL_INT   DstImageSizeX, DstImageSizeY, DstImageType, DstImagePitchByte;
  MIL_INT   x, y;
  MIL_INT  *SlaveReturnValuePtr;

  /* Read the parameters. */
  MfuncParamValue(Func, 1, &SrcImageId);
  MfuncParamValue(Func, 2, &DstImageId);
  MfuncParamValue(Func, 3, &ConstantToAdd); 
  MfuncParamValue(Func, 4, &SlaveReturnValuePtr); 

  /* Lock buffers for direct access. */
  MbufControl(SrcImageId, M_LOCK, M_DEFAULT);
  MbufControl(DstImageId, M_LOCK, M_DEFAULT);

  /* Read image information. */
  MbufInquire(SrcImageId, M_HOST_ADDRESS, &SrcImageDataPtr);
  MbufInquire(SrcImageId, M_SIZE_X,       &SrcImageSizeX);
  MbufInquire(SrcImageId, M_SIZE_Y,       &SrcImageSizeY);
  MbufInquire(SrcImageId, M_TYPE,         &SrcImageType);
  MbufInquire(SrcImageId, M_PITCH_BYTE,   &SrcImagePitchByte);
  MbufInquire(DstImageId, M_HOST_ADDRESS, &DstImageDataPtr);
  MbufInquire(DstImageId, M_SIZE_X,       &DstImageSizeX);
  MbufInquire(DstImageId, M_SIZE_Y,       &DstImageSizeY);
  MbufInquire(DstImageId, M_TYPE,         &DstImageType);
  MbufInquire(DstImageId, M_PITCH_BYTE,   &DstImagePitchByte);

  /* Reduce the destination area to process if necessary. */
  if (SrcImageSizeX < DstImageSizeX)   DstImageSizeX = SrcImageSizeX;
  if (SrcImageSizeY < DstImageSizeY)   DstImageSizeY = SrcImageSizeY;

  /* If images have the proper type and a valid host pointer,
     execute the operation using custom C code.
   */
  if ((SrcImageType == DstImageType) && (SrcImageType == FUNCTION_SUPPORTED_IMAGE_TYPE) &&
      (SrcImageDataPtr != M_NULL) && (DstImageDataPtr != M_NULL)
     )
     {
     /* Add the constant to the image. */
     for (y= 0; y < DstImageSizeY; y++)
        {
        for (x= 0; x < DstImageSizeX; x++) 
           {
           /* Calculate the value to write. */
           TempValue = (MIL_INT)SrcImageDataPtr[x] + (MIL_INT)ConstantToAdd;

           /* Write the value if no overflow, else saturate. */
           if (TempValue <= 0xff)
              DstImageDataPtr[x] = (unsigned char)TempValue;
           else 
              DstImageDataPtr[x] = 0xff;
           }

         /* Move pointer to the next line taking into account the image's pitch. */
         SrcImageDataPtr += SrcImagePitchByte;
         DstImageDataPtr += DstImagePitchByte;
        }
     
     /* Return a null error code to the Master function. */
     *SlaveReturnValuePtr = M_NULL;
     }
  else 
     {
     /* Buffer cannot be processed. Report an error. */ 
     MfuncErrorReport(Func,M_FUNC_ERROR+FUNCTION_PARAMETER_ERROR_CODE,
                      MIL_TEXT("Invalid parameter."),
                      MIL_TEXT("Image type not supported or invalid target system."),
                      MIL_TEXT("Image must be 8-bit and have a valid host address."),
                      M_NULL
                     );

     /* Return an error code to the Master function. */
     *SlaveReturnValuePtr = M_FUNC_ERROR+FUNCTION_PARAMETER_ERROR_CODE;
     }

  /* Unlock buffers. */
  MbufControl(SrcImageId, M_UNLOCK, M_DEFAULT);
  MbufControl(DstImageId, M_UNLOCK, M_DEFAULT);

  /* Signal to MIL that the destination buffer was modified. */
  MbufControl(DstImageId, M_MODIFIED, M_DEFAULT); 
}


/* Main to test the custom function. */
/* --------------------------------- */

int MosMain(void)
{
   MIL_ID  MilApplication,  /* Application identifier.       */
           MilSystem,       /* System identifier.            */ 
           MilDisplay,      /* Display identifier.           */
           MilImage;        /* Image buffer identifier.      */
   MIL_INT ReturnValue;     /* Return value of the function. */

   /* Allocate default application, system, display and image. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Load source image into a Host memory image buffer. */
   MbufAlloc2d(MilSystem, 
               MbufDiskInquire(IMAGE_FILE, M_SIZE_X, M_NULL), 
               MbufDiskInquire(IMAGE_FILE, M_SIZE_Y, M_NULL), 
               8+M_UNSIGNED, 
               M_IMAGE+M_DISP+M_HOST_MEMORY,
               &MilImage);   
   MbufLoad(IMAGE_FILE, MilImage);
   
   /* Display the image. */
   MdispSelect(MilDisplay, MilImage);
   
   /* Pause. */
   MosPrintf(MIL_TEXT("\nMIL FUNCTION DEVELOPER'S TOOLKIT:\n"));
   MosPrintf(MIL_TEXT("---------------------------------\n\n"));
   MosPrintf(MIL_TEXT("This example creates a custom MIL function that processes\n"));
   MosPrintf(MIL_TEXT("an image using its data pointer directly.\n\n"));
   MosPrintf(MIL_TEXT("Target image was loaded.\n"));
   MosPrintf(MIL_TEXT("Press a key to continue.\n\n"));
   MosGetch();

   /* Run the custom function only if the target system's memory is local and accessible. */
   if (MsysInquire(MilSystem, M_LOCATION, M_NULL) == M_LOCAL)
      {
      /* Process the image directly with the custom MIL function. */
      ReturnValue = AddConstant(MilImage, MilImage, 0x40);
   
      /* Print a conclusion message. */
      if (ReturnValue == M_NULL)
         MosPrintf(MIL_TEXT("The white level of the image was augmented.\n"));
      else
         MosPrintf(MIL_TEXT("An error was returned by the Slave function.\n"));
      }
   else
      {
      /* Print that the example don't run remotely. */
      MosPrintf(MIL_TEXT("This example doesn't run with Distributed MIL.\n"));
      }
   
   /* Wait for a key to terminate. */
   MosPrintf(MIL_TEXT("Press a key to terminate.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(MilImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}
