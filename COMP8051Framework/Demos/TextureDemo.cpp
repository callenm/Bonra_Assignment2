///////////////////////////////////////////////////////////////////////////////	
//
//  TextureDemo
//
//  COMP 8051 Assignment 3 demo subsystem
//
//  (c) 2010-2013 Borna Noureddin
//
///////////////////////////////////////////////////////////////////////////////	

#include "stdafx.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <atlconv.h>
#include <math.h>
#include "MainLoop\MainLoop.h"
#include "GameSystems\RenderSystem.h"
#include "TextureDemo.h"

///////////////////////////////////////////////////////////////////////////////	
// Object (model) default values

static const GLfloat zoomAmt = 0.1f;	// Zoom factor

// Color definitions
#define WHITE      {1,  1,  1}
#define BLACK      {0,  0,  0}
#define RED        {1,  0,  0}
#define GREEN      {0,  1,  0}
#define BLUE       {0,  0,  1}
#define MAGENTA    {1,  0,  1}
#define YELLOW     {0,  1,  1}
#define CYAN       {1,  1,  0}

// Default colours for polygon faces
static const int numColors = 8;

static const GLfloat colors[][3] = {RED, GREEN, BLUE, WHITE, MAGENTA, CYAN, BLACK, YELLOW};

// Default display object
static const TexCoord texCoords[] = {{0.0,0.0}, {0.0,1.0}, {1.0,1.0}, {1.0,0.0}};

static const Vertex vertices[] = {{-1.0, -1.0, -1.0}, { 1.0, -1.0, -1.0},
									{ 1.0,  1.0, -1.0}, {-1.0,  1.0, -1.0},
									{-1.0, -1.0,  1.0}, { 1.0, -1.0,  1.0},
									{ 1.0,  1.0,  1.0}, {-1.0,  1.0,  1.0}};

static const int faces[][4] = {{0,3,2,1}, {2,3,7,6}, {0,4,7,3},
									{1,2,6,5}, {4,5,6,7}, {0,1,5,4}};


///////////////////////////////////////////////////////////////////////////////	
// Methods

bool TextureDemo::Initialize()
{
	USES_CONVERSION;

	theModel = new Object3D();

	// Parse command-line arguments
	int argc;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	bool objLoaded = false;
	mTexImg = NULL;
	int m = -1, n = -1;
	if (argc > 1) {
		// Read in the PPM image file (ASCII-encoded) as the texture image
		bool err = false;
		fprintf(stderr, "Reading texture image file...\n");
		FILE *fp = fopen(W2A(argv[1]), "r");
		char buf[1024];
		int lineNo = 1, i,  N = 0;
		static char seps[] = " \t\r\n", *tok;
		if (!fgets(buf, sizeof(buf), fp))
			err = true;
		if (!err && strncmp(buf, "P3", 2))
			err = true;
		while (!err && fgets(buf, sizeof(buf), fp)) {
			if (buf[0] == '#')
				continue;
			lineNo++;
			if (lineNo == 2) {
				sscanf(buf, "%d %d", &m, &n);
				mTexImg = (GLubyte *)malloc(m * n * 3 * sizeof(GLubyte));
				continue;
			}
			if (lineNo == 3) {
				sscanf(buf, "%d", &N);
				i = 0;
				continue;
			}
			// read in the whole image into a big chunk of memory
			// and let OpenGL worry about how to interpret it
			tok = strtok(buf, seps);
			while (tok) {
				mTexImg[i++] = atoi(tok);
				tok = strtok(NULL, seps);
			}
		}
		fclose(fp);
		fprintf(stderr, "...done (read %dx%d image of %d bytes)\n", m, n, i);
		if (err)
			fprintf(stderr, "Error reading texture file!\n");
		else
			mUseTexture = true;
		LocalFree(argv);
	}

	if (mUseTexture) {
		// Enable texture mapping and set up the parameters
		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D,0,3,m,n,0,GL_RGB,GL_UNSIGNED_BYTE, mTexImg);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}

	mUseColor = true;
	theModel->SetColorFlag(mUseColor);
	theModel->SetTextureFlag(mUseTexture);
	for (int i = 0; i < (sizeof(faces)/sizeof(faces[0])); i++) {
		Polygon3D p;
		for (int j = 0; j < (sizeof(faces[0]) / sizeof(faces[0][0])); j++) {
			Vertex v;
			memcpy(&v.tc, &texCoords[j%4], sizeof(v.tc));
			memcpy(&v.v, &vertices[faces[i][j]], sizeof(v.v));
			memcpy(&v.color, &colors[j + i % 8], sizeof(v.color));
			p.AddVertex(v);
		}
		theModel->AddFace(p);
	}

	return true;
}

void TextureDemo::Purge()
{
	if (mTexImg)
		free(mTexImg);
	if (theModel)
		delete theModel;
}

bool TextureDemo::Update(float elapsedTime)
{
	if (MainLoop::Get()->keys[VK_HOME]) {
		theModel->Reset();
		MainLoop::Get()->keys[VK_HOME] = false;
	}

	if (MainLoop::Get()->keys['C']) {
		mUseColor = !mUseColor;
		theModel->SetColorFlag(mUseColor);
		MainLoop::Get()->keys['C'] = false;
	}

	if (MainLoop::Get()->keys['T']) {
		if (mUseTexture) {
			mUseTexture = false;
			glDisable(GL_TEXTURE_2D);
		} else {
			mUseTexture = true;
			glEnable(GL_TEXTURE_2D);
		}
		theModel->SetTextureFlag(mUseTexture);
		MainLoop::Get()->keys['T'] = false;
	}

	if (MainLoop::Get()->inDrag) {
		if (!dragStarted) {
			dragStarted = true;
			dragStartThetaX = theModel->GetRotation(Object3D::y_axis);
			dragStartThetaY = theModel->GetRotation(Object3D::x_axis);
		} else {
		}
		theModel->Rotate(Object3D::y_axis, MainLoop::Get()->startMouseX, (int)dragStartThetaX + MainLoop::Get()->mouseX);
		theModel->Rotate(Object3D::x_axis, MainLoop::Get()->startMouseY, (int)dragStartThetaY + MainLoop::Get()->mouseY);
	} else
		dragStarted = false;
	if (MainLoop::Get()->keys['Z'] && !MainLoop::Get()->keys[VK_SHIFT]) {
		theModel->Zoom(zoomAmt);
		MainLoop::Get()->keys['Z'] = false;
	}
	if (MainLoop::Get()->keys['Z'] && MainLoop::Get()->keys[VK_SHIFT]) {
		theModel->Zoom(-zoomAmt);
		MainLoop::Get()->keys['Z'] = false;
	}

	return true;
}

bool TextureDemo::Render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	MainLoop *ml = MainLoop::Get();
	if (ml)
	{
		RenderSystem *rs = ml->RenderSys();
		if (rs)
		{
			unsigned int w = rs->screenWidth();
			unsigned int h = rs->screenHeight();
			if (w <= h)
				glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
					2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
			else
				glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
					2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	theModel->Draw();
	glPopAttrib();

	return true;
}
