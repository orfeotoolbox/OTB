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
#include "itkPoint.h"
#include "otbGeographicalDistance.h"

int otbGeographicalDistanceNew(int argc, char * argv[])
{
  typedef itk::Point<double, 2> PointType;
  typedef otb::GeographicalDistance<PointType> DistanceType;

  // Instantiation
  DistanceType::Pointer distance = DistanceType::New();

  return EXIT_SUCCESS;
}

int otbGeographicalDistance(int argc, char * argv[])
{
  typedef itk::Point<double, 2> PointType;
  typedef otb::GeographicalDistance<PointType> GeoDistanceType;

  GeoDistanceType::Pointer geoDistance = GeoDistanceType::New();

  PointType a, b;
  a[0] = atof(argv[1]);
  a[1] = atof(argv[2]);
  b[0] = atof(argv[3]);
  b[1] = atof(argv[4]);

  GeoDistanceType::OriginType o(2);
  o[0] = b[0];
  o[1] = b[1];

  double expectedDistance = atof(argv[5]);
  double tolerance = atof(argv[6]);

  double distEvaluate2 = geoDistance->Evaluate(a, b);
  double distEvaluate2Null = geoDistance->Evaluate(a, a);

  geoDistance->SetOrigin(o);
  double distEvaluate1 = geoDistance->Evaluate(a);
  double distEvaluate1Null = geoDistance->Evaluate(b);

  // Check results
  bool failFlag = false;
  std::cerr.setf(std::ios_base::fixed);
  std::cerr.precision(5);
  if(vcl_abs(distEvaluate1 - expectedDistance) > tolerance)
    {
    std::cerr<<"Evaluate("<<a<<") with origin "<<o<<" is innacurate: expected "<< expectedDistance<<", found "<<distEvaluate1<<std::endl;
    failFlag = true;
    }

  if(vcl_abs(distEvaluate2 - expectedDistance) > tolerance)
    {
    std::cerr<<"Evaluate("<<a<<", "<<b<<") is innacurate: expected "<< expectedDistance<<", found "<<distEvaluate2<<std::endl;
    failFlag = true;
    }

  if(distEvaluate2Null > tolerance)
    {
    std::cerr<<"Evaluate("<<a<<", "<<a<<") is innacurate: expected 0., found "<<distEvaluate2Null<<std::endl;
    failFlag = true;
    }

    if(distEvaluate1Null > tolerance)
    {
    std::cerr<<"Evaluate("<<b<<") with origin "<<o<< " is innacurate: expected 0., found "<<distEvaluate2Null<<std::endl;
    failFlag = true;
    }

    if(failFlag)
      return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
