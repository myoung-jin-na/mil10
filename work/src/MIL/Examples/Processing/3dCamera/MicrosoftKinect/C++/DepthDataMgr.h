//***************************************************************************************/
// 
// File name: DepthDataMgr.h  
//
// Synopsis:  Declaration of the CDepthDataMgr a class that manages, transform
//            and register depth data of the Microsoft Kinect 2 camera system.
//

#ifndef __DEPTH_DATA_MANAGER_H
#define __DEPTH_DATA_MANAGER_H

struct SDepthMapGenParam  
   {
   MIL_DOUBLE WorldPosX;
   MIL_DOUBLE WorldPosY;
   MIL_DOUBLE WorldPosZ;
   MIL_DOUBLE PixelSizeX;
   MIL_DOUBLE PixelSizeY;
   MIL_DOUBLE GrayLevelSizeZ;
   };

class CDepthDataMgr
   {
   public:

      // Constructor.
      CDepthDataMgr(MIL_ID MilSystem, MIL_INT DepthImageSizeX, MIL_INT DepthImageSizeY);

      // Destructor.
      virtual ~CDepthDataMgr();

      // Function to calculate the multipliers.
      void CalculateMultipliers(MIL_ID MilCalibration);

      // Function to calculate the world points.
      template <bool GenAssociatedDepthPixel>
      void CalculateWorldPoints(MIL_ID MilDepthImage);

      // Function to move the world points.
      void MoveWorldPoints(MIL_ID MilCalibration,
                           MIL_INT SrcCoordinateSystem,
                           MIL_INT DstCoordinateSystem);

      // Function to generate the color luts.
      void GenColorLuts(MIL_ID MilColorCameraCalibration, bool GenWorldLut);

      // Function to create the depth map.
      void CreateDepthMap(MIL_ID MilWorldPackedColorImage,
                          MIL_ID MilDepthMapImage,
                          MIL_ID MilColorDepthMapImage,
                          const SDepthMapGenParam& rGenParam) const;
      
      // Function to warp the color image using the color luts.
      void WarpColorImage(MIL_ID MilColorImage, MIL_ID MilWarpedColorImage, bool UseWorldLut) const;

      // Function to get the nb of world points.
      MIL_INT GetNbWorld() const {return m_NbWorld;}

   protected:
   private:
      
      // Function to fill the color to world or depth luts.
      void FillColorToWorldLuts();
      void FillColorToDepthLuts();
      
      // Function to extract the depth map.
      template <bool GenColorDepthMap>
      void ExtractDepthMapNoFill(MIL_ID MilWorldPackedColorImage,
                                 MIL_ID MilDepthMapImage,
                                 MIL_ID MilColorDepthMapImage,
                                 const SDepthMapGenParam& rGenParam) const;

      // Function to fill the holes of the depth map.
      void FillDepthMapHoles(MIL_ID MilDepthMapImage,
                             MIL_ID MilColorDepthMapImage,
                             bool GenColorDepthMap) const;

      // The owner system.
      MIL_ID m_MilSystem;

      // The size of the depth map.
      MIL_INT m_DepthImageSizeX;
      MIL_INT m_DepthImageSizeY;

      // The intermediate depth map images to fill the holes.
      MIL_ID m_MilDepthFilledHoles;
      MIL_ID m_MilColorFilledHoles;

      // The color luts.
      MIL_ID m_MilColorToDepthLutX;
      MIL_ID m_MilColorToDepthLutY;
      MIL_ID m_MilColorToWorldLutX;
      MIL_ID m_MilColorToWorldLutY;
      MIL_ID m_MilColorToWorldLutXContainer;
      MIL_ID m_MilColorToWorldLutYContainer;

      // The lut nb of fixed point.
      MIL_INT m_LutNbFixedPoint;

      // The nb of iteration to fill the holes in the depth map.
      MIL_INT m_FillHolesCloseIter;

      // The multipliers generated to get the world X and Y position based
      // and X, Y pixel and Z world value.
      MIL_DOUBLE* m_pXMultipliers;
      MIL_DOUBLE* m_pYMultipliers;

      // The arrays of world points with their corresponding depth X and Y values.
      bool        m_DepthValid;
      MIL_INT     m_NbWorld;
      MIL_DOUBLE* m_pXWorld;
      MIL_DOUBLE* m_pYWorld;
      MIL_DOUBLE* m_pZWorld;
      MIL_UINT16* m_pDepthXPixel;
      MIL_UINT16* m_pDepthYPixel;
      MIL_DOUBLE* m_pColorXPixel;
      MIL_DOUBLE* m_pColorYPixel;
   };

// Utility function to calculate the maximum number of fixed point given a maximum value.
MIL_INT Calculate16BitNbFixedPoint(MIL_DOUBLE MaxAbsValue);

#endif // __DEPTH_DATA_MANAGER_H