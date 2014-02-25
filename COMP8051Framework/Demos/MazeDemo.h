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



#ifndef MAZEDEMO_H
#define MAZEDEMO_H
#include "Demos/DemoBase.h"
#include "Demos/Object3D.h"
#include "maze.h"
#include <gl\gl.h>
#include <gl\glu.h>

using namespace std;

typedef struct {
	GLubyte *img;
	int m, n;
} PPMImage;

class MazeDemo : public DemoBase
{
public:

	// Constructor/Destructor
	MazeDemo()	{ theMaze = NULL; };
	virtual ~MazeDemo()	{};

	// System Interface
	bool Initialize();
	void Purge();
	bool Update(float elapsedTime);
	bool Render();

	int GetMazeWidth() { return numCols; };
	int GetMazeHeight() { return numRows; };

private:

	// Maze members and methods
	Maze *theMaze;
	int numRows, numCols;	// maze size
	float currz, currx, angle, vertViewOffset, curry;
	bool walkThroughWalls;
	Direction curDir;
	void CreateMaze();
	void DrawMaze();
	void DrawWall();
	void DrawCell(MazeCell &cell);

	// Texturing members and methods
	PPMImage xImg, arrowImg, dblArrowImg;
	GLuint xTex, arrowTex, dblArrowTex;
	void InitTextures();
	bool ReadPPMImage(char *ppmFileName, PPMImage &img);
	void SelectTexture(bool leftWall, bool rightWall, float dblAngle, float singleAngle);

};


#endif
