//*************************************************************************
// Copyright (C) 2004 Intelligence Data Sytems, Inc.  All rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
//**************************************************************************
// $Id: ossimMutex.cpp,v 1.3 2005/11/07 22:06:07 dburken Exp $

#include <base/common/ossimMutex.h>

ossimMutex::ossimMutex()
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
   : theMutex()
#  endif
#endif
{}

ossimMutex::~ossimMutex()
{}

int ossimMutex::lock()
{
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
   return theMutex.lock();
#  endif
#endif
   return 0;
}

int ossimMutex::unlock()
{
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
   return theMutex.unlock();
#  endif
#endif
   return 0;
}

int ossimMutex::trylock()
{
#ifdef OSSIM_HAS_OPEN_THREADS
#  if OSSIM_HAS_OPEN_THREADS
   return theMutex.trylock();
#  endif
#endif
   return 0;
}
