#include "stdafx.h"
#include <iostream>
#include <windows.h>							// Header File For Windows
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library

#include "Utilities\Debug.h"
#include "RenderSystem.h"
#include "Utilities\GLFont.h"

// ----------------------------------------------------------------------------
// RenderSystem Implementation
// ----------------------------------------------------------------------------

// Useful constants
const unsigned int	MAX_WIDTH	=	2048;			// Max window width (arbitrary - just picked a safe value)
const unsigned int	MAX_HEIGHT	=	1536;			// Max window height (arbitrary - just picked a safe value)
const unsigned int  DEFAULT_WIDTH = 800;			// Safe value for most screens
const unsigned int	DEFAULT_HEIGHT = 600;			// Safe value for most screens



// Constructor/Destructor
RenderSystem::RenderSystem() : 
		mWidth(DEFAULT_WIDTH), mHeight(DEFAULT_HEIGHT), mIsWindowed(true), mRenderContext(NULL),
		mDeviceContext(NULL), mWindowHandle(NULL), mDebugFont(NULL), mFrameTime(0.0f), mAvgFrameRate(0.0f)
{
}

RenderSystem::~RenderSystem()
{
}


// Configuration Parameters
void RenderSystem::SetAppInstance(HINSTANCE appInstance)
{
	ASSERT_MSG(appInstance, "Bad Application Instance.");
	mAppInstance = appInstance;
}

void RenderSystem::SetResolution(unsigned int width, unsigned int height)
{
	ASSERT_MSG(width <= MAX_WIDTH, "Specified screen width is too large");
	ASSERT_MSG(height <= MAX_HEIGHT, "Specified screen height is too large");

	mWidth = width;
	mHeight = height;
}

void RenderSystem::SetWindowed(bool inWindow)
{
	mIsWindowed = inWindow;
}


// GameSystem Methods
GameSystemResult RenderSystem::Init()
{
	// Create Our OpenGL Window
	if (!CreateGLWindow("Comp 8051 - Texture Demo", mWidth, mHeight,16,!mIsWindowed))
	{
		std::cerr << "Could not create OpenGL window.\n";
		return GSR_FAILURE;							// Quit If Window Was Not Created
	}

	// Create Debug Font
	mDebugFont = new GLFont(this, "Courier New", 16);

	// Set Some Basic Render State
	glShadeModel(GL_FLAT);						// Enables Smooth Shading
	glClearColor(0.0, 0.0, 0.0, 1.0);			// Black Background
	glClearDepth(1.0f);							// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	
	return GSR_SUCCESS;

}

GameSystemResult RenderSystem::Purge()
{
	delete mDebugFont;
	KillGLWindow();
	return GSR_SUCCESS;
}

GameSystemResult RenderSystem::Update(float elapsedTime)
{
	// Just update our framerate counters right now
	if (elapsedTime > 0.0f)
	{
		mFrameTime = elapsedTime;
		if (mAvgFrameRate == 0.0f)
			mAvgFrameRate = 1.0f/mFrameTime;
		else
			mAvgFrameRate = mAvgFrameRate*0.99f + 1.0f/mFrameTime*0.01f;
	}
	return GSR_SUCCESS;
}

GameSystemResult RenderSystem::Render()
{
	// Set up for 2D Drawing
	glMatrixMode(GL_PROJECTION);				// Select projection matrix
	glLoadIdentity();							// Reset projection Matrix
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);					// Select modelview matrix
	glLoadIdentity();							// Reset modelview matrix

	// Draw Framerate
	mDebugFont->setColor(1.0f,1.0f,1.0f);
	mDebugFont->drawTextXY(10.0f,mHeight-20.0f,"Framerate: %f Average:%d", 1.0f/mFrameTime, static_cast<int>(mAvgFrameRate));

	return GSR_SUCCESS;
}


// Begin/End Frame
void RenderSystem::BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glViewport(0, 0, mWidth, mHeight);			// Reset The Current Viewport
}

void RenderSystem::EndFrame()
{
	SwapBuffers(mDeviceContext);			// Swap Buffers (Double Buffering)
}


// OpenGL Utility Methods - Thanks to NeHe (http://nehe.gamedev.net) for the most part on these
BOOL RenderSystem::CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;				// Holds The Results After Searching For A Match
	DWORD		dwExStyle;					// Window Extended Style
	DWORD		dwStyle;					// Window Style

	RECT WindowRect;						// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;				// Set Left Value To 0
	WindowRect.right=(long)width;			// Set Right Value To Requested Width
	WindowRect.top=(long)0;					// Set Top Value To 0
	WindowRect.bottom=(long)height;			// Set Bottom Value To Requested Height



	if (fullscreenflag)								// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;					// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
					mIsWindowed = true;				// Select Windowed Mode (Fullscreen=FALSE)
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;					// Exit And Return FALSE
			}
		}
	}


	if (fullscreenflag)								// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;					// Window Extended Style
		dwStyle=WS_POPUP;						// Windows Style
		ShowCursor(FALSE);						// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	if (!(mWindowHandle=CreateWindowEx(	dwExStyle,				// Extended Style For The Window
					"OpenGL",				// Class Name
					title,					// Window Title
					WS_CLIPSIBLINGS |			// Required Window Style
					WS_CLIPCHILDREN |			// Required Window Style
					dwStyle,				// Selected Window Style
					0, 0,					// Window Position
					WindowRect.right-WindowRect.left,	// Calculate Adjusted Window Width
					WindowRect.bottom-WindowRect.top,	// Calculate Adjusted Window Height
					NULL,					// No Parent Window
					NULL,					// No Menu
					mAppInstance,				// Instance
					NULL)))					// Don't Pass Anything To WM_CREATE
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}


	static	PIXELFORMATDESCRIPTOR pfd=					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(mDeviceContext=GetDC(mWindowHandle)))							// Did We Get A Device Context?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(mDeviceContext,&pfd)))				// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if(!SetPixelFormat(mDeviceContext,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if (!(mRenderContext=wglCreateContext(mDeviceContext)))					// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	if(!wglMakeCurrent(mDeviceContext,mRenderContext))						// Try To Activate The Rendering Context
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	ShowWindow(mWindowHandle,SW_SHOW);			// Show The Window
	SetForegroundWindow(mWindowHandle);			// Slightly Higher Priority
	SetFocus(mWindowHandle);					// Sets Keyboard Focus To The Window
	ResizeWindow(mWidth, mHeight);				// Set Up Our Perspective GL Screen

	return TRUE;								// Success
}





void RenderSystem::ResizeWindow(unsigned int width, unsigned int height)				// Resize And Initialize The GL Window
{
	// Update the object's notion of window size
	mWidth = width;
	mHeight = height;
}

void RenderSystem::KillGLWindow()							// Properly Kill The Window
{    
	if (!mIsWindowed)								// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);			// If So Switch Back To The Desktop
		ShowCursor(TRUE);						// Show Mouse Pointer
	}

	if (mRenderContext)								// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
				MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}	

		if (!wglDeleteContext(mRenderContext))					// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		mRenderContext=NULL;							// Set RC To NULL
	}

	if (mDeviceContext && !ReleaseDC(mWindowHandle,mDeviceContext))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		mDeviceContext=NULL;							// Set DC To NULL
	}

	if (mWindowHandle && !DestroyWindow(mWindowHandle))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release mWindowHandle.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		mWindowHandle=NULL;							// Set mWindowHandle To NULL
	}

	
}



