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
#include <iomanip>

#include "otbMacro.h"
#include "otbImage.h"
#include "otbGeocentricTransform.h"

int otbGeocentricTransform(int itkNotUsed(argc), char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  typedef otb::GeocentricTransform<otb::TransformDirection::FORWARD> ProjectionType;
  ProjectionType::Pointer projection = ProjectionType::New();

  file << std::setprecision(15);
  file << "Forward projection: " << std::endl;
  itk::Point<double, 3> point;
  point[0] = 0; //lon
  point[1] = 0; //lat
  point[2] = 0; //elev

  file << point << " -> ";
  file << projection->TransformPoint(point);
  file << std::endl << std::endl;

//   itk::Point<double, 3> point;
  point[0] = 1.4835345f; //lon
  point[1] = 43.559682611111107f; //lat
  point[2] = 192.15; //elev

  file << point << " -> ";
  file << projection->TransformPoint(point);
  file << std::endl << std::endl;

  typedef otb::GeocentricTransform<otb::TransformDirection::INVERSE> InverseProjectionType;
  InverseProjectionType::Pointer inverseProjection = InverseProjectionType::New();

  file << "Inverse projection: " << std::endl;

  point[0] = 3771793.97; //X
  point[1] = 140253.34; //Y
  point[2] = 5124421.30; //Z

  file << point << " -> ";
  file << inverseProjection->TransformPoint(point);
  file << std::endl << std::endl;

  file.close();

  return EXIT_SUCCESS;

}
