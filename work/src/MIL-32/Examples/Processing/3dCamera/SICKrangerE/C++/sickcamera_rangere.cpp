/*****************************************************************************/
/* 
 * File name: sickcamera_rangere.cpp
 *
 * Synopsis: All camera-specific code
 *            
 */

#include "cameraconfig.h"           /* User options - change them to suit your needs. */

#include <mil.h>
#include "mil3dcam_sickrangere.h"   /* Parameter struct definition.                   */
#include "sickcamera_rangere.h"     /* Prototypes to interact with ranger camera.     */

#if !USE_REAL_CAMERA

/*
 * We implement stub functions for the camera API for cleaner main code.
 * The only important function here is FillParams(), which fill the conversion structure
 * with hard-coded values that corresponds to the mode used to obtain profile data
 * in the saved images.
 */

MIL_INT CreateCameraSystem(SSickCameraSystem* pCameraSystem)
   {
   return SICK_CAMERA_OK;
   }

MIL_INT InitCameraSystem(SSickCameraSystem* pCameraSystem)
   {
   return SICK_CAMERA_OK;
   }

MIL_INT FillParams(SSickCameraSystem* pCameraSystem,
                   RangerParams* pConvParams,
                   const std::string&)
   {
   pConvParams->LaserMeasurementMode         = M_RANGER_Hi3D_MODE;
   pConvParams->RangeStreamOffset            = 10752;
   pConvParams->LaserIntensityStreamOffset   = 9216;
   pConvParams->RangeSizeBit                 = 16;
   pConvParams->LaserIntensitySizeBit        = 8;
   pConvParams->LaserScatterSizeBit          = NO_INFORMATION;
   pConvParams->LaserScatterStreamOffset     = NO_INFORMATION;

   pConvParams->GrayStreamOffset             = NO_INFORMATION;
   pConvParams->HiResGrayStreamOffset        = NO_INFORMATION;
   pConvParams->ColorStreamOffset            = NO_INFORMATION;
   pConvParams->HiResColorStreamOffset       = 0;
   pConvParams->ScatterStreamOffset          = NO_INFORMATION;   

   pConvParams->ImageROISizeX          = 1536;
   pConvParams->ImageROIOffsetX        = 0;
   pConvParams->ImageROISizeY          = 512;
   pConvParams->ImageROIOffsetY        = 0;
   pConvParams->MeasROISizeX           = 1536;
   pConvParams->MeasROIOffsetX         = 0;
   pConvParams->LaserROISizeY          = 464;
   pConvParams->LaserROIOffsetY        = 48;
   pConvParams->ProfileDataSizeByte    = 13824;
   pConvParams->FlipY                  = 1;
   pConvParams->FixedPoint             = 4;
   pConvParams->ROIOffsetX             = 0;
   pConvParams->ROIOffsetY             = 48;
   pConvParams->CycleTimeMicroseconds  = 5000;

   return SICK_CAMERA_OK;
   }

void CloseDown(SSickCameraSystem* pCameraSystem)
   {

   }

#else


#include "icon_auto_link.h"
#include "icon_api.h"         /* Include the icon api                           */
#include "ethernetcamera.h"   /* Ethernet camera specific prototypes            */
#include "framegrabber.h"     /* Framegrabber specific prototypes               */
#include "dataformat.h"       /* Icon-specific structure                        */

/* ICON API name spaces. */
using namespace std;
using namespace icon;

/*****************************************************************************
 Defines and constants.
*****************************************************************************/

/* Basic camera parameters. */

/* Static string definitions to search in XML camera configuration. */
static const string BasePath                = "<ROOT><CAMERA name='" CAMERA_NAME "'>";

static const string ImageConfigPath         =
   BasePath        + "<CONFIGURATION name='" IMAGE_MODE_NAME       "'>";
static const string ImageCompPath           =
   ImageConfigPath + "<COMPONENT name='"     IMAGE_COMP_NAME       "'>";

static const string MeasConfigPath          =
   BasePath        + "<CONFIGURATION name='" MEASUREMENT_MODE_NAME "'>";

static const string ColorCompPath           =
   MeasConfigPath + "<COMPONENT name='"     COLOR_COMP_NAME "'>";

static const string HiresColorCompPath           =
   MeasConfigPath + "<COMPONENT name='"     HIRES_COLOR_COMP_NAME "'>";

static const string NumberOfColumnsName     = "number of columns";
static const string NumberOfRowsName        = "number of rows";
static const string RangeAxisName           = "range axis";
static const string SubPixelingName         = "sub pixeling";
static const string NumberOfThrsName        = "number of thrs";
static const string OutputSelectName        = "output select";
static const string AcquistionDirectionName = "acquistion direction";
static const string StartColumnName         = "start column";
static const string StartRowName            = "start row";
static const string CycleTimeName           = "cycle time";
static const string EnableScatterName       = "enable scatter";
static const string BalanceGreenName        = "balance green";
static const string GainGreenName           = "gain green";
static const string BalanceBlueName         = "balance blue";
static const string GainBlueName            = "gain blue";
static const string GainRedName             = "gain red";
static const string ExposureTimeRedName     = "exposure time red";

static const string HorThrValueType         = "HorThr";
static const string HorMaxValueType         = "HorMax";
static const string HorMaxThrValueType      = "HorMaxThr";
static const string Hi3DCOGValueType        = "COG";
static const string Hi3DValueType           = "DCM";
static const string ColorValueType          = "RGB";
static const string HiResColorValue         = "HiresRGB";
static const string GrayValueType           = "Gray";
static const string HiResGrayValue          = "HiresGray";
static const string ScatterValueType        = "Scatter";

static const string ImageValueType          = "image";

static const string ByteTypeName            = "BYTE";
static const string WordTypeName            = "WORD";

static const char* const XSumSubCompName      = "XSum";
static const char* const SumSubCompName       = "Sum";
static const char* const RangeSubCompName     = "Range";
static const char* const IntensitySubCompName = "Intensity";
static const char* const ScatterSubCompName   = "Scatter";

/* Basic frame grabber parameters. */
#define FRAME_BUFFER_SIZE           50
#define CONVEYOR_STARTUP_DELAY_MS   300

/* Static active frame grabbers */
#define MAX_FRAME_GRABBER 4
static FrameGrabber* ActiveFrameGrabber[MAX_FRAME_GRABBER] = {NULL, NULL, NULL, NULL}; 

/* Helper function prototypes. */
MIL_INT CreateCamera(EthernetCamera** ppCam);
MIL_INT StopCamera(EthernetCamera* pCam);
MIL_INT GetCameraDataFormat(EthernetCamera* pCam, DataFormat* pDataFormat);
MIL_INT GetCameraMilIntParam(EthernetCamera* pCam,
                             const string& rPath,
                             const string& rParamName,
                             MIL_INT*      pIntValue);

MIL_INT SetCameraMilIntParam(EthernetCamera* pCam,
                             const string& rPath,
                             const string& rParamName,
                             MIL_INT       IntValue);

MIL_INT CreateFrameGrabber(FrameGrabber** ppFrameGrab);
MIL_INT StartGrab(SSickCameraSystem* pCameraSystem, MIL_ID StartStageEvent = M_NULL);
MIL_INT StopGrab(SSickCameraSystem* pCameraSystem);
void ProcessCallback(IconBuffer *pBuffer, FrameGrabber* pFrameGrab, void* UserData);
MIL_INT GetNewActiveIndex();
bool IsActive(FrameGrabber* pFrameGrab);
bool RemoveActiveFrameGrabber(FrameGrabber* pFrameGrab);

/* Laser modes parameters filling functions */
MIL_INT FillHorThrParams(RangerParams* pConvParams,
                         MIL_INT MeasStreamOffset,
                         const Component* pMeasComponent,
                         RangerE* pCam,
                         const string MeasCompPath );
MIL_INT FillHorMaxParams(RangerParams* pConvParams,
                         MIL_INT MeasStreamOffset,
                         const Component* pMeasComponent,
                         RangerE* pCam,
                         const string MeasCompPath);
MIL_INT FillHorMaxThrParams(RangerParams* pConvParams,
                            MIL_INT MeasStreamOffset,
                            const Component* pMeasComponent,
                            RangerE* pCam,
                            const string MeasCompPath );
MIL_INT FillHi3DParams(RangerParams* pConvParams,
                       MIL_INT MeasStreamOffset,
                       const Component* pMeasComponent,
                       RangerE* pCam,
                       const string MeasCompPath );
MIL_INT FillHi3DCogParams(RangerParams* pConvParams,
                          MIL_INT MeasStreamOffset,
                          const Component* pMeasComponent,
                          RangerE* pCam,
                          const string MeasCompPath );


/*****************************************************************************
 API function implementations.
*****************************************************************************/

/* Constructs camera object and frame grabber object. */
MIL_INT CreateCameraSystem(SSickCameraSystem* pCameraSystem)
   {
   if(CreateCamera(&pCameraSystem->pCam) == SICK_CAMERA_NOT_OK)
      return SICK_CAMERA_NOT_OK;

   return CreateFrameGrabber(&(pCameraSystem->pFrameGrab));
   }

/* Constructs camera object and set the pointer *ppCam to it. */
MIL_INT CreateCamera(EthernetCamera** ppCam)
   {
   *ppCam = NULL;

   /* Call camera factory in the icon api. Ask for a RangerE camera. */
   Camera* pGeneric = createCamera(EthernetCamera::cameraType, "MyCamera");

   /* Check result from factory and cast it to a RangerE camera. */
   if(pGeneric == NULL || pGeneric->getCameraType() != EthernetCamera::cameraType)
      {
      delete pGeneric;
      return SICK_CAMERA_NOT_OK;
      }

   *ppCam = static_cast<EthernetCamera*>(pGeneric);
   return SICK_CAMERA_OK;
   }

/* Constructs camera object and set the pointer *ppFrameGrab to it. */
MIL_INT CreateFrameGrabber(FrameGrabber** ppFrameGrab)
   {
   *ppFrameGrab = NULL;

   /* Call frame grabber factory in the icon api. Ask for a EthernetFast frame grabber */
   *ppFrameGrab = createFrameGrabber("FGEthernetFast", "MyGrabber");

   /* Check result from factory */
   if(*ppFrameGrab == NULL)
      {
      delete *ppFrameGrab;
      return SICK_CAMERA_NOT_OK;
      }
   return SICK_CAMERA_OK;
   }

/* Initialize the camera and frame grabber system */
MIL_INT InitCameraSystem(SSickCameraSystem* pCameraSystem)
   {
   int Ret;

   FGEthernetFastParameters* pFrameGrabberParams = static_cast<FGEthernetFastParameters*>(pCameraSystem->pFrameGrab->getParameters());

   /* Set communication parameters */
   Ret = pCameraSystem->pCam->setComParameters(CAMERA_IP,
                                               pFrameGrabberParams->getFrameGrabberPort(),
                                               pFrameGrabberParams->getRedundancyPort(),
                                               EthernetCamera::HIGH_PERFORMANCE_DATA_CHANNEL);
   if (Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Init camera. */
   Ret = pCameraSystem->pCam->init();
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Set the camera IP and buffer size*/
   pFrameGrabberParams->setCameraIP(CAMERA_IP);
   pFrameGrabberParams->setBufferSize(FRAME_BUFFER_SIZE);
   pFrameGrabberParams->setRecoveryInterval(10);

   /* Set the camera data port */
   int CameraDataPort;
   Ret = pCameraSystem->pCam->getCameraDataPort(CameraDataPort);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;
   else 
      pFrameGrabberParams->setCameraPort(CameraDataPort);

   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Ask the camera configuration and fill conversion structure accordingly. */
MIL_INT FillParams(SSickCameraSystem* pCameraSystem,
                   RangerParams* pConvParams,
                   const string& rParamFilename)
   {
   MIL_INT Ret;
   int Ret2;

   /* Switch the camera mode to image */
   Ret = SwitchGrabMode(pCameraSystem, IMAGE_MODE_NAME);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   /* Load parameter file to the camera. */
   Ret2 = pCameraSystem->pCam->fileLoadParameters(rParamFilename);
   if(Ret2 != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Inquire characteristics of the image mode. */
   Ret = GetCameraMilIntParam(pCameraSystem->pCam, ImageConfigPath, StartColumnName, &pConvParams->ImageROIOffsetX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCameraSystem->pCam, ImageCompPath, StartRowName, &pConvParams->ImageROIOffsetY);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCameraSystem->pCam, ImageConfigPath, NumberOfColumnsName, &pConvParams->ImageROISizeX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCameraSystem->pCam, ImageCompPath, NumberOfRowsName, &pConvParams->ImageROISizeY);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   /* Inquire characteristics of the measurement mode. 
      We must switch camera mode to get data format. */
   Ret = SwitchGrabMode(pCameraSystem, MEASUREMENT_MODE_NAME);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   DataFormat CameraMeasFormat;
   Ret = GetCameraDataFormat(pCameraSystem->pCam, &CameraMeasFormat);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   if (CameraMeasFormat.getConfigType() != DataFormat::Linescan_T)
      return SICK_CAMERA_NOT_OK;

   pConvParams->ProfileDataSizeByte = CameraMeasFormat.dataSize();
      
   Ret = GetCameraMilIntParam(pCameraSystem->pCam, MeasConfigPath, CycleTimeName, &pConvParams->CycleTimeMicroseconds);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCameraSystem->pCam, MeasConfigPath, NumberOfColumnsName, &pConvParams->MeasROISizeX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   Ret = GetCameraMilIntParam(pCameraSystem->pCam, MeasConfigPath, StartColumnName, &pConvParams->MeasROIOffsetX);
   if (Ret != SICK_CAMERA_OK)
      return Ret;
      
   /*
   * Since the SICK camera grabs flipped images, we flip them horizontally before using
   * them for camera and laser calibration. However, after flipping, ROI X offset is not
   * good anymore, we must compute the X offset on the right side (before flip) instead
   * of the left side.
   */
   pConvParams->ROIOffsetX = (pConvParams->ImageROIOffsetX + pConvParams->ImageROISizeX) -  (pConvParams->MeasROIOffsetX  + pConvParams->MeasROISizeX);   

   /* Initialize all the components */
   pConvParams->GrayStreamOffset = NO_INFORMATION;
   pConvParams->HiResGrayStreamOffset = NO_INFORMATION;
   pConvParams->ColorStreamOffset = NO_INFORMATION;
   pConvParams->HiResColorStreamOffset = NO_INFORMATION;
   pConvParams->ScatterStreamOffset = NO_INFORMATION; 
   pConvParams->LaserMeasurementMode = NO_INFORMATION;

   /* Mode-specific section. */
   for(uint_t CompIdx = 0; CompIdx < CameraMeasFormat.numComponents(); CompIdx++)
      {
      const Component* pMeasComponent =  CameraMeasFormat.getComponent(CompIdx);
      string MeasCompName = pMeasComponent->getName();
      string MeasValueTypeString = pMeasComponent->getValueType();
      MIL_INT MeasStreamOffset = CameraMeasFormat.getComponentOffset(CompIdx);
      const string MeasCompPath = MeasConfigPath + "<COMPONENT name='" + MeasCompName + "'>";
            
      bool IsLaser = true;
      if (MeasValueTypeString == HorThrValueType)
         Ret = FillHorThrParams(pConvParams, MeasStreamOffset, pMeasComponent, pCameraSystem->pCam, MeasCompPath);         
      else if (MeasValueTypeString == HorMaxValueType)
         Ret = FillHorMaxParams(pConvParams, MeasStreamOffset, pMeasComponent, pCameraSystem->pCam, MeasCompPath);
      else if (MeasValueTypeString == HorMaxThrValueType)
         Ret = FillHorMaxThrParams(pConvParams, MeasStreamOffset, pMeasComponent, pCameraSystem->pCam, MeasCompPath);
      else if (MeasValueTypeString == Hi3DValueType)
         Ret = FillHi3DParams(pConvParams, MeasStreamOffset, pMeasComponent, pCameraSystem->pCam, MeasCompPath); 
      else if (MeasValueTypeString == Hi3DCOGValueType)
         Ret = FillHi3DCogParams(pConvParams, MeasStreamOffset, pMeasComponent, pCameraSystem->pCam, MeasCompPath); 
      else 
         {
         IsLaser = false;
         if (MeasValueTypeString == GrayValueType)
            pConvParams->GrayStreamOffset = MeasStreamOffset;
         else if(MeasValueTypeString == HiResGrayValue)
            pConvParams->HiResGrayStreamOffset = MeasStreamOffset;
         else if(MeasValueTypeString == ColorValueType)
            pConvParams->ColorStreamOffset = MeasStreamOffset;
         else if(MeasValueTypeString == HiResColorValue)
            pConvParams->HiResColorStreamOffset = MeasStreamOffset;
         else if(MeasValueTypeString == ScatterValueType)
            pConvParams->ScatterStreamOffset = MeasStreamOffset;
         else
            return SICK_CAMERA_NOT_OK;
         }

      if(IsLaser)
         {
         MIL_INT MeasROIOffsetY;
         Ret = GetCameraMilIntParam(pCameraSystem->pCam, MeasCompPath, StartRowName, &MeasROIOffsetY);
         if (Ret != SICK_CAMERA_OK)
            return Ret;

         MIL_INT MeasROISizeY;
         Ret = GetCameraMilIntParam(pCameraSystem->pCam, MeasCompPath, NumberOfRowsName, &MeasROISizeY);
         if (Ret != SICK_CAMERA_OK)
            return Ret;
         pConvParams->LaserROIOffsetY = MeasROIOffsetY;
         pConvParams->LaserROISizeY = MeasROISizeY;           
         pConvParams->ROIOffsetY = MeasROIOffsetY - pConvParams->ImageROIOffsetY;
         }
      }

   return SICK_CAMERA_OK;
   }


/* Set the color gain */
MIL_INT SetColorGain(SSickCameraSystem* pCameraSystem, MIL_DOUBLE RedMean, MIL_DOUBLE GreenMean, MIL_DOUBLE BlueMean, bool UseHighres)
   {
   const std::string& CompPath = UseHighres ? HiresColorCompPath : ColorCompPath;
   MIL_INT GainRed = RedMean < 64 ? 4 : (RedMean < 85 ? 3 : 1);
   MIL_INT Ret = SetCameraMilIntParam(pCameraSystem->pCam, CompPath, GainRedName, GainRed);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   MIL_INT GainGreen = GreenMean < 64 ? 4 : (GreenMean < 85 ? 3 : 1);
   Ret = SetCameraMilIntParam(pCameraSystem->pCam, CompPath, GainGreenName, GainGreen);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   MIL_INT GainBlue = BlueMean < 64 ? 4 : (BlueMean < 85 ? 3 : 1);
   Ret = SetCameraMilIntParam(pCameraSystem->pCam, CompPath, GainBlueName, GainBlue);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Set the white balancing coefficient of the color component if there is one */
MIL_INT SetWhiteBalancing(SSickCameraSystem* pCameraSystem, MIL_DOUBLE RedMean, MIL_DOUBLE GreenMean, MIL_DOUBLE BlueMean, bool UseHighres)
   {
   MIL_INT Ret;
   
   const std::string& CompPath = UseHighres ? HiresColorCompPath : ColorCompPath;

   /* Calculate the balances */
   MIL_INT BalanceGreen = (MIL_INT)((RedMean/GreenMean*100.0) + 0.5);
   MIL_INT BalanceBlue = (MIL_INT)((RedMean/BlueMean*100.0) + 0.5);
             
   /* Get the exposure time of the red */
   MIL_INT ExposureTimeRed;
   Ret = GetCameraMilIntParam(pCameraSystem->pCam, CompPath, ExposureTimeRedName, &ExposureTimeRed);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Get the cycle time */
   MIL_INT CycleTime;
   Ret = GetCameraMilIntParam(pCameraSystem->pCam, MeasConfigPath, CycleTimeName, &CycleTime);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Set the exposure time of the red. The maximum exposure time of a band need to be 50 us less than the 
      cycle time is explicitly reset. */
   ExposureTimeRed = CycleTime - ExposureTimeRed < 100 ? CycleTime - 100 : ExposureTimeRed;
   MIL_INT NewExposureTimeGreen = (MIL_INT)(BalanceGreen > 100 ? (MIL_DOUBLE)ExposureTimeRed / BalanceGreen * 100: ExposureTimeRed); 
   MIL_INT NewExposureTimeBlue = (MIL_INT)(BalanceBlue > 100 ? (MIL_DOUBLE)ExposureTimeRed / BalanceBlue * 100: ExposureTimeRed);

   Ret = SetCameraMilIntParam(pCameraSystem->pCam, CompPath, ExposureTimeRedName, NewExposureTimeGreen < NewExposureTimeBlue ? NewExposureTimeGreen : NewExposureTimeBlue);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;  

   /* Set the balance green */
   Ret = SetCameraMilIntParam(pCameraSystem->pCam, CompPath, BalanceGreenName, BalanceGreen);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Set the balance blue */   
   Ret = SetCameraMilIntParam(pCameraSystem->pCam, CompPath, BalanceBlueName, BalanceBlue);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Terminate communication with camera and destroys the frame grabber object. */
void CloseDown(SSickCameraSystem* pCameraSystem)
   {
   /* Stop any grabbing in progress */
   StopGrab(pCameraSystem);

   if(pCameraSystem->pCam != NULL)
      { 
      pCameraSystem->pCam->close(); 
      delete pCameraSystem->pCam;
      }

   if(pCameraSystem->pFrameGrab != NULL)
      { 
      pCameraSystem->pFrameGrab->disconnect();
      delete pCameraSystem->pFrameGrab;
      }
   closeApi();
   }

/* Frame grabber function */
void FrameGrabberProcess(SSickCameraSystem* pCameraSystem, MIL_INT Operation, MIL_INT NbGrab /*= M_DEFAULT*/, MIL_ID StartStageEvent /*= M_NULL*/)
   {
   /* Whatever the mode is we must stop the grab */
   StopGrab(pCameraSystem);

   if(Operation & M_STOP)
      return;
     
   FGEthernetFastParameters* pFGEThernetParams = static_cast<FGEthernetFastParameters*>(pCameraSystem->pFrameGrab->getParameters());

   if(Operation & M_START && pFGEThernetParams->getMode() == FrameGrabber::FGCALLBACK)
      {
      /* Start the grab */
      StartGrab(pCameraSystem, StartStageEvent);      
      }
   else
      {
      MIL_INT Increment = 0;
      MIL_INT GrabCount = 1;
      if(Operation == M_SEQUENCE && NbGrab != M_DEFAULT)
         {
         Increment = 1;
         GrabCount = NbGrab; 
         }

      /* Start the grab */
      StartGrab(pCameraSystem, StartStageEvent); 
           
      /* Get a dummy first buffer */
      /* For some reason the first buffer gotten with getNextIconBuffer contains noisy data */
      IconBuffer* pBuffer;
      MIL_INT Ret = pCameraSystem->pFrameGrab->getNextIconBuffer(&pBuffer, ULONG_MAX); 
      
      for(MIL_INT FrameIdx = 0; FrameIdx < GrabCount && IsActive(pCameraSystem->pFrameGrab); FrameIdx += Increment)
         {
         MIL_INT Ret = pCameraSystem->pFrameGrab->getNextIconBuffer(&pBuffer, ULONG_MAX);

         /* Call the process callback */
         ProcessCallback(pBuffer, pCameraSystem->pFrameGrab, pFGEThernetParams->getUserData());         
         }

      /* Stop the grab */
      StopGrab(pCameraSystem);
      }
   }

/* Switch the camera between area-scan (image) and line-scan (measurement) modes.
   rModeName can be IMAGE_MODE_NAME or MEASUREMENT_MODE_NAME (see cameraconfig.h). */
MIL_INT SwitchGrabMode(SSickCameraSystem* pCameraSystem,
                       const std::string& rModeName,
                       MIL_UINT NbScansPerGrab /* = 1 */,
                       MIL_INT GrabType /* = M_NULL */,
                       FrameGrabberHookStruct* pFrameGrabberHook /* = NULL */)
   {
   int Ret;
   MIL_INT Ret2;
      
   /* Stop the grab */
   Ret2 = StopGrab(pCameraSystem);
   if(Ret2 != SICK_CAMERA_OK)
      return Ret2; 

   /* Disconnect frame grabber. */
   if(pCameraSystem->pFrameGrab->isConnected())
      {
      Ret = pCameraSystem->pFrameGrab->disconnect();
      if(Ret != FrameGrabber::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;
      }

   /* Set the camera mode */
   Ret = pCameraSystem->pCam->setActiveConfiguration(rModeName);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   /* Get the frame grabber parameters */
   FGEthernetFastParameters* pFGEThernetParams = static_cast<FGEthernetFastParameters*>(pCameraSystem->pFrameGrab->getParameters());

   /* Set the data format */
   string CameraDataFormat;
   Ret = pCameraSystem->pCam->getDataFormat("", CameraDataFormat);
   if(Ret == Camera::E_ALL_OK)
      {
      unsigned long PacketSize;
      Ret = pCameraSystem->pCam->getPacketSize(PacketSize);
      if(Ret == Camera::E_ALL_OK)
         pFGEThernetParams->setDataFormat(CameraDataFormat, PacketSize);
      }

   /* Set the number of scans */
   pFGEThernetParams->setNoScans((int)NbScansPerGrab);
   pCameraSystem->pCam->setBufferHeight((unsigned long)NbScansPerGrab);

   /* Set the grab mode */
   pFGEThernetParams->setMode(GrabType == M_ASYNCHRONOUS? FrameGrabber::FGCALLBACK : FrameGrabber::FGPOLLING);

   /* Set the callback function of the frame grabber */
   pFGEThernetParams->setCallback(ProcessCallback);

   /* Set the user data */
   pFGEThernetParams->setUserData(pFrameGrabberHook);    

   /* Connect frame grabber. */
   Ret = pCameraSystem->pFrameGrab->connect();
   if(Ret != FrameGrabber::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;
   
   return SICK_CAMERA_OK;
   }

/*****************************************************************************
 Helper functions.
*****************************************************************************/

/* If not already in that state, send the camera in the stopped state. */
MIL_INT StopCamera(EthernetCamera* pCam)
   {
   int Ret, Status;

   Ret = pCam->checkCamStatus(Status);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   if(Status != EthernetCamera::Stopped)
      {
      Ret = pCam->stop();
      if(Ret != Camera::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;
      }

   return SICK_CAMERA_OK;
   }

/* Fills a DataFormat structure by getting it from the camera. */
MIL_INT GetCameraDataFormat(EthernetCamera* pCam, DataFormat* pDataFormat)
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
MIL_INT GetCameraMilIntParam(EthernetCamera*      pCam,
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

/* Set the camera parameter */
MIL_INT SetCameraMilIntParam(EthernetCamera*      pCam,
                             const string& rPath,
                             const string& rParamName,
                             MIL_INT       IntValue)
   {
   bool UpdateRequired, DataFormatChanged;
   char ParamStringArray[256]; 

   _itoa_s((int)IntValue, ParamStringArray, 10);
   string ParamString = ParamStringArray;   
   int Ret = pCam->setParameterValue(rPath, rParamName, ParamString, UpdateRequired, DataFormatChanged);
   if(Ret != Camera::E_ALL_OK)
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Frame grabber processing hook function */
void ProcessCallback(IconBuffer *pBuffer, FrameGrabber* pFrameGrab, void* UserData)
   {
   /* Get the FrameGrabberHookStruct */
   FrameGrabberHookStruct* pFrameGrabHookStruct = (FrameGrabberHookStruct*) UserData;

   /* Create a mil image on the ranger data */
   MIL_ID MilRangerImage = MbufCreate2d(pFrameGrabHookStruct->MilSystem,
      pBuffer->getWidth(),
      pBuffer->getHeight(),
      8 + M_UNSIGNED,
      M_IMAGE + M_PROC,
      M_HOST_ADDRESS + M_PITCH_BYTE,
      pBuffer->getWidth(),
      pBuffer->getWritePointer(0),
      M_NULL);

   /* Call the Mil hook function */
   (*(pFrameGrabHookStruct->HookHandlerPtr))(0, MilRangerImage, pFrameGrabHookStruct->UserDataPtr);

   /* Free the created buffer */
   MbufFree(MilRangerImage);
   }

/* Start grab function */
MIL_INT StartGrab(SSickCameraSystem* pCameraSystem, MIL_ID StartStageEvent /* = M_NULL */)
   {
   /* Get the index of a free frame grabber */
   MIL_INT Index  = GetNewActiveIndex();
   if(Index != -1)
      {
      MIL_INT Ret;

      /* Set the frame grabber as active */
      ActiveFrameGrabber[Index] = pCameraSystem->pFrameGrab;

      /* Start the frame grabber grab */
      Ret = pCameraSystem->pFrameGrab->startGrab();
      if(Ret != FrameGrabber::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;

      /* Start the camera */
      Ret = pCameraSystem->pCam->start();
      if(Ret != Camera::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;

      /* Start the conveyor and wait for its speed to become constant */
      if(StartStageEvent)
         {
         MthrControl(StartStageEvent, M_EVENT_SET, M_SIGNALED);
         MosSleep(CONVEYOR_STARTUP_DELAY_MS);
         }
      }
   else
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Stop grab function */
MIL_INT StopGrab(SSickCameraSystem* pCameraSystem)
   {
   MIL_INT Ret;
   MIL_INT Ret2;

   /* Remove from active */
   if(RemoveActiveFrameGrabber(pCameraSystem->pFrameGrab))
      {
      /* Stop the grab */
      Ret = pCameraSystem->pFrameGrab->stopGrab();
      if(Ret != FrameGrabber::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;

      /* Stop the camera */
      int Status;
      Ret2 = pCameraSystem->pCam->checkCamStatus(Status);
      if(Ret2 != Camera::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;

      if(Status != RangerE::Stopped)
         {
         Ret = pCameraSystem->pCam->stop();
         if(Ret != Camera::E_ALL_OK)
            return SICK_CAMERA_NOT_OK;
         }    
      }

   return SICK_CAMERA_OK;
   }

/* Function to get the new active index in the list */
MIL_INT GetNewActiveIndex()
   {
   for(MIL_INT i = 0; i < MAX_FRAME_GRABBER; i++)
      {
      if(ActiveFrameGrabber[i] == NULL)
         return i;
      }
   return -1;
   }

bool IsActive(FrameGrabber* pFrameGrab)
   {
   for(MIL_INT i = 0; i < MAX_FRAME_GRABBER; i++)
      {
      if(ActiveFrameGrabber[i] == pFrameGrab)
         return true;
      }
   return false;
   }

/* Function to remove the active frame grabber */
bool RemoveActiveFrameGrabber(FrameGrabber* pFrameGrab)
   {
   for(MIL_INT i = 0; i < MAX_FRAME_GRABBER; i++)
      {
      if(ActiveFrameGrabber[i] == pFrameGrab)
         {
         ActiveFrameGrabber[i] = NULL;
         return true;
         }
      }
   return false;
   }

/* Fill the parameters for the HorThr mode */
MIL_INT FillHorThrParams(RangerParams* pConvParams,
                         MIL_INT MeasStreamOffset,
                         const Component* pMeasComponent,
                         RangerE* pCam,
                         const string MeasCompPath ) 
   {
   MIL_INT Ret;

   pConvParams->LaserMeasurementMode = RANGER_HOR_THR_MODE;

   pConvParams->RangeStreamOffset     = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(RangeSubCompName);
   pConvParams->LaserIntensityStreamOffset = NO_INFORMATION;
   pConvParams->LaserIntensitySizeBit      = NO_INFORMATION;
   pConvParams->LaserScatterStreamOffset   = NO_INFORMATION;
   pConvParams->LaserScatterSizeBit        = NO_INFORMATION;

   if ( pConvParams->RangeStreamOffset == DataFormat::E_BAD_NAME )
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
      pConvParams->LaserROISizeY != 512 )
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Fill the parameters for the HorMax mode */
MIL_INT FillHorMaxParams(RangerParams* pConvParams,
                         MIL_INT MeasStreamOffset,
                         const Component* pMeasComponent,
                         RangerE* pCam,
                         const string MeasCompPath) 
   {
   MIL_INT Ret;

   pConvParams->LaserMeasurementMode = RANGER_HOR_MAX_MODE;

   pConvParams->RangeStreamOffset     = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(RangeSubCompName);
   pConvParams->LaserIntensityStreamOffset = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(IntensitySubCompName);
   pConvParams->LaserIntensitySizeBit      = 8;
   pConvParams->LaserScatterStreamOffset   = NO_INFORMATION;
   pConvParams->LaserScatterSizeBit        = NO_INFORMATION;

   if ( pConvParams->RangeStreamOffset     == DataFormat::E_BAD_NAME ||
      pConvParams->LaserIntensityStreamOffset == DataFormat::E_BAD_NAME )
      return SICK_CAMERA_NOT_OK;

   Ret = GetCameraMilIntParam(pCam, MeasCompPath, SubPixelingName,
      &pConvParams->FixedPoint);
   if (Ret != SICK_CAMERA_OK)
      return Ret;

   /* Unfortunately, sub-pixeling option does not seem to work on the Ranger E55
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
      pConvParams->LaserROISizeY != 512 )
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Fill the parameters for the HorMaxThr mode */
MIL_INT FillHorMaxThrParams(RangerParams* pConvParams,
                            MIL_INT MeasStreamOffset,
                            const Component* pMeasComponent,
                            RangerE* pCam,
                            const string MeasCompPath ) 
   {
   MIL_INT Ret;

   pConvParams->LaserMeasurementMode = RANGER_HOR_MAX_AND_THRES_MODE;

   pConvParams->RangeStreamOffset     = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(RangeSubCompName);
   pConvParams->LaserIntensityStreamOffset = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(IntensitySubCompName);
   pConvParams->LaserIntensitySizeBit      = 8;
   pConvParams->LaserScatterStreamOffset   = NO_INFORMATION;
   pConvParams->LaserScatterSizeBit        = NO_INFORMATION;

   if ( pConvParams->RangeStreamOffset     == DataFormat::E_BAD_NAME ||
      pConvParams->LaserIntensityStreamOffset == DataFormat::E_BAD_NAME )
      return SICK_CAMERA_NOT_OK;

   /* The fixed point is always supposed to be 1 in this measurement mode */
   pConvParams->FixedPoint = 1;

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
      pConvParams->LaserROISizeY != 512 )
      return SICK_CAMERA_NOT_OK;

   return SICK_CAMERA_OK;
   }

/* Fill the parameters for the Hi3D mode */
MIL_INT FillHi3DParams(RangerParams* pConvParams,
                       MIL_INT MeasStreamOffset,
                       const Component* pMeasComponent,
                       RangerE* pCam,
                       const string MeasCompPath ) 
   {
   MIL_INT Ret;

   pConvParams->LaserMeasurementMode = M_RANGER_Hi3D_MODE;

   pConvParams->RangeStreamOffset     = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(RangeSubCompName);
   pConvParams->LaserIntensityStreamOffset = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(IntensitySubCompName);
   pConvParams->LaserIntensitySizeBit      = 8;

   if ( pConvParams->RangeStreamOffset     == DataFormat::E_BAD_NAME ||
        pConvParams->LaserIntensityStreamOffset == DataFormat::E_BAD_NAME )
      return SICK_CAMERA_NOT_OK;

   MIL_INT EnableScatterValue;
   Ret = GetCameraMilIntParam(pCam, MeasCompPath, EnableScatterName, &EnableScatterValue);
   if(Ret =! SICK_CAMERA_NOT_OK)
      return Ret;
   
   if(EnableScatterValue == 0)
      {
      pConvParams->LaserScatterStreamOffset   = NO_INFORMATION;
      pConvParams->LaserScatterSizeBit        = NO_INFORMATION;
      }
   else if(EnableScatterValue == 1)
      {
      pConvParams->LaserScatterStreamOffset = MeasStreamOffset +
         pMeasComponent->getSubComponentOffset(ScatterSubCompName);
      pConvParams->LaserScatterSizeBit = 8;
      }
   else
      return SICK_CAMERA_NOT_OK;
 
   /* The fixed point value for the Hi3D mode is fixed at 4*/
   pConvParams->FixedPoint   = RANGER_E55_DEFAULT_HI3D_FIXED_POINT;
   pConvParams->RangeSizeBit = 16;

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

   return SICK_CAMERA_OK;
   }

/* Fill the parameters for the Hi3DCog mode */
MIL_INT FillHi3DCogParams(RangerParams* pConvParams,
                          MIL_INT MeasStreamOffset,
                          const Component* pMeasComponent,
                          RangerE* pCam,
                          const string MeasCompPath ) 
   {
   MIL_INT Ret;

   /* Set the laser measurement mode */
   pConvParams->LaserMeasurementMode = M_RANGER_Hi3D_COG_MODE;

   pConvParams->RangeStreamOffset     = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset(XSumSubCompName);
   pConvParams->LaserIntensityStreamOffset = MeasStreamOffset +
      pMeasComponent->getSubComponentOffset( SumSubCompName);
   pConvParams->LaserIntensitySizeBit      = 16;
   pConvParams->LaserScatterStreamOffset   = NO_INFORMATION;
   pConvParams->LaserScatterSizeBit        = NO_INFORMATION;

   if ( pConvParams->RangeStreamOffset     == DataFormat::E_BAD_NAME ||
      pConvParams->LaserIntensityStreamOffset == DataFormat::E_BAD_NAME )
      return SICK_CAMERA_NOT_OK;

   pConvParams->FixedPoint            = RANGER_E55_DEFAULT_COG_FIXED_POINT;

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

   return SICK_CAMERA_OK;
   }

#endif
