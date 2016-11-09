/*****************************************************************************/
/* 
 * File name: sickcamera_rangerc.cpp
 *
 * Synopsis: All camera-specific code
 *            
 */

#include "cameraconfig.h"           /* User options - change them to suit your needs. */

#include <mil.h>
#include "mil3dcam_sickrangerc.h"   /* Parameter struct definition.                   */
#include "sickcamera_rangerc.h"     /* Prototypes to interact with ranger camera.     */

#if !USE_REAL_CAMERA

/*
 * We implement stub functions for the camera API for cleaner main code.
 * The only important function here is FillParams(), which fill the conversion structure
 * with hard-coded values that corresponds to the mode used to obtain profile data
 * in the saved images.
 */

MIL_INT CreateCamera(icon::RangerC**)
   {
   return SICK_CAMERA_OK;
   }

MIL_INT InitCamera(icon::RangerC*)
   {
   return SICK_CAMERA_OK;
   }

MIL_INT FillParams(icon::RangerC*,
                   RangerParams* pConvParams,
                   const std::string&,
                   const char*)
   {
   pConvParams->MeasurementMode        = RANGER_HOR_THR_MODE;
   pConvParams->RangeStreamOffset      = 0;
   pConvParams->IntensityStreamOffset  = NO_INTENSITY_INFORMATION;
   pConvParams->RangeSizeBit           = 16;
   pConvParams->IntensitySizeBit       = NO_INTENSITY_INFORMATION;
   pConvParams->ImageROISizeX          = 1536;
   pConvParams->ImageROISizeY          = 512;
   pConvParams->MeasROISizeX           = 1536;
   pConvParams->MeasROISizeY           = 512;
   pConvParams->ProfileDataSizeByte    = 3072;
   pConvParams->FlipY                  = 1;
   pConvParams->FixedPoint             = 1;
   pConvParams->ROIOffsetX             = 0;
   pConvParams->ROIOffsetY             = 0;
   pConvParams->CycleTimeMicroseconds  = 5000;

   return SICK_CAMERA_OK;
   }

MIL_INT SwitchCameraMode(icon::RangerC*, const std::string&)
   {
   return SICK_CAMERA_OK;
   }

void CloseDown(icon::RangerC*)
   {

   }

#else

/* Link with correct iCon library, depending on Visual Studio version. */
#if M_MIL_USE_64BIT
   #error iCon library is not supported under 64 bits platforms.
#else
   #ifdef _MSC_VER
      #if   _MSC_VER >= 1400
         #ifdef NDEBUG
            #pragma comment(lib, "icon_vc80")
         #else
            #pragma comment(lib, "icon_vc80d")
         #endif
      #elif _MSC_VER >= 1310
         #ifdef NDEBUG
            #pragma comment(lib, "icon_vc71")
         #else
            #pragma comment(lib, "icon_vc71d")
         #endif
      #else
         #error Unsupported Visual Studio version for iCon library.
      #endif
   #else
      #error Unsupported compiler for iCon library.
   #endif
#endif

#include "icon_api.h"      /* Include the icon api                           */
#include "rangerc.h"       /* Ranger c specific prototypes                   */
#include "dataformat.h"    /* Icon-specific structure                        */

/* ICON API name spaces. */
using namespace std;
using namespace icon;

/*****************************************************************************
 Defines and constants.
*****************************************************************************/

/* Basic camera parameters. */
#define CAMERA_COM_PORT_SPEED       115200
#define CAMERA_PROTOCOL_TIMEOUT     15000

/* Static string definitions to search in XML camera configuration. */
static const string BasePath                = "<ROOT><CAMERA name='" CAMERA_NAME "'>";

static const string ImageConfigPath         =
   BasePath        + "<CONFIGURATION name='" IMAGE_MODE_NAME       "'>";
static const string ImageCompPath           =
   ImageConfigPath + "<COMPONENT name='"     IMAGE_COMP_NAME       "'>";

static const string MeasConfigPath          =
   BasePath        + "<CONFIGURATION name='" MEASUREMENT_MODE_NAME "'>";

static const string NumberOfColumnsName     = "number of columns";
static const string NumberOfRowsName        = "number of rows";
static const string RangeAxisName           = "range axis";
static const string SubPixelingName         = "sub pixeling";
static const string RowStepSizeName         = "row step size";
static const string NumberOfThrsName        = "number of thrs";
static const string OutputSelectName        = "output select";
static const string AcquistionDirectionName = "acquistion direction";
static const string StartColumnName         = "start column";
static const string StartRowName            = "start row";
static const string CycleTimeName           = "cycle time";

static const string HorThrValueType         = "HorThr";
static const string HorMaxValueType         = "HorMax";
static const string Hi3DValueType           = "Hi3D";

static const string ByteTypeName            = "BYTE";
static const string WordTypeName            = "WORD";

static const char* const XSumSubCompName      = "XSum";
static const char* const SumSubCompName       = "Sum";
static const char* const RangeSubCompName     = "Range";
static const char* const IntensitySubCompName = "Intensity";

/* Helper function prototypes. */
MIL_INT StopCamera(RangerC* pCam);
MIL_INT SwitchCameraMode(RangerC* pCam, const string& rModeName, bool CallStart);
MIL_INT GetCameraDataFormat(RangerC* pCam, DataFormat* pDataFormat);
MIL_INT GetCameraMilIntParam(RangerC*      pCam,
                             const string& rPath,
                             const string& rParamName,
                             MIL_INT*      pIntValue);

/* SwitchCameraMode() CallStart values. */
#define DO_CALL_START      true
#define DO_NOT_CALL_START  false

/*****************************************************************************
 API function implementations.
*****************************************************************************/

/* Constructs camera object and set the pointer *ppCam to it. */
MIL_INT CreateCamera(RangerC** ppCam)
   {
   *ppCam = NULL;

   /* Call camera factory in the icon api. Ask for a RangerC camera. */
   Camera* pGeneric = createCamera(RangerC::cameraType, "MyCamera");

   /* Check result from factory and cast it to a RangerC camera. */
   if(pGeneric == NULL || pGeneric->getCameraType() != RangerC::cameraType)
      {
      delete pGeneric;
      return SICK_CAMERA_NOT_OK;
      }

   *ppCam = static_cast<RangerC*>(pGeneric);
   return SICK_CAMERA_OK;
   }

/* Initializes communication with the camera and load parameter file,
   must be called once before grabbing. */
MIL_INT InitCamera(RangerC* pCam)
   {
   int Ret;

   /* Set baudrate and timeout. */
   Ret = pCam->setComParameters(CAMERA_COM_PORT, CAMERA_COM_PORT_SPEED);
   if (Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   Ret = pCam->setProtocolTimeout(CAMERA_PROTOCOL_TIMEOUT); // 15 s timeout
   if (Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Init camera. */
   Ret = pCam->init();
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Ask the camera configuration and fill conversion structure accordingly. */
MIL_INT FillParams(RangerC*      pCam,
                   RangerParams* pConvParams,
                   const string& rParamFilename,
                   const char*   pMeasCompName)
   {
   MIL_INT Ret;
   int Ret2;

   const string MeasCompPath =
      MeasConfigPath + "<COMPONENT name='" + pMeasCompName + "'>";

   /* Load parameter file to the camera. */

   Ret = StopCamera(pCam);
   if(Ret != SICK_CAMERA_OK)
      return Ret;

   Ret2 = pCam->fileLoadParameters(rParamFilename);
   if(Ret2 != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Inquire characteristics of the image mode. */

   MIL_INT ImageROIOffsetX, ImageROIOffsetY;

   Ret = GetCameraMilIntParam(pCam, ImageConfigPath, StartColumnName, &ImageROIOffsetX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCam, ImageCompPath, StartRowName, &ImageROIOffsetY);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCam, ImageConfigPath, NumberOfColumnsName,
                              &pConvParams->ImageROISizeX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCam, ImageCompPath, NumberOfRowsName,
                              &pConvParams->ImageROISizeY);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   /* Inquire characteristics of the measurement mode. */
   /* We must switch camera mode to get data format. */
   Ret = SwitchCameraMode(pCam, MEASUREMENT_MODE_NAME, DO_NOT_CALL_START);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   DataFormat CameraMeasFormat;
   Ret = GetCameraDataFormat(pCam, &CameraMeasFormat);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   if (CameraMeasFormat.getConfigType() != DataFormat::Linescan_T)
      return SICK_CAMERA_NOT_OK;

   pConvParams->ProfileDataSizeByte = CameraMeasFormat.dataSize();

   const Component* pMeasComponent = CameraMeasFormat.getNamedComponent(pMeasCompName);

   if (pMeasComponent == NULL)
      return SICK_CAMERA_NOT_OK;

   MIL_INT MeasStreamOffset = CameraMeasFormat.getComponentOffset(pMeasCompName);

   string MeasValueTypeString = pMeasComponent->getValueType();
   if (MeasValueTypeString == HorThrValueType)
      pConvParams->MeasurementMode = RANGER_HOR_THR_MODE;
   else if (MeasValueTypeString == HorMaxValueType)
      pConvParams->MeasurementMode = RANGER_HOR_MAX_MODE;
   else if (MeasValueTypeString == Hi3DValueType)
      pConvParams->MeasurementMode = M_RANGER_Hi3D_MODE;
   else
      return SICK_CAMERA_NOT_OK;

   Ret = GetCameraMilIntParam(pCam, MeasConfigPath, CycleTimeName,
                              &pConvParams->CycleTimeMicroseconds);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCam, MeasConfigPath, NumberOfColumnsName,
                              &pConvParams->MeasROISizeX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCam, MeasCompPath, NumberOfRowsName,
                              &pConvParams->MeasROISizeY);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   MIL_INT MeasROIOffsetX;
   Ret = GetCameraMilIntParam(pCam, MeasConfigPath, StartColumnName, &MeasROIOffsetX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   MIL_INT MeasROIOffsetY;
   Ret = GetCameraMilIntParam(pCam, MeasCompPath, StartRowName, &MeasROIOffsetY);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   /*
    * Since the SICK camera grabs flipped images, we flip them horizontally before using
    * them for camera and laser calibration. However, after flipping, ROI X offset is not
    * good anymore, we must compute the X offset on the right side (before flip) instead
    * of the left side.
    */
   pConvParams->ROIOffsetX = (ImageROIOffsetX + pConvParams->ImageROISizeX) -
                             (MeasROIOffsetX  + pConvParams->MeasROISizeX ); 
   pConvParams->ROIOffsetY = MeasROIOffsetY - ImageROIOffsetY;

   /* Mode-specific section. */

   if (pConvParams->MeasurementMode == RANGER_HOR_THR_MODE)
      {
      pConvParams->RangeStreamOffset     = MeasStreamOffset +
         pMeasComponent->getSubComponentOffset(RangeSubCompName);
      pConvParams->IntensityStreamOffset = NO_INTENSITY_INFORMATION;
      pConvParams->IntensitySizeBit      = NO_INTENSITY_INFORMATION;

      if ( pConvParams->RangeStreamOffset == DataFormat::E_BAD_NAME )
         return SICK_CAMERA_NOT_OK;

      MIL_INT RowStepSizeValue;
      Ret = GetCameraMilIntParam(pCam, MeasCompPath, RowStepSizeName, &RowStepSizeValue);
      if (Ret != SICK_CAMERA_OK)
         return Ret;
      if (RowStepSizeValue != 1)
         return SICK_CAMERA_NOT_OK;

      MIL_INT NumberOfThrsValue;
      Ret = GetCameraMilIntParam(pCam, MeasCompPath, NumberOfThrsName, &NumberOfThrsValue);
      if (Ret != SICK_CAMERA_OK)
         return Ret;

      MIL_INT OutputSelectValue;
      Ret = GetCameraMilIntParam(pCam, MeasCompPath, OutputSelectName, &OutputSelectValue);
      if (Ret != SICK_CAMERA_OK)
         return Ret;

      switch(NumberOfThrsValue)
         {
         case 1:
            pConvParams->FixedPoint = 1;
            break;
      
         case 2:
            switch(OutputSelectValue)
               {
               case 1:
               case 2:
               case 3:
                  pConvParams->FixedPoint = 1;
                  break;
            
               case 0:
               case 4:
                  pConvParams->FixedPoint = 2;
                  break;

               case 10:
                  return SICK_CAMERA_NOT_OK; /* The raw mode is not supported. */
            
               default:
                  return SICK_CAMERA_NOT_OK;
               }
            break;
      
         default:
            return SICK_CAMERA_NOT_OK;
         }

      const SubComponent* pRangeSubComponent =
         pMeasComponent->getNamedSubComponent(RangeSubCompName);
      if (pRangeSubComponent == NULL)
         return SICK_CAMERA_NOT_OK;

      string RangeSizeBitString = pRangeSubComponent->getValueType();
      if (RangeSizeBitString == ByteTypeName)
         pConvParams->RangeSizeBit = 8;
      else if (RangeSizeBitString == WordTypeName)
         pConvParams->RangeSizeBit = 16;
      else
         return SICK_CAMERA_NOT_OK;

      MIL_INT RangeAxisValue;
      Ret = GetCameraMilIntParam(pCam, MeasCompPath, RangeAxisName, &RangeAxisValue);
      if (Ret != SICK_CAMERA_OK)
         return Ret;

      if (RangeAxisValue != 0 && RangeAxisValue != 1)
         return SICK_CAMERA_NOT_OK;

      MIL_INT AcquistionDirectionValue;
      Ret = GetCameraMilIntParam(pCam, MeasCompPath, AcquistionDirectionName,
                                 &AcquistionDirectionValue);
      if (Ret != SICK_CAMERA_OK)
         return Ret;

      if (AcquistionDirectionValue != 0 && AcquistionDirectionValue != 1)
         return SICK_CAMERA_NOT_OK;

      if (AcquistionDirectionValue == RangeAxisValue)
         pConvParams->FlipY = DO_FLIP_Y_AXIS;
      else
         pConvParams->FlipY = DO_NOT_FLIP_Y_AXIS;

      /* We found that, with our current SICK camera, ROI is not handled well in some
         situations, so let's skip it. Try at your own risk. */
      if ( pConvParams->RangeSizeBit == 16  &&
           RangeAxisValue            == 0   &&
           pConvParams->MeasROISizeY != 512 )
         return SICK_CAMERA_NOT_OK;
      }
   else if (pConvParams->MeasurementMode == RANGER_HOR_MAX_MODE)
      {
      pConvParams->RangeStreamOffset     = MeasStreamOffset +
         pMeasComponent->getSubComponentOffset(RangeSubCompName);
      pConvParams->IntensityStreamOffset = MeasStreamOffset +
         pMeasComponent->getSubComponentOffset(IntensitySubCompName);
      pConvParams->IntensitySizeBit      = 8;

      if ( pConvParams->RangeStreamOffset     == DataFormat::E_BAD_NAME ||
           pConvParams->IntensityStreamOffset == DataFormat::E_BAD_NAME )
         return SICK_CAMERA_NOT_OK;

      Ret = GetCameraMilIntParam(pCam, MeasCompPath, SubPixelingName,
                                 &pConvParams->FixedPoint);
      if (Ret != SICK_CAMERA_OK)
         return Ret;

      /* Unfortunately, sub-pixeling option does not seem to work on the Ranger C55
         in HorMax mode. */
      if (pConvParams->FixedPoint != 0)
         return SICK_CAMERA_NOT_OK;

      const SubComponent* pRangeSubComponent =
         pMeasComponent->getNamedSubComponent(RangeSubCompName);
      if (pRangeSubComponent == NULL)
         return SICK_CAMERA_NOT_OK;

      string RangeSizeBitString = pRangeSubComponent->getValueType();
      if (RangeSizeBitString == ByteTypeName)
         pConvParams->RangeSizeBit = 8;
      else if (RangeSizeBitString == WordTypeName)
         pConvParams->RangeSizeBit = 16;
      else
         return SICK_CAMERA_NOT_OK;

      pConvParams->FlipY = DO_FLIP_Y_AXIS;

      /* We found that, with our current SICK camera, ROI is not handled well in some
         situations, so let's skip it. Try at your own risk. */
      if ( pConvParams->RangeSizeBit == 16  &&
           pConvParams->MeasROISizeY != 512 )
         return SICK_CAMERA_NOT_OK;
      }
   else if (pConvParams->MeasurementMode == M_RANGER_Hi3D_MODE)
      {
      pConvParams->RangeStreamOffset     = MeasStreamOffset +
         pMeasComponent->getSubComponentOffset(XSumSubCompName);
      pConvParams->IntensityStreamOffset = MeasStreamOffset +
         pMeasComponent->getSubComponentOffset( SumSubCompName);
      pConvParams->IntensitySizeBit      = 16;

      if ( pConvParams->RangeStreamOffset     == DataFormat::E_BAD_NAME ||
           pConvParams->IntensityStreamOffset == DataFormat::E_BAD_NAME )
         return SICK_CAMERA_NOT_OK;

      pConvParams->FixedPoint            = RANGER_C55_DEFAULT_FIXED_POINT;

      pConvParams->RangeSizeBit          = 16;

      MIL_INT RangeAxisValue;
      Ret = GetCameraMilIntParam(pCam, MeasCompPath, RangeAxisName, &RangeAxisValue);
      if (Ret != SICK_CAMERA_OK)
         return Ret;

      if (RangeAxisValue == 0)
         pConvParams->FlipY = DO_FLIP_Y_AXIS;
      else if (RangeAxisValue == 1)
         pConvParams->FlipY = DO_NOT_FLIP_Y_AXIS;
      else
         return SICK_CAMERA_NOT_OK;
      }

   return SICK_CAMERA_OK;
   }

/* Switch the camera between area-scan (image) and line-scan (measurement) modes.
   rModeName can be IMAGE_MODE_NAME or MEASUREMENT_MODE_NAME (see cameraconfig.h). */
MIL_INT SwitchCameraMode(RangerC* pCam, const string& rModeName)
   {
   return SwitchCameraMode(pCam, rModeName, DO_CALL_START);
   }

/* Terminate communication with camera and destroys the camera object. */
void CloseDown(RangerC* pCam)
   {
   if(pCam != NULL)
      { 
      pCam->close(); 
      delete pCam;
      }
   closeApi();
   }

/*****************************************************************************
 Helper functions.
*****************************************************************************/

/* If not already in that state, send the camera in the stopped state. */
MIL_INT StopCamera(RangerC* pCam)
   {
   int Ret, Status;

   Ret = pCam->checkCamStatus(Status);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   if(Status != RangerC::Stopped)
      {
      Ret = pCam->stop();
      if(Ret != Camera::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;
      }

   return SICK_CAMERA_OK;
   }

/* Switch the camera between area-scan (image) and line-scan (measurement) modes.
   rModeName can be IMAGE_MODE_NAME or MEASUREMENT_MODE_NAME (see cameraconfig.h).
   Also optionally allows to start grabbing or not. */
MIL_INT SwitchCameraMode(RangerC* pCam, const string& rModeName, bool CallStart)
   {
   int Ret;
   MIL_INT Ret2;

   Ret2 = StopCamera(pCam);
   if(Ret2 != SICK_CAMERA_OK)
      return Ret2;

   Ret = pCam->setActiveConfiguration(rModeName);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   // Start camera
   if (CallStart)
      {
      Ret = pCam->start();
      if(Ret != Camera::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;
      }

   return SICK_CAMERA_OK;
   }

/* Fills a DataFormat structure by getting it from the camera. */
MIL_INT GetCameraDataFormat(RangerC* pCam, DataFormat* pDataFormat)
   {
   int Ret;
   string DataFormatString;
   
   Ret = pCam->getDataFormat("", DataFormatString);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   Ret = pDataFormat->init(DataFormatString.c_str());
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Ask the camera the value of a parameter along a specified path in the XML configuration,
   read it as an integer and put it in *pIntValue. */
MIL_INT GetCameraMilIntParam(RangerC*      pCam,
                             const string& rPath,
                             const string& rParamName,
                             MIL_INT*      pIntValue)
   {
   string ParamValueString;
   int Ret = pCam->getParameterValue(rPath, rParamName, ParamValueString);
   if (Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   *pIntValue = atol( ParamValueString.c_str() );
   return SICK_CAMERA_OK;
   }

#endif
