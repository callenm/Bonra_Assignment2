// ----------------------------------------------------------------------------
// GLFont - OpenGL Font Wrapper for Windows Fonts
// ----------------------------------------------------------------------------
#include "stdafx.h"
#include "Utilities/Debug.h"
#include "GameSystems/RenderSystem.h"
#include "GLFont.h"

#define START_CHARACTER		32			// Creates font starting at ASCII 32
#define NUM_CHARACTERS		96			// Number of characters to create

// Constructor/Destructor
GLFont::GLFont(RenderSystem* renderSystem, char* fontName, unsigned int fontSize) : mR(1.0f), mG(1.0f), mB(1.0f), mRenderSystem(renderSystem)
{
	ASSERT(renderSystem && fontName);
	ASSERT_MSG(fontSize > 0, "Bad font size.");

	HFONT	font;						// Windows Font ID
	HFONT	oldfont;					// Used For Good House Keeping

	mBase = glGenLists(96);					// Storage For 96 Characters

	font = CreateFont(	-static_cast<int>(fontSize),		// Height Of Font
						0,				// Font Width
						0,				// Angle Of Escapement
						0,				// Orientation Angle
						FW_BOLD,		// Font Weight
						FALSE,			// Italic
						FALSE,			// Underline
						FALSE,			// Strikeout
						ANSI_CHARSET,			// Character Set Identifier
						OUT_TT_PRECIS,			// Output Precision
						CLIP_DEFAULT_PRECIS,		// Clipping Precision
						ANTIALIASED_QUALITY,		// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
						fontName );
	ASSERT_MSG(font, "Could not create font.");
	
	oldfont = (HFONT)SelectObject(renderSystem->DeviceContext(), font);		// Select new font
	wglUseFontBitmaps(renderSystem->DeviceContext(), START_CHARACTER, NUM_CHARACTERS, mBase);			// Builds 96 Characters Starting At Character 32
	SelectObject(renderSystem->DeviceContext(), oldfont);						// Restores previous font
	DeleteObject(font);								// Delete The Font
}

GLFont::~GLFont()
{
	glDeleteLists(mBase, NUM_CHARACTERS);
}


void GLFont::drawTextXY(float x, float y, const char* fmt, ...)
{
	char		text[256];				// Holds Our String
	va_list		ap;						// Pointer To List Of Arguments

	va_start(ap, fmt);					// Parses The String For Variables
	    vsprintf_s(text, fmt, ap);		// And Converts Symbols To Actual Numbers
	va_end(ap);							// Results Are Stored In Text

	ASSERT_MSG(strlen(text) < 256, "Buffer Overrun");


	glTranslatef(0.0f,0.0f,-1.0f);		// Move One Unit Into The Screen
	glColor3f(mR, mG, mB);				// Set drawing color
	glRasterPos2f(x/static_cast<float>(mRenderSystem->screenWidth()), y/static_cast<float>(mRenderSystem->screenHeight()));


	glPushAttrib(GL_LIST_BIT);			// Pushes The Display List Bits	
	glListBase(mBase - 32);				// Sets The Base Character to 32

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits

}

