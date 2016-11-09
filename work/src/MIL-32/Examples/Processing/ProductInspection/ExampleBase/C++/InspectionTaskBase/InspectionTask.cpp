//***************************************************************************************
// 
// File name: InspectionTask.cpp
//
// Synopsis: This file contains the implementation of the CInspectionTask class
//           which is the base class of all the inspection tasks.
//
#include <mil.h>
#include "InspectionTask.h"


//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CInspectionTask::CInspectionTask(MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : m_MilWorkImage            (M_NULL),
   m_MilOutputImage          (M_NULL),
   m_MilOutputRegionGraList  (M_NULL),
   m_ColorConversion         (ColorConversion),
   m_FixtureProvider         (FixtureProvider),
   m_ImageProvider           (ImageProvider),
   m_RegionProvider          (RegionProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CInspectionTask::~CInspectionTask()
   {
   CInspectionTask::Free();  
   }  

//*****************************************************************************
// Initialization function.
//*****************************************************************************
void CInspectionTask::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   }

//*****************************************************************************
// Free function.
//*****************************************************************************
void CInspectionTask::Free()
   {
   if(m_MilWorkImage)
      {
      MbufFree(m_MilWorkImage);
      m_MilWorkImage = M_NULL;
      }
   if(m_MilOutputImage)
      {
      MbufFree(m_MilOutputImage);
      m_MilOutputImage = M_NULL;
      }
   if(m_MilOutputRegionGraList)
      {
      MgraFree(m_MilOutputRegionGraList);
      m_MilOutputRegionGraList = M_NULL;
      }
   }  

//*****************************************************************************
// Function to inspect an image.
//*****************************************************************************
void CInspectionTask::InspectImage(MIL_ID MilImage)
   {
   // Reset the work image if necessary.
   if(m_MilWorkImage)
      {
      MbufFree(m_MilWorkImage);
      m_MilWorkImage = M_NULL;
      }
      
   // If one of the providers has failed.
   if((m_FixtureProvider && !m_FixtureProvider->IsResultValid()) ||
      (m_ImageProvider && !m_ImageProvider->IsResultValid())     ||
      (m_RegionProvider && !m_RegionProvider->IsResultValid()))
      m_IsResultValid = false;
   else
      {
      // Create the converted image.
      m_MilWorkImage = CreateConvertedImage(m_ImageProvider ? m_ImageProvider->GetOutputImage() : MilImage, m_ColorConversion);

      // Set the fixture in the image.
      SetImageFixture(m_MilWorkImage);

      // Inspect the image.
      m_IsResultValid = Inspect(m_MilWorkImage);
      }   
   }

//*****************************************************************************
// Function to set the fixture.
//*****************************************************************************
void CInspectionTask::SetOutputFixture(MIL_INT LocationType, MIL_ID FixturingOffset, MIL_INT CalOrLocSourceId, MIL_DOUBLE Param1, MIL_DOUBLE Param2, MIL_DOUBLE Param3, MIL_DOUBLE Param4)
   {
   McalFixture(m_MilWorkImage, FixturingOffset, M_MOVE_RELATIVE, LocationType, CalOrLocSourceId, Param1, Param2, Param3, Param4);
   }

void CInspectionTask::SetImageFixture(MIL_ID MilImage)
   {
   if(m_FixtureProvider)
      {
      McalAssociate(m_FixtureProvider->GetFixture(), MilImage, M_DEFAULT);
      }
   else
      {
      McalAssociate(M_NULL, MilImage, M_DEFAULT);      
      McalUniform(MilImage, 0.0, 0.0, 1.0, 1.0, 0.0, M_DEFAULT);
      McalRelativeOrigin(MilImage, 0, 0, 0, 0, M_DEFAULT); 
      }
   }

//*****************************************************************************
// Functions allocates the output image and region graphics list.
//*****************************************************************************
void CInspectionTask::AllocateOutputImage(MIL_ID MilSystem, MIL_INT NbBands, MIL_INT SizeX, MIL_INT SizeY, MIL_INT Type, MIL_INT64 BufAttribute)
   {
   if(m_MilOutputImage)
      MbufFree(m_MilOutputImage);
   MbufAllocColor(MilSystem, NbBands, SizeX, SizeY, Type, BufAttribute, &m_MilOutputImage);
   }

void CInspectionTask::AllocateRegionGraList(MIL_ID MilSystem)
   {
   if(m_MilOutputRegionGraList)
      MgraFree(m_MilOutputRegionGraList);
   MgraAllocList(MilSystem, M_DEFAULT, &m_MilOutputRegionGraList);
   }

//*****************************************************************************
// Function that draw the graphical result.
//*****************************************************************************
void CInspectionTask::DrawInspectionGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   if(m_IsResultValid)
      {
      if(m_FixtureProvider)
         {
         // Set the calibration of the graphic context.
         MgraControl(MilGraContext, M_GRAPHIC_SOURCE_CALIBRATION, (MIL_DOUBLE)m_FixtureProvider->GetFixture());

         // Put the drawing in world units.
         MgraControl(MilGraContext, M_INPUT_UNITS, M_WORLD);
         }
      else
         {
         // Set the calibration of the graphic context.
         MgraControl(MilGraContext, M_GRAPHIC_SOURCE_CALIBRATION, M_NULL);

         // Put the drawing in pixel units.
         MgraControl(MilGraContext, M_INPUT_UNITS, M_PIXEL);
         }
      DrawGraphicalResult(MilGraContext, MilDest);
      }   
   }

//*****************************************************************************
// Function to draw the output fixture.
//*****************************************************************************
void CInspectionTask::DrawOutputFixture(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   McalDraw(MilGraContext, m_MilWorkImage, MilDest, M_DRAW_RELATIVE_COORDINATE_SYSTEM + M_DRAW_FRAME, M_DEFAULT, M_DEFAULT);
   }

//*****************************************************************************
// Function to draw the input region.
//*****************************************************************************
void CInspectionTask::DrawInputRegion(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   if(HasRegionProvider())
      m_RegionProvider->DrawOutputRegion(MilGraContext, MilDest);
   }

//*****************************************************************************
// Function to convert the image.
//*****************************************************************************
MIL_ID CInspectionTask::CreateConvertedImage(MIL_ID MilColorImage, MIL_INT ConversionType)
   {
   MIL_ID MilConvertedImage;

   switch(ConversionType)
      {
      case M_RGB_TO_L:
      case M_RGB_TO_Y:
         MbufAlloc2d(MbufInquire(MilColorImage, M_OWNER_SYSTEM, M_NULL),
                     MbufInquire(MilColorImage, M_SIZE_X, M_NULL),
                     MbufInquire(MilColorImage, M_SIZE_Y, M_NULL),
                     8+M_UNSIGNED,
                     M_IMAGE+M_PROC,
                     &MilConvertedImage);
         MimConvert(MilColorImage, MilConvertedImage, ConversionType);
         break;

      case M_RED:
      case M_GREEN:
      case M_BLUE:
         MbufChildColor(MilColorImage, ConversionType, &MilConvertedImage);
         break;

      case M_NONE:
      default:
         MbufCreateColor(MbufInquire(MilColorImage, M_OWNER_SYSTEM, M_NULL),
                         M_DEFAULT,
                         M_DEFAULT,
                         M_DEFAULT,
                         M_DEFAULT,
                         M_IMAGE+M_PROC,
                         M_MIL_ID,
                         M_DEFAULT,
                         (void**)&MilColorImage,
                         &MilConvertedImage);         
         break;
      }

   return MilConvertedImage;
   }