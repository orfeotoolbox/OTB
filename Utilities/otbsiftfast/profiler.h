#ifndef SMALL_PROFILER_H
#define SMALL_PROFILER_H

typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed long long s64;

#include <assert.h>

////
// profiling
///

extern int g_bWriteProfile; // global variable to enable/disable profiling (if DVPROFILE is defined)

// IMPORTANT: For every Reigster there must be an End
void DVProfRegister(const char* pname);			// first checks if this profiler exists in g_listProfilers
void DVProfEnd(u32 dwUserData);
void DVProfWrite(const char* pfilename, u32 frames = 1);
void DVProfClear();						// clears all the profilers

#if defined(DVPROFILE)

#ifdef _MSC_VER

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#endif

#define DVSTARTPROFILE() DVProfileFunc _pf(__FUNCTION__);

class DVProfileFunc
{
public:
	u32 dwUserData;
	DVProfileFunc(const char* pname) { DVProfRegister(pname); dwUserData = 0; }
	DVProfileFunc(const char* pname, u32 dwUserData) : dwUserData(dwUserData) { DVProfRegister(pname); }
	~DVProfileFunc() { DVProfEnd(dwUserData); }
};

#else

#define DVSTARTPROFILE()

class DVProfileFunc
{
public:
	u32 dwUserData;
    inline DVProfileFunc(const char* pname) {}
    inline DVProfileFunc(const char* pname, u32 dwUserData) { }
	~DVProfileFunc() {}
};

#endif

#endif
