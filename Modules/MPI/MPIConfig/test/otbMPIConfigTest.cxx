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
#include <iostream>
#include "itkMultiThreader.h"

int otbMPIConfigTest(int argc, char* argv[]) {

  // Mono-thread execution
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);

  // MPI Configuration
  typedef otb::MPIConfig    MPIConfigType;
  MPIConfigType::Pointer config = MPIConfigType::Instance();
  config->Init(argc,argv,true);

  // Test
  std::cout<<"OTB MPI config test. I am process "<<config->GetMyRank()<<" among "<<config->GetNbProcs()<<"."<<std::endl;

  return EXIT_SUCCESS;
}

