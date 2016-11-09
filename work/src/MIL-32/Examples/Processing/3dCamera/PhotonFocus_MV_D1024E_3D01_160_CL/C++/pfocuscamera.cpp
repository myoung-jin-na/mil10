/*****************************************************************************/
/* 
 * File name: pfocuscamera.cpp
 *
 * Synopsis: All camera-specific code
 *            
 */

#include "cameraconfig.h"    /* User options - change them to suit your needs. */

#include <mil.h>
#include "mil3dcam_pfocus.h" /* Parameter struct definition.                   */
#include "pfocuscamera.h"    /* Prototypes to interact with PhotonFocus camera.*/

#if !USE_REAL_CAMERA

/*
 * We implement stub functions for the camera API for cleaner main code.
 * The only important function here is FillParams(), which fill the conversion structure
 * with hard-coded values that corresponds to the mode used to obtain profile data
 * in the saved images.
 */

MIL_INT InitCamera(int /*Port*/)
   {
   return PFOCUS_CAMERA_OK;
   }

MIL_INT FillParams(int /*Port*/, PFocusParams* pConvParams)
   {
   pConvParams->DataResolution        = 8;
   pConvParams->StatusLine            = STATUS_LINE_DISABLED;
   pConvParams->CycleTimeMicroseconds = 16715;

   return PFOCUS_CAMERA_OK;
   }

MIL_INT SwitchCameraToImage(int     /*Port      */,
                            MIL_INT /*ROIOffsetX*/,
                            MIL_INT /*ROIOffsetY*/,
                            MIL_INT /*ROISizeX  */,
                            MIL_INT /*ROISizeY  */)
   {
   return PFOCUS_CAMERA_OK;
   }

MIL_INT SwitchCameraToMeasure(int     /*Port      */,
                              MIL_INT /*ROIOffsetX*/,
                              MIL_INT /*ROIOffsetY*/,
                              MIL_INT /*ROISizeX  */,
                              MIL_INT /*ROISizeY  */)
   {
   return PFOCUS_CAMERA_OK;
   }

void CloseDown(int /*Port*/)
   {

   }

#else

/* Link with PhotonFocus library. */
#if M_MIL_USE_64BIT
   #error PhotonFocus library is not supported under 64 bits platforms.
#else
   #ifdef _MSC_VER
      #pragma comment(lib, "pfcam.lib")
   #else
      #error Unsupported compiler for this example. Include lib manually.
   #endif
#endif

#include "pfcam.h"      /* Include the PhotonFocus API. */

/*****************************************************************************
 Defines and constants.
*****************************************************************************/

/* Basic camera parameters. */
#define CAMERA_COM_PORT_SPEED    57600

/* Helper function prototypes. */
MIL_INT SwitchCameraMode(int     Port      ,
                         MIL_INT Mode      ,
                         MIL_INT ROIOffsetX,
                         MIL_INT ROIOffsetY,
                         MIL_INT ROISizeX  ,
                         MIL_INT ROISizeY  );
void HandleError(int Error);
MIL_INT GetPropertyString(int Port, const char* pPropertyName, char* pOut, MIL_INT OutLen);
MIL_INT GetPropertyInt   (int Port, const char* pPropertyName, MIL_INT* pOut    );
MIL_INT GetPropertyFloat (int Port, const char* pPropertyName, float*   pOut    );
MIL_INT SetPropertyInt   (int Port, const char* pPropertyName, MIL_INT  IntValue);

static const char*   SUPPORTED_CAMERA_NAME   = "MV-D1024E-3D01-160";
static const MIL_INT EXPECTED_DECIMATION     = 1;
static const MIL_INT EXPECTED_MROI_SETTING   = 0;
static const MIL_INT EXECUTE_RESET           = 1;
static const MIL_INT RESOLUTION_12BITS_VALUE = 0;
static const MIL_INT RESOLUTION_10BITS_VALUE = 1;
static const MIL_INT RESOLUTION_8BITS_VALUE  = 2;
static const MIL_INT OUTPUT_2D_ONLY          = 0;
static const MIL_INT OUTPUT_2D_AND_3D        = 1;
static const MIL_INT OUTPUT_3D_ONLY          = 2;

static const char* CAMERA_NAME_PROPNAME      = "CameraName";
static const char* DECIMATION_PROPNAME       = "Decimation.Y";
static const char* MROI_PROPNAME             = "MROI.Enable";
static const char* RESET_PROPNAME            = "Reset";
static const char* RESOLUTION_PROPNAME       = "DataResolution";
static const char* STATUS_LINE_PROPNAME      = "EnStatusLine";
static const char* FRAME_TIME_PROPNAME       = "FrameTime";
static const char* DETECTOR_MODE_PROPNAME    = "PeakDetector.Mode";
static const char* ROI_OFFSET_X_PROPNAME     = "Window.X";
static const char* ROI_OFFSET_Y_PROPNAME     = "Window.Y";
static const char* ROI_SIZE_X_PROPNAME       = "Window.W";
static const char* ROI_SIZE_Y_PROPNAME       = "Window.H";

/*****************************************************************************
 API function implementations.
*****************************************************************************/

/* Initialize the camera and verify that its current settings are supported. */
MIL_INT InitCamera(int Port)
   {
   int Ret;
   MIL_INT Ret2;

   /* Initialize com ports. */
   int NbPorts;
   Ret = pfPortInit(&NbPorts);
   if (Ret != 0)
      {
      HandleError(Ret);
      return PFOCUS_CAMERA_NOT_OK;
      }

   /* Open camera port. */
   Ret = pfDeviceOpen(Port);
   if (Ret != 0)
      {
      HandleError(Ret);
      return PFOCUS_CAMERA_NOT_OK;
      }

   /* Change baud rate, if possible. */
   int IsSupported;
   IsSupported = pfIsBaudRateSupported(Port, CAMERA_COM_PORT_SPEED);

   if (IsSupported == 1)
      {
      Ret = pfSetBaudRate(Port, CAMERA_COM_PORT_SPEED);
      if (Ret != 0)
         {
         HandleError(Ret);
         return PFOCUS_CAMERA_NOT_OK;
         }
      }
   else
      {
      HandleError(Ret);
      return PFOCUS_CAMERA_NOT_OK;
      }

   /* Check for unsupported options. */
   const MIL_INT MAX_STRING_LENGTH = 64;
   char CameraName[MAX_STRING_LENGTH];
   Ret2 = GetPropertyString(Port, CAMERA_NAME_PROPNAME, CameraName, MAX_STRING_LENGTH);
   if (Ret2 != PFOCUS_CAMERA_OK)
      return Ret2;

   if (strncmp(CameraName, SUPPORTED_CAMERA_NAME, MAX_STRING_LENGTH) != 0)
      {
      printf("Error: Unsupported camera: '%s'.", CameraName);
      return PFOCUS_CAMERA_NOT_OK;
      }

   MIL_INT PropertyValue;
   Ret2 = GetPropertyInt(Port, DECIMATION_PROPNAME, &PropertyValue);
   if (Ret2 != PFOCUS_CAMERA_OK)
      return Ret2;
   if (PropertyValue != EXPECTED_DECIMATION)
      {
      printf("Error: Decimation is not supported in this example.\n");
      return PFOCUS_CAMERA_NOT_OK;
      }

   Ret2 = GetPropertyInt(Port, MROI_PROPNAME, &PropertyValue);
   if (Ret2 != PFOCUS_CAMERA_OK)
      return Ret2;
   if (PropertyValue != EXPECTED_MROI_SETTING)
      {
      printf("Error: MROI is not supported in this example.\n");
      return PFOCUS_CAMERA_NOT_OK;
      }

   /* Reset camera. */
   /* We don't do this for now to avoid changing the camera resolution here. */
   /*Ret2 = SetPropertyInt(RESET_PROPNAME, EXECUTE_RESET);
   if (Ret2 != PFOCUS_CAMERA_OK)
      return Ret2;*/

   return PFOCUS_CAMERA_OK;
   }

/* Ask the camera configuration and fill conversion structure accordingly. */
MIL_INT FillParams(int Port, PFocusParams* pConvParams)
   {
   MIL_INT Ret;

   /* Get camera resolution. */
   MIL_INT DataResolution;
   Ret = GetPropertyInt(Port, RESOLUTION_PROPNAME, &DataResolution);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   switch(DataResolution)
      {
      case RESOLUTION_12BITS_VALUE:
         pConvParams->DataResolution = 12;
         break;
   
      case RESOLUTION_10BITS_VALUE:
         pConvParams->DataResolution = 10;
         break;
   
      case RESOLUTION_8BITS_VALUE:
         pConvParams->DataResolution = 8;
         break;
   
      default:
         printf("Error: Unexpected camera resolution.\n");
         return PFOCUS_CAMERA_NOT_OK;
      }

   /* Check if status line is enabled. */
   MIL_INT StatusLine;
   Ret = GetPropertyInt(Port, STATUS_LINE_PROPNAME, &StatusLine);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   if (StatusLine == 0)
      pConvParams->StatusLine = STATUS_LINE_DISABLED;
   else
      pConvParams->StatusLine = STATUS_LINE_ENABLED;

   /* Get camera cycle time. */
   float FrameTimeMilliseconds;
   Ret = GetPropertyFloat(Port, FRAME_TIME_PROPNAME, &FrameTimeMilliseconds);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   pConvParams->CycleTimeMicroseconds = static_cast<MIL_INT>(FrameTimeMilliseconds*1000.0);

   return PFOCUS_CAMERA_OK;
   }

/* Change camera to 2d mode only and change its ROI. */
MIL_INT SwitchCameraToImage(int     Port      ,
                            MIL_INT ROIOffsetX,
                            MIL_INT ROIOffsetY,
                            MIL_INT ROISizeX  ,
                            MIL_INT ROISizeY  )
   {
   return SwitchCameraMode(Port, OUTPUT_2D_ONLY,
                           ROIOffsetX, ROIOffsetY, ROISizeX, ROISizeY);
   }

/* Change camera to 2d and 3d mode and change its ROI. */
MIL_INT SwitchCameraToMeasure(int     Port      ,
                              MIL_INT ROIOffsetX,
                              MIL_INT ROIOffsetY,
                              MIL_INT ROISizeX  ,
                              MIL_INT ROISizeY  )
   {
   return SwitchCameraMode(Port, OUTPUT_2D_AND_3D,
                           ROIOffsetX, ROIOffsetY, ROISizeX, ROISizeY);
   }

/* Terminate communication with camera. */
void CloseDown(int Port)
   {
   pfDeviceClose(Port);
   }

/*****************************************************************************
 Helper functions.
*****************************************************************************/

/* Change camera mode and ROI. */
MIL_INT SwitchCameraMode(int     Port      ,
                         MIL_INT Mode      ,
                         MIL_INT ROIOffsetX,
                         MIL_INT ROIOffsetY,
                         MIL_INT ROISizeX  ,
                         MIL_INT ROISizeY  )
   {
   MIL_INT Ret;

   /* Set camera to output 2d and 3d data. */
   Ret = SetPropertyInt(Port, DETECTOR_MODE_PROPNAME, Mode);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   /* Set temporary camera ROI offset to 0. */
   Ret = SetPropertyInt(Port, ROI_OFFSET_X_PROPNAME, 0);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   Ret = SetPropertyInt(Port, ROI_OFFSET_Y_PROPNAME, 0);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   /* Set camera ROI size. */
   Ret = SetPropertyInt(Port, ROI_SIZE_X_PROPNAME, ROISizeX);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   Ret = SetPropertyInt(Port, ROI_SIZE_Y_PROPNAME, ROISizeY);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   /* Set camera ROI offset. */
   Ret = SetPropertyInt(Port, ROI_OFFSET_X_PROPNAME, ROIOffsetX);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   Ret = SetPropertyInt(Port, ROI_OFFSET_Y_PROPNAME, ROIOffsetY);
   if (Ret != PFOCUS_CAMERA_OK)
      return Ret;

   return PFOCUS_CAMERA_OK;
   }

/* Print error or warning message according to return value. */
void HandleError(int Error)
   {
	const char* pErrMsg = pfGetErrorString(Error);
	if (Error < 0)
		printf("Error: %s\n"  , pErrMsg);
	else
		printf("Warning: %s\n", pErrMsg);
   }

/* Query pPropertyName, and put it as a string in pOut, which has a size of OutLen. */
MIL_INT GetPropertyString(int Port, const char* pPropertyName, char* pOut, MIL_INT OutLen)
   {
	TOKEN Token = pfProperty_ParseName(Port, pPropertyName);
	if (Token == INVALID_TOKEN)
      {
      printf("Unable to read property: '%s'\n", pPropertyName);
      return PFOCUS_CAMERA_NOT_OK;
	   }

   int Ret = pfDevice_GetProperty_String(Port, Token, pOut, OutLen);
	if(Ret < 0)
      {
      HandleError(Ret);
      return PFOCUS_CAMERA_NOT_OK;
      }
	return PFOCUS_CAMERA_OK;
   }

/* Query pPropertyName, and put it as an integer in pOut. */
MIL_INT GetPropertyInt(int Port, const char* pPropertyName, MIL_INT* pOut)
   {
	TOKEN Token = pfProperty_ParseName(Port, pPropertyName);
	if (Token == INVALID_TOKEN)
      {
      printf("Unable to read property: '%s'\n", pPropertyName);
      return PFOCUS_CAMERA_NOT_OK;
	   }

   PFValue Value;
	int Ret = pfDevice_GetProperty(Port, Token, &Value);
	if (Ret < 0)
      {
		HandleError(Ret);
      return PFOCUS_CAMERA_NOT_OK;
	   }
	*pOut = static_cast<MIL_INT>( Value.value.i );
	return PFOCUS_CAMERA_OK;
   }

/* Query pPropertyName, and put it as a float in pOut. */
MIL_INT GetPropertyFloat(int Port, const char* pPropertyName, float* pOut)
   {
	TOKEN Token = pfProperty_ParseName(Port, pPropertyName);
	if (Token == INVALID_TOKEN)
      {
      printf("Unable to read property: '%s'\n", pPropertyName);
      return PFOCUS_CAMERA_NOT_OK;
	   }

   PFValue Value;
	int Ret = pfDevice_GetProperty(Port, Token, &Value);
	if (Ret < 0)
      {
		HandleError(Ret);
      return PFOCUS_CAMERA_NOT_OK;
	   }
	*pOut = Value.value.f;
	return PFOCUS_CAMERA_OK;
   }

/* Change pPropertyName, setting it to the integer IntValue. */
MIL_INT SetPropertyInt(int Port, const char* pPropertyName, MIL_INT IntValue)
   {
	TOKEN Token = pfProperty_ParseName(Port, pPropertyName);
	if (Token == INVALID_TOKEN)
      {
      printf("Unable to find property: '%s'\n", pPropertyName);
      return PFOCUS_CAMERA_NOT_OK;
	   }

   PFValue Value;
	Value.type = pfProperty_GetType(Port, Token);
	Value.value.i = static_cast<int>( IntValue );
	int Ret = pfDevice_SetProperty(Port, Token, &Value);
	if (Ret < 0)
      {
		HandleError(Ret);
      return PFOCUS_CAMERA_NOT_OK;
      }
	return PFOCUS_CAMERA_OK;
   }

#endif
