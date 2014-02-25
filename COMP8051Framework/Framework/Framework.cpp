// Framework.cpp : Defines the entry point for the application.
//
#include "stdafx.h"


#include <windows.h>							// Header File For Windows
#include <shellapi.h>

#include "Framework.h"
#include "MainLoop/MainLoop.h"
#include "GameSystems/RenderSystem.h"
#include "Demos/DemoStub.h"

#define MIN_FRAMERATE_TIME		(1.0f/10.0f)	// Timer is capped at a low end of 10fps to avoid problems

bool	active=TRUE;							// Window Active Flag Set To TRUE By Default
bool	fullscreen=FALSE;						// Fullscreen Flag Set To Fullscreen Mode By Default



// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HINSTANCE			hInstance;					// App Instance




LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
				UINT	uMsg,					// Message For This Window
				WPARAM	wParam,					// Additional Message Information
				LPARAM	lParam)					// Additional Message Information
{
	switch (uMsg)								// Check For Windows Messages
	{
		case WM_ACTIVATE:						// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;					// Program Is Active
			}
			else
			{
				active=FALSE;					// Program Is No Longer Active
			}

			return 0;						// Return To The Message Loop
		}
		
		
		case WM_SYSCOMMAND:						// Intercept System Commands
		{
			switch (wParam)						// Check System Calls
			{
				case SC_SCREENSAVE:				// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;					// Prevent From Happening
			}
			break;							// Exit
		}

		case WM_CLOSE:							// Did We Receive A Close Message?
		{
			PostQuitMessage(0);					// Send A Quit Message
			return 0;						// Jump Back
		}

		case WM_KEYDOWN:						// Is A Key Being Held Down?
		{
			MainLoop::Get()->keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;						// Jump Back
		}

		case WM_KEYUP:							// Has A Key Been Released?
		{
			MainLoop::Get()->keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;						// Jump Back
		}

		case WM_LBUTTONDOWN:				// Mouse button pressed (start drag)
		{
			MainLoop::Get()->inDrag = true;
			MainLoop::Get()->mouseX = MainLoop::Get()->startMouseX = LOWORD(lParam);
			MainLoop::Get()->mouseY = MainLoop::Get()->startMouseY = HIWORD(lParam);
			return 0;						// Jump Back
		}

		case WM_LBUTTONUP:					// Mouse button released (end drag)
		{
			MainLoop::Get()->inDrag = false;
			return 0;						// Jump Back
		}

		case WM_MOUSEMOVE:					// Mouse moved (possibly update drag)
		{
			MainLoop::Get()->mouseX = LOWORD(lParam);
			MainLoop::Get()->mouseY = HIWORD(lParam);
			return 0;						// Jump Back
		}

		case WM_SIZE:							// Resize The OpenGL Window
		{
			RenderSystem* renderSys = MainLoop::Get()->RenderSys();
			if (renderSys)
				renderSys->ResizeWindow(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width, HiWord=Height
			return 0;						// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG	msg;								// Windows Message Structure
	BOOL	done=FALSE;						// Bool Variable To Exit Loop

	WNDCLASS	wc;

	hInstance		= GetModuleHandle(NULL);					// Grab An Instance For Our Window
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Move, And Own DC For Window
	wc.lpfnWndProc	= (WNDPROC) WndProc;						// WndProc Handles Messages
	wc.cbClsExtra	= 0;										// No Extra Window Data
	wc.cbWndExtra	= 0;										// No Extra Window Data
	wc.hInstance	= hInstance;								// Set The Instance
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);				// Load The Default Icon
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);				// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))						// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Exit And Return FALSE
	}

	// Initialize Main Loop / Subsystems
	RenderSystem* renderSys = new RenderSystem();
	renderSys->SetAppInstance(hInstance);
	renderSys->SetResolution(800,600);


	MainLoop::Get()->Initialize();
	MainLoop::Get()->RegisterAndInitSubsystem(renderSys, SUBSYSTEM_RENDER);

	// Set up demo
	DemoBase* demo = CreateDemoInstance();
	MainLoop::Get()->RegisterAndInitDemo(demo);

	
	// Grab initial timer and frequency
	LARGE_INTEGER lastTime, frequency;
	QueryPerformanceCounter(&lastTime);
	QueryPerformanceFrequency(&frequency);

	while(!done)								// Loop That Runs Until done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
		{

		if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
		{
				done=TRUE;					// If So done=TRUE
			}
			else							// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else								// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)						// Program Active?
			{
				if (MainLoop::Get()->keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;				// ESC Signalled A Quit
				}
				else						// Not Time To Quit, Update Screen
				{
					// How much time has elapsed?
					LARGE_INTEGER currentTime, elapsed;
					QueryPerformanceCounter(&currentTime);
					
					elapsed.QuadPart = currentTime.QuadPart-lastTime.QuadPart;
					
					// Wrap around?
					if (elapsed.QuadPart < 0)
						elapsed.QuadPart += frequency.QuadPart;

					// Compute ET in fractions of a second
					float elapsedTime = static_cast<float>((double)elapsed.QuadPart/(double)frequency.QuadPart);

					// Cap elapsed time to 10fps to avoid debugger problems
					if (elapsedTime > MIN_FRAMERATE_TIME)
					{
						elapsedTime = MIN_FRAMERATE_TIME;
					}


					// Timestep and render
					MainLoop::Get()->Update(elapsedTime);
					MainLoop::Get()->Render();

					lastTime = currentTime;
				}
			}
		}
	}
	// Shutdown
	MainLoop::Get()->PurgeDemo();
	MainLoop::Get()->Purge();
	delete demo;

	if (!UnregisterClass("OpenGL",hInstance))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// Set mAppInstance To NULL
	}
	
	return (msg.wParam);							// Exit The Program
}
