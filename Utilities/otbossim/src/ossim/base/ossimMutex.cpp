//*************************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
//**************************************************************************
// $Id: ossimMutex.cpp 10888 2007-05-11 14:10:15Z dburken $

#include <ossim/base/ossimMutex.h>
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
#    include <OpenThreads/Mutex>
static OpenThreads::Mutex* toOssimMutex(void* ptr)
{
	return (OpenThreads::Mutex*)ptr;
}
#  endif
#endif

ossimMutex::ossimMutex()
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
   : thePrivateData(new OpenThreads::Mutex())
#  else
   : thePrivateData(0)   
#  endif
#else
   : thePrivateData(0)   
#endif
{}

ossimMutex::~ossimMutex()
{}

int ossimMutex::lock()
{
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
	return toOssimMutex(thePrivateData)->lock();
#  endif
#endif
   return 0;
}

int ossimMutex::unlock()
{
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
   return  toOssimMutex(thePrivateData)->unlock();
#  endif
#endif
   return 0;
}

int ossimMutex::trylock()
{
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
   return  toOssimMutex(thePrivateData)->trylock();
#  endif
#endif
   return 0;
}
