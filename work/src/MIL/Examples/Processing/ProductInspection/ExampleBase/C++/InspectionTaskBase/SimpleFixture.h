//***************************************************************************************
// 
// File name: SimpleFixture.h
//
// Synopsis: This file contains the declaration of the CSimpleFixture.
//

#ifndef SIMPLE_FIXTURE_H
#define SIMPLE_FIXTURE_H

#include "InspectionTask.h"

class CSimpleFixture : public CInspectionTask
   {
   public:

      // Constructor.
      CSimpleFixture(MIL_DOUBLE PosX, MIL_DOUBLE PosY, MIL_DOUBLE Angle, CInspectionTask* FixtureProvider = M_NULL);

      // Destructor.
      virtual ~CSimpleFixture();

      // Inspection function.
      virtual bool Inspect(MIL_ID MilImage);
                 
      // Drawing functions
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);

   protected:

   private:

      // The information of the fixture
      MIL_DOUBLE m_PosX;
      MIL_DOUBLE m_PosY;
      MIL_DOUBLE m_Angle;
   };

#endif // SIMPLE_FIXTURE_H