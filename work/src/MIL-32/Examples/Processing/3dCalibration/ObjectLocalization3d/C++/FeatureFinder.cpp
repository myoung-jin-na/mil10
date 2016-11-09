//***************************************************************************************/
// 
// File name: FeatureFinder.cpp  
//
// Synopsis:  Implements the CFeatureFinder class.
//

#include "common.h"
#include "FeatureFinder.h"
#include "AdaptiveThresholder.h"
#include "BlackFiducialFinder.h"
#include "WhiteFiducialFinder.h"
#include "PharmacodeReader.h"

//*****************************************************************************
// Structure representing one fiducial in the CAD: its pharmacode string and
// its 3d position in the CAD.
//*****************************************************************************
struct SCADFeature
   {
   const MIL_TEXT_CHAR* m_CodeString;
   MIL_DOUBLE           m_PosX;
   MIL_DOUBLE           m_PosY;
   MIL_DOUBLE           m_PosZ;
   };

//*****************************************************************************
// Constants.
//*****************************************************************************

static const SCADFeature CAD_FEATURE_ARRAY[] =
   {//   CodeString     PosX   PosY   PosZ
      {MIL_TEXT("16"),   68.6,  178.9,  -93.6},
      {MIL_TEXT("17"),  101.8,  178.8,  -93.8},
      {MIL_TEXT("18"),  112.1,   11.0,  -60.9},
      {MIL_TEXT("20"),   55.5,   12.6,  -61.0},
      {MIL_TEXT("22"),  107.7,   92.6,  -92.8},
      {MIL_TEXT("26"),   61.4,   92.5,  -92.7},
      {MIL_TEXT("64"),  127.5,  177.5,  -73.2},
      {MIL_TEXT("65"),   43.1,  178.6,  -73.5},
      {MIL_TEXT("66"),  123.8,  120.8,  -84.8},
      {MIL_TEXT("67"),   45.4,  120.2,  -85.4},
      {MIL_TEXT("68"),  133.4,  157.5,  -62.3},
      {MIL_TEXT("69"),   37.3,  159.1,  -63.2},
      {MIL_TEXT("72"),  134.5,   81.2,  -56.9},
      {MIL_TEXT("73"),   34.5,   79.7,  -58.3}
   };

static const MIL_INT NB_CAD_FEATURES = sizeof(CAD_FEATURE_ARRAY)/sizeof(CAD_FEATURE_ARRAY[0]);

//*****************************************************************************
// Constructor. Allocates all necessary objects.
//*****************************************************************************
CFeatureFinder::CFeatureFinder(MIL_ID MilSystem, MIL_INT SizeX, MIL_INT SizeY)
: m_pAdaptiveThresholder(new CAdaptiveThresholder(MilSystem, SizeX, SizeY)),
  m_pBlackFiducialFinder(new CBlackFiducialFinder(MilSystem)),
  m_pWhiteFiducialFinder(new CWhiteFiducialFinder(MilSystem)),
  m_pPharmacodeReader   (new CPharmacodeReader   (MilSystem))
   {
   MbufAlloc2d(MilSystem, SizeX, SizeY, 1+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilBinarizedImage);
   MbufChild2d(m_MilBinarizedImage, 0, 0, 1, 1, &m_MilBinarizedChild);
   }

//*****************************************************************************
// Destructor. Free all allocated objects.
//*****************************************************************************
CFeatureFinder::~CFeatureFinder()
   {
   delete m_pAdaptiveThresholder;
   delete m_pBlackFiducialFinder;
   delete m_pWhiteFiducialFinder;
   delete m_pPharmacodeReader   ;

   MbufFree(m_MilBinarizedChild);
   MbufFree(m_MilBinarizedImage);
   }

//*****************************************************************************
// Find and identify all fiducials in the image, and draw them in the overlay.
//*****************************************************************************
void CFeatureFinder::Find(MIL_ID MilDisplay, MIL_ID MilDisplayImage, SObjectFeatures* pObjectFeatures)
   {
   // Get overlay Id.
   MIL_ID MilOverlayImage;
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   // Initialize output structure.
   pObjectFeatures->m_NumPoint = 0;

   // Use adaptive thresholding to binarize the image.
   m_pAdaptiveThresholder->Binarize(MilDisplayImage, m_MilBinarizedImage);

   // Find the black fiducials, i.e. black mostly circular or elliptical blobs
   // with exactly one white hole inside.
   MIL_INT NbBlackFiducials = m_pBlackFiducialFinder->Find(m_MilBinarizedImage);

   // For each black fiducial...
   for (MIL_INT BlackFiducialIdx = 0; BlackFiducialIdx < NbBlackFiducials; ++BlackFiducialIdx)
      {
      // Define a rectangular region around the black fiducial.
      MIL_INT ChildOffsetX, ChildOffsetY, ChildSizeX, ChildSizeY;
      m_pBlackFiducialFinder->GetChildRect(BlackFiducialIdx, &ChildOffsetX, &ChildOffsetY, &ChildSizeX, &ChildSizeY);
      MbufChildMove(m_MilBinarizedChild, ChildOffsetX, ChildOffsetY, ChildSizeX, ChildSizeY, M_DEFAULT);

      // The white hole is expected to be approximately in the center of the region.
      MIL_DOUBLE ApproxWhiteBlobX = 0.5*static_cast<MIL_DOUBLE>(ChildSizeX+1);
      MIL_DOUBLE ApproxWhiteBlobY = 0.5*static_cast<MIL_DOUBLE>(ChildSizeY+1);

      // Find the center of the white hole.
      bool WhiteFiducialFound = m_pWhiteFiducialFinder->Find(m_MilBinarizedChild, ApproxWhiteBlobX, ApproxWhiteBlobY);
      MIL_DOUBLE WhiteFiducialX = m_pWhiteFiducialFinder->GetCoGX() + static_cast<MIL_DOUBLE>(ChildOffsetX);
      MIL_DOUBLE WhiteFiducialY = m_pWhiteFiducialFinder->GetCoGY() + static_cast<MIL_DOUBLE>(ChildOffsetY);

      if (WhiteFiducialFound)
         {
         // Fixture the image such that the origin is the center of gravity of the white
         // hole and the positive X axis is going through the pharmacode (in the direction
         // of the black fiducial center of gravity).
         McalFixture(MilDisplayImage, M_NULL, M_MOVE_RELATIVE, M_POINT_AND_DIRECTION_POINT, M_NULL,
                     WhiteFiducialX, WhiteFiducialY,
                     m_pBlackFiducialFinder->GetCoGX(BlackFiducialIdx), m_pBlackFiducialFinder->GetCoGY(BlackFiducialIdx));
         // Now both MilDisplayImage and its overlay are fixtured.

         // Read the pharmacode.
         const MIL_TEXT_CHAR* PharmacodeString = m_pPharmacodeReader->Read(MilDisplayImage);

         if ( PharmacodeString != NULL )
            {
            // A pharmacode was read. Check that it exists in the CAD feature table.
            for (MIL_INT CADFeatureIdx = 0; CADFeatureIdx < NB_CAD_FEATURES; ++CADFeatureIdx)
               {
               if (MosStrcmp(PharmacodeString, CAD_FEATURE_ARRAY[CADFeatureIdx].m_CodeString) == 0)
                  {
                  // The string was found. Copy the feature coordinates in the output structure.
                  MIL_INT FoundFeatureIdx = pObjectFeatures->m_NumPoint;

                  pObjectFeatures->m_XPixArray  [FoundFeatureIdx] = WhiteFiducialX;
                  pObjectFeatures->m_YPixArray  [FoundFeatureIdx] = WhiteFiducialY;
                  pObjectFeatures->m_XWorldArray[FoundFeatureIdx] = CAD_FEATURE_ARRAY[CADFeatureIdx].m_PosX;
                  pObjectFeatures->m_YWorldArray[FoundFeatureIdx] = CAD_FEATURE_ARRAY[CADFeatureIdx].m_PosY;
                  pObjectFeatures->m_ZWorldArray[FoundFeatureIdx] = CAD_FEATURE_ARRAY[CADFeatureIdx].m_PosZ;
                  pObjectFeatures->m_NumPoint++;

                  // Draw the red cross.
                  m_pWhiteFiducialFinder->Draw(MilOverlayImage);

                  // Draw the pharmacode label.
                  m_pPharmacodeReader->Draw(MilOverlayImage);
      
                  // No need to continue looping on the features.
                  break;
                  }
               }
            }
         }

      // Avoid buffer overrun.
      if (pObjectFeatures->m_NumPoint == MAX_NB_FEATURES)
         break; 
      }

   // Remove image fixture by removing its associated calibration.
   McalAssociate(M_NULL, MilDisplayImage, M_DEFAULT);
   }
