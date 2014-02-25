// ----------------------------------------------------------------------------
// GameSystem.h - GameSystem Base Class
// ----------------------------------------------------------------------------
// GameSystem is the base class for all of the subsystems we add to the
// game engine, and describes a basic interface to all of our systems. 
//
// It is very simple and is basically here to define a "contract" with
// our game systems and game system manager
//


#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------
// This enumeration is a bit redundant with a boolean, however, in the future, it enables
// us to add additional states and conditions without a major re-write
enum GameSystemResult
{
	GSR_FAILURE,
	GSR_SUCCESS
};

inline bool Succeeded(GameSystemResult gsr)	{ return gsr >= GSR_SUCCESS; }
inline bool Failed(GameSystemResult gsr) { return gsr < GSR_SUCCESS; }

// ----------------------------------------------------------------------------
// GameSystem Class
// ----------------------------------------------------------------------------
// This class forms the contract for interfacing with all of the game systems in the game

class GameSystem
{
public:
	
	// System Management
	virtual GameSystemResult	Init() = 0;						// Initializes the subsystem and prepares it for use
	virtual GameSystemResult	Purge() = 0;					// Terminates the system and cleans up
	
	// Update message - called in the pre-render pass
	virtual GameSystemResult	Update(float elapsedTime) = 0;						// Update function

	// Drawing - called to display the results of this system in the view (if needed)
	virtual GameSystemResult	Render()	{ return GSR_SUCCESS; }					// Render is an optional method
	
protected:

	// Constructor/destructor - not meant to be called directly
	GameSystem()	{};
	virtual ~GameSystem()	{};

};


#endif
