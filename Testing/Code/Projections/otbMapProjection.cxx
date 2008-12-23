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

#include "otbMacro.h"
#include "otbImage.h"
#include "otbMapProjections.h"


//TODO add control baseline
int otbMapProjection( int argc, char* argv[] )
{
  const char * outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  otb::UtmInverseProjection::Pointer lUtmProjection = otb::UtmInverseProjection::New();
  file << lUtmProjection->GetWkt() << std::endl << std::endl;


  file.close();
  return EXIT_SUCCESS;

}