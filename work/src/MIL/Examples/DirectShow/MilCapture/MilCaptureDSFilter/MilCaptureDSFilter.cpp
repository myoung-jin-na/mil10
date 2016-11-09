/*************************************************************************************/
/* 
 * File name:  MilCaptureDSFilter.cpp
 *
 * Synopsis:   This program creates a DirectShow graph, adds a MilCapture filter to
 *             the graph and render the graph. It also shows how to use the DirectShow
 *             interfaces to control and inquire system.
 *
 * Requirements: If using Visual Studio 2005:
 *                   Windows SDK (http://msdn.microsoft.com/en-us/bb980924.aspx)
 *                   Run the Windows SDK Configuration tool (http://blogs.msdn.com/b/windowssdk/archive/2008/03/01/integrating-windows-sdk-and-vs-with-new-sdk-configuration-tool.aspx)
 *                   Patch for Visual Studio 2005 SP1 available from: http://support.microsoft.com/kb/949009/.
 *
 */

#include <mil.h>

#include <dshow.h>

/* Includes all the necessary interface for IMilCapture and IMilCapturePin */
#include "MilDSFCapture.h"

#include "MilCaptureDSFilter.h"


/* Function declaration */
MIL_INT MFTYPE GrabEndFunction(MIL_ID DigitizerId, MIL_INT HookType, void* UserDataPtr);

/* User's processing function hook data structure. */
typedef struct
   {
   MIL_INT ProcessedImageCount;
   } HookDataStruct;


/* Main function.*/
/* --------------*/

int MosMain(void)
   {
   HRESULT hr = S_OK;

   HookDataStruct grabEndHookData;

   /* DirectShow interfaces.*/
   IMilCapture*      pMilCapture    = NULL;  /* MIL system DirectShow interface.    */
   IMilCapturePin*   pMilCapturePin = NULL;  /* MIL digitizer DirectShow interface. */

   IBaseFilter*      pVMR9filter     = NULL;  /* Enhanced video renderer filter */

   IQualProp*        pQualRenderer = NULL;   /* Interface for renderer quality informations */

   IGraphBuilder*    pGraphBuilder  = NULL;  /* DirectShow FilterGraph building utility.  */
   IMediaControl*    pMediaControl  = NULL;  /* DirectShow FilterGraph control.           */

   MosPrintf(MIL_TEXT("\nMatrox Capture DirectShow Filter:\n"));
   MosPrintf(MIL_TEXT("---------------------------------\n\n"));

   hr = CoInitialize(NULL);

   if(SUCCEEDED(hr))
      {
      /* Allocate a FilterGraph Builder.*/
      hr = CoCreateInstance(  CLSID_FilterGraph,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_IGraphBuilder,
                              (void**)&pGraphBuilder);

         /* Gets FilterGraph Control Interface.*/
         if(SUCCEEDED(hr))
            hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
         else
            MosPrintf(MIL_TEXT("\nERROR\n\nFailed to initialize IMediaControl Interface\n"));
      }
   else
      MosPrintf(MIL_TEXT("\nERROR\n\nFailed to initialize DirectShow FilterGraph Builder\n"));

   /* Catch the possible error caught while getting FilterGraph Builder.*/
   CHECK_FOR_ERROR(hr);
   
   /* Building the FilterGraph and rendering pin.*/
   /* -------------------------------------*/

   /* Try allocating Video Mixing Renderer 9 if available */
   hr = CoCreateInstance(  CLSID_VideoMixingRenderer9,
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           IID_IBaseFilter,
                           (void**)&pVMR9filter);

   if(SUCCEEDED(hr))
      {
      /* Adds Video Mixing Renderer 9 filter to the FilterGraph. */
      pGraphBuilder->AddFilter(pVMR9filter, L"Video Mixing Renderer 9");
      }


   /* Allocate the MilCapture Filter (No MIL system are allocated).*/
   hr = CoCreateInstance(  CLSID_MilCapture,
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           IID_IMilCapture,
                           (void**)&pMilCapture);

   if(SUCCEEDED(hr))
      {
      /* Set the desired information about the system before it is allocated (Internal MIL system).*/
      /* MUST be set before adding the filter to the FilterGraph, if not it will use defaults values.*/
      pMilCapture->setSystemInfo(MIL_TEXT("M_SYSTEM_DEFAULT"), M_DEFAULT);

      /* Adds the filter to the FilterGraph (MIL system allocation is done here).*/
      pGraphBuilder->AddFilter(pMilCapture, NULL);
      MosPrintf(MIL_TEXT("Matrox Capture Filter Added to the FilterGraph\n"));

      /* Find last added pin in the filter (one pin per digitizer).*/
      /* Each time a pin is connected a new one is created using default values.*/
      IPin* pIPin = NULL;
      pMilCapture->FindPin(L"default", &pIPin);

      /* Gets the MilCapturePin interface from the pin.*/
      if ( pIPin )
         hr = pIPin->QueryInterface(IID_IMilCapturePin, (void**)&pMilCapturePin);
      else
         {
         hr = E_FAIL;
         MosPrintf(MIL_TEXT("\nERROR\n\nFailed to find desired Pin\n"));
         }

      CHECK_FOR_ERROR(hr);

      HELPER_RELEASE(pIPin);

      /* Set the desired information about the pin (digitizer).*/
      /* The digitizer is allocated only when the pin gets connected.*/
      /* The pin changes name depending on the device number.*/
      pMilCapturePin->set_DigitizerInfo(M_DEFAULT, M_DEFAULT, MIL_TEXT("M_DEFAULT"));

      /* Sets the priority list order depending on your preference.*/
      /* It is possible to set only the preferred format at position 0 to use only this one.*/
      pMilCapturePin->Set_CaptureBufferFormatPriority(CBF_MONO8, 0);
      pMilCapturePin->Set_CaptureBufferFormatPriority(CBF_YUV16_YUYV, 1);
      pMilCapturePin->Set_CaptureBufferFormatPriority(CBF_BGR32, 2);

      pMilCapturePin->Set_PinMediaSubTypePriority(MF_RGB32, 0);
      pMilCapturePin->Set_PinMediaSubTypePriority(MF_YUV16, 1);

      /* The FilterGraph tries to connect the filter to get an output.*/
      hr = pGraphBuilder->Render(pMilCapturePin);

      /* Output the name of the renderer filter connected to the capture output pin*/
      IPin* connectedPin = NULL;
      PIN_INFO connectedPinInfo;
      FILTER_INFO connectedPinFilterInfo;
      pMilCapturePin->ConnectedTo(&connectedPin);
      connectedPin->QueryPinInfo(&connectedPinInfo);
      connectedPinInfo.pFilter->QueryFilterInfo(&connectedPinFilterInfo);
      MosPrintf(MIL_TEXT("Pins are connected to render filter called (%s).\n\n"), connectedPinFilterInfo.achName);
      MosPrintf(MIL_TEXT("**********************************************************************\n"));
      MosPrintf(MIL_TEXT("Renderer performance can vary depending on your system specifications.\n"));
      MosPrintf(MIL_TEXT("If you encounter any problem on the display,\n"));
      MosPrintf(MIL_TEXT("we suggest using another renderer if available.\n"));
      MosPrintf(MIL_TEXT("**********************************************************************\n\n"));

      connectedPinInfo.pFilter->QueryInterface(IID_IQualProp, (void**)&pQualRenderer);

      HELPER_RELEASE(connectedPinFilterInfo.pGraph);
      HELPER_RELEASE(connectedPinInfo.pFilter);
      HELPER_RELEASE(connectedPin);


      /* Functions mapping any MIL command should be called after connecting the pin.*/
      /* ----------------------------------------------------------------------------*/

      /* Registering a hook function.*/
      grabEndHookData.ProcessedImageCount = 0;
      pMilCapturePin->digHookFunction(M_GRAB_END, GrabEndFunction, &grabEndHookData);

      /* System related var to inquire.*/
      MIL_INT sysDevNum = -1;

      /* Digitizer related var to inquire.*/
      MIL_INT digCurChannelNum = NULL;
      MIL_INT digChannelNum = NULL;
      MIL_INT digSizeX = NULL;
      MIL_INT digSizeY = NULL;
      MIL_DOUBLE digScaleX = 0.0;
      MIL_DOUBLE digScaleY = 0.0;

      /* Accessing system related information s(MsysControl and MsysInquire).*/
      hr = pMilCapture->sysInquire(M_NUMBER, &sysDevNum);

      MIL_INT sysDescriptorNameLen = 0;
      hr = pMilCapture->sysInquire(M_SYSTEM_DESCRIPTOR_SIZE, &sysDescriptorNameLen);
      MIL_TEXT_PTR sysDescriptorName = new MIL_TEXT_CHAR[sysDescriptorNameLen+2];
      hr = pMilCapture->sysInquire(M_SYSTEM_DESCRIPTOR, sysDescriptorName);

      /* Accessing digitizer related informations (MdigControl and MdigInquire).*/
      MIL_INT digFormatLen = 0;
      hr = pMilCapturePin->digInquire(M_FORMAT_SIZE, &digFormatLen);
      MIL_TEXT_PTR digFormat = new MIL_TEXT_CHAR[digFormatLen+2];
      hr = pMilCapturePin->digInquire(M_FORMAT, digFormat);

      hr = pMilCapturePin->digInquire(M_CHANNEL_NUM, &digChannelNum);
      hr = pMilCapturePin->digInquire(M_SIZE_X, &digSizeX);
      hr = pMilCapturePin->digInquire(M_SIZE_Y, &digSizeY);
      hr = pMilCapturePin->digInquire(M_GRAB_SCALE_X, &digScaleX);
      hr = pMilCapturePin->digInquire(M_GRAB_SCALE_Y, &digScaleY);

      /* Prints informations to the console.*/
      MosPrintf(MIL_TEXT("\nSystem Information:\n"));
      MosPrintf(MIL_TEXT("System descriptor: %s (%d)\n"), sysDescriptorName, sysDevNum);
      MosPrintf(MIL_TEXT("Digitizer format: %s\n"), digFormat);
      MosPrintf(MIL_TEXT("Total Number of Channel : %d\n"), digChannelNum);
      MosPrintf(MIL_TEXT("Grab Size : %dx%d\n"), digSizeX, digSizeY);
      MosPrintf(MIL_TEXT("Grab Scale X: %f \tY: %f\n"), digScaleX, digScaleY);
      MosPrintf(MIL_TEXT("\n"));

      delete [] sysDescriptorName;
      delete [] digFormat;
      }
   else
      {
      MosPrintf(MIL_TEXT("\nERROR\n\nFailed to instantiate MilCaptureFilter\n"));
      }

   CHECK_FOR_ERROR(hr);


   MosPrintf(MIL_TEXT("FilterGraph is running.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));

   MosPrintf(MIL_TEXT("| Buffer ID | Grab frame |    Avg.    | Frame dropped |\n"));
   MosPrintf(MIL_TEXT("|    used   |    count   | frame rate |  by renderer  |\n"));


   /* Starts the FilterGraph.*/
   pMediaControl->Run();

   bool endloop = false;
   bool printedFrameRate = false;

   while (!endloop)
      {
      if( !printedFrameRate && grabEndHookData.ProcessedImageCount % 10 == 0)
         {
         int framerate = 0;
         int totalFrameDropped = 0;
         if(pQualRenderer)
            {
            pQualRenderer->get_AvgFrameRate(&framerate);
            pQualRenderer->get_FramesDroppedInRenderer(&totalFrameDropped);
            MosPrintf(MIL_TEXT("| %10.2f | %13d |"), framerate/100.0, totalFrameDropped);
            }
         printedFrameRate = true;
         }
      if(printedFrameRate && grabEndHookData.ProcessedImageCount % 10 != 0)
         {
         printedFrameRate = false;
         }

      if(pVMR9filter)
         {
         MSG msg;
         while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
            DispatchMessage(&msg);
            }
         }

      if(MosKbhit())
         {
         endloop = true;
         }
      }

   pMediaControl->Stop();

   HELPER_RELEASE(pVMR9filter);

   CleanUpDirectShow(&pMilCapture, &pMilCapturePin, &pGraphBuilder, &pMediaControl);

   return 0;
   }


/* Helper function to simplify releasing interfaces.*/
/* -------------------------------------------------*/

void CleanUpDirectShow( IMilCapture** pMilCapture, IMilCapturePin** pMilCapturePin, IGraphBuilder** pGraphBuilder, IMediaControl** pMediaControl )
   {
   HELPER_RELEASE((*pMilCapturePin));
   HELPER_RELEASE((*pMilCapture))
   HELPER_RELEASE((*pMediaControl));
   HELPER_RELEASE((*pGraphBuilder));
   CoUninitialize();
   }


/* Hook Function on M_GRAB_END.*/
/* ----------------------------*/

MIL_INT MFTYPE GrabEndFunction(MIL_ID DigitizerId, MIL_INT HookType, void* UserDataPtr)
   {
   /* Display the number frame grabbed.*/
   /* -------------------------------------------------------------*/
   HookDataStruct* pUserData = (HookDataStruct*)UserDataPtr;

   MIL_ID ModifiedBufferId = M_NULL;
   if(HookType != M_NULL)
      {
      MdigGetHookInfo(HookType, M_MODIFIED_BUFFER+M_BUFFER_ID, &ModifiedBufferId);
      }
   MosPrintf(MIL_TEXT("\r|%10d | %10d "), ModifiedBufferId, pUserData->ProcessedImageCount++);

   return 0;
   }
