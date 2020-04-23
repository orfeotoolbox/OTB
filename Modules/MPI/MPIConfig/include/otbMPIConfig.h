/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbMPIConfig_h
#define otbMPIConfig_h

#include "itkObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"

namespace otb
{

/** \class MPI config
  *  \brief Manage MPI resources
  * TODO
  *
  *
  * \ingroup OTBMPIConfig
  */
class MPIConfig : public itk::LightObject
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
  itkGetMacro(NbProcs, unsigned int);

  /** Initialize MPI Processus */
  void Init(int& argc, char**& argv, bool abortOnException = true);

  /** Shuts down the MPI environment. */
  void terminate();

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
  MPIConfig(const MPIConfig&) = delete;
  void operator=(const MPIConfig&) = delete;

  // MPI rank
  unsigned int m_MyRank;
  // Number of MPI processus
  unsigned int m_NbProcs;
  // Boolean to abort on exception
  bool m_abortOnException;
  // Boolean to test if the MPI environment is initialized
  bool m_initialized;
  // Boolean to test if the MPI environment is terminated
  bool m_terminated;

  static Pointer m_Singleton;
};

} // End namespace otb

#endif //__otbMPIConfig_h
