//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimMpi.cpp,v 1.20 2005/11/07 22:03:24 dburken Exp $

#include <parallel/ossimMpi.h>
#include <base/common/ossimTrace.h>
#include <base/context/ossimNotifyContext.h>
#include <time.h>

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimMpi.cpp,v 1.20 2005/11/07 22:03:24 dburken Exp $";
#endif

static ossimTrace traceDebug = ossimTrace("ossimMpi:debug");

ossimMpi* ossimMpi::theInstance=0;

ossimMpi* ossimMpi::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimMpi;
      theInstance->setEnabledFlag(true);
   }
   
   return theInstance;
}

bool ossimMpi::isEnabled()const
{
   return theEnabledFlag;
}

ossim_float64 ossimMpi::getTime()const
{
#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      return MPI_Wtime();
   }
   else
   {
      return time(0);
   }
#  else
   return time(0);
#  endif
#else
   return time(0);
#endif
}

void ossimMpi::initialize(int* argc, char*** argv)
{
   if(theEnabledFlag)
   {
#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
      int success=0;
      success = MPI_Init(argc, argv);
      if(traceDebug())
      {
#    ifdef OSSIM_ID_ENABLED
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimMpi::initialize: OSSIM_ID = "
            << OSSIM_ID << std::endl;
#    endif
         if(success == MPI_SUCCESS)
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimMpi::initialize\n"
               << "MPI is initialized and running with "
               << getNumberOfProcessors()
               << " processors..."
               << std::endl;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimMpi::initialize:  MPI is not initialized."
               << std::endl;
         }
      }
#  else
      if(traceDebug())
      {
#    ifdef OSSIM_ID_ENABLED
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimMpi::initialize: OSSIM_ID = "
            << OSSIM_ID << std::endl;
#    endif
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimMpi::initialize: Not compiled with mpi!"
            << std::endl;
      }
#  endif
#else
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimMpi::initialize: Not compiled with mpi!"
         << std::endl;   
#endif
   }
}
void ossimMpi::setEnabledFlag(bool flag)
{
   theEnabledFlag = flag;
}

void ossimMpi::finalize()
{
#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      int success;
      success = MPI_Finalize();
      if(traceDebug())
      {
         if(success == MPI_SUCCESS)
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimMpi::finalize(): mpi is finalized" << std::endl;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimMpi::finalize(): "
               << "mpi is not finalized successfully" << std::endl;
         }
      }
   }
#  endif
#endif
}

int ossimMpi::getRank()const
{
   int result = 0;

#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      MPI_Comm_rank(MPI_COMM_WORLD, &result);
   }
#  endif
#endif
   
   return result;
}

int ossimMpi::getNumberOfProcessors()const
{
   int result = 1;

#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      MPI_Comm_size(MPI_COMM_WORLD, &result);
   }
#  endif
#endif

   return result;
}
