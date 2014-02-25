///////////////////////////////////////////////////////////////////////////////	
//
//  Polygon3D
//
//  COMP 8051 Game Framework 3D polygon and vertex definitions
//
//  (c) 2010-2013 Borna Noureddin
//
///////////////////////////////////////////////////////////////////////////////	


#include "stdafx.h"
#include <Demos/Polygon3D.h>

void Polygon3D::AddVertex(Vertex v)
{
	vertices.push_back(v);
	mUseColor = true;
}

void Polygon3D::Draw()
{
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < vertices.size(); i++) {
		if (mUseColor)
			glColor3fv(vertices[i].color);
		if (mUseTexture)
			glTexCoord2f(vertices[i].tc[0], vertices[i].tc[1]); 
		glVertex3fv(vertices[i].v);
	}
	glEnd();
}
