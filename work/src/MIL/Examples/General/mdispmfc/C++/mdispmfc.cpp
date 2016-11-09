// MdispMFC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MdispMFC.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MdispMFCDoc.h"
#include "MdispMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCApp

BEGIN_MESSAGE_MAP(CMdispMFCApp, CWinApp)
   //{{AFX_MSG_MAP(CMdispMFCApp)
   ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
   // Standard file-based document commands
   ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
   ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
   // Standard print setup command
   ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCApp construction

CMdispMFCApp::CMdispMFCApp()
{
   // TODO: add construction code here
   m_isCurrentlyHookedOnErrors = false;
   // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMdispMFCApp object

CMdispMFCApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCApp initialization

BOOL CMdispMFCApp::InitInstance()
{

   LoadStdProfileSettings();  // Load standard INI file options (including MRU)


   /////////////////////////////////////////////////////////////////////////
   // MIL: Write your one-time initialization code here 
   /////////////////////////////////////////////////////////////////////////   

   // Allocate an application and a system [CALL TO MIL]
   MappAllocDefault(M_DEFAULT,&m_MilApplication, &m_MilSystem, M_NULL, M_NULL, M_NULL);
   
   // Hook MIL error on function DisplayError() [CALL TO MIL]
   MappHookFunction(M_DEFAULT, M_ERROR_CURRENT,DisplayErrorExt,this);

   m_isCurrentlyHookedOnErrors = true;
   
   // Disable the typical MIL error message display [CALL TO MIL]
   MappControl(M_DEFAULT, M_ERROR,M_PRINT_DISABLE);
      
   // Inquire the number of digitizers available on the system [CALL TO MIL]
   MsysInquire(m_MilSystem,M_DIGITIZER_NUM,&m_numberOfDigitizer);
   
   // Digitizer is available
   if (m_numberOfDigitizer)   
   {
      // Allocate a digitizer [CALL TO MIL]
      MdigAlloc(m_MilSystem,M_DEFAULT,MIL_TEXT("M_DEFAULT"),M_DEFAULT,&m_MilDigitizer);

      // Inquire digitizer information [CALL TO MIL]
      MdigInquire(m_MilDigitizer,M_SIZE_X,&m_digitizerSizeX);
      MdigInquire(m_MilDigitizer,M_SIZE_Y,&m_digitizerSizeY);     
      MdigInquire(m_MilDigitizer,M_SIZE_BAND,&m_digitizerNbBands);
   }

   // Initialize the state of the grab
   m_isGrabStarted = FALSE;
   
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write your one-time initialization code here 
   /////////////////////////////////////////////////////////////////////////
    
    
   // Register the application's document templates.  Document templates
   // serve as the connection between documents, frame windows and views.

   CMultiDocTemplate* pDocTemplate;
   pDocTemplate = new CMultiDocTemplate(
      IDR_MDISPTYPE,
      RUNTIME_CLASS(CMdispMFCDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CMdispMFCView));
   AddDocTemplate(pDocTemplate);

   // Create main MDI Frame window
   CMainFrame* pMainFrame = new CMainFrame;
   if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
      return FALSE;
   m_pMainWnd = pMainFrame;

   // Parse command line for standard shell commands, DDE, file open
   CCommandLineInfo cmdInfo;
   ParseCommandLine(cmdInfo);

   // Dispatch commands specified on the command line
   if (!ProcessShellCommand(cmdInfo))
      return FALSE;
    
    // Show and update the initialized main window.
   pMainFrame->ShowWindow(m_nCmdShow);
   pMainFrame->UpdateWindow();

   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
   CAboutDlg();

// Dialog Data
   //{{AFX_DATA(CAboutDlg)
   enum { IDD = IDD_ABOUTBOX };
   //}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CAboutDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   HICON m_hIcon;
   //{{AFX_MSG(CAboutDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
   //{{AFX_DATA_INIT(CAboutDlg)
   //}}AFX_DATA_INIT
   m_hIcon = AfxGetApp()->LoadIcon(IDI_IMAGING);

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CAboutDlg)
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
   //{{AFX_MSG_MAP(CAboutDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();
   
   SetIcon(m_hIcon, TRUE);       // Set big icon
   SetIcon(m_hIcon, FALSE);     // Set small icon
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CMdispMFCApp::OnAppAbout()
{
   CAboutDlg aboutDlg;
   aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCApp commands

int CMdispMFCApp::ExitInstance() 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write your code that will be executed on application exit
   /////////////////////////////////////////////////////////////////////////   
   
   //Free the digitizer [CALL TO MIL]
   if(m_MilDigitizer)    
      MdigFree (m_MilDigitizer);   
   
   //Free the system [CALL TO MIL]
   if(m_MilSystem)       
      MsysFree (m_MilSystem);   
   
   if(m_MilApplication)  
      {
      // Enable the typical MIL error message display[CALL TO MIL]
      MappControl(M_DEFAULT, M_ERROR,M_PRINT_ENABLE);
      
      // Unhook MIL error on function DisplayError() [CALL TO MIL]
      if(m_isCurrentlyHookedOnErrors)
         {
         MappHookFunction(M_DEFAULT, M_ERROR_CURRENT+M_UNHOOK,DisplayErrorExt,this);
         m_isCurrentlyHookedOnErrors = false;
         }
      
      // Free the application [CALL TO MIL]
      MappFree(m_MilApplication);
      }
   
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write your code that will be executed on application exit
   /////////////////////////////////////////////////////////////////////////
   
   return CWinApp::ExitInstance();
   }

/////////////////////////////////////////////////////////////////////////
// MIL: Hook-handler function: DisplayError()
/////////////////////////////////////////////////////////////////////////

MIL_INT MFTYPE DisplayErrorExt(MIL_INT HookType, MIL_ID EventId, void* UserDataPtr)
{
   //If user clicks NO on error message, unhook to errors.
   if(((CMdispMFCApp*) AfxGetApp())->DisplayError(HookType, EventId, 
                              (CWinApp*) UserDataPtr) == M_NO)
      {
      MappHookFunction(M_DEFAULT, M_ERROR_CURRENT+M_UNHOOK,DisplayErrorExt,UserDataPtr);
      ((CMdispMFCApp*) AfxGetApp())->HookedOnErrors(false);
      }

   return M_NULL;
}


long MFTYPE CMdispMFCApp::DisplayError(MIL_INT HookType,
                                       MIL_ID EventId,
                                       void* UserDataPtr)
{
   MIL_TEXT_CHAR ErrorMessageFunction[M_ERROR_MESSAGE_SIZE] = MIL_TEXT("");
   MIL_TEXT_CHAR ErrorMessage[M_ERROR_MESSAGE_SIZE]         = MIL_TEXT("");
   MIL_TEXT_CHAR ErrorSubMessage1[M_ERROR_MESSAGE_SIZE]     = MIL_TEXT("");
   MIL_TEXT_CHAR ErrorSubMessage2[M_ERROR_MESSAGE_SIZE]     = MIL_TEXT("");
   MIL_TEXT_CHAR ErrorSubMessage3[M_ERROR_MESSAGE_SIZE]     = MIL_TEXT("");
   MIL_INT NbSubCode;
   CString   CErrorMessage;

   //Retrieve error message [CALL TO MIL]
   MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_FCT,ErrorMessageFunction);
   MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT,ErrorMessage);
   MappGetHookInfo(EventId,M_CURRENT_SUB_NB,&NbSubCode);   
   
   if (NbSubCode > 2)
      MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_SUB_3,ErrorSubMessage3);   
   if (NbSubCode > 1)   
      MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_SUB_2,ErrorSubMessage2);
   if (NbSubCode > 0)
      MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_SUB_1,ErrorSubMessage1);

   CErrorMessage = ErrorMessageFunction;
   CErrorMessage = CErrorMessage + "\n";
   CErrorMessage = CErrorMessage + ErrorMessage;
   
   if(NbSubCode > 0)
   {
      CErrorMessage = CErrorMessage + "\n";
      CErrorMessage = CErrorMessage + ErrorSubMessage1;
   }
   
   if(NbSubCode > 1)
   {
      CErrorMessage = CErrorMessage + "\n";
      CErrorMessage = CErrorMessage + ErrorSubMessage2;
   }
   
   if(NbSubCode > 2)
   {
      CErrorMessage = CErrorMessage + "\n";
      CErrorMessage = CErrorMessage + ErrorSubMessage3;
   }
   
   CErrorMessage = CErrorMessage + "\n\n";
   CErrorMessage = CErrorMessage + "Do you want to continue error print?";
   
   return (AfxMessageBox(CErrorMessage,MB_YESNO,0) == IDYES)?M_YES:M_NO;
}

/////////////////////////////////////////////////////////////////////////
// MIL: Hook-handler function: DisplayError()
/////////////////////////////////////////////////////////////////////////


