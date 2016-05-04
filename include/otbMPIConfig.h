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

#include <sstream>
#include <string>

#include <mpi.h>

#include "itkObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"

namespace otb {

namespace mpi {

/** \class MPI config
  *  \brief Manage MPI ressources
  * TODO
  *
  *
  * \ingroup MPIModule
  */
class MPIConfig: public itk::Object
{
public:
   /** Standard class typedefs. */
   typedef MPIConfig                     Self;
   typedef itk::Object                   Superclass;
   typedef itk::SmartPointer<Self>       Pointer;
   typedef itk::SmartPointer<const Self> ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Run-time type information (and related methods). */
   itkTypeMacro(MPIConfig, itk::Object);

   /** MPI Parameters accessors */
   itkGetMacro(MyRank, int);
   itkGetMacro(NbProcs, int);

   /** Initialize MPI Processus */
   void Init(int& argc, char** &argv, bool abortOnException = true);

   /** Abort all MPI processus. */
   void abort(int errCode);

   /** Log error */
   void logError(const std::string message);

   /** Log info */
   void logInfo(const std::string message);

   //** MPI Update */
   template <typename TImage> void UpdateMPI (TImage *img, const std::string &prefix, bool useStreaming, bool writeVRTFile);

protected:
   /** Constructor */
   MPIConfig();

   /** Destructor */
   virtual ~MPIConfig();

private:

   MPIConfig(const MPIConfig &); //purposely not implemented
   void operator =(const MPIConfig&); //purposely not implemented

   // MPI rank
   int m_MyRank;
   // Number of MPI processus
   int m_NbProcs;
   // Boolean to abort on exception
   bool m_abortOnException;
   // Boolean to test if the MPI environment is initialized
   bool m_initialized;

};

/**
  * Call the MPI routine MPIFunc with arguments Args (surrounded by
  * parentheses). If the result is not MPI_SUCCESS, throw an exception.
  */
#define OTB_MPI_CHECK_RESULT( MPIFunc, Args )                         \
  { \
    int _result = MPIFunc Args;                                  \
    if (_result != MPI_SUCCESS)                                  \
    {                                                            \
      std::stringstream message; \
      message << "otb::mpi::ERROR: " << #MPIFunc << " (Code = " << _result; \
      ::itk::ExceptionObject _e(__FILE__, __LINE__, message.str().c_str()); \
      throw _e; \
    }\
  }

} // End namespace mpi

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMPIConfig.txx"
#endif

#endif //__otbMPIConfig_h
