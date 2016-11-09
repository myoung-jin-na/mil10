// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "MdispMFC.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT indicators[] =
{
   ID_INDICATOR_FRAMERATE,
   ID_INDICATOR_SCALE,
   ID_INDICATOR_MOUSE,
};

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
   //{{AFX_MSG_MAP(CChildFrame)
   ON_WM_CREATE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
   // TODO: add member initialization code here
   
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
   CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
   CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
   if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
      return -1;
   
   if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
      {
      TRACE0("Failed to create status bar\n");
      return -1;      // fail to create
      }

   ///////
   // MIL document status bar
   //
   //Section for frame rate
   m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_FRAMERATE, SBPS_NORMAL, 140);
   
   //Section for scale
   m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_SCALE, SBPS_NORMAL, 100);
      
   //Section for Mouse
   m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_MOUSE, SBPS_NORMAL, 180);

   return 0;
}


////////////////////////////////////////////////////////////////////////
// MIL: Used to update status bar with frame rate
////////////////////////////////////////////////////////////////////////
void CChildFrame::UpdateStatusBarWithFrameRate(MIL_DOUBLE CurrentRate)
   {
   CString strCurrentRate;

   if(CurrentRate == M_INVALID)
      {
      strCurrentRate.Format(MIL_TEXT("Display Updates: Not Available"));
      }
   else
      {
      strCurrentRate.Format(MIL_TEXT("Display Updates: %.2f fps"),CurrentRate);
      }

   m_wndStatusBar.SetPaneText(0,strCurrentRate);
   }

////////////////////////////////////////////////////////////////////////
// MIL: Used to update status bar with zoom factor
////////////////////////////////////////////////////////////////////////
void CChildFrame::UpdateStatusBarWithScale(MIL_DOUBLE CurrentScaleX, MIL_DOUBLE CurrentScaleY)
   {
   CString strCurrentScale;
   strCurrentScale.Format(MIL_TEXT("%.4f, %.4f"),CurrentScaleX, CurrentScaleY);
   m_wndStatusBar.SetPaneText(1,strCurrentScale);
   }

void CChildFrame::UpdateStatusBarWithMouse(MIL_INT DispPositionX, MIL_INT DispPositionY, MIL_DOUBLE BufPositionX, MIL_DOUBLE BufPositionY)
   {
   CString strMouse;
   strMouse.Format(MIL_TEXT("M:(%d,%d)->(%.2f,%.2f)"),(MIL_INT)DispPositionX, (MIL_INT)DispPositionY, 
                                                               BufPositionX, BufPositionY);
   m_wndStatusBar.SetPaneText(2,strMouse);
   }
////////////////////////////////////////////////////////////////////////
// MIL: Used to retrieve size of the status bar
////////////////////////////////////////////////////////////////////////
CSize CChildFrame::GetStatusBarSize()
   {
   RECT statusBarRECT;

   m_wndStatusBar.GetStatusBarCtrl().GetClientRect(&statusBarRECT);
   
   //construct a csize object to receive size of the status bar
   CSize statusBarSize(statusBarRECT.right  - statusBarRECT.left, 
                       statusBarRECT.bottom - statusBarRECT.top);
   
   return statusBarSize;
   }
