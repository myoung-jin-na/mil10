// MdispMFCDoc.cpp : implementation of the CMdispMFCDoc class
//

#include "stdafx.h"
#include "MdispMFC.h"

#include "MdispMFCDoc.h"
#include "MdispMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IMAGE_FILE   M_IMAGE_PATH MIL_TEXT("BaboonRGB.mim")

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCDoc

IMPLEMENT_DYNCREATE(CMdispMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CMdispMFCDoc, CDocument)
   //{{AFX_MSG_MAP(CMdispMFCDoc)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCDoc construction/destruction

CMdispMFCDoc::CMdispMFCDoc()
{
   // TODO: add one-time construction code here

}

CMdispMFCDoc::~CMdispMFCDoc()
{
}

BOOL CMdispMFCDoc::OnNewDocument()
{
   // TODO: add re-initialization code here
   // (SDI documents will reuse this document)

   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when a new document is created
   /////////////////////////////////////////////////////////////////////////
                                                     
   // Set buffer settings
   if(((CMdispMFCApp*)AfxGetApp())->m_numberOfDigitizer)
      {
      m_bufferAttributes=M_IMAGE+M_DISP+M_GRAB+M_PROC;
      m_imageSizeX   = ((CMdispMFCApp*)AfxGetApp())->m_digitizerSizeX;
      m_imageSizeY   = ((CMdispMFCApp*)AfxGetApp())->m_digitizerSizeY;
      m_NbBands      = ((CMdispMFCApp*)AfxGetApp())->m_digitizerNbBands;
   
      // Allocate a buffer [CALL TO MIL]
      MbufAllocColor(((CMdispMFCApp*)AfxGetApp())->m_MilSystem,
                       m_NbBands,
                       m_imageSizeX,
                       m_imageSizeY,
                       8+M_UNSIGNED,
                       m_bufferAttributes,
                       &m_MilImage);


      // Clear the buffer [CALL TO MIL] 
      MbufClear(m_MilImage,M_COLOR_BLACK);
      }
   else
      {
      //Import image in buffer [CALL TO MIL]
      MbufImport(IMAGE_FILE,M_DEFAULT,M_RESTORE,((CMdispMFCApp*)AfxGetApp())->m_MilSystem,&m_MilImage);

      // Set SizeX and SizeY variable to the size of the buffer [CALL TO MIL]
      if (m_MilImage) 
         {
         m_imageSizeX   = MbufInquire(m_MilImage, M_SIZE_X, M_NULL);
         m_imageSizeY   = MbufInquire(m_MilImage, M_SIZE_Y, M_NULL);
         m_NbBands      = MbufInquire(m_MilImage, M_SIZE_BAND, M_NULL);
         }
      }
      
   
   // If not able to allocate a buffer, do not create a new document
   if(!m_MilImage)
      return FALSE;
     
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when a new document is created
   /////////////////////////////////////////////////////////////////////////   

   return CDocument::OnNewDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCDoc serialization

void CMdispMFCDoc::Serialize(CArchive& ar)
{
   if (ar.IsStoring())
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }
}

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCDoc diagnostics

#ifdef _DEBUG
void CMdispMFCDoc::AssertValid() const
{
   CDocument::AssertValid();
}

void CMdispMFCDoc::Dump(CDumpContext& dc) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCDoc commands

void CMdispMFCDoc::OnCloseDocument() 
{
   // TODO: Add your specialized code here and/or call the base class
   
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when a document is closed
   /////////////////////////////////////////////////////////////////////////
   
   // Halt the grab, deselected the display, free the display and the image buffer
   // only if MbufAlloc was successful
   if (m_MilImage)
   {
      CMdispMFCView   *CurView;
      POSITION         CurPos;
            
      // Make sure all views displays are deselected and grab is halt
      CurPos = GetFirstViewPosition();
      while(CurView = (CMdispMFCView *)GetNextView(CurPos))
         {
         CurView->KillTimer(TIMER_FRAMERATE);
         CurView->RemoveFromDisplay();
         }
         
      // Free image buffer [CALL TO MIL]
      MbufFree(m_MilImage);
   }

   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when a document is closed
   /////////////////////////////////////////////////////////////////////////
   
   CDocument::OnCloseDocument();
}

BOOL CMdispMFCDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
   if (!CDocument::OnOpenDocument(lpszPathName))
      return FALSE;
   
   // TODO: Add your specialized creation code here

   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when a document is opened
   /////////////////////////////////////////////////////////////////////////
   
   //Import image in buffer [CALL TO MIL]
   MbufImport((MIL_TEXT_CHAR *)lpszPathName,M_DEFAULT,M_RESTORE,((CMdispMFCApp*)AfxGetApp())->m_MilSystem,&m_MilImage);

   // Set SizeX and SizeY variable to the size of the buffer [CALL TO MIL]
   if (m_MilImage) 
   {
      m_imageSizeX = MbufInquire(m_MilImage,M_SIZE_X,M_NULL);
      m_imageSizeY = MbufInquire(m_MilImage,M_SIZE_Y,M_NULL);
      return TRUE;
   }
   else
      return FALSE;
   
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when a document is opened
   /////////////////////////////////////////////////////////////////////////
}

BOOL CMdispMFCDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
   // TODO: Add your specialized code here and/or call the base class
   
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when a document is saved
   /////////////////////////////////////////////////////////////////////////
   
   BOOL    SaveStatus;
   CString TempPath;
   long FileFormat ;
   
   // Get extension for file format determination
   TempPath = lpszPathName;
   TempPath.MakeUpper();
   //Set the file format to M_MIL when the file path extension is ".MIM"
   if (TempPath.Find(MIL_TEXT(".MIM")) != -1)
      FileFormat = M_MIL;
   //Set the file format to M_TIFF when the file path extension is ".TIF"
   if (TempPath.Find(MIL_TEXT(".TIF")) != -1)
      FileFormat = M_TIFF;
   //Set the file format to M_BMP when the file path extension is ".BMP"
   if (TempPath.Find(MIL_TEXT(".BMP")) != -1)
      FileFormat = M_BMP;
   //Set the file format to M_JPEG_LOSSY when the file path extension is ".JPG"
   if (TempPath.Find(MIL_TEXT(".JPG")) != -1)
      FileFormat = M_JPEG_LOSSY;
   //Set the file format to M_JPEG2000_LOSSLESS when the file path extension is ".JP2"
   if (TempPath.Find(MIL_TEXT(".JP2")) != -1)
      FileFormat = M_JPEG2000_LOSSLESS;
   //Set the file format to M_RAW when the file path extension is ".RAW"
   if (TempPath.Find(MIL_TEXT(".RAW")) != -1)
      FileFormat = M_RAW;
   //Set the file format to M_PNG when the file path extension is ".PNG"
   if (TempPath.Find(MIL_TEXT(".PNG")) != -1)
      FileFormat = M_PNG;

   // To get the first view in the list of views:
   POSITION pos = GetFirstViewPosition();
   CView* pFirstView = GetNextView( pos );
   
   // Halt the grab if the current view has it [CALL TO MIL]
   if((((CMdispMFCApp*)AfxGetApp())->m_pGrabView == pFirstView) &&
      (((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted == TRUE))
      MdigHalt(((CMdispMFCApp*)AfxGetApp())->m_MilDigitizer);
      
   // Save the current buffer [CALL TO MIL]
   MbufExport((MIL_TEXT_CHAR *)lpszPathName, FileFormat,m_MilImage);
   
   // Verify if save operation was successful [CALL TO MIL]
   SaveStatus = (MappGetError(M_DEFAULT, M_CURRENT,M_NULL) == M_NULL_ERROR);

   // Document has been saved
   if (!((((CMdispMFCApp*)AfxGetApp())->m_pGrabView == pFirstView) &&
         (((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted == TRUE)))
      SetModifiedFlag(FALSE);
      
   // Restart the grab if the current view had it [CALL TO MIL]
   if((((CMdispMFCApp*)AfxGetApp())->m_pGrabView == pFirstView) &&
      (((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted == TRUE))
     MdigGrabContinuous(((CMdispMFCApp*)AfxGetApp())->m_MilDigitizer, m_MilImage);
   
   return SaveStatus;

}
