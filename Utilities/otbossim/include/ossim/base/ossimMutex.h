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
// $Id: ossimMutex.h 10694 2007-04-12 13:53:34Z gpotts $
#ifndef ossimMutex_HEADER
#define ossimMutex_HEADER

#include <ossim/ossimConfig.h>  /* to pick up platform defines */
#include <ossim/base/ossimReferenced.h>


/**
 * ossimMutex is a wrapper around OpenThreads::Mutex to allow for builds with
 * or without the package.
 */
class ossimMutex : public ossimReferenced
{
public:

   /**
    *  Constructor
    */
   ossimMutex();

   /**
    *  Destructor
    */
   ~ossimMutex();
   
    /**
     *  Lock the mutex
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     *
     *  @note If #ifdef OSSIM_HAS_OPEN_THREADS is not set, always returns 0.
     */
   int lock();

    /**
     *  Unlock the mutex
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     *
     *  @note If #ifdef OSSIM_HAS_OPEN_THREADS is not set, always returns 0.
     */
   int unlock();

    /**
     *  Test if mutex can be locked.
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     *
     *  @note If #ifdef OSSIM_HAS_OPEN_THREADS is not set, always returns 0.
     */
   int trylock();

private:
     void* thePrivateData;   
};
#endif /* ossimMutex_HEADER */
