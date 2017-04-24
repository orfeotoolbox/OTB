/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <fstream>
#include <iomanip>

#include "otbMacro.h"
#include "otbImage.h"
#include "otbMapProjections.h"

int otbMapProjection(int itkNotUsed(argc), char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;

  itk::Point<double, 2> point;
  point[0] = 1.44;
  point[1] = 43.6050;
  itk::Point<double, 2> point2;
  file.open(outFileName);
  file << std::setprecision(15);

  /** Test the output of the projection in Wkt format*/
  otb::UtmForwardProjection::Pointer lUtmProjection2 = otb::UtmForwardProjection::New();
  point2 = lUtmProjection2->TransformPoint(point);
  file << lUtmProjection2->GetWkt() << std::endl << std::endl;
  file << "Forward projection: " << std::endl;
  file << point << " -> ";
  file << point2;
  file << std::endl << std::endl;


  otb::UtmInverseProjection::Pointer lUtmProjection = otb::UtmInverseProjection::New();
  file << lUtmProjection->GetWkt() << std::endl << std::endl;
  file << "Inverse projection: " << std::endl;
  file << point2 << " -> ";
  file << lUtmProjection->TransformPoint(point2);
  file << std::endl << std::endl;


  otb::Lambert2EtenduForwardProjection::Pointer lLambert2Etendu2 = otb::Lambert2EtenduForwardProjection::New();
  point2 = lLambert2Etendu2->TransformPoint(point);
  file << lLambert2Etendu2->GetWkt() << std::endl << std::endl;
  file << "Forward projection: " << std::endl;
  file << point << " -> ";
  file << point2;
  file << std::endl << std::endl;

  otb::Lambert2EtenduInverseProjection::Pointer lLambert2Etendu = otb::Lambert2EtenduInverseProjection::New();
  file << lLambert2Etendu->GetWkt() << std::endl << std::endl;
  file << "Inverse projection: " << std::endl;
  file << point2 << " -> ";
  file << lLambert2Etendu->TransformPoint(point2);
  file << std::endl << std::endl;


  otb::Lambert93ForwardProjection::Pointer lLambert93_2 = otb::Lambert93ForwardProjection::New();
  point2 = lLambert93_2->TransformPoint(point);
  file << lLambert93_2->GetWkt() << std::endl << std::endl;
  file << "Forward projection: " << std::endl;
  file << point << " -> ";
  file << point2;
  file << std::endl << std::endl;

  otb::Lambert93InverseProjection::Pointer lLambert93 = otb::Lambert93InverseProjection::New();
  file << lLambert93->GetWkt() << std::endl << std::endl;
  file << "Inverse projection: " << std::endl;
  file << point2 << " -> ";
  file << lLambert93->TransformPoint(point2);
  file << std::endl << std::endl;

  typedef otb::GenericMapProjection<otb::TransformDirection::FORWARD> MercatorForwardProjection;
  MercatorForwardProjection::Pointer lMercatorProjection2 = MercatorForwardProjection::New();
  lMercatorProjection2->SetWkt(std::string("Mercator"));
  point2 = lMercatorProjection2->TransformPoint(point);
  file << lMercatorProjection2->GetWkt() << std::endl << std::endl;
  file << "Forward projection: " << std::endl;
  file << point << " -> ";
  file << point2;
  file << std::endl << std::endl;

  typedef otb::GenericMapProjection<otb::TransformDirection::INVERSE> MercatorInverseProjection;
  MercatorInverseProjection::Pointer lMercatorProjection = MercatorInverseProjection::New();
  lMercatorProjection->SetWkt(std::string("Mercator"));
  file << lMercatorProjection->GetWkt() << std::endl << std::endl;
  file << "Inverse projection: " << std::endl;
  file << point2 << " -> ";
  file << lMercatorProjection->TransformPoint(point2);
  file << std::endl << std::endl;


  // Moving to GenericMapProjection, change the access
  lUtmProjection->SetParameter("MetersPerPixelX", "1");
  lUtmProjection->SetParameter("MetersPerPixelY", "1");

  file << "\n\n *** Test accessors *** \n\n";
  file << std::setprecision(15);
  file << "Origin: \t" << lUtmProjection->GetParameter("Origin") << std::endl;
  file << "False northing: \t" << lUtmProjection->GetParameter("FalseNorthing") << std::endl;
  file << "False easting: \t" << lUtmProjection->GetParameter("FalseEasting") << std::endl;
  file << "Parallel 1: \t" << lUtmProjection->GetParameter("StandardParallel1") << std::endl;
  file << "Parallel 2: \t" << lUtmProjection->GetParameter("StandardParallel2") << std::endl;
//  file << "Name: \t" << lUtmProjection->GetProjectionName() << std::endl;
//  file << "Geo: \t" << lUtmProjection->IsGeographic() << std::endl;
  file << "A: \t" << lUtmProjection->GetParameter("A") << std::endl;
  file << "B: \t" << lUtmProjection->GetParameter("B") << std::endl;
  file << "F: \t" << lUtmProjection->GetParameter("F") << std::endl;
  file << "m per pix: \t" << lUtmProjection->GetParameter("MetersPerPixel") << std::endl;
  file << "degree per pix: \t" << lUtmProjection->GetParameter("DecimalDegreesPerPixel") << std::endl;

  file.close();
  return EXIT_SUCCESS;

}
