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
#include "otbTileMapTransform.h"

int otbTileMapTransform(int itkNotUsed(argc), char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  typedef otb::TileMapTransform<otb::TransformDirection::FORWARD> ProjectionType;
  ProjectionType::Pointer projection = ProjectionType::New();
  projection->SetLevel(10);

  file << std::setprecision(15);
  file << "Forward projection: level " << projection->GetLevel() << std::endl;
  itk::Point<double, 2> point;
  point[0] = 0; //lon
  point[1] = 0; //lat

  file << point << " -> ";
  file << projection->TransformPoint(point);
  file << std::endl << std::endl;

  point[0] = 1.48; //lon
  point[1] = 43.55; //lat

  file << point << " -> ";
  file << projection->TransformPoint(point);
  file << std::endl << std::endl;

  typedef otb::TileMapTransform<otb::TransformDirection::INVERSE> InverseProjectionType;
  InverseProjectionType::Pointer inverseProjection = InverseProjectionType::New();
  inverseProjection->SetLevel(10);

  file << "Inverse projection: level " << inverseProjection->GetLevel() << std::endl;

  point[0] = 132149; //X
  point[1] = 95774; //Y

  file << point << " -> ";
  file << inverseProjection->TransformPoint(point);
  file << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
