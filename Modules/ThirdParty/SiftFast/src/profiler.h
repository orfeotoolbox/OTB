// exact C++ implementation of lowe's sift program
// author: zerofrog(@gmail.com), Sep 2008
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//Lesser GNU General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

// This source code was carefully calibrated to match David Lowe's SIFT features program
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
