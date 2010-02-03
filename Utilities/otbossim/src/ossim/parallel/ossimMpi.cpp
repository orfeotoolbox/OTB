//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimMpi.cpp 13602 2008-09-25 14:23:32Z gpotts $

#include <ossim/parallel/ossimMpi.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <time.h>
#if OSSIM_HAS_MPI
#  include <mpi.h>
#endif

#if OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimMpi.cpp 13602 2008-09-25 14:23:32Z gpotts $";
#endif

static ossimTrace traceDebug = ossimTrace("ossimMpi:debug");

ossimMpi* ossimMpi::theInstance=0;

ossimMpi* ossimMpi::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimMpi;
   }
   
   return theInstance;
}

bool ossimMpi::isEnabled()const
{
   return theEnabledFlag;
}

void ossimMpi::barrier()
{
#if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      // Blocks until all processes have reached this routine.
      MPI_Barrier( MPI_COMM_WORLD );
   }
#endif
}

ossim_float64 ossimMpi::getTime()const
{
#if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      return MPI_Wtime();
   }
   else
   {
      return time(0);
   }
#else
   return time(0);
#endif
}

void ossimMpi::initialize(int* argc, char*** argv)
{
#if OSSIM_HAS_MPI
   int success=0;
   success = MPI_Init(argc, argv);
   if(success == MPI_SUCCESS)
   {
      theEnabledFlag = true;
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimMpi::initialize\n"
            << "MPI is initialized and running with "
            << getNumberOfProcessors()
            << " processors..."
            << std::endl;
      }
   }
   else
   {
      theEnabledFlag = false;
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimMpi::initialize:  MPI is not initialized."
            << std::endl;
      }
   }
#else
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimMpi::initialize: Not compiled with mpi!"
         << std::endl;
   }
#endif
}
void ossimMpi::setEnabledFlag(bool flag)
{
   theEnabledFlag = flag;
}

void ossimMpi::finalize()
{
#if OSSIM_HAS_MPI
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
#endif
}

int ossimMpi::getRank()const
{
   int result = 0;

#if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      MPI_Comm_rank(MPI_COMM_WORLD, &result);
   }
#endif
   
   return result;
}

int ossimMpi::getNumberOfProcessors()const
{
   int result = 1;

#if OSSIM_HAS_MPI
   if(theEnabledFlag)
   {
      MPI_Comm_size(MPI_COMM_WORLD, &result);
   }
#endif

   return result;
}

ossimMpi::ossimMpi()
   : theEnabledFlag(false)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimMpi::ossimMpi entered..."
         << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID = " << OSSIM_ID << std::endl;
#endif
   }
}

ossimMpi::ossimMpi(const ossimMpi& obj)
   : theEnabledFlag(obj.theEnabledFlag)
{}

void ossimMpi::operator=(const ossimMpi& rhs)
{
   theEnabledFlag = rhs.theEnabledFlag; 
}
