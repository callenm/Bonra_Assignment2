#ifndef MAINLOOP_H
#define MAINLOOP_H

class RenderSystem;
class GameSystem;
class DemoBase;

// ----------------------------------------------------------------------------
// Subsystem ID's
// ----------------------------------------------------------------------------
// Because certain subsystems are initialized at various times in the launch
// process, we accumulate them internally in the main loop by index
enum SystemID
{
	SUBSYSTEM_RENDER,
	SUBSYSTEM_KEYBOARD,
	NUM_SUBSYSTEMS
};


// ----------------------------------------------------------------------------
// MainLoop.h - Game Main Loop Module
// ----------------------------------------------------------------------------
// The MainLoop module is a very crude layer of abstraction, one step removed
// from the underlying OS calls that drive the main loop. We do this to encapsulate
// the logic and data in a form that could be extracted or extended to other
// platforms should the need arise.
//
// MainLoop is a singleton, which means that only one instance of it can be created
// and accessed via the Get() method. This facilitates allowing the OS to create and
// initialize it in the right spot as well as allowing for Universal access to the
// subsystems that it owns.

class MainLoop
{
public:
	
    bool	keys[256];								// Array Used For The Keyboard Routine
	bool	inDrag;
	int		mouseX, mouseY, startMouseX, startMouseY;

	// Singleton accessor
	static MainLoop*	Get();			// Retrieves the master instance of the class

	// System operations
	bool	Initialize();				// Initializes main loop
	bool	Purge();					// Shuts down main loop

	// Subsystem manager
	bool	RegisterAndInitSubsystem(GameSystem* system, SystemID systemID);
	bool	PurgeSubsystem(SystemID systemID);

	// Demo manager
	void	RegisterAndInitDemo(DemoBase* demo);
	void	PurgeDemo();

	bool	Update(float elapsedTime);	// Updates all core subsystems
	bool	Render();					// Calls draw on all core subsystems

	// System access
	inline	RenderSystem*		RenderSys()		{ return reinterpret_cast<RenderSystem*>(mSubsystems[SUBSYSTEM_RENDER]); }		// Access to global render system

private:

	// Constructor/Destructor - You cannot create an instance of this class directly because it is a Singleton
	MainLoop();
	virtual ~MainLoop();

	// Singleton instance
	static MainLoop*			sInstance;

	// Subsystems
	GameSystem*					mSubsystems[NUM_SUBSYSTEMS];

	// Demo
	DemoBase*					mCurrentDemo;
};


#endif