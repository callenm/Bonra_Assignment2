#include "stdafx.h"
#include <iostream>
#include "GameSystems/RenderSystem.h"
#include "Utilities/debug.h"
#include "MainLoop.h"
#include "Demos/DemoBase.h"

// ----------------------------------------------------------------------------
// MainLoop - Game Main Loop Module
// ----------------------------------------------------------------------------


// Singleton accessor
MainLoop* MainLoop::sInstance = NULL;

MainLoop* MainLoop::Get()
{
	// Do we already have an instance?
	if (sInstance == NULL)
	{
		sInstance = new MainLoop();
		ASSERT(sInstance != NULL);
	}
	return sInstance;
}

// Constructor/Destructor
MainLoop::MainLoop() : mCurrentDemo(NULL)
{
	// Clean out storage for subsystems
	memset(mSubsystems, 0, sizeof(GameSystem*)*NUM_SUBSYSTEMS);
    memset(keys, 0, sizeof(bool)*256);
	inDrag = false;
}

MainLoop::~MainLoop()
{
}

// Init/Purge
bool MainLoop::Initialize()
{
	return true;
}


bool MainLoop::Purge()
{
	bool result = true;

	// Clean up subsystems that are still active
	for (int ss = 0; ss < NUM_SUBSYSTEMS; ++ss)
	{
		if (mSubsystems[ss])
		{
			// Make sure we got a clean shutdown
			if (PurgeSubsystem(static_cast<SystemID>(ss)) == false)
			{
				WARNING("Subsystem %d failed to shut down properly.");
				result = false;
			}
		}
	}

	return result;
}


bool MainLoop::Update(float elapsedTime)
{
	// Update all registered systems
	bool result = true;
	for (int ss = 0; ss < NUM_SUBSYSTEMS; ++ss)
	{
		if (mSubsystems[ss])
		{
			if (mSubsystems[ss]->Update(elapsedTime) == GSR_FAILURE)
			{
				WARNING("Update failure in subsystem %d", ss);
				result = false;
			}
		}
	}

	// Update current demo
	if (mCurrentDemo)
	{
		mCurrentDemo->Update(elapsedTime);
	}
	return result;
}



bool MainLoop::Render()
{

	if (RenderSys())
	{
		RenderSys()->BeginFrame();

		// Render all registered subsystems
		bool result = true;
		for (int ss = 0; ss < NUM_SUBSYSTEMS; ++ss)
		{
			if (mSubsystems[ss])
			{
				if (mSubsystems[ss]->Render() == GSR_FAILURE)
				{
					WARNING("Render failure in subsystem %d", ss);
					result = false;
				}
			}
		}

		// Render current demo
		if (mCurrentDemo)
		{
			mCurrentDemo->Render();
		}

		RenderSys()->EndFrame();

		return result;
	}
	else return false;
}


// Subsystem manager
bool MainLoop::RegisterAndInitSubsystem(GameSystem* system, SystemID systemID)
{
	ASSERT_MSG(systemID < NUM_SUBSYSTEMS, "Invalid subsystem ID");
	ASSERT_MSG(system != NULL, "Invalid system pointer.");
	ASSERT_MSG(mSubsystems[systemID] == NULL, "System is already initialized.");

	// Init and stash a pointer to the system
	if (system->Init() == GSR_FAILURE)
	{
		FAIL_MSG("Failed to initialize subsystem %d", systemID);
		return false;
	}
	else
	{
		mSubsystems[systemID] = system;
		return true;
	}
}

bool MainLoop::PurgeSubsystem(SystemID systemID)
{
	ASSERT_MSG(systemID < NUM_SUBSYSTEMS, "Invalid subsystem ID");
	ASSERT_MSG(mSubsystems[systemID] != NULL, "System is not initialized.");

	// Clean up
	if (mSubsystems[systemID]->Purge() == GSR_FAILURE)
	{
		FAIL_MSG("Failed to purge subsystem %d", systemID);
		return false;
	}
	else
	{
		mSubsystems[systemID] = NULL;
		return true;
	}	
}


// Demo manager
void MainLoop::RegisterAndInitDemo(DemoBase* demo)
{
	ASSERT(demo);
	ASSERT_MSG(!mCurrentDemo, "There is already a registered demo.");

	if (!demo->Initialize())
	{
		FAIL_MSG("Could not initialize demo.");
	}
	mCurrentDemo = demo;
}

void MainLoop::PurgeDemo()
{
	if (mCurrentDemo)
	{
		mCurrentDemo->Purge();
	}
}

