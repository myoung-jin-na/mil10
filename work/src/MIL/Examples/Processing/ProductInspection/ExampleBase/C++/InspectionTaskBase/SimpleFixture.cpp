//***************************************************************************************
// 
// File name: SimpleFixture.cpp
//
// Synopsis: This file contains the implementation of the CSimpleFixture class.
//

#include <mil.h>
#include "SimpleFixture.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CSimpleFixture::CSimpleFixture(MIL_DOUBLE PosX, MIL_DOUBLE PosY, MIL_DOUBLE Angle, CInspectionTask* FixtureProvider /* = M_NULL */)
   : CInspectionTask(M_NONE, FixtureProvider),
     m_PosX(PosX),
     m_PosY(PosY),
     m_Angle(Angle)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CSimpleFixture::~CSimpleFixture()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CSimpleFixture::Inspect(MIL_ID MilImage)
   {
   // Set the output fixture.
   SetOutputFixture(M_POINT_AND_ANGLE, M_NULL, M_DEFAULT, m_PosX, m_PosY, m_Angle, M_DEFAULT);

   return true;
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CSimpleFixture::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CSimpleFixture::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   }
