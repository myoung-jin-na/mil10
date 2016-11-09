//***************************************************************************************
//
// File name: BlackFiducialFinder.cpp
//
// Synopsis:  Implements the CBlackFiducialFinder class.
//

#include "common.h"
#include "BlackFiducialFinder.h"

// All arrays will be resized dynamically. This is the initial size of the arrays.
static const MIL_INT STARTING_ARRAY_SIZE = 16;

// Expected criteria of the black fiducial blob.
static const MIL_DOUBLE MIN_AREA       =   80.0;   // in pixels
static const MIL_DOUBLE MAX_AREA       =  800.0;   // in pixels
static const MIL_DOUBLE EXPECTED_HOLES =    1.0;
static const MIL_DOUBLE MIN_FERET_MAX  =   12.0;   // in pixels
static const MIL_DOUBLE MAX_FERET_MAX  =   40.0;   // in pixels
static const MIL_DOUBLE MAX_ROUGHNESS  =    1.65;

//*****************************************************************************
// Constructor. Allocate and setup MIL objects.
//*****************************************************************************
CBlackFiducialFinder::CBlackFiducialFinder(MIL_ID MilSystem)
   {
   // Allocate and setup MIL objects for milblob.
   MblobAllocFeatureList(MilSystem, &m_MilFeatureList);
   MblobAllocResult(MilSystem, &m_MilBlobResult);
   
   MblobControl(m_MilBlobResult, M_FOREGROUND_VALUE, M_ZERO       );
   MblobControl(m_MilBlobResult, M_LATTICE         , M_8_CONNECTED);

   // Allocate dynamic arrays.
   AllocateArrays(STARTING_ARRAY_SIZE);
   }

//*****************************************************************************
// Destructor. Free all MIL objects and memory.
//*****************************************************************************
CBlackFiducialFinder::~CBlackFiducialFinder()
   {
   DestroyArrays();
   MblobFree(m_MilBlobResult);
   MblobFree(m_MilFeatureList);
   }

//*****************************************************************************
// Find the black fiducials, i.e. black mostly circular or elliptical blobs
// with exactly one white hole inside. Returns the number of fiducials.
//*****************************************************************************
MIL_INT CBlackFiducialFinder::Find(MIL_ID MilBinarizedImage)
   {
   // Clear feature list from previous call.
   MblobSelectFeature(m_MilFeatureList, M_NO_FEATURES);

   // Set the first features in the list.
   MblobSelectFeature(m_MilFeatureList, M_AREA);
   MblobSelectFeature(m_MilFeatureList, M_NUMBER_OF_HOLES);

   // Find all black blobs.
   MblobCalculate(MilBinarizedImage, M_NULL, m_MilFeatureList, m_MilBlobResult);

   // Remove blobs that do not have exactly one hole or whose area is not in the expected range.
   MblobSelect(m_MilBlobResult, M_DELETE, M_AREA, M_OUT_RANGE, MIN_AREA, MAX_AREA);
   MblobSelect(m_MilBlobResult, M_DELETE, M_NUMBER_OF_HOLES, M_NOT_EQUAL, EXPECTED_HOLES, M_NULL);

   // Add other features to compute on the remaining blobs.
   MblobSelectFeature(m_MilFeatureList, M_FERET_MAX_DIAMETER);
   MblobSelectFeature(m_MilFeatureList, M_ROUGHNESS);

   // Calculate the new features only on the remaining blobs.
   MblobCalculate(MilBinarizedImage, M_NULL, m_MilFeatureList, m_MilBlobResult);

   // Remove the blobs whose max feret is not in the expected range (those that are too
   // elongated) or those who are not really smooth and convex. This should yield mostly
   // circular or elliptical black blobs with exactly one white hole inside.
   MblobSelect(m_MilBlobResult, M_DELETE, M_FERET_MAX_DIAMETER, M_OUT_RANGE, MIN_FERET_MAX, MAX_FERET_MAX);
   MblobSelect(m_MilBlobResult, M_DELETE, M_ROUGHNESS, M_GREATER, MAX_ROUGHNESS, M_NULL);

   // Get the number of fiducials found.
   MIL_INT NbBlobs = MblobGetNumber(m_MilBlobResult, M_NULL);

   if (NbBlobs > 0)
      {
      // Calculate necessary features on the found fiducials: CoG and bounding box.
      MblobSelectFeature(m_MilFeatureList, M_CENTER_OF_GRAVITY);
      MblobSelectFeature(m_MilFeatureList, M_BOX_X_MIN);
      MblobSelectFeature(m_MilFeatureList, M_BOX_X_MAX);
      MblobSelectFeature(m_MilFeatureList, M_BOX_Y_MIN);
      MblobSelectFeature(m_MilFeatureList, M_BOX_Y_MAX);

      MblobCalculate(MilBinarizedImage, M_NULL, m_MilFeatureList, m_MilBlobResult);
 
      // Get the features.
      ReserveArraySpace(NbBlobs);
      MblobGetResult(m_MilBlobResult, M_BOX_X_MIN,                    m_BoxXMinArray);
      MblobGetResult(m_MilBlobResult, M_BOX_Y_MIN,                    m_BoxYMinArray);
      MblobGetResult(m_MilBlobResult, M_BOX_X_MAX,                    m_BoxXMaxArray);
      MblobGetResult(m_MilBlobResult, M_BOX_Y_MAX,                    m_BoxYMaxArray);
      MblobGetResult(m_MilBlobResult, M_CENTER_OF_GRAVITY_X+M_BINARY, m_CoGXArray   );
      MblobGetResult(m_MilBlobResult, M_CENTER_OF_GRAVITY_Y+M_BINARY, m_CoGYArray   );
      }

   return NbBlobs;
   }

//*****************************************************************************
// Return the necessary information to create a child buffer around the fiducial
// with the given index.
//*****************************************************************************
void CBlackFiducialFinder::GetChildRect(MIL_INT  FiducialIdx,
                                        MIL_INT* pChildOffsetX, MIL_INT* pChildOffsetY,
                                        MIL_INT* pChildSizeX,   MIL_INT* pChildSizeY)
   {
   *pChildOffsetX = static_cast<MIL_INT>( m_BoxXMinArray[FiducialIdx] );
   *pChildOffsetY = static_cast<MIL_INT>( m_BoxYMinArray[FiducialIdx] );
   *pChildSizeX   = static_cast<MIL_INT>( m_BoxXMaxArray[FiducialIdx]+1.0 ) - *pChildOffsetX;
   *pChildSizeY   = static_cast<MIL_INT>( m_BoxYMaxArray[FiducialIdx]+1.0 ) - *pChildOffsetY;
   }

//*****************************************************************************
// If there is not enough space in the arrays, free and reallocate them. Else,
// do nothing.
//*****************************************************************************
void CBlackFiducialFinder::ReserveArraySpace(MIL_INT MinArraySize)
   {
   if (m_ArraySize < MinArraySize)
      {
      DestroyArrays();
      AllocateArrays(MinArraySize);
      }
   }

//*****************************************************************************
// Set new size for the dynamic arrays and allocate them.
//*****************************************************************************
void CBlackFiducialFinder::AllocateArrays(MIL_INT ArraySize)
   {
   m_BoxXMinArray = new MIL_DOUBLE[ArraySize];
   m_BoxYMinArray = new MIL_DOUBLE[ArraySize];
   m_BoxXMaxArray = new MIL_DOUBLE[ArraySize];
   m_BoxYMaxArray = new MIL_DOUBLE[ArraySize];
   m_CoGXArray    = new MIL_DOUBLE[ArraySize];
   m_CoGYArray    = new MIL_DOUBLE[ArraySize];
   m_ArraySize = ArraySize;
   }

//*****************************************************************************
// Free the memory of the dynamic arrays.
//*****************************************************************************
void CBlackFiducialFinder::DestroyArrays()
   {
   delete [] m_CoGYArray;
   delete [] m_CoGXArray;
   delete [] m_BoxYMaxArray;
   delete [] m_BoxXMaxArray;
   delete [] m_BoxYMinArray;
   delete [] m_BoxXMinArray;
   }
