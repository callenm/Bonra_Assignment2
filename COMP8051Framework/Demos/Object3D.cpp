///////////////////////////////////////////////////////////////////////////////	
//
//  Object3D
//
//  COMP 8051 Game Framework generic 3D object class
//
//  (c) 2010-2013 Borna Noureddin
//
///////////////////////////////////////////////////////////////////////////////	


#include "stdafx.h"
#include "Demos/Object3D.h"


void Object3D::AddFace(Polygon3D p)
{
	p.SetTextureFlag(mUseTexture);
	p.SetColorFlag(mUseColor);
	faces.push_back(p);
}

void Object3D::Draw()
{
	glTranslatef(translation[x_axis], translation[y_axis], translation[z_axis]);
	glRotatef(theta[x_axis], 1.0, 0.0, 0.0);
	glRotatef(theta[y_axis], 0.0, 1.0, 0.0);
	glRotatef(theta[z_axis], 0.0, 0.0, 1.0);
	glScalef(1.0f + zoomFactor, 1.0f + zoomFactor, 1.0f + zoomFactor);
	for (unsigned int i = 0; i < faces.size(); i++) {
		faces[i].SetColorFlag(mUseColor);
		faces[i].SetTextureFlag(mUseTexture);
		faces[i].Draw();
	}
}

void Object3D::Rotate(int axis, int previousCoord, int currentCoord)
{
	theta[axis] = (float)(currentCoord - previousCoord);
	if (theta[axis] > 360.0)
		theta[axis] -= 360.0;
	if (theta[axis] < 0.0)
		theta[axis] += 360.0;
}

void Object3D::Translate(int axis, GLfloat amount)
{
	translation[axis] += amount;
}

void Object3D::Zoom(GLfloat amount)
{
	zoomFactor += amount;
}

void Object3D::Reset()
{
	theta[x_axis] = 5;
	theta[y_axis] = 15;
	theta[z_axis] = 0;

	memset(translation, 0, sizeof(translation) * sizeof(GLfloat) / sizeof(translation[0]));

	zoomFactor = 0.0;
}


GLfloat Object3D::GetRotation(int axis)
{
	return theta[axis];
}
