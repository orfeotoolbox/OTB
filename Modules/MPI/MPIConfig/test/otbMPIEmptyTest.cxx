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
#include <cstdlib>
#include "otbMPIConfig.h"
#include "itkMultiThreader.h"

int otbMPIEmptyTest(int, char **) {
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1); 
  return EXIT_SUCCESS;
}

