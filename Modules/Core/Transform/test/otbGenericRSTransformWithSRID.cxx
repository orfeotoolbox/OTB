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
#include <iostream>

#include "otbGenericRSTransform.h"
#include <ogr_spatialref.h>

/**
  * This test is the same than the otbGenericRSTransform
  * except using the SRID to build the wkt.
  */

int otbGenericRSTransformWithSRID(int itkNotUsed(argc), char* argv[])
{
  OGRSpatialReference oSRS;

  typedef otb::GenericRSTransform<>     TransformType;
  typedef TransformType::InputPointType PointType;

  PointType geoPoint;

  geoPoint[0] = atof(argv[1]);
  geoPoint[1] = atof(argv[2]);
  const char * outfname = argv[3];

  TransformType::Pointer wgs2utm        = TransformType::New();
  wgs2utm->SetInputProjectionRef("EPSG:4326");   // WGS 84
  wgs2utm->SetOutputProjectionRef("EPSG:32631"); // UTM 31 N
  wgs2utm->InstantiateTransform();

  TransformType::Pointer utm2wgs        = TransformType::New();
  wgs2utm->GetInverse(utm2wgs);

  TransformType::Pointer wgs2lambert    = TransformType::New();
  wgs2lambert->SetInputProjectionRef("EPSG:4326");   // WGS 84
  wgs2lambert->SetOutputProjectionRef("EPSG:27572");

  wgs2lambert->InstantiateTransform();

  TransformType::Pointer lambert2wgs    = TransformType::New();
  wgs2lambert->GetInverse(lambert2wgs);

  TransformType::Pointer wgs2wgs        = TransformType::New();
  wgs2wgs->SetInputProjectionRef("EPSG:4326");   // WGS 84
  wgs2wgs->SetOutputProjectionRef("EPSG:4326");  // WGS 84
  wgs2wgs->InstantiateTransform();

  TransformType::Pointer utm2utm        = TransformType::New();
  utm2utm->SetInputProjectionRef("EPSG:32631");   // UTM 31 N
  utm2utm->SetOutputProjectionRef("EPSG:32631");  // UTM 31 N
  utm2utm->InstantiateTransform();

  TransformType::Pointer lambert2lambert = TransformType::New();
  lambert2lambert->SetInputProjectionRef("EPSG:27572");
  lambert2lambert->SetOutputProjectionRef("EPSG:27572");

  lambert2lambert->InstantiateTransform();
  
  TransformType::Pointer utm2lambert    = TransformType::New();
  utm2lambert->SetInputProjectionRef("EPSG:32631");  // UTM
  utm2lambert->SetOutputProjectionRef("EPSG:27572");

  utm2lambert->InstantiateTransform();

  TransformType::Pointer lambert2utm    = TransformType::New();
  utm2lambert->GetInverse(lambert2utm);

  std::ofstream ofs;
  ofs.open(outfname);

  // Set floatfield to format writing properly
  ofs.setf(std::ios::fixed, std::ios::floatfield);
  ofs.precision(10);

  ofs << "Testing geopoint: " << geoPoint << std::endl << std::endl;

  ofs << "Testing wgs84 to wgs84: " << geoPoint << " -> " << wgs2wgs->TransformPoint(geoPoint) << std::endl;

  PointType utmPoint, lambertPoint, tmtPoint;

  utmPoint = wgs2utm->TransformPoint(geoPoint);
  lambertPoint = wgs2lambert->TransformPoint(geoPoint);
  ofs.precision(3);

  ofs << "Testing utm 31 north to utm 31 north: " << utmPoint << " -> " << utm2utm->TransformPoint(utmPoint) <<
  std::endl;
  ofs << "Testing lambert 93 to lambert 93: " << lambertPoint << " -> " <<
  lambert2lambert->TransformPoint(lambertPoint) << std::endl;

  ofs << std::endl << "Testing geo 2 carto ..." << std::endl << std::endl;

  ofs.precision(10);

  ofs << "Testing wgs84 to utm 31 north: " << geoPoint;
  ofs.precision(3);
  ofs << " -> " << utmPoint << std::endl;
  ofs << "Testing utm 31 north to wgs84: " << utmPoint;
  ofs.precision(10);
  ofs << " -> " << utm2wgs->TransformPoint(utmPoint) << std::endl;

  ofs << "Testing wgs84 to lambert 93: " << geoPoint;
  ofs.precision(3);
  ofs << " -> " << lambertPoint << std::endl;
  ofs << "Testing lambert 93 to wgs84: " << lambertPoint;
  ofs.precision(10);
  ofs << " -> " << lambert2wgs->TransformPoint(lambertPoint) << std::endl;

  ofs << std::endl << "Testing cross geo ..." << std::endl << std::endl;

  ofs.precision(3);

  ofs << "Testing lambert 93 to utm 31 north: " << lambertPoint << " -> " <<
  lambert2utm->TransformPoint(lambertPoint) << std::endl;
  ofs << "Testing utm 31 north to lambert 93: " << utmPoint << " -> " << utm2lambert->TransformPoint(utmPoint) <<
  std::endl;
  

  ofs.close();

  return EXIT_SUCCESS;
}
