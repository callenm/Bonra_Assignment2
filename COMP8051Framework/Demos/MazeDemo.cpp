////////////////////////////////////////////////////////////////////////////
//
// MazeDemo - demo for 3D maze
//
// (c) 2010-2013 Borna Noureddin
//      British Columbia Institute of Technology
//      School of Computing and Academic Studies
//      Bachelor of Technology Program
//      Games Development Option
//
////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <atlconv.h>
#include <math.h>
#include "MainLoop\MainLoop.h"
#include "GameSystems\RenderSystem.h"

#include "MazeDemo.h"

///////////////////////////////////////////////////////////////////////////////	
// Global constants

#define IsFloating	((curry < -3) || (curry > 3))	// user is floating above/below maze
#define wallSeparation (1.99f)						// how far apart front/back sides of maze walls should be

const int mazeSize = 4;
const float cellSize = 4.0f;

///////////////////////////////////////////////////////////////////////////////	
// Methods

bool MazeDemo::Initialize()
{
	USES_CONVERSION;

	theMaze = NULL;
	numRows = numCols = mazeSize;
	CreateMaze();

	currz = currx = curry = angle = vertViewOffset = 0;
	walkThroughWalls = false;
	curDir = dirNORTH;	// default is to face North

	InitTextures();

	return true;
}

void MazeDemo::Purge()
{
	if (theMaze)
		delete theMaze;
}

bool MazeDemo::Update(float elapsedTime)
{
	int curRow = (int)(currz / cellSize);
	int curCol = (int)(currx / cellSize);
	bool inMaze = (curRow >= 0) && (curRow < theMaze->cols) && (curCol >= 0) && (curCol < theMaze->rows);
	if (MainLoop::Get()->keys[VK_LEFT]) {
		if (IsFloating)
			angle -= 10.0;
		else {
			curDir = (curDir == dirNORTH) ? dirWEST : (Direction)(curDir - 1);
			angle -= 90.0;
		}
		MainLoop::Get()->keys[VK_LEFT] = false;
	}
	if (MainLoop::Get()->keys[VK_RIGHT]) {
		if (IsFloating)
			angle += 10.0;
		else {
			curDir = (curDir == dirWEST) ? dirNORTH : (Direction)(curDir + 1);
			angle += 90.0;
		}
		MainLoop::Get()->keys[VK_RIGHT] = false;
	}
	if (MainLoop::Get()->keys[VK_UP]) {
		if (MainLoop::Get()->keys[VK_CONTROL]) {
			bool wasFloating = IsFloating;
			curry -= 1.0f;
			if (wasFloating && !IsFloating)
				angle = 0.0f;
		} else if (!IsFloating || (fmod(angle, 90) == 0)) {
			switch(curDir) {
			case dirNORTH:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).southWallPresent)
					currz += cellSize;
				break;
			case dirEAST:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).westWallPresent)
					currx -= cellSize;
				break;
			case dirSOUTH:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).northWallPresent)
					currz -= cellSize;
				break;
			case dirWEST:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).eastWallPresent)
					currx += cellSize;
				break;
			default:
				break;
			}
		}
		MainLoop::Get()->keys[VK_UP] = false;
	}
	if (MainLoop::Get()->keys[VK_DOWN]) {
		if (MainLoop::Get()->keys[VK_CONTROL]) {
			bool wasFloating = IsFloating;
			curry += 1.0f;
			if (wasFloating && !IsFloating)
				angle = 0.0f;
		} else if (!IsFloating || (fmod(angle, 90) == 0)) {
			switch(curDir) {
			case dirNORTH:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).northWallPresent)
					currz -= cellSize;
				break;
			case dirEAST:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).eastWallPresent)
					currx += cellSize;
				break;
			case dirSOUTH:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).southWallPresent)
					currz += cellSize;
				break;
			case dirWEST:
				if (walkThroughWalls || IsFloating || !inMaze || !theMaze->GetCell(curRow, curCol).westWallPresent)
					currx -= cellSize;
				break;
			default:
				break;
			}
		}
		MainLoop::Get()->keys[VK_DOWN] = false;
	}
	if (MainLoop::Get()->keys[VK_HOME]) {
		curDir = dirNORTH;
		currz = currx = curry = angle = vertViewOffset = 0;
		MainLoop::Get()->keys[VK_HOME] = false;
	}

	if (MainLoop::Get()->keys[VK_PRIOR]) {
		if (vertViewOffset < 1.5f)
			vertViewOffset += 0.1f;
		MainLoop::Get()->keys[VK_PRIOR] = false;
	}
	if (MainLoop::Get()->keys[VK_NEXT]) {
		if (vertViewOffset > -1.5f)
			vertViewOffset -= 0.1f;
		MainLoop::Get()->keys[VK_NEXT] = false;
	}

	if (MainLoop::Get()->keys['W'] && !MainLoop::Get()->keys[VK_SHIFT]) {
		walkThroughWalls = true;
		MainLoop::Get()->keys['W'] = false;
	}
	if (MainLoop::Get()->keys['W'] && MainLoop::Get()->keys[VK_SHIFT]) {
		walkThroughWalls = false;
		MainLoop::Get()->keys['W'] = false;
	}

	return true;
}

bool MazeDemo::Render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0, 1, 0.5, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 2.0, 0.0f,						//camera postion
		      0.0f, 2.0 + vertViewOffset, 1.0f,		//camera viewpoint
			  0.0f, 1.0f, 0.0f);					//up vector (straight up)
	glRotatef(angle, 0.0, 1.0, 0.0);
	glTranslatef(-currx, curry, -currz);

	DrawMaze();

	return true;
}

void MazeDemo::CreateMaze()
{
	theMaze = new Maze(numRows, numCols);
	theMaze->Create();
}

void MazeDemo::DrawWall()
{
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0,		cellSize);	glVertex3f(-cellSize/2, 0.0,  cellSize/2);  // top left
		glTexCoord2f(0.0,		0.0);		glVertex3f(-cellSize/2, 0.0, -cellSize/2);  // bottom left
		glTexCoord2f(cellSize,	0.0);		glVertex3f( cellSize/2, 0.0, -cellSize/2);  // bottom right
		glTexCoord2f(cellSize,	cellSize);	glVertex3f( cellSize/2, 0.0,  cellSize/2);  // top right
	glEnd();
}

void MazeDemo::DrawCell(MazeCell &cell)
{
	if (cell.northWallPresent) {
		SelectTexture(cell.eastWallPresent, cell.westWallPresent, 0.0, 0.0);
		glColor3f(1.0, 0.0, 0.0);	// North wall always drawn in red
		glNormal3f(0.0, 0.0, -1.0);
		glPushMatrix();
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glTranslatef(0.0f, -2.0f, -wallSeparation);
		DrawWall();
		glPopMatrix();
	}

	if (cell.southWallPresent) {
		SelectTexture(cell.westWallPresent, cell.eastWallPresent, 0.0, 180.0);
		glColor3f(0.0, 1.0, 0.0);	// South wall always drawn in green
		glNormal3f(0.0, 0.0, 1.0);
		glPushMatrix();
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glTranslatef(0.0f, -wallSeparation, 2.0f);
		DrawWall();
		glPopMatrix();
	}

	if (cell.eastWallPresent) {
		SelectTexture(cell.northWallPresent, cell.southWallPresent, 90.0, 90.0);
		glColor3f(0.0, 0.0, 1.0);	// East wall always drawn in blue
		glNormal3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glRotatef(90.0, 0.0, 0.0, 1.0);
		glTranslatef(2.0f, -wallSeparation, 0.0f);
		DrawWall();
		glPopMatrix();
	}

	if (cell.westWallPresent) {
		SelectTexture(cell.southWallPresent, cell.northWallPresent, 90.0, -90.0);
		glColor3f(1.0, 1.0, 0.0);	// South wall always drawn in yellow
		glNormal3f(-1.0, 0.0, 0.0);
		glPushMatrix();
		glRotatef(-90.0, 0.0, 0.0, 1.0);
		glTranslatef(-2.0f, -wallSeparation, 0.0f);
		DrawWall();
		glPopMatrix();
	}
}

void MazeDemo::DrawMaze()
{
	for (int i=0; i<theMaze->rows; i++) {
		glPushMatrix();
		// Draw each cell, and move over one
		for (int j=0; j<theMaze->cols; j++) {
			DrawCell(theMaze->GetCell(i,j));
			glTranslatef(cellSize, 0.0, 0.0);
		}
		glPopMatrix();
		// Move up one after each cell
		glTranslatef(0.0, 0.0, cellSize);
	}
}

void MazeDemo::InitTextures()
{
	//*** First, we need to read in the texture images
	//------------------------------------------------------------------------
	if (!ReadPPMImage("x.ppm", xImg))
		exit(1);
	if (!ReadPPMImage("arrow.ppm", arrowImg)) {
		free(xImg.img);
		exit(1);
	}
	if (!ReadPPMImage("doublearrow.ppm", dblArrowImg)) {
		free(xImg.img);
		free(arrowImg.img);
		exit(1);
	}
	//------------------------------------------------------------------------

	//*** Then we need to enable textures and set the appropriate parameters
	//------------------------------------------------------------------------
	glEnable(GL_TEXTURE_2D);	// enable textures
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// repeat on wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// also in t direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// linear filter for magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// linear filter + mipmapping for minification
	//------------------------------------------------------------------------

	//*** Finally, we need to generate, bind and build all the textures
	//------------------------------------------------------------------------
	glGenTextures(1, &xTex);
	glBindTexture(GL_TEXTURE_2D, xTex);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, xImg.m, xImg.n, GL_RGB, GL_UNSIGNED_BYTE, xImg.img);

	glGenTextures(1, &arrowTex);
	glBindTexture(GL_TEXTURE_2D, arrowTex);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, arrowImg.m, arrowImg.n, GL_RGB, GL_UNSIGNED_BYTE, arrowImg.img);

	glGenTextures(1, &dblArrowTex);
	glBindTexture(GL_TEXTURE_2D, dblArrowTex);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, dblArrowImg.m, dblArrowImg.n, GL_RGB, GL_UNSIGNED_BYTE, dblArrowImg.img);
	//------------------------------------------------------------------------
}

bool MazeDemo::ReadPPMImage(char *ppmFileName, PPMImage &img)
{
	img.img = NULL;
	img.m = img.n = -1;
	bool err = false;
	fprintf(stderr, "Reading texture image file...\n");
	FILE *fp = fopen(ppmFileName, "r");
	if (!fp) {
		fprintf(stderr, "Could not find image file <%s>!\n", ppmFileName);
		return false;
	}
	static char buf[1024];
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
			sscanf(buf, "%d %d", &img.m, &img.n);
			img.img = (GLubyte *)malloc(img.m * img.n * 3 * sizeof(GLubyte));
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
			img.img[i++] = atoi(tok);
			tok = strtok(NULL, seps);
		}
	}
	fclose(fp);
	fprintf(stderr, "...done (read %dx%d image of %d bytes)\n", img.m, img.n, i);
	if (err) {
		fprintf(stderr, "Error reading texture file <%s>!\n", ppmFileName);
		if (img.img)
			free(img.img);
		return false;
	}
	return true;
}

void MazeDemo::SelectTexture(bool leftWall, bool rightWall, float dblAngle, float singleAngle)
{
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	if (leftWall && rightWall) {
		glBindTexture(GL_TEXTURE_2D, xTex);
	} else if (leftWall) {
		glRotatef(singleAngle + 180.0, 0.0, 0.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, arrowTex);	// to the right
	} else if (rightWall) {
		glRotatef(singleAngle, 0.0, 0.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, arrowTex); // to the left
	} else {
		glRotatef(dblAngle, 0.0, 0.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, dblArrowTex);
	}
	glMatrixMode(GL_MODELVIEW);
}
