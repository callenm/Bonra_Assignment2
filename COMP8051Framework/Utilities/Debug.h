// ----------------------------------------------------------------------------
// Debug.h - Handy Debugging Macros and Functions
// ----------------------------------------------------------------------------
// Often, this type of stuff is very platform/compiler dependent, so we put it all
// in one place to make it easy to create different versions as needed.

#include <stdio.h>

// At the top of every file define a new MACRO
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// deine your own ASSERT in a header file as follows
#ifdef _DEBUG
#define ASSERT(f) \
do \
{ \
	if (!(f) && printf("ASSERTION FAILED: %s %s",THIS_FILE,__LINE__)) \
CustomDebugBreak(); \
} while (0)


#define ASSERT_MSG(f, msg, ...) \
do \
{ \
	if (!(f) && printf("ASSERTION FAILED: %s %s",THIS_FILE,__LINE__) && printf(msg, __VA_ARGS__)) \
CustomDebugBreak(); \
} while (0)

#define WARNING		printf

#define FAIL_MSG(msg, ...) \
	printf("CRITICAL FAIL:"); printf(msg, __VA_ARGS__); \
	CustomDebugBreak();


#else
#define ASSERT(f)
#define ASSERT_MSG(f, msg, ...)
#define WARNING
#define FAIL_MSG 
#endif

// Lots of ways to define AssertFailedLine and CustomDebugBreak
// just watch out for asserts inside asserts

// _M_IX86Defined for x86 processors.
// DebugBreak is a Win32 function
#if defined(_M_IX86)
#define CustomDebugBreak() _asm { int 3 }
#else
#define CustomDebugBreak() DebugBreak()
#endif
