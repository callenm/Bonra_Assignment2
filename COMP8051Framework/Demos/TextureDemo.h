///////////////////////////////////////////////////////////////////////////////	
//
//  TextureDemo
//
//  COMP 8051 Texture demo subsystem
//
//  (c) 2010-2013 Borna Noureddin
//
///////////////////////////////////////////////////////////////////////////////	

#ifndef LAB3_H
#define LAB3_H

#include "DemoBase.h"
#include "Demos/Object3D.h"

using namespace std;

typedef struct
{
	GLfloat u, v;
} TexCoord;

class TextureDemo : public DemoBase
{
public:

	// Constructor/Destructor
	TextureDemo()	{ mUseTexture = false; };
	virtual ~TextureDemo()	{};

	// System Interface
	bool Initialize();
	void Purge();
	bool Update(float elapsedTime);
	bool Render();

private:

	// Texture members and methods
	bool mUseTexture;
	GLubyte *mTexImg;

	// Object (model) members and methods
	Object3D *theModel;
	bool dragStarted;
	GLfloat dragStartThetaX, dragStartThetaY;
	bool mUseColor;

};


#endif