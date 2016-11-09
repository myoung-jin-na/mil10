//***************************************************************************************
// 
// File name: RegionManager.cpp
//
// Synopsis: This file contains the implementation of the CRegionMngr class
//           which is the class that manages the use of a region in an inspection task.
//

#include <mil.h>
#include "RegionManager.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CRegionMngr::CRegionMngr()
 : m_MilRasterRegion(M_NULL)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CRegionMngr::~CRegionMngr()
   {
   CRegionMngr::Free();   
   }  

//*****************************************************************************
// Initialization function.
//*****************************************************************************
void CRegionMngr::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   // Allocate the raster region and clear it.
   if(ImageSizeX && ImageSizeY)
      {
      MbufAlloc2d(MilSystem, ImageSizeX, ImageSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilRasterRegion);
      MbufClear(m_MilRasterRegion, 0);
      }
   }

//*****************************************************************************
// Function to set the region in an image.
//*****************************************************************************
void CRegionMngr::SetRegion(MIL_ID MilImage, MIL_ID MilRegionGraList)
   {
   MbufSetRegion(MilImage, MilRegionGraList, M_DEFAULT, M_NO_RASTERIZE, M_DEFAULT);   
   }

//*****************************************************************************
// Function that rasterizes the region in the raster image.
// Returns the rasterized image id.
//*****************************************************************************
MIL_ID CRegionMngr::RasterizeRegion(MIL_ID MilCalibration, MIL_ID MilRegionGraList)
   {
   // Associate the calibration to the rasterize image.
   McalAssociate(MilCalibration, m_MilRasterRegion, M_DEFAULT);

   // Draw the region in the buffer.
   MgraDraw(MilRegionGraList, m_MilRasterRegion, M_DEFAULT);
     
   return m_MilRasterRegion;
   }

//*****************************************************************************
// Function that frees the region manager.
//*****************************************************************************
void CRegionMngr::Free()
   {
   if(m_MilRasterRegion)
      {
      MbufFree(m_MilRasterRegion);
      m_MilRasterRegion = M_NULL;
      }
   }

//*****************************************************************************
// Function that draws the text in the region.
//*****************************************************************************
void CRegionMngr::DrawTextInRegion(MIL_ID MilGraContext, MIL_ID MilRegionGraList, MIL_ID MilDest, MIL_INT RegionIndex, MIL_CONST_TEXT_PTR String)
   {
   if(MilRegionGraList)
      {
      // Get the original text alignment.
      MIL_INT CurTextAlignHorizontal = MgraInquire(MilGraContext, M_TEXT_ALIGN_HORIZONTAL, M_NULL);
      MIL_INT CurTextAlignVertical = MgraInquire(MilGraContext, M_TEXT_ALIGN_VERTICAL, M_NULL);
      
      // Draw the text centered on the position.
      MIL_DOUBLE PosX, PosY;
      MgraInquireList(MilRegionGraList, M_GRAPHIC_INDEX(RegionIndex), M_DEFAULT, M_POSITION_X, &PosX);
      MgraInquireList(MilRegionGraList, M_GRAPHIC_INDEX(RegionIndex), M_DEFAULT, M_POSITION_Y, &PosY);
      MgraControl(MilGraContext, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
      MgraControl(MilGraContext, M_TEXT_ALIGN_VERTICAL, M_CENTER);

      // Draw the text.
      MgraText(MilGraContext, MilDest, PosX, PosY, String);

      // Put the text alignment back.
      MgraControl(MilGraContext, M_TEXT_ALIGN_HORIZONTAL, (MIL_DOUBLE)CurTextAlignHorizontal);
      MgraControl(MilGraContext, M_TEXT_ALIGN_VERTICAL, (MIL_DOUBLE)CurTextAlignVertical);
      }
   else
      // Draw the text.
      MgraText(MilGraContext, MilDest, 0, 0, String);
   }