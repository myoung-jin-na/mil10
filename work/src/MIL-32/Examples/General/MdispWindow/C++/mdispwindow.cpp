/*****************************************************************************/
/* 
 * File name: MDispWindow.cpp
 *
 * Synopsis:  This program displays a welcoming message in a user-defined
 *            window and grabs (if supported) in it. It uses 
 *            the MIL system and the MdispSelectWindow() function 
 *            to display the MIL buffer in a user-created client window. 
 *
 */
#include <windows.h> /* windows.h must precede mil.h. */
#include <mil.h>

/* Window title. */
#define MIL_APPLICATION_NAME       MIL_TEXT("MIL Application")
#define MIL_APPLICATION_NAME_SIZE  128

/* Default image dimensions. */
#define DEFAULT_IMAGE_SIZE_X       640
#define DEFAULT_IMAGE_SIZE_Y       480
#define DEFAULT_IMAGE_SIZE_BAND      1

/* Background color of the window client area. */
#define BACKCOLORRED               160
#define BACKCOLORGREEN             160
#define BACKCOLORBLUE              164

/* Defines for menu. */
#define IDM_START_MAIN             100

/* Function prototypes. */
void MilApplication(HWND UserWindowHandle);
void MilApplicationPaint(HWND UserWindowHandle);

HWND hwndChild;
HWND hwndMain;

/***************************************************************************/
/*/
* Name:         MilApplication()
*
* Synopsis:     This function is the core of the MIL application that
*               is executed when the "Start" menu item of this
*               Windows program is selected. See WinMain() below
*               for the program entry point.
*
*               It uses MIL to display a welcoming message in the 
*               specified user window and to grab in it (if it is supported) 
*               using the target system.
*               
*/

void MilApplication(HWND UserWindowHandle)
{
   /* MIL variables */
   MIL_ID MilApplication,  /* MIL Application identifier.  */
          MilSystem,       /* MIL System identifier.       */
          MilDisplay,      /* MIL Display identifier.      */
          MilDigitizer,    /* MIL Digitizer identifier.    */
          MilImage;        /* MIL Image buffer identifier. */

   MIL_INT BufSizeX;
   MIL_INT BufSizeY;
   MIL_INT BufSizeBand;

   /* Allocate a MIL application. */
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);

   /* Allocate a MIL system. */
   MsysAlloc(M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_DEFAULT, &MilSystem);

   /* Allocate a MIL display. */
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_WINDOWED, &MilDisplay);

   /* Allocate a MIL digitizer, if supported, and set the target image size. */
   if (MsysInquire(MilSystem, M_DIGITIZER_NUM, M_NULL) > 0)
      {
      MIL_INT MaxSizeX = 0, MaxSizeY = 0;
      UINT uFlags = SWP_NOZORDER|SWP_NOACTIVATE;
      MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
      MdigInquire(MilDigitizer, M_SIZE_X,    &BufSizeX);
      MdigInquire(MilDigitizer, M_SIZE_Y,    &BufSizeY);
      MdigInquire(MilDigitizer, M_SIZE_BAND, &BufSizeBand);

      /* Resize the display window */
      if((BufSizeX > DEFAULT_IMAGE_SIZE_X) || (BufSizeY > DEFAULT_IMAGE_SIZE_Y))
         {
         SetWindowPos(hwndMain, 0, 0, 0, (int)BufSizeX, (int)BufSizeY, 
                                       SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
         SetWindowPos(UserWindowHandle, 0, 0, 0, (int)BufSizeX, (int)BufSizeY, 
                                       SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE); 
         }
      }
   else
      {
      MilDigitizer = M_NULL;
      BufSizeX     = DEFAULT_IMAGE_SIZE_X;
      BufSizeY     = DEFAULT_IMAGE_SIZE_Y;
      BufSizeBand  = DEFAULT_IMAGE_SIZE_BAND;
      }

   /* Allocate a MIL buffer. */
   MbufAllocColor(MilSystem, BufSizeBand, BufSizeX, BufSizeY, 8+M_UNSIGNED,
                  (MilDigitizer ? M_IMAGE+M_DISP+M_GRAB : M_IMAGE+M_DISP), &MilImage);

   /* Clear the buffer. */
   MbufClear(MilImage,0);

   /* Select the MIL buffer to be displayed in the user-specified window. */
   MdispSelectWindow(MilDisplay, MilImage, UserWindowHandle);

   /* Print a string in the image buffer using MIL.
      Note: When a MIL buffer is modified using a MIL command, the display 
      automatically updates the window passed to MdispSelectWindow().
   */
      MgraFont(M_DEFAULT, M_FONT_DEFAULT_LARGE);
      MgraText(M_DEFAULT, MilImage, (BufSizeX/8)*2, BufSizeY/2, 
                                    MIL_TEXT(" Welcome to MIL !!! "));
      MgraRect(M_DEFAULT, MilImage, ((BufSizeX/8)*2)-60, (BufSizeY/2)-80, 
                                    ((BufSizeX/8)*2)+370, (BufSizeY/2)+100);
      MgraRect(M_DEFAULT, MilImage, ((BufSizeX/8)*2)-40, (BufSizeY/2)-60, 
                                    ((BufSizeX/8)*2)+350, (BufSizeY/2)+80);
      MgraRect(M_DEFAULT, MilImage, ((BufSizeX/8)*2)-20, (BufSizeY/2)-40, 
                                    ((BufSizeX/8)*2)+330, (BufSizeY/2)+60);
     
   /* Open a message box to wait for a key press. */
   MessageBox(0,MIL_TEXT("\"Welcome to MIL !!!\" was printed"), 
                MIL_TEXT("MIL application example"),
                MB_APPLMODAL | MB_ICONEXCLAMATION);

   /* Grab in the user window if supported. */
   if (MilDigitizer)
      {
      /* Grab continuously. */
      MdigGrabContinuous(MilDigitizer, MilImage);

      /* Open a message box to wait for a key press. */
      MessageBox(0,MIL_TEXT("Continuous grab in progress"), 
                   MIL_TEXT("MIL application example"),
                   MB_APPLMODAL | MB_ICONEXCLAMATION );
                   
      /* Stop continuous grab. */
      MdigHalt(MilDigitizer);
      }

   /* Remove the MIL buffer from the display. */
   MdispSelect(MilDisplay, M_NULL);

   /* Free allocated objects. */
   MbufFree(MilImage);

   if (MilDigitizer)
      MdigFree(MilDigitizer);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);
}


/***************************************************************************/
/*/
* Name:       MilApplicationPaint()
*
* synopsis:  This function can be used to complete the work done by
*            the MIL paint manager when the client window receives
*            a paint message.
*
*            This is called every time WindowProc receives a
*            WM_PAINT message
*/

void MilApplicationPaint(HWND UserWindowHandle)
{
   /* Nothing is done here because nothing gets drawn on top of
    * or around our displayed MIL buffer in the window.
    */
   UserWindowHandle = UserWindowHandle; /* line to remove warning */
}

/***************************************************************************/
/* 
 * Synopsis:  The next functions lets the user to quickly create a Windows 
 *            program which will call the MilApplication() function each
 *            time the Start menu item is selected. The user may also
 *            use the MilApplicationPaint() function that is called 
 *            each time the window receives a paint message to
 *            complete the work already done by the MIL paint manager.
 */

/* Prototypes for Windows application */
int WINAPI MosWinMain(HINSTANCE, HINSTANCE, LPTSTR, int);
long WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApplication(HINSTANCE, int, HBRUSH);

/***************************************************************************/
/*
 *   Name:     WinMain()
 *
 *   Synopsis: Calls initialization function, processes message loop.
 */

int WINAPI MosWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPTSTR lpCmdLine, int nCmdShow)
{
   MSG msg;

   //create un brush for the window's background
   HBRUSH brush = CreateSolidBrush(RGB(BACKCOLORRED,
                                       BACKCOLORGREEN,
                                       BACKCOLORBLUE));

   /* Avoid warning for the unused parameter. */
   lpCmdLine = lpCmdLine;

   /* Allow only one instance of program. */
   if (hPrevInstance)
      {
      MessageBox(0, MIL_TEXT("Sample MIL Host Windows Application already active.\n")
                    MIL_TEXT("This version sample application does not allow ")
                    MIL_TEXT("multiple instances."),
                    MIL_TEXT("Sample MIL Host Application - Error"), 
                     MB_OK | MB_ICONSTOP);
      return (FALSE);
      }

   /* Exit if unable to initialize. */
   if (!InitApplication(hInstance,nCmdShow,brush)) 
      return (FALSE);                      

   /* Get and dispatch messages until a WM_QUIT message is received. */
   while (GetMessage(&msg,(HWND)NULL,(UINT)NULL,(UINT)NULL))      
      {
      TranslateMessage(&msg);   /* Translates virtual key codes. */
      DispatchMessage(&msg);    /* Dispatches message to window. */
      }

   DeleteObject (brush);

   return ((MIL_INT32)msg.wParam);    /* Returns the value from PostQuitMessage. */
}


/***************************************************************************/
/*
 *   Name:      MainWndProc()
 *
 *   Synopsis:  Processes messages:
 *              WM_COMMAND     - application menu.
 *              WM_ENDSESSION  - destroy window.
 *              WM_CLOSE       - destroy window.
 *              WM_DESTROY     - destroy window.
 */

long WINAPI MainWndProc(HWND   hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   HMENU hAppMenu;

   switch (message)
      {
      case WM_COMMAND:     /* message: command from application menu. */
         switch(wParam)
            {
            case IDM_START_MAIN:
               hAppMenu = GetMenu(hWnd);
               EnableMenuItem(hAppMenu,MF_BYCOMMAND | IDM_START_MAIN,MF_GRAYED);
               DrawMenuBar(hWnd);
               MilApplication(hwndChild);
               if (!GetWindowLong(hWnd,sizeof(LPVOID)))
                  {
                  PostQuitMessage(0);
                  }
               else
                  {
                  EnableMenuItem(hAppMenu,MF_BYCOMMAND | IDM_START_MAIN,MF_ENABLED);
                  DrawMenuBar(hWnd);
                  }
               break;
            default:
               return ((MIL_INT32)DefWindowProc(hWnd, message, wParam, lParam));
            }
         break;
      case WM_PAINT:
         MilApplicationPaint(hWnd);
         return ((MIL_INT32)DefWindowProc(hWnd, message, wParam, lParam));
      case WM_ENDSESSION:
      case WM_CLOSE:
         PostMessage(hWnd, WM_DESTROY, (WORD)0, (LONG)0);
         break;
      case WM_DESTROY:
         /* Free the reserved extra window LPSTR to szAppName. */
         if (GetWindowLongPtr(hWnd,sizeof(LPVOID)))
            {
            free((char *)GetWindowLongPtr(hWnd,sizeof(LPVOID)));
            SetWindowLongPtr(hWnd,sizeof(LPVOID),0L);
            }
         PostQuitMessage(0);
         //destroy the window to avoid memory leaks
         DestroyWindow(hwndChild);
         DestroyWindow(hWnd);

      default:
         return ((MIL_INT32)DefWindowProc(hWnd, message, wParam, lParam));
      }
   return(0L);
}


/***************************************************************************/
/*
 *   Name:     InitApplication()
 *             
 *   Synopsis: Initializes window data, registers window class and 
 *             creates main window.
 */

BOOL InitApplication(HINSTANCE hInstance, int nCmdShow, HBRUSH brush)
{
   WNDCLASS        wc;
   HWND            hwnd;
   MIL_TEXT_CHAR   TmpName[MIL_APPLICATION_NAME_SIZE+1];
   LPTSTR          szAppName;

   /* Allocation and storage of application name. */
   szAppName = (LPTSTR)malloc((MIL_APPLICATION_NAME_SIZE+40) * sizeof(MIL_TEXT_CHAR));
   MosStrcpy((MIL_TEXT_PTR)szAppName, MIL_APPLICATION_NAME_SIZE+40, MIL_APPLICATION_NAME);

   wc.style          = CS_VREDRAW | CS_HREDRAW;
   wc.lpfnWndProc    = (WNDPROC)MainWndProc;
   wc.cbClsExtra     = 0;
   wc.cbWndExtra     = sizeof(LPVOID)+sizeof(LPTSTR);
   wc.hInstance      = hInstance;
   wc.hIcon          = LoadIcon(0,IDI_APPLICATION);
   wc.hCursor        = LoadCursor(0, IDC_ARROW);
   wc.hbrBackground  = brush;
   wc.lpszMenuName   = MIL_TEXT("MILAPPLMENU");
   wc.lpszClassName  = szAppName;

   /* Register the class. */
   if (!RegisterClass(&wc))
      return (FALSE);

   /* Create the main window. */
   hwnd = CreateWindow (szAppName,
                        szAppName,
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        0, 0, hInstance, 0) ;
   if (!hwnd)
      return (FALSE);

   hwndMain = hwnd;

   /* Create a child window for the image. */
   /* Note: Because the main window is larger than the image buffer, the
    * dispSelectWindow() will be done in a child window, with the same
    * size as the image buffer, to avoid display problem in DirectX 10.
    */
   hwndChild = CreateWindow (szAppName,
                             szAppName,
                             WS_CHILDWINDOW | WS_VISIBLE,
                             0, 0,
                             DEFAULT_IMAGE_SIZE_X, DEFAULT_IMAGE_SIZE_Y,
                             hwnd, 0, hInstance, 0) ;
   if (!hwndChild)
      return (FALSE);

   /* Set the main window title */
   lstrcat(szAppName,MIL_TEXT(" - "));
   GetModuleFileName(hInstance,TmpName,MIL_APPLICATION_NAME_SIZE);
   TmpName[MIL_APPLICATION_NAME_SIZE] = MIL_TEXT('\0');
   lstrcat(szAppName,TmpName);
   SetWindowText(hwnd,szAppName);

   /* Set the reserved extra window LPVOID to NULL. */
   SetWindowLongPtr(hwnd,0,0L);

   /* Set the reserved extra window LPSTR to szAppName. */
   SetWindowLongPtr(hwnd,sizeof(LPVOID),(LONG_PTR)szAppName);

   /* Display the window. */
   ShowWindow (hwnd, nCmdShow) ;
   UpdateWindow (hwnd) ;

   return (TRUE);
}
