// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MdispMFC.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
   //{{AFX_MSG_MAP(CMainFrame)
   ON_WM_CREATE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
   {
   ID_SEPARATOR,           // status line indicator
   ID_INDICATOR_CAPS,
   ID_INDICATOR_NUM,
   ID_INDICATOR_SCRL,
   };

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
   {
   }

CMainFrame::~CMainFrame()
   {
   }

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
   {
   if(CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
      return -1;

   DWORD dwToolbarStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP | CCS_ADJUSTABLE | 
                     CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
   
   if(!m_wndToolBar.Create(this, dwToolbarStyle) ||
      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
      {
      TRACE0("Failed to create toolbar\n");
      return -1;      // fail to create
      }

   if(!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
      {
      TRACE0("Failed to create status bar\n");
      return -1;      // fail to create
      }

   //Set toolbar titles
   m_wndToolBar.SetWindowText(MIL_TEXT("Standard"));

   //Add view Mode combo box into display toolbar
   ///////////////////////////////////////////////////////
   CRect rectComboViewMode;

   //Associate combo boxes identifiers to an empty space in toolbar
   m_wndToolBar.SetButtonInfo(12, IDC_CB_VIEW_MODE, TBBS_SEPARATOR, 140);

   //Retrieve position of the empty items
   m_wndToolBar.GetItemRect(12,&rectComboViewMode);

   //Offset the rect to center in bar
   rectComboViewMode.OffsetRect(5,7);

   //Allow drop-down
   rectComboViewMode.bottom = rectComboViewMode.top + 900;

   m_viewModeCombo.Create(WS_TABSTOP|CBS_AUTOHSCROLL|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,
                           rectComboViewMode,&m_wndToolBar,IDC_CB_VIEW_MODE);

   //Fill combo box with available view modes
   m_viewModeCombo.AddString(MIL_TEXT("M_DEFAULT"));
   m_viewModeCombo.AddString(MIL_TEXT("M_TRANSPARENT"));
   m_viewModeCombo.AddString(MIL_TEXT("M_AUTO_SCALE"));
   m_viewModeCombo.AddString(MIL_TEXT("M_MULTI_BYTES"));
   m_viewModeCombo.AddString(MIL_TEXT("M_BIT_SHIFT:2"));
   m_viewModeCombo.AddString(MIL_TEXT("M_BIT_SHIFT:4"));
   m_viewModeCombo.AddString(MIL_TEXT("M_BIT_SHIFT:8"));

   // Set the combo boxes font properly
   CFont fontComboBox;
   fontComboBox.Attach(GetStockObject(DEFAULT_GUI_FONT));
   m_viewModeCombo.SetFont(&fontComboBox);

   //Initial selection of the view mode
   m_viewModeCombo.SelectString(0,MIL_TEXT("M_TRANSPARENT"));

   // TODO: Delete these lines if you don't want the toolbars to
   // be dockable
   m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
   EnableDocking(CBRS_ALIGN_ANY);

   DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

   // To make sure mainframe is at least the size of the toolbar, tweak the size.
   RECT rectWindow;
   GetWindowRect(&rectWindow);

   SetWindowPos(0,0,0,850,rectWindow.bottom - rectWindow.top,SWP_NOMOVE|SWP_NOZORDER);
   return 0;
   }

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
   {
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CMDIFrameWnd::PreCreateWindow(cs);
   }

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
   {
   CMDIFrameWnd::AssertValid();
   }

void CMainFrame::Dump(CDumpContext& dc) const
   {
   CMDIFrameWnd::Dump(dc);
   }

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
