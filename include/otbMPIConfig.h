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
#ifndef __MPIConfig_h
#define __MPIConfig_h

#include <mpi.h>

namespace otb 
{ 
namespace mpi 
{

/** \class MPIConfig
 *
 * Manage MPI ressources
 *
 */
class MPIConfig
{
public:
  /** 
   * Initialize the MPI environment.
   *
   *  If the MPI environment has not already been initialized,
   *  initializes MPI with a call to MPI_Init.
   *
   *  @param argc Number of arguments 
   *
   *  @param argv Array of arguments
   *
   *  @param abort_on_exception When true, this object will abort the
   *  program if it is destructed due to an uncaught exception.
   */
  MPIConfig(int& argc, char** &argv, bool abortOnException = true);

  /** 
   * Shuts down the MPI environment.
   */
  ~MPIConfig();

  /** 
   * Abort all MPI processes.
   *
   *  @param errcode 
   */
  void abort(int errcode);

  /** 
   * Determine if the MPI environment has already been initialized.
   *
   *  @returns @c true if the MPI environment has been initialized.
   */
  bool initialized();

  /** 
   * Determine if the MPI environment has already been finalized.
   *
   *  @returns @c true if the MPI environment has been finalized.
   */
  bool finalized();

private:
  /// Whether this environment object called MPI_Init
  bool m_initialized;

  /// Whether we should abort if the destructor is
  bool m_abortOnException;

};

} // end namespace mpi
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMPIConfig.txx"
#endif

#endif // MPIConfig
