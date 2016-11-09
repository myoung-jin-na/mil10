// MdispMFC.h : main header file for the MDISPMFC application
//

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// MIL include
#include "mil.h"


MIL_INT MFTYPE DisplayErrorExt(MIL_INT HookType, MIL_ID EventId, void* UserDataPtr);

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCApp:
// See MdispMFC.cpp for the implementation of this class
//

class CMdispMFCApp : public CWinApp
{
public:
   CMdispMFCApp();

   MIL_INT  m_digitizerSizeX;    // Digitizer input width
   MIL_INT  m_digitizerSizeY;    // Digitizer input height
   MIL_INT  m_digitizerNbBands;  // Number of input color bands of the digitizer
   BOOL     m_isGrabStarted;     // State of the grab
   CView*   m_pGrabView;         // Pointer to the view that has the grab
   MIL_INT  m_numberOfDigitizer; // Number of digitizers available on the system

   MIL_ID   m_MilApplication;    // Application identifier.  
   MIL_ID   m_MilSystem;         // System identifier.       
   MIL_ID   m_MilDigitizer;      // Digitizer identifier.

   long MFTYPE DisplayError(MIL_INT HookType, MIL_ID EventId, void* UserDataPtr);
   void HookedOnErrors(bool IsHooked){m_isCurrentlyHookedOnErrors = IsHooked;}
private:
   bool m_isCurrentlyHookedOnErrors;
// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CMdispMFCApp)
   public:
   virtual BOOL InitInstance();
   virtual int ExitInstance();
   //}}AFX_VIRTUAL

// Implementation

   //{{AFX_MSG(CMdispMFCApp)
   afx_msg void OnAppAbout();
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
