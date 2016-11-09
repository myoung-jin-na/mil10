//***************************************************************************************
// 
// File name: RegionManager.h
//
// Synopsis: This file contains the declaration of the CRegionMngr class
//           which is the class that manages the use of a region in an inspection task.
//

#ifndef REGION_MANAGER_H
#define REGION_MANAGER_H

class CRegionMngr
   {
   public:

      // Constructor.
      CRegionMngr();

      // Destructor.
      virtual ~CRegionMngr();

      // Initialization function.
      void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0);

      // Free function.
      void Free();

      // Function that associate the region to an image buffer.
      void SetRegion(MIL_ID MilImage, MIL_ID MilRegionGraList);

      // Function that rasterizes the region.
      MIL_ID RasterizeRegion(MIL_ID MilCalibration, MIL_ID MilRegionGraList);

      // Function to get the rasterized region.
      MIL_ID GetRasterRegion() const {return m_MilRasterRegion;}
      
   protected:
            
      // Function that draws some text in the middle of the region.
      void DrawTextInRegion(MIL_ID MilGraContext, MIL_ID MilRegionGraList, MIL_ID MilDest, MIL_INT RegionIndex, MIL_CONST_TEXT_PTR String);    

   private:
           
      // The rasterized image
      MIL_ID m_MilRasterRegion;
   };

#endif // REGION_MANAGER_H