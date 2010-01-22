//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimMpi.h 13602 2008-09-25 14:23:32Z gpotts $
#ifndef ossimMpi_HEADER
#define ossimMpi_HEADER

#include <ossim/ossimConfig.h>
#include <ossim/base/ossimConstants.h>

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

   /**
    * @brief MPI Barrier method.
    * Blocks until all processes have reached this routine.
    *
    * If MPI is enabled this will call:  MPI_Barrier( MPI_COMM_WORLD );
    * else it will simply return.
    */
   void barrier();
   
protected:
   ossimMpi(); // hide 
   ossimMpi(const ossimMpi&); // hide
   void operator =(const ossimMpi&); // hide

   /*!
    * We will have only a single instance of
    * this class.
    */
   static ossimMpi* theInstance;
   bool theEnabledFlag;
};

#endif
