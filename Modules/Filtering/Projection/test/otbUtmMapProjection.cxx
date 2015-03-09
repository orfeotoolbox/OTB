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

#include <fstream>

#include "otbMacro.h"
#include "otbImage.h"
#include "otbMapProjections.h"
#include "otbUtils.h"

int otbUtmMapProjection(int itkNotUsed(argc), char* argv[])
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

  lUtmProjection2->SetHemisphere('S');
  file << "We should be in the south hemisphere now\n";
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  lUtmProjection2->SetZone(48);
  file << "We should be in zone 48\n";
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  lUtmProjection2->SetHemisphere('N');
  file << "And back in the north hemisphere\n";
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  lUtmProjection2->SetHemisphere('S'); //Back in the south to make sure the following works
  otb::UtmForwardProjection::InputPointType groundPoint;
  groundPoint[0] = -4.5;
  groundPoint[1] = 48.5;
  lUtmProjection2->SetZoneAndHemisphereFromGeoPoint(groundPoint);
  file << "We should be in zone 30 in north hemisphere\n";
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  //And checking accesors
  file << "Zone should be 30: " << lUtmProjection2->GetZone() << "\n";
  file << "Hemisphere should be N: " << lUtmProjection2->GetHemisphere() << "\n";

  groundPoint[0] = 1.5;
  groundPoint[1] = 43.5;
  file << "Zone should be 31: " <<  otb::Utils::GetZoneFromGeoPoint(groundPoint[0], groundPoint[1]) << "\n";

  groundPoint[0] = 5;
  groundPoint[1] = 60;
  file << "Zone should be 32 (exception near Norway): " << otb::Utils::GetZoneFromGeoPoint(groundPoint[0], groundPoint[1]) << "\n";

  file.close();
  return EXIT_SUCCESS;

}
