// ----------------------------------------------------------------------------
// RenderSystem.h - OpenGL Rendering Subsystem
// ----------------------------------------------------------------------------
// Manages all OpenGL and Render Hardware Access
//

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "GameSystem.h"

class GLFont;

// ----------------------------------------------------------------------------
// RenderSystem Class
// ----------------------------------------------------------------------------
// OpenGL and Hardware Manager. Usage pattern:
//
//		1) Construct object
//		2) Configure object
//		3) Initialize
//		4) Use
//		5) Purge

class RenderSystem : public GameSystem
{
public:
	
	// Constructor/Destructor
	RenderSystem();
	virtual ~RenderSystem();

	// Configuration Parameters
	void SetResolution(unsigned int width, unsigned int height);		// Defaults to 800 x 600
	inline unsigned int screenWidth() { return mWidth; }
	inline unsigned int screenHeight()	{ return mHeight; }

	void SetWindowed(bool inWindow);									// Defaults to windowed "on"
	inline bool isWindowed() { return mIsWindowed; }

	// Windows OS Specific Messages
	void SetAppInstance(HINSTANCE appInstance);							// Required from Windows OS framework
	void ResizeWindow(unsigned int width, unsigned int height);			// Called when the OS Posts a Resize Message
	inline HDC DeviceContext()	{ return mDeviceContext; }

	// Begin/End Frame
	void BeginFrame();			// Prepares a frame for rendering
	void EndFrame();			// Completes rendering and does page flip

	// GameSystem Methods
	virtual GameSystemResult	Init();
	virtual GameSystemResult	Purge();
	virtual GameSystemResult	Update(float elapsedTime);
	virtual GameSystemResult	Render();

	
private:

	unsigned int		mWidth;
	unsigned int		mHeight;
	bool				mIsWindowed;

	// Debug Framerate Display
	GLFont*				mDebugFont;					// Font for drawing stats, framerate, etc.
	float				mFrameTime;					// Time for last frame
	float				mAvgFrameRate;				// Historical frame rate

	// Windows/GL Interface Variables	
	HINSTANCE		mAppInstance;					// Provided by OS framework
	HGLRC           mRenderContext;					// Permanent Rendering Context
	HDC             mDeviceContext;					// Private GDI Device Context
	HWND            mWindowHandle;					// Holds Our Window Handle
	
	// GL Utility Methods (see http://nehe.gamedev.net for reference)
	BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
	void KillGLWindow();
		
};






#endif
