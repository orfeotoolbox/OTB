//*************************************************************************
// Copyright (C) 2004 Intelligence Data Sytems, Inc.  All rights reserved.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// NOTES: This was taken from OpenThreads::ScopedLock
//       
//
//************************************************************************** 
// $Id: ossimScopedLock.h 7793 2005-07-15 11:05:10Z gpotts $
#ifndef ossimScopedLock_HEADER
#define ossimScopedLock_HEADER

template <class T> class ossimScopedLock
{
    private:
        T& theLock;
        ossimScopedLock(const ossimScopedLock&); // prevent copy
        ossimScopedLock& operator=(const ossimScopedLock&); // prevent assign
    public:
        explicit ossimScopedLock(T& m):theLock(m) {theLock.lock();}
        ~ossimScopedLock(){theLock.unlock();}
};

template <class T> class ossimReverseScopedLock
{
    private:
        T& theLock;
        ossimReverseScopedLock(const ossimReverseScopedLock&); // prevent copy
        ossimReverseScopedLock& operator=(const ossimReverseScopedLock&); // prevent assign
    public:
        explicit ossimReverseScopedLock(T& m):theLock(m) {theLock.unlock();}
        ~ossimReverseScopedLock(){theLock.lock();}
};

#endif
