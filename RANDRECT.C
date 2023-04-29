/*---------------------------------------------------------------
   RANDRECT.C -- Random Rectangles for OS/2 Presentation Manager
                 (c) 1988, Ziff Communications Co.
                 PC Magazine * Charles Petzold, July 1988
  ---------------------------------------------------------------*/

#define INCL_WINCOMMON
#define INCL_WINMESSAGEMGR
#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#define ID_TIMER 1

MRESULT EXPENTRY ClientWndProc (HWND, USHORT, MPARAM, MPARAM) ;
HAB  hab ;
CHAR	szClassName[] = "RandRect";

int main (void)
     {
     // unsigned char szClientClass[] = "RandRect" ;
     HMQ         hmq ;
     HWND       hwndClient, hwndFrame ;
     QMSG       qmsg ;
     ULONG 		ctldata;


     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;
     WinRegisterClass( hab,
                           (PCH)szClassName,
			   (PFNWP)ClientWndProc,
                           CS_SYNCPAINT | CS_SIZEREDRAW,
			   0);
    ctldata = FCF_STANDARD & ~FCF_MENU & ~FCF_ACCELTABLE & ~FCF_ICON;

    hwndFrame = WinCreateStdWindow( HWND_DESKTOP,
					 WS_VISIBLE,
  					 &ctldata,
					 (PCH)szClassName,
					 NULL,
                                         0L,
                                         (HMODULE)NULL,
					 0,
					 (HWND FAR *)&hwndClient );

     if (hwndFrame != '\0')
          {
          while (WinGetMsg (hab, &qmsg, 0, 0, 0))
               WinDispatchMsg (hab, &qmsg) ;

          WinDestroyWindow (hwndFrame) ;
          }
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

VOID DrawIt (HPS hps, SHORT cxClient, SHORT cyClient)
     {
     POINTL ptl ;

     GpiSetPattern (hps, 1L + rand() % 16) ;           // Pattern = 1 to 16
     GpiSetColor (hps, (LONG) (rand() % 16)) ;         // Color = 0 to 15
     GpiSetBackColor (hps, (LONG) (rand () % 16)) ;    // Background color
     GpiSetBackMix (hps, BM_OVERPAINT) ;               // Background mix

     ptl.x = rand() % cxClient ;                       // First corner
     ptl.y = rand() % cyClient ;
     GpiMove (hps, &ptl) ;

     ptl.x = rand() % cxClient ;                       // Opposite corner
     ptl.y = rand() % cyClient ;
     GpiBox (hps, DRO_FILL, &ptl, 0L, 0L) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
     {
     static SHORT cxClient, cyClient ;
     HPS          hps ;

     switch (msg)
          {
          case WM_CREATE:                          // Start the timer
               if (!WinStartTimer (hab, hwnd, ID_TIMER, 0))
                    {
                    WinMessageBox (HWND_DESKTOP, hwnd,
                         (PCSZ) "Cannot run program - Too many clocks or timers",
                         (PCSZ)"RANDRECT", 0, MB_OK | MB_ICONEXCLAMATION) ;
                    return (MRESULT) TRUE;
                    }
               return 0 ;

          case WM_SIZE:                           // Save size of client
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;
               return 0 ;

          case WM_TIMER:                          // Draw a rectangle
               hps = WinGetPS (hwnd) ;

               DrawIt (hps, cxClient, cyClient) ;

               WinReleasePS (hps) ;
               return 0 ;

          case WM_ERASEBACKGROUND:                // Erase the background
               return (MRESULT) TRUE ;

          case WM_DESTROY:                        // Stop the timer
               WinStopTimer (hab, hwnd, ID_TIMER) ;
               return 0 ;
	  default:
	       return( (MRESULT)WinDefWindowProc(hwnd, msg, mp1, mp2));
          }
     }
