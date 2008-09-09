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
#include "otbLogPolarTransform.h"
#include <cstdlib>

int otbLogPolarTransformNew(int argc, char* argv[])
{
  typedef double PrecisionType;
  typedef otb::LogPolarTransform<PrecisionType> LogPolarTransformType;
  
  // Instantiation
  LogPolarTransformType::Pointer transform = LogPolarTransformType::New();


 return EXIT_SUCCESS;
}
