///////////////////////////////////////////////////////////////////////////////	
//
//  DemoBase
//
//  COMP 8051 Game Framework Demo Interface
//  This class defines an interface to a typical demo - they can be installed
//  in the MainLoop to be executed using MainLoop::RegisterAndInitDemo()
//
//  (c) 2010-2013 Borna Noureddin
//
///////////////////////////////////////////////////////////////////////////////	


#ifndef DEMOBASE_H
#define DEMOBASE_H

class DemoBase
{
public:

	// System Interface
	bool Initialize() = 0;
	void Purge() = 0;
	bool Update(float elapsedTime) = 0;
	bool Render() = 0;
};

#endif