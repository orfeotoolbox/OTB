/*=========================================================================

   Program:   ORFEO Toolbox
   Language:  C++
   Date:      $Date$
   Version:   $Revision$


   Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
   See OTBCopyright.txt for details.


      This software is distributed WITHOUT ANY WARRANTY; without even
      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
      PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/
#include "otbMPIConfig.h"

#include <exception>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#include <mpi.h>

/**
  * Call the MPI routine MPIFunc with arguments Args (surrounded by
  * parentheses). If the result is not MPI_SUCCESS, throw an exception.
  */
#define OTB_MPI_CHECK_RESULT( MPIFunc, Args )                           \
  {                                                                     \
    int _result = MPIFunc Args;                                         \
    if (_result != MPI_SUCCESS)                                         \
      {                                                                 \
      std::stringstream message;                                        \
      message << "otb::mpi::ERROR: " << #MPIFunc << " (Code = " << _result; \
      ::itk::ExceptionObject _e(__FILE__, __LINE__, message.str().c_str()); \
      throw _e;                                                         \
      }                                                                 \
  }



namespace otb {

/** Initialize the singleton */
MPIConfig::Pointer MPIConfig::m_Singleton = NULL;

MPIConfig::Pointer MPIConfig::Instance()
{
  if(m_Singleton.GetPointer() == NULL)
    {
    m_Singleton = itk::ObjectFactory<Self>::Create();

    if(m_Singleton.GetPointer() == NULL)
      {
      m_Singleton = new MPIConfig;
      }
    m_Singleton->UnRegister();
    }

  return m_Singleton;
}


/** CreateInitialize MPI environment */
MPIConfig::MPIConfig()
  :  m_MyRank(-1),
     m_NbProcs(0),
     m_abortOnException(true),
     m_initialized(false)
{
}

/** Shuts down the MPI environment. */
MPIConfig::~MPIConfig()
{
   if (m_initialized)
   {
     if (std::uncaught_exception() && m_abortOnException)
     {
       abort(EXIT_FAILURE);
     }
     else
     {
       int finalized;
       OTB_MPI_CHECK_RESULT(MPI_Finalized,(&finalized));
       if (!finalized)
       {
         OTB_MPI_CHECK_RESULT(MPI_Finalize,());
       }
     }
   }
}

/** Initialize MPI environment */
void MPIConfig::Init(int& argc, char** &argv, bool abortOnException) {
   // Abort on exception
   m_abortOnException = abortOnException;
   // Initialize
   int initialized;
   OTB_MPI_CHECK_RESULT(MPI_Initialized,(&initialized));
   m_initialized = (initialized == 1);
   if (!m_initialized) {
     OTB_MPI_CHECK_RESULT(MPI_Init, (&argc, &argv));
     m_initialized = true;
   }
   // Get MPI rank
   int irank = 0;
   OTB_MPI_CHECK_RESULT(MPI_Comm_rank, (MPI_COMM_WORLD , &irank));

   if(irank<0)
     {
     logError("Negative MPI rank");
     abort(EXIT_FAILURE);
     }
   
   m_MyRank = static_cast<unsigned int>(irank);
   
   // Get MPI NbProocs

   int inbprocs=0;
   
   OTB_MPI_CHECK_RESULT(MPI_Comm_size, (MPI_COMM_WORLD , &inbprocs));

   if(inbprocs<1)
     {
     logError("Negative or null number of processes");
     abort(EXIT_FAILURE);
     }

   m_NbProcs = static_cast<unsigned int>(inbprocs);
}

void MPIConfig::abort(int errCode)
{
   OTB_MPI_CHECK_RESULT(MPI_Abort, (MPI_COMM_WORLD, errCode)); }

void MPIConfig::logError(const std::string message) {
   if (m_MyRank == 0)
   {
     std::cerr << message << std::endl;
   }
}

void MPIConfig::logInfo(const std::string message) {
   if (m_MyRank == 0)
   {
     std::cout << message << std::endl;
   }
}

} // End namespace otb

