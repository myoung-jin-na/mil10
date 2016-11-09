//***************************************************************************************
//
// File name: BlackFiducialFinder.h
//
// Synopsis:  Defines a class used to find special black blobs used as fiducials.
//

#ifndef BLACK_FIDUCIAL_FINDER_H
#define BLACK_FIDUCIAL_FINDER_H

//*****************************************************************************
// Class used to find special black blobs used as fiducials.
//*****************************************************************************
class CBlackFiducialFinder
   {
   public:
      explicit CBlackFiducialFinder(MIL_ID MilSystem);
      ~CBlackFiducialFinder();

      MIL_INT Find(MIL_ID MilBinarizedImage);

      void GetChildRect(MIL_INT  FiducialIdx,
                        MIL_INT* pChildOffsetX, MIL_INT* pChildOffsetY,
                        MIL_INT* pChildSizeX,   MIL_INT* pChildSizeY);

      inline MIL_DOUBLE GetCoGX(MIL_INT FiducialIdx) const;
      inline MIL_DOUBLE GetCoGY(MIL_INT FiducialIdx) const;

   private:
      // Disallow copy.
      CBlackFiducialFinder(const CBlackFiducialFinder&);
      CBlackFiducialFinder& operator=(const CBlackFiducialFinder&);

      void ReserveArraySpace(MIL_INT MinArraySize);
      void AllocateArrays(MIL_INT ArraySize);
      void DestroyArrays();

      MIL_ID m_MilFeatureList;      // MIL object used to specified features to calculate with milblob.
      MIL_ID m_MilBlobResult;       // MIL object used to contain found blobs.

      MIL_DOUBLE* m_BoxXMinArray;   // X of top-left corner of the bounding box of the blobs.
      MIL_DOUBLE* m_BoxYMinArray;   // Y of top-left corner of the bounding box of the blobs.
      MIL_DOUBLE* m_BoxXMaxArray;   // X of bottom-right corner of the bounding box of the blobs.
      MIL_DOUBLE* m_BoxYMaxArray;   // Y of bottom-right corner of the bounding box of the blobs.
      MIL_DOUBLE* m_CoGXArray;      // X of center of gravity of the blobs.
      MIL_DOUBLE* m_CoGYArray;      // Y of center of gravity of the blobs.
      MIL_INT     m_ArraySize;      // Maximum size of the above arrays.
   };

//*****************************************************************************
// Return the X coordinate of the center of gravity of the black fiducial.
// Find() must have been called before using this function.
//*****************************************************************************
inline MIL_DOUBLE CBlackFiducialFinder::GetCoGX(MIL_INT FiducialIdx) const
   {
   return m_CoGXArray[FiducialIdx];
   }

//*****************************************************************************
// Return the Y coordinate of the center of gravity of the black fiducial.
// Find() must have been called before using this function.
//*****************************************************************************
inline MIL_DOUBLE CBlackFiducialFinder::GetCoGY(MIL_INT FiducialIdx) const
   {
   return m_CoGYArray[FiducialIdx];
   }

#endif
