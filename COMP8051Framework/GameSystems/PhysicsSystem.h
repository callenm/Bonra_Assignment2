// ----------------------------------------------------------------------------
// PhysicsSystem.h - Physics Engine Controls
// ----------------------------------------------------------------------------
// PhysicsSystem is a wrapper for the Box2D Physics Engine that allows us to
// integrate it with our game systems and manage it as part of the GL Framework

#include "GameSystem.h"
#include "Box2D.h"

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

// ----------------------------------------------------------------------------
// PhysicsSystem Class
// ----------------------------------------------------------------------------
// This class forms the contract for interfacing with all of the game systems in the game

class PhysicsSystem : public GameSystem
{
public:

	PhysicsSystem();
	virtual ~PhysicsSystem();

	// System Configuration
	void SetWorldExtents(float minX, float maxX, float minY, float maxY);		// Objects that move outside the world extents are suspended
	void SetGravity(float x, float y);											// Gravity can be applied in an arbitrary direction

	// Access
	inline b2World*	World()		{ return mWorld; }

	// System Management
	virtual GameSystemResult	Init();
	virtual GameSystemResult	Purge();

	// Update message - called in the pre-render pass
	virtual GameSystemResult	Update(float elapsedTime);

private:

	b2AABB	mExtents;			// Axis aligned bounding box - world extents
	b2Vec2	mGravity;			// Gravity vector

	b2World*	mWorld;			// Main physics world

};


#endif
