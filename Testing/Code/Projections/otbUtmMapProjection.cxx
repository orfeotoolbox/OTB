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

int otbUtmMapProjection(int argc, char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  /** Test the output of the projection in Wkt format*/
  otb::UtmInverseProjection::Pointer lUtmProjection = otb::UtmInverseProjection::New();
  file << lUtmProjection->GetWkt() << std::endl << std::endl;

  lUtmProjection->SetHemisphere('S');
  file << "We should be in the south hemisphere now\n";
  file << lUtmProjection->GetWkt() << std::endl << std::endl;

  lUtmProjection->SetZone(48);
  file << "We should be in zone 48\n";
  file << lUtmProjection->GetWkt() << std::endl << std::endl;

  lUtmProjection->SetHemisphere('N');
  file << "And back in the north hemisphere\n";
  file << lUtmProjection->GetWkt() << std::endl << std::endl;

  otb::UtmForwardProjection::Pointer lUtmProjection2 = otb::UtmForwardProjection::New();
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  lUtmProjection->SetHemisphere('S');
  file << "We should be in the south hemisphere now\n";
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  lUtmProjection->SetZone(48);
  file << "We should be in zone 48\n";
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  lUtmProjection->SetHemisphere('N');
  file << "And back in the north hemisphere\n";
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;


  file.close();
  return EXIT_SUCCESS;

}
