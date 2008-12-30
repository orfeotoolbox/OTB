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

  otb::UtmForwardProjection::Pointer lUtmProjection2 = otb::UtmForwardProjection::New();
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;

  otb::Lambert2EtenduInverseProjection::Pointer lLambert2Etendu = otb::Lambert2EtenduInverseProjection::New();
  file << lLambert2Etendu->GetWkt() << std::endl << std::endl;

  otb::Lambert2EtenduForwardProjection::Pointer lLambert2Etendu2 = otb::Lambert2EtenduForwardProjection::New();
  file << lLambert2Etendu2->GetWkt() << std::endl << std::endl;

  otb::Lambert93InverseProjection::Pointer lLambert93 = otb::Lambert93InverseProjection::New();
  file << lLambert93->GetWkt() << std::endl << std::endl;

  otb::Lambert93ForwardProjection::Pointer lLambert93_2 = otb::Lambert93ForwardProjection::New();
  file << lLambert93_2->GetWkt() << std::endl << std::endl;


  otb::MercatorInverseProjection::Pointer lMercatorProjection = otb::MercatorInverseProjection::New();
  file << lMercatorProjection->GetWkt() << std::endl << std::endl;

  otb::MercatorForwardProjection::Pointer lMercatorProjection2 = otb::MercatorForwardProjection::New();
  file << lMercatorProjection2->GetWkt() << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;

}