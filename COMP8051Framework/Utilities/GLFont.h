#ifndef GLFONT_H
#define GLFONT_H

#include <gl/gl.h>
#include <gl/glu.h>

class RenderSystem;

// ----------------------------------------------------------------------------
// GLFont.h - OpenGL Font Wrapper for Windows Fonts
// ----------------------------------------------------------------------------
// Once again, we can thank NeHe (nehe.gamedev.net) for this utility function
// that allows us to convert Windows fonts into GL Bitmaps for quick debug
// output.
//
// Note: This leverages some platform-specific Windows code - there are alternatives
// for other GL platforms around. You'll have to dig them up.

class GLFont
{
public:

	// Constructor/Destructor
	GLFont(RenderSystem* renderSystem, char* fontName, unsigned int fontSize);
	virtual ~GLFont();

	// Drawing
	inline void setColor(float r, float g, float b)	{ mR=r;mB=b;mG=g; }	// Sets text color (0.0 - 1.0 range)
	void drawTextXY(float x, float y, const char* fmt, ...);			// Draws text at an XY point on screen

private:
	RenderSystem*	mRenderSystem;		// Need this for screen space calculations
	GLuint			mBase;				// Display list for character set
	GLfloat			mR,mG,mB;			// Current font color
};



#endif