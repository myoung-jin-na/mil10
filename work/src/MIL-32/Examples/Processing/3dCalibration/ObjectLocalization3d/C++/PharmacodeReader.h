//***************************************************************************************
//
// File name: PharmacodeReader.h
//
// Synopsis:  Defines a class used to read pharmacodes in a fixtured image.
//

#ifndef PHARMACODE_READER_H
#define PHARMACODE_READER_H

//*****************************************************************************
// Class used to wrap the "pharmacode read" operation and its necessary resources.
//*****************************************************************************
class CPharmacodeReader
   {
   public:
      explicit CPharmacodeReader(MIL_ID MilSystem);
      ~CPharmacodeReader();

      const MIL_TEXT_CHAR* Read(MIL_ID MilImage);

      void Draw(MIL_ID MilOverlayImage) const;

   private:
      // No code will have a greater length than this in this example.
      static const MIL_INT MAX_STRING_LEN = 6;

      // Disallow copy.
      CPharmacodeReader(const CPharmacodeReader&);
      CPharmacodeReader& operator=(const CPharmacodeReader&);

      MIL_ID m_MilGraList;       // Graphic list used to draw the codes found.
      MIL_ID m_MilCodeContext;   // Code reader context used to read the pharmacodes.
      MIL_ID m_MilCodeResult;    // Code result buffer filled with the codes found.

      // Temporary storage for getting a code string from a code result buffer.
      MIL_TEXT_CHAR m_CodeString[MAX_STRING_LEN+1];
   };

#endif
