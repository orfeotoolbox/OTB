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

int otbMapProjection(int argc, char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  /** Test the output of the projection in Wkt format*/
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

  file << "\n\n *** Test accessors *** \n\n";
  file << std::setprecision(15);
  file << "Origin: \t" << lUtmProjection->Origin() << std::endl;
  file << "False northing: \t" << lUtmProjection->GetFalseNorthing() << std::endl;
  file << "False easting: \t" << lUtmProjection->GetFalseEasting() << std::endl;
  file << "Parallel 1: \t" << lUtmProjection->GetStandardParallel1() << std::endl;
  file << "Parallel 2: \t" << lUtmProjection->GetStandardParallel2() << std::endl;
  file << "Name: \t" << lUtmProjection->GetProjectionName() << std::endl;
  file << "Geo: \t" << lUtmProjection->IsGeographic() << std::endl;
  file << "A: \t" << lUtmProjection->GetA() << std::endl;
  file << "B: \t" << lUtmProjection->GetB() << std::endl;
  file << "F: \t" << lUtmProjection->GetF() << std::endl;
  file << "m per pix: \t" << lUtmProjection->GetMetersPerPixel() << std::endl;
  file << "degree per pix: \t" << lUtmProjection->GetDecimalDegreesPerPixel() << std::endl;

  file.close();
  return EXIT_SUCCESS;

}
