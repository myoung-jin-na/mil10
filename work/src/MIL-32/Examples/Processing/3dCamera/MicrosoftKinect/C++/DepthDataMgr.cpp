//***************************************************************************************/
// 
// File name: DepthDataMgr.cpp  
//
// Synopsis:  Implementation of the CDepthDataMgr a class that manages, transforms
//            and registers depth data of the Kinect 2 camera system.
//

#include <Mil.h>
#include <math.h>
#include "DepthDataMgr.h"
#include "McalTransformCoordinate3dListMP.h"

// Useful defines.
#define M_Round(x) ((MIL_INT)((x) + ((x) >= 0 ? 0.5 : -0.5)))

// Constants.
static const MIL_INT DEFAULT_FILL_HOLES_CLOSE_ITER = 2;

// Templates instantiation.
template void CDepthDataMgr::CalculateWorldPoints<true>(MIL_ID MilDepthImage);
template void CDepthDataMgr::CalculateWorldPoints<false>(MIL_ID MilDepthImage);

//****************************************************************************
// Constructor.
//****************************************************************************
CDepthDataMgr::CDepthDataMgr(MIL_ID MilSystem, MIL_INT DepthImageSizeX, MIL_INT DepthImageSizeY)
   : m_MilSystem(MilSystem),
     m_DepthImageSizeX(DepthImageSizeX),
     m_DepthImageSizeY(DepthImageSizeY),
     m_DepthValid(false),
     m_NbWorld(0),
     m_FillHolesCloseIter(DEFAULT_FILL_HOLES_CLOSE_ITER)
   {
   MIL_INT MaxNbWorldPoints = DepthImageSizeX * DepthImageSizeY;

   // Allocate the arrays.
   m_pXWorld = new MIL_DOUBLE[MaxNbWorldPoints];
   m_pYWorld = new MIL_DOUBLE[MaxNbWorldPoints];
   m_pZWorld = new MIL_DOUBLE[MaxNbWorldPoints];
   m_pDepthXPixel = new MIL_UINT16[MaxNbWorldPoints];
   m_pDepthYPixel = new MIL_UINT16[MaxNbWorldPoints];
   m_pColorXPixel = new MIL_DOUBLE[MaxNbWorldPoints];
   m_pColorYPixel = new MIL_DOUBLE[MaxNbWorldPoints];
   m_pXMultipliers = new MIL_DOUBLE[MaxNbWorldPoints];
   m_pYMultipliers = new MIL_DOUBLE[MaxNbWorldPoints];

   // Allocate the temporary images to fill the holes.
   MbufAlloc2d(MilSystem, DepthImageSizeX, DepthImageSizeY, 16 + M_UNSIGNED, M_IMAGE+M_PROC, &m_MilDepthFilledHoles);
   MbufAllocColor(MilSystem, 3, DepthImageSizeX, DepthImageSizeY, 8 + M_UNSIGNED, M_IMAGE+M_PROC, &m_MilColorFilledHoles);

   // Allocate the luts.
   MbufAlloc2d(MilSystem, DepthImageSizeX, DepthImageSizeY, 16+M_SIGNED, M_LUT, &m_MilColorToDepthLutX);
   MbufAlloc2d(MilSystem, DepthImageSizeX, DepthImageSizeY, 16+M_SIGNED, M_LUT, &m_MilColorToDepthLutY);
   MbufAlloc1d(MilSystem, MaxNbWorldPoints, 16+M_SIGNED, M_LUT, &m_MilColorToWorldLutXContainer);
   MbufAlloc1d(MilSystem, MaxNbWorldPoints, 16+M_SIGNED, M_LUT, &m_MilColorToWorldLutYContainer);
   MbufChild1d(m_MilColorToWorldLutXContainer, 0, 1, &m_MilColorToWorldLutX);
   MbufChild1d(m_MilColorToWorldLutYContainer, 0, 1, &m_MilColorToWorldLutY);

   // Calculate the number of fixed points of the lut.
   MIL_DOUBLE MaxAbsValue = (MIL_DOUBLE)(DepthImageSizeX > DepthImageSizeY ? DepthImageSizeX : DepthImageSizeY);
   m_LutNbFixedPoint = Calculate16BitNbFixedPoint(MaxAbsValue);
   }

//****************************************************************************
// Destructor.
//****************************************************************************
CDepthDataMgr::~CDepthDataMgr()
   {
   // Free the luts.
   MbufFree(m_MilColorToWorldLutY);
   MbufFree(m_MilColorToWorldLutX);
   MbufFree(m_MilColorToWorldLutYContainer);
   MbufFree(m_MilColorToWorldLutXContainer);
   MbufFree(m_MilColorToDepthLutY);
   MbufFree(m_MilColorToDepthLutX);

   // Delete the temporary images to fill the holes.
   MbufFree(m_MilColorFilledHoles);
   MbufFree(m_MilDepthFilledHoles);

   // Free the arrays.
   delete [] m_pYMultipliers;
   delete [] m_pXMultipliers;

   delete [] m_pColorYPixel;
   delete [] m_pColorXPixel;
   delete [] m_pDepthYPixel;
   delete [] m_pDepthXPixel;
   delete [] m_pZWorld;
   delete [] m_pYWorld;
   delete [] m_pXWorld;
   }

//****************************************************************************
// Function to calculate the multipliers.
//****************************************************************************
void CDepthDataMgr::CalculateMultipliers(MIL_ID MilCalibration)
   {
   // Allocate temporary arrays.
   MIL_INT TotalNbPixels = m_DepthImageSizeX * m_DepthImageSizeY;
   MIL_DOUBLE* XPixel = new MIL_DOUBLE[TotalNbPixels];
   MIL_DOUBLE* YPixel = new MIL_DOUBLE[TotalNbPixels];
   MIL_DOUBLE* XUnitVec = new MIL_DOUBLE[TotalNbPixels];
   MIL_DOUBLE* YUnitVec = new MIL_DOUBLE[TotalNbPixels];
   MIL_DOUBLE* ZUnitVec = new MIL_DOUBLE[TotalNbPixels];

   // Fill the pixel coordinates.
   for(MIL_INT Y = 0, i = 0; Y < m_DepthImageSizeY; Y++)
      {
      for(MIL_INT X =0; X <m_DepthImageSizeX; X++, i++)
         {
         XPixel[i] = (MIL_DOUBLE)X;
         YPixel[i] = (MIL_DOUBLE)Y;
         }
      }

   // Get the units vectors of each pixel of the depth image.
   McalTransformCoordinate3dList(MilCalibration,
                                 M_PIXEL_COORDINATE_SYSTEM,
                                 M_CAMERA_COORDINATE_SYSTEM,
                                 TotalNbPixels,
                                 XPixel,
                                 YPixel,
                                 M_NULL,
                                 XUnitVec,
                                 YUnitVec,
                                 ZUnitVec,
                                 M_UNIT_DIRECTION_VECTOR);

   // Calculate the multipliers.
   for(MIL_INT PixelIdx = 0; PixelIdx < TotalNbPixels; PixelIdx++)
      {
      m_pXMultipliers[PixelIdx] = XUnitVec[PixelIdx] / ZUnitVec[PixelIdx];
      m_pYMultipliers[PixelIdx] = YUnitVec[PixelIdx] / ZUnitVec[PixelIdx];      
      }

   // Delete the arrays.
   delete [] ZUnitVec;
   delete [] YUnitVec;
   delete [] XUnitVec;
   delete [] YPixel;
   delete [] XPixel;
   }

//****************************************************************************
// Function to calculate the world points.
//****************************************************************************
template <bool GenAssociatedDepthPixel>
void CDepthDataMgr::CalculateWorldPoints(MIL_ID MilDepthImage)
   {
   // Get the host address and pitch of the depth image.
   MIL_UINT16* pDepthData = (MIL_UINT16*)MbufInquire(MilDepthImage, M_HOST_ADDRESS, M_NULL);
   MIL_INT DepthPitch = MbufInquire(MilDepthImage, M_PITCH, M_NULL);
   
   m_NbWorld = 0;
   MIL_INT MultIdx = 0;
   MIL_INT DepthIdx = 0;
   for(MIL_UINT16 Y = 0; Y < m_DepthImageSizeY; Y++)
      {
      for(MIL_UINT16 X = 0; X < m_DepthImageSizeX; X++)
         {
         if(pDepthData[DepthIdx] != 0)
            {
            // Calculate the world position of the point in the depth image.
            m_pXWorld[m_NbWorld] = pDepthData[DepthIdx] * m_pXMultipliers[MultIdx];
            m_pYWorld[m_NbWorld] = pDepthData[DepthIdx] * m_pYMultipliers[MultIdx];
            m_pZWorld[m_NbWorld] = (MIL_DOUBLE)pDepthData[DepthIdx];
            if(GenAssociatedDepthPixel)
               {
               m_pDepthXPixel[m_NbWorld] = X;
               m_pDepthYPixel[m_NbWorld] = Y;
               }
            m_NbWorld++;
            }
         MultIdx++;
         DepthIdx++;
         }
      DepthIdx += DepthPitch - m_DepthImageSizeX;
      }
   }

//****************************************************************************
// Function to move the world points.
//****************************************************************************
void CDepthDataMgr::MoveWorldPoints(MIL_ID MilCalibration, MIL_INT SrcCoordinateSystem, MIL_INT DstCoordinateSystem)
   {
   McalTransformCoordinate3dListMP(m_MilSystem,
                                   MilCalibration,
                                   SrcCoordinateSystem,
                                   DstCoordinateSystem,
                                   m_NbWorld,
                                   m_pXWorld,
                                   m_pYWorld,
                                   m_pZWorld,
                                   m_pXWorld,
                                   m_pYWorld,
                                   m_pZWorld,
                                   M_DEFAULT);
   }
//****************************************************************************
// Function that warps the color image.
//****************************************************************************
void CDepthDataMgr::WarpColorImage(MIL_ID MilColorImage, MIL_ID MilWarpedColorImage, bool UseWorldLut) const
   {
   if(UseWorldLut)
      MimWarp(MilColorImage, MilWarpedColorImage, m_MilColorToWorldLutX, m_MilColorToWorldLutY,
              M_WARP_LUT + M_FIXED_POINT + m_LutNbFixedPoint, M_BILINEAR+M_OVERSCAN_CLEAR);
   else
      MimWarp(MilColorImage, MilWarpedColorImage, m_MilColorToDepthLutX, m_MilColorToDepthLutY,
              M_WARP_LUT + M_FIXED_POINT + m_LutNbFixedPoint, M_BILINEAR+M_OVERSCAN_CLEAR);
   }

//****************************************************************************
// Function to generate the color luts.
//****************************************************************************
void CDepthDataMgr::GenColorLuts(MIL_ID MilColorCameraCalibration, bool GenWorldLut)
   {
   if(m_NbWorld)
      {
      // Get the pixel coordinates of the 3d points in the color image.
      McalTransformCoordinate3dListMP(m_MilSystem,
                                       MilColorCameraCalibration,
                                       M_TOOL_COORDINATE_SYSTEM,
                                       M_PIXEL_COORDINATE_SYSTEM,
                                       m_NbWorld,
                                       m_pXWorld,
                                       m_pYWorld,
                                       m_pZWorld,
                                       m_pColorXPixel,
                                       m_pColorYPixel,
                                       M_NULL,
                                       M_DEFAULT);

      

      // Generate the luts.
      if(GenWorldLut)
         {
         MbufChildMove(m_MilColorToWorldLutX, 0, 0, m_NbWorld, 1, M_DEFAULT);
         MbufChildMove(m_MilColorToWorldLutY, 0, 0, m_NbWorld, 1, M_DEFAULT);
         FillColorToWorldLuts();
         }
      else
         FillColorToDepthLuts();
      }
   }

//****************************************************************************
// Function to fill the color to world or depth luts.
//****************************************************************************
void CDepthDataMgr::FillColorToDepthLuts()
   {
   // Get the host address of the luts.
   MIL_INT16* pLutXData = (MIL_INT16*)MbufInquire(m_MilColorToDepthLutX, M_HOST_ADDRESS, M_NULL);
   MIL_INT16* pLutYData = (MIL_INT16*)MbufInquire(m_MilColorToDepthLutY, M_HOST_ADDRESS, M_NULL);
   MIL_INT LutPitch = MbufInquire(m_MilColorToDepthLutX, M_PITCH, M_NULL);

   // Get the multiplication factor of the luts.
   MIL_DOUBLE LutMultFactor = (MIL_DOUBLE)(1 << m_LutNbFixedPoint);

   // Clear the luts.
   MbufClear(m_MilColorToDepthLutX, -1*LutMultFactor);
   MbufClear(m_MilColorToDepthLutY, -1*LutMultFactor);   

   for(MIL_INT WorldIdx = 0; WorldIdx < m_NbWorld; WorldIdx++)
      {
      MIL_INT Offset = m_pDepthXPixel[WorldIdx] + m_pDepthYPixel[WorldIdx]*LutPitch;
      pLutXData[Offset] = (MIL_INT16)M_Round(m_pColorXPixel[WorldIdx] * LutMultFactor);
      pLutYData[Offset] = (MIL_INT16)M_Round(m_pColorYPixel[WorldIdx] * LutMultFactor);
      }
   }

void CDepthDataMgr::FillColorToWorldLuts()
   {
   // Get the host address of the luts.
   MIL_INT16* pLutXData = (MIL_INT16*)MbufInquire(m_MilColorToWorldLutX, M_HOST_ADDRESS, M_NULL);
   MIL_INT16* pLutYData = (MIL_INT16*)MbufInquire(m_MilColorToWorldLutY, M_HOST_ADDRESS, M_NULL);

   // Get the multiplication factor of the luts.
   MIL_DOUBLE LutMultFactor = (MIL_DOUBLE)(1 << m_LutNbFixedPoint);

   // Generate the luts.
   for(MIL_INT WorldIdx = 0; WorldIdx < m_NbWorld; WorldIdx++)
      {
      pLutXData[WorldIdx] = (MIL_INT16)M_Round(m_pColorXPixel[WorldIdx] * LutMultFactor);
      pLutYData[WorldIdx] = (MIL_INT16)M_Round(m_pColorYPixel[WorldIdx] * LutMultFactor);
      }
   }

//****************************************************************************
// Function to create the depth map.
//****************************************************************************
void CDepthDataMgr::CreateDepthMap(MIL_ID MilWorldPackedColorImage,
                                   MIL_ID MilDepthMapImage,
                                   MIL_ID MilColorDepthMapImage,
                                   const SDepthMapGenParam& rGenParam) const
   {
   // Create the depth map with holes.
   if(MilColorDepthMapImage == M_NULL)
      ExtractDepthMapNoFill<false>(MilWorldPackedColorImage, MilDepthMapImage, MilColorDepthMapImage, rGenParam);
   else
      ExtractDepthMapNoFill<true>(MilWorldPackedColorImage, MilDepthMapImage, MilColorDepthMapImage, rGenParam);
      
   // Fill the holes.
   FillDepthMapHoles(MilDepthMapImage, MilColorDepthMapImage, MilColorDepthMapImage != M_NULL);
   }

//****************************************************************************
// Function to extract the depth map.
//****************************************************************************
template <bool GenColorDepthMap>
void CDepthDataMgr::ExtractDepthMapNoFill(MIL_ID MilWorldPackedColorImage,
                                          MIL_ID MilDepthMapImage,
                                          MIL_ID MilColorDepthMapImage,
                                          const SDepthMapGenParam& rGenParam) const
   {
   // Get the pointer to the depth map buffer and get the pitch.
   MIL_UINT16* pDepthMap = (MIL_UINT16*)MbufInquire(MilDepthMapImage, M_HOST_ADDRESS, M_NULL);
   MIL_INT DepthMapPitch = MbufInquire(MilDepthMapImage, M_PITCH, M_NULL);
   MIL_INT DepthMapSizeX = MbufInquire(MilDepthMapImage, M_SIZE_X, M_NULL);
   MIL_INT DepthMapSizeY = MbufInquire(MilDepthMapImage, M_SIZE_Y, M_NULL);
   MbufClear(MilDepthMapImage, MIL_UINT16_MAX); // MIL_UINT16_MAX means invalid data.

   MIL_UINT32* pColorMap;
   MIL_INT ColorMapPitch;
   MIL_UINT32* pWorldPackedColor;
   if(GenColorDepthMap)
      {
      pColorMap = (MIL_UINT32*)MbufInquire(MilColorDepthMapImage, M_HOST_ADDRESS, M_NULL);
      ColorMapPitch = MbufInquire(MilColorDepthMapImage, M_PITCH, M_NULL); 
      MbufClear(MilColorDepthMapImage, 0);   

      // Get the data pointer of the world packed color buffer.
      pWorldPackedColor = (MIL_UINT32*)MbufInquire(MilWorldPackedColorImage, M_HOST_ADDRESS, M_NULL);
      }

   for(MIL_INT WorldIdx = 0; WorldIdx < m_NbWorld; WorldIdx++)
      {
      MIL_INT DrawX = M_Round((m_pXWorld[WorldIdx] - rGenParam.WorldPosX) / rGenParam.PixelSizeX);
      if(DrawX >=0 && DrawX < DepthMapSizeX)
         {
         MIL_INT DrawY = M_Round((m_pYWorld[WorldIdx] - rGenParam.WorldPosY) / rGenParam.PixelSizeY);
         if(DrawY >=0 && DrawY < DepthMapSizeY)
            {
            MIL_INT DrawZ = M_Round((m_pZWorld[WorldIdx] - rGenParam.WorldPosZ) / rGenParam.GrayLevelSizeZ);
            if(DrawZ < 0)
               DrawZ = 0;
            else if(DrawZ > MIL_UINT16_MAX-1)
               DrawZ = MIL_UINT16_MAX-1;

            if(pDepthMap[DrawX + DrawY*DepthMapPitch] == MIL_UINT16_MAX || DrawZ < pDepthMap[DrawX + DrawY*DepthMapPitch])
               {
               pDepthMap[DrawX + DrawY*DepthMapPitch] = (MIL_UINT16)DrawZ;
               if(GenColorDepthMap)
                  pColorMap[DrawX + DrawY*ColorMapPitch] = pWorldPackedColor[WorldIdx];
               }
            }
         }
      }

   // Calibrate the depth map.
   McalUniform(MilDepthMapImage, rGenParam.WorldPosX, rGenParam.WorldPosY,
               rGenParam.PixelSizeX, rGenParam.PixelSizeY, 0.0, M_DEFAULT);
   McalControl(MilDepthMapImage, M_GRAY_LEVEL_SIZE_Z, rGenParam.GrayLevelSizeZ);
   McalControl(MilDepthMapImage, M_WORLD_POS_Z, rGenParam.WorldPosZ);
   }

//****************************************************************************
// Function to fill the holes.
//****************************************************************************
void CDepthDataMgr::FillDepthMapHoles(MIL_ID MilDepthMapImage, MIL_ID MilColorDepthMapImage, bool GenColorDepthMap) const
   {
   // Try to fill the holes with some morphology.
   MimOpen(MilDepthMapImage, m_MilDepthFilledHoles, m_FillHolesCloseIter, M_GRAYSCALE);   
   if(GenColorDepthMap)
      {
      MimClose(MilColorDepthMapImage, m_MilColorFilledHoles, m_FillHolesCloseIter, M_GRAYSCALE);
      MbufCopyCond(m_MilColorFilledHoles, MilColorDepthMapImage, MilDepthMapImage, M_EQUAL, MIL_UINT16_MAX);
      }
   MbufCopyCond(m_MilDepthFilledHoles, MilDepthMapImage, MilDepthMapImage, M_EQUAL, MIL_UINT16_MAX);  
   }

//****************************************************************************
// Utility function to calculate the number of fixed point.
//****************************************************************************
MIL_INT Calculate16BitNbFixedPoint(MIL_DOUBLE MaxAbsValue)
   {
   MIL_INT MaxIntValue = (MIL_INT)ceil(MaxAbsValue);
   MIL_INT NbFixedPoint = 14;
   if (MaxIntValue > 0x7F) { MaxIntValue >>= 8; NbFixedPoint -= 8;}
   if (MaxIntValue > 0x7) { MaxIntValue >>= 4; NbFixedPoint -= 4;}
   if (MaxIntValue > 0x3) { MaxIntValue >>= 2; NbFixedPoint -= 2;}
   if (MaxIntValue > 0x1) { MaxIntValue >>= 1; NbFixedPoint -= 1;}
   return NbFixedPoint;
   }