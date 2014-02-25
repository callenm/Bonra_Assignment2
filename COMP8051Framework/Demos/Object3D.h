///////////////////////////////////////////////////////////////////////////////	
//
//  Object3D
//
//  COMP 8051 Game Framework generic 3D object class
//
//  (c) 2010-2013 Borna Noureddin
//
///////////////////////////////////////////////////////////////////////////////	


#include "Demos/Polygon3D.h"

class Object3D
{
public :

	enum Axis {x_axis, y_axis, z_axis};

	// Initializes the orientation of the object
	Object3D() { Reset(); }

	// Add a face to the object
	void AddFace(Polygon3D p);

	// Draw the object
	void Draw();

	// Rotate the object by a given amount
	void Rotate(int axis, int previousCoord, int currentCoord);
	GLfloat GetRotation(int axis);

	// Translate the object over the given Axis
	void Translate(int axis, GLfloat amount);

	// Zoom in or out for the view of the object
	void Zoom(GLfloat amount);

	// Reset the shape to the default orientation
	void Reset();

	// Set/get flag for using color/texture
	void SetColorFlag(bool status = true) { mUseColor = status; };
	bool GetColorFlag() { return mUseColor; };
	void SetTextureFlag(bool status = true) { mUseTexture = status; };
	bool GetTextureFlag() { return mUseTexture; };


private:

	// The set of faces that makes up the object
	std::vector<Polygon3D> faces;

	// Current rotation angles for each direction
	GLfloat theta[3];

	// Current translation in each direction
	GLfloat translation[3];

	// Current zoom amount
	GLfloat zoomFactor;

	bool mUseColor, mUseTexture;

};
