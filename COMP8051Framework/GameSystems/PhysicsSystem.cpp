#include "stdafx.h"
#include "Utilities/Debug.h"
#include "PhysicsSystem.h"

// ----------------------------------------------------------------------------
// PhysicsSystem Implementation
// ----------------------------------------------------------------------------
PhysicsSystem::PhysicsSystem(): mGravity(0.0f, -9.83f)
{
	// Init some basic bounds for a unit space
	mExtents.lowerBound.Set(0.0f,0.0f);
	mExtents.upperBound.Set(1.0f,1.0f);
}

PhysicsSystem::~PhysicsSystem()
{
}

// System Configuration
void PhysicsSystem::SetWorldExtents(float minX, float maxX, float minY, float maxY)
{
	ASSERT(minX < maxX && minY < maxY);		// Basic sanity check.

	mExtents.lowerBound.Set(minX,minY);
	mExtents.upperBound.Set(maxX,maxY);
}

void PhysicsSystem::SetGravity(float x, float y)
{
	mGravity.Set(x,y);
}

// System Management
GameSystemResult PhysicsSystem::Init()
{
	// Initialize our physics world
	mWorld = new b2World(mExtents, mGravity, true);
	ASSERT(mWorld);


	return GSR_SUCCESS;
}

GameSystemResult PhysicsSystem::Purge()
{
	if (mWorld)
	{
		delete mWorld;
		mWorld = NULL;
	}
	return GSR_SUCCESS;
}

GameSystemResult PhysicsSystem::Update(float elapsedTime)
{
	if (mWorld)
	{
		const float MAX_TIMESTEP = 1.0f/60.0f;
		const int NUM_ITERATIONS = 10;

		while (elapsedTime >= MAX_TIMESTEP)
		{
			// Take steps
			mWorld->Step(MAX_TIMESTEP, NUM_ITERATIONS);
			elapsedTime -= MAX_TIMESTEP;
		}

		// Remainder
		if (elapsedTime > 0.0f)
		{
			mWorld->Step(elapsedTime, NUM_ITERATIONS);
		}
	}

	return GSR_SUCCESS;
}

