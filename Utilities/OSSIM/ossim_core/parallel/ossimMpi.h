//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimMpi.h,v 1.10 2005/11/07 22:03:24 dburken Exp $
#ifndef ossimMpi_HEADER
#define ossimMpi_HEADER

#include <ossimConfig.h>
#include <base/common/ossimConstants.h>

#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
#    include <mpi.h>
#  endif
#endif

class OSSIM_DLL ossimMpi
{
public:
   static ossimMpi* instance();

   ossim_float64 getTime()const;
   /*!
    * Typically called only one time
    */
   void initialize(int* argc, char*** argv);

   /*!
    * Called only once when we would like to
    * shutdown mpi
    */
   void finalize();
   int getRank()const;
   int getNumberOfProcessors()const;
   void setEnabledFlag(bool flag);
   bool isEnabled()const;
protected:
   ossimMpi(){} // hide 
   ossimMpi(const ossimMpi&){} // hide
   void operator =(const ossimMpi&){} // hide

   /*!
    * We will have only a single instance of
    * this class.
    */
   static ossimMpi* theInstance;
   bool theEnabledFlag;
};

#endif
