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

namespace otb {

namespace mpi {

/** CreateInitialize MPI environment */
MPIConfig::MPIConfig()
   : m_initialized(false),
     m_abortOnException(true),
     m_MyRank(-1),
     m_NbProcs(0)

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
   OTB_MPI_CHECK_RESULT(MPI_Comm_rank, (MPI_COMM_WORLD , &m_MyRank));
   // Get MPI NbProocs
   OTB_MPI_CHECK_RESULT(MPI_Comm_size, (MPI_COMM_WORLD , &m_NbProcs)); }

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

} // End namespace mpi

} // End namespace otb

