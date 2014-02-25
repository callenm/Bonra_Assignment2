////////////////////////////////////////////////////////////////////////////
//
// DemoBase - game framework launcher for demos
//
// (c) 2010-2013 Borna Noureddin
//      British Columbia Institute of Technology
//      School of Computing and Academic Studies
//      Bachelor of Technology Program
//      Games Development Option
//
////////////////////////////////////////////////////////////////////////////



#ifndef DEMO_STUB_H
#define DEMO_STUB_H

#include "MazeDemo.h"

inline DemoBase* CreateDemoInstance()	{ return new MazeDemo; }

#endif
