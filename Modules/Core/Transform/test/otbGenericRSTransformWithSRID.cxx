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

  // Build Lambert II ref
  double stdParallel1  = 45.89891944;
  double stdParallel2  = 47.69601389;
  double originLatL2     = 46.8;
  double originLongL2    = 2.33722778;
  double falseEastingL2  = 600000;
  double falseNorthingL2 = 2200000;
  oSRS.SetProjCS("Lambert II ");
  oSRS.SetLCC(stdParallel1, stdParallel2, originLatL2, originLongL2, falseEastingL2, falseNorthingL2);
  char * lambertRef = NULL;
  oSRS.exportToWkt(&lambertRef);

  // Build the Transmercator ref
  double originLatTmt   = 49.83;
  double originLongTmt  = 6.16;

  oSRS.SetProjCS("Transmercator ");
  oSRS.SetWellKnownGeogCS("WGS84");
  oSRS.SetTM(originLatTmt, originLongTmt, 1, 0, 0);
  char * tmtRef = NULL;
  oSRS.exportToWkt(&tmtRef);

  TransformType::Pointer wgs2utm        = TransformType::New();
  wgs2utm->SetInputProjectionRef("4326");   // WGS 84
  wgs2utm->SetOutputProjectionRef("32631"); // UTM 31 N
  wgs2utm->InstanciateTransform();

  TransformType::Pointer utm2wgs        = TransformType::New();
  wgs2utm->GetInverse(utm2wgs);

  TransformType::Pointer wgs2lambert    = TransformType::New();
  wgs2lambert->SetInputProjectionRef("4326");   // WGS 84
  wgs2lambert->SetOutputProjectionRef(lambertRef);

  wgs2lambert->InstanciateTransform();

  TransformType::Pointer lambert2wgs    = TransformType::New();
  wgs2lambert->GetInverse(lambert2wgs);

  TransformType::Pointer wgs2tmt        = TransformType::New();
  wgs2tmt->SetInputProjectionRef("4326");   // WGS 84
  wgs2tmt->SetOutputProjectionRef(tmtRef);

  wgs2tmt->InstanciateTransform();

  TransformType::Pointer tmt2wgs        = TransformType::New();
  wgs2tmt->GetInverse(tmt2wgs);

  TransformType::Pointer wgs2wgs        = TransformType::New();
  wgs2wgs->SetInputProjectionRef("4326");   // WGS 84
  wgs2wgs->SetOutputProjectionRef("4326");  // WGS 84
  wgs2wgs->InstanciateTransform();

  TransformType::Pointer utm2utm        = TransformType::New();
  utm2utm->SetInputProjectionRef("32631");   // UTM 31 N
  utm2utm->SetOutputProjectionRef("32631");  // UTM 31 N
  utm2utm->InstanciateTransform();

  TransformType::Pointer lambert2lambert = TransformType::New();
  lambert2lambert->SetInputProjectionRef(lambertRef);
  lambert2lambert->SetOutputProjectionRef(lambertRef);

  lambert2lambert->InstanciateTransform();

  TransformType::Pointer tmt2tmt        = TransformType::New();
  tmt2tmt->SetInputProjectionRef(tmtRef);
  tmt2tmt->SetOutputProjectionRef(tmtRef);
  tmt2tmt->InstanciateTransform();

  TransformType::Pointer utm2lambert    = TransformType::New();
  utm2lambert->SetInputProjectionRef("32631");  // UTM
  utm2lambert->SetOutputProjectionRef(lambertRef);

  utm2lambert->InstanciateTransform();

  TransformType::Pointer lambert2utm    = TransformType::New();
  utm2lambert->GetInverse(lambert2utm);

  TransformType::Pointer utm2tmt        = TransformType::New();
  utm2tmt->SetInputProjectionRef("32631");
  utm2tmt->SetOutputProjectionRef(tmtRef);
  utm2tmt->InstanciateTransform();

  TransformType::Pointer tmt2utm        = TransformType::New();
  utm2tmt->GetInverse(tmt2utm);

  TransformType::Pointer lambert2tmt    = TransformType::New();
  lambert2tmt->SetInputProjectionRef(lambertRef);
  lambert2tmt->SetOutputProjectionRef(tmtRef);
  lambert2tmt->InstanciateTransform();

  TransformType::Pointer tmt2lambert    = TransformType::New();
  lambert2tmt->GetInverse(tmt2lambert);

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
  tmtPoint = wgs2tmt->TransformPoint(geoPoint);

  ofs.precision(3);

  ofs << "Testing utm 31 north to utm 31 north: " << utmPoint << " -> " << utm2utm->TransformPoint(utmPoint) <<
  std::endl;
  ofs << "Testing lambert 2 to lambert 2: " << lambertPoint << " -> " <<
  lambert2lambert->TransformPoint(lambertPoint) << std::endl;
  ofs << "Testing transmercator 31 north to transmercator: " << tmtPoint << " -> " <<
  tmt2tmt->TransformPoint(tmtPoint) << std::endl;

  ofs << std::endl << "Testing geo 2 carto ..." << std::endl << std::endl;

  ofs.precision(10);

  ofs << "Testing wgs84 to utm 31 north: " << geoPoint;
  ofs.precision(3);
  ofs << " -> " << utmPoint << std::endl;
  ofs << "Testing utm 31 north to wgs84: " << utmPoint;
  ofs.precision(10);
  ofs << " -> " << utm2wgs->TransformPoint(utmPoint) << std::endl;

  ofs << "Testing wgs84 to lambert 2: " << geoPoint;
  ofs.precision(3);
  ofs << " -> " << lambertPoint << std::endl;
  ofs << "Testing lambert 2 to wgs84: " << lambertPoint;
  ofs.precision(10);
  ofs << " -> " << lambert2wgs->TransformPoint(lambertPoint) << std::endl;

  ofs << "Testing wgs84 to transmeractor: " << geoPoint;
  ofs.precision(3);
  ofs << " -> " << tmtPoint << std::endl;
  ofs << "Testing transmercator to wgs84: " << tmtPoint;
  ofs.precision(10);
  ofs << " -> " << tmt2wgs->TransformPoint(tmtPoint) << std::endl;

  ofs << std::endl << "Testing cross geo ..." << std::endl << std::endl;

  ofs.precision(3);

  ofs << "Testing lambert 2 to utm 31 north: " << lambertPoint << " -> " <<
  lambert2utm->TransformPoint(lambertPoint) << std::endl;
  ofs << "Testing utm 31 north to lambert 2: " << utmPoint << " -> " << utm2lambert->TransformPoint(utmPoint) <<
  std::endl;

  ofs << "Testing lambert 2 to transmercator: " << lambertPoint << " -> " <<
  lambert2tmt->TransformPoint(lambertPoint) << std::endl;
  ofs << "Testing transmercator to lambert 2: " << tmtPoint << " -> " << tmt2lambert->TransformPoint(tmtPoint) <<
  std::endl;

  ofs << "Testing transmercator to utm 31 north: " << tmtPoint << " -> " << tmt2utm->TransformPoint(tmtPoint) <<
  std::endl;
  ofs << "Testing utm 31 north to transmercator: " << utmPoint << " -> " << utm2tmt->TransformPoint(utmPoint) <<
  std::endl;

  ofs.close();

  return EXIT_SUCCESS;
}
