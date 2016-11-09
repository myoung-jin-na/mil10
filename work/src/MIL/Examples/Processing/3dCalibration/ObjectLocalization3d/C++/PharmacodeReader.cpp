//***************************************************************************************/
// 
// File name: PharmacodeReader.cpp  
//
// Synopsis:  Implements the CPharmacodeReader class.
//

#include "common.h"
#include "PharmacodeReader.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

// Minimum contrast between the foreground and background in the target image.
static const MIL_DOUBLE MIN_CONTRAST = 30.0;

// Color used to draw the code string.
static const MIL_DOUBLE CODE_STRING_COLOR = M_COLOR_GREEN;

// Size of the rectangular region in which to search for the code.
static const MIL_DOUBLE CODE_CHILD_WIDTH  = 156.0;
static const MIL_DOUBLE CODE_CHILD_HEIGHT =  60.0;

//*****************************************************************************
// Constructor. Allocates all necessary MIL objects and set the code context up.
//*****************************************************************************
CPharmacodeReader::CPharmacodeReader(MIL_ID MilSystem)
   {
   // We will use a rectangular region in fixtured (pixel) coordinates to look for the code.
   // This region definition will be kept in a graphic list object.
   MgraAllocList(MilSystem, M_DEFAULT, &m_MilGraList);

   // Use fixtured units, i.e. M_WORLD (even if the world unit is the pixel).
   MgraControl(M_DEFAULT, M_INPUT_UNITS, M_WORLD);
   MgraRectFill(M_DEFAULT, m_MilGraList, 0.0, -CODE_CHILD_HEIGHT/2.0, CODE_CHILD_WIDTH, CODE_CHILD_HEIGHT/2.0);

   // Allocates the code context and its result buffer.
   McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &m_MilCodeContext);
   McodeAllocResult(MilSystem, M_DEFAULT, &m_MilCodeResult);

   // We look for pharmacodes.
   McodeModel(m_MilCodeContext, M_ADD, M_PHARMACODE, M_NULL, M_DEFAULT, M_NULL);
   
   // Add some options for a good compromise between speed and robustness.
   McodeControl(m_MilCodeContext, M_SCANLINE_HEIGHT, 2.0);
   McodeControl(m_MilCodeContext, M_SCANLINE_STEP, 2.0);
   McodeControl(m_MilCodeContext, M_THRESHOLD_MODE, M_ADAPTIVE);
   McodeControl(m_MilCodeContext, M_MINIMUM_CONTRAST, MIN_CONTRAST);

   // We will search the code in a rectangular region which could have any angle.
   McodeControl(m_MilCodeContext, M_SEARCH_ANGLE, M_ACCORDING_TO_REGION);

   // Initialize the code string to the empty string.
   m_CodeString[0] = MIL_TEXT('\0');
   }

//*****************************************************************************
// Destructor. Free all MIL objects.
//*****************************************************************************
CPharmacodeReader::~CPharmacodeReader()
   {
   McodeFree(m_MilCodeResult);
   McodeFree(m_MilCodeContext);
   MgraFree(m_MilGraList);
   }

//*****************************************************************************
// Find a pharmacode and return the code string. Works in fixtured images only.
// If no code is found, returns NULL.
//*****************************************************************************
const MIL_TEXT_CHAR* CPharmacodeReader::Read(MIL_ID MilImage)
   {
   const MIL_TEXT_CHAR* CodeString = NULL;

   // Read code in a rectangular region near the black fiducial.
   MbufSetRegion(MilImage, m_MilGraList, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McodeRead(m_MilCodeContext, MilImage, m_MilCodeResult);
   MbufSetRegion(MilImage, M_NULL, M_DEFAULT, M_DELETE, M_DEFAULT);

   // Check that a code was read.
   MIL_INT NbCodes;
   McodeGetResult(m_MilCodeResult, M_NUMBER+M_TYPE_MIL_INT, &NbCodes);
   if (NbCodes == 1)
      {
      // Get the code string.
      MIL_INT StringSize;
      McodeGetResult(m_MilCodeResult, M_STRING_SIZE+M_TYPE_MIL_INT, &StringSize);
      if (StringSize <= MAX_STRING_LEN)
         {
         McodeGetResult(m_MilCodeResult, M_STRING, m_CodeString);
         CodeString = m_CodeString;
         }
      }

   return CodeString;
   }

//*****************************************************************************
// Draw the code found in the last call to Read().
//*****************************************************************************
void CPharmacodeReader::Draw(MIL_ID MilOverlayImage) const
   {
   // Get the position where the code was found.
   MIL_DOUBLE PosX, PosY;
   McodeGetResult(m_MilCodeResult, M_POSITION_X, &PosX);
   McodeGetResult(m_MilCodeResult, M_POSITION_Y, &PosY);

   // This position is in fixtured coordinates, so let's use M_WORLD input units.
   MgraControl(M_DEFAULT, M_INPUT_UNITS, M_WORLD);

   // Draw the code string.
   MgraColor(M_DEFAULT, CODE_STRING_COLOR);
   MgraText(M_DEFAULT, MilOverlayImage, PosX, PosY, m_CodeString);
   }
