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
#ifndef __otbMPIConfig_h
#define __otbMPIConfig_h

#include "itkObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"

namespace otb {

/** \class MPI config
  *  \brief Manage MPI ressources
  * TODO
  *
  *
  * \ingroup OTBMPIConfig
  */
class MPIConfig: public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef MPIConfig                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Retrieve the singleton instance */
  static Pointer Instance();
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(MPIConfig, itk::LightObject);

  /** MPI Parameters accessors */
  itkGetMacro(MyRank, unsigned int);
  itkGetMacro(NbProcs,unsigned int);

  /** Initialize MPI Processus */
  void Init(int& argc, char** &argv, bool abortOnException = true);

  /** Abort all MPI processus. */
  void abort(int errCode);

  /** Blocks until all processes have reached this routine */
  void barrier();

  /** Log error */
  void logError(const std::string message);

  /** Log info */
  void logInfo(const std::string message);

protected:
  /** Constructor */
  MPIConfig();

  /** Destructor */
  virtual ~MPIConfig();

private:

  MPIConfig(const MPIConfig &); //purposely not implemented
  void operator =(const MPIConfig&); //purposely not implemented

  // MPI rank
  unsigned int m_MyRank;
  // Number of MPI processus
  unsigned int m_NbProcs;
  // Boolean to abort on exception
  bool m_abortOnException;
  // Boolean to test if the MPI environment is initialized
  bool m_initialized;

  static Pointer m_Singleton;
};

} // End namespace otb

#endif //__otbMPIConfig_h
