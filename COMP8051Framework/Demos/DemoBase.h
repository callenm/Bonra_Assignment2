// ----------------------------------------------------------------------------
// DemoBase.h - Demo Interface
// ----------------------------------------------------------------------------
// This class defines an interface to a typical demo - they can be installed
// in the MainLoop to be executed using MainLoop::RegisterAndInitDemo()



#ifndef DEMOBASE_H
#define DEMOBASE_H

class DemoBase
{
public:

	// System Interface
	virtual bool Initialize() = 0;
	virtual void Purge() = 0;
	virtual bool Update(float elapsedTime) = 0;
	virtual bool Render() = 0;
};

#endif