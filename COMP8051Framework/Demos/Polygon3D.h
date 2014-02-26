///////////////////////////////////////////////////////////////////////////////	
//
//  Polygon3D
//
//  COMP 8051 Game Framework 3D polygon and vertex definitions
//
//  (c) 2010-2013 Borna Noureddin
//
///////////////////////////////////////////////////////////////////////////////	

//#include <include\GL\glew.h>
#include <GL/GL.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

typedef struct
{
	
	GLfloat v[3], tc[2];
	GLfloat color[3];
} Vertex;


// A 2 dimensional polygon drawn in three dimensions
class Polygon3D
{
public:

	// Add a vertex to the polygon (assumes vertices are all on same plane).
	void AddVertex(Vertex v);
	
	// Draw the polygon on the screen.
	void Draw();

	// Set/get flag for using color/texture
	void SetColorFlag(bool status = true) { mUseColor = status; };
	bool GetColorFlag() { return mUseColor; };
	void SetTextureFlag(bool status = true) { mUseTexture = status; };
	bool GetTextureFlag() { return mUseTexture; };

private:

	std::vector<Vertex> vertices;
	bool mUseColor, mUseTexture;

};
