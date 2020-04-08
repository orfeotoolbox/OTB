/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbGenericRSTransform.h"
#include "otbGeographicalDistance.h"
#include "itkEuclideanDistanceMetric.h"
#include "otbSensorModelAdapter.h"
#include "otbRPCSolverAdapter.h"

typedef otb::Image<double>                 ImageType;
typedef otb::ImageFileReader<ImageType>    ReaderType;
typedef otb::GenericRSTransform<>          RSTranformType;
typedef otb::RPCSolverAdapter::Point2DType Point2DType;
typedef otb::RPCSolverAdapter::Point3DType Point3DType;
typedef otb::GenericRSTransform<double, 3, 3> RSTranform3dType;
typedef itk::Statistics::EuclideanDistanceMetric<Point2DType> EuclideanDistanceMetricType;
typedef otb::GeographicalDistance<Point2DType>                GeoDistanceType;

int otbRPCSolverAdapterTest(int argc, char* argv[])
{
  if (argc < 7)
  {
    std::cout << "Usage: test_driver input grid_size geo_tol img_tol dem_dir geoid" << std::endl;
    return EXIT_FAILURE;
  }
  // This test takes a sensor model (possibly a rpc one), use it to
  // generate gcps and estimate a rpc model. It then checks the
  // precision of both forward and inverse transform
  std::string        infname  = argv[1];
  const unsigned int gridSize = atoi(argv[2]);
  const double       geoTol   = atof(argv[3]);
  const double       imgTol   = atof(argv[4]);
  const std::string  demdir   = argv[5];
  const std::string  geoid    = argv[6];

  if (gridSize == 0)
  {
    std::cerr << "Grid size is null!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "GeoTol: " << geoTol << " meters" << std::endl;
  std::cout << "ImgTol: " << imgTol << " pixels" << std::endl;

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  demHandler->SetDefaultHeightAboveEllipsoid(0);
  if (demdir != "no")
    demHandler->OpenDEMDirectory(demdir);
  if (geoid != "no")
    demHandler->OpenGeoidFile(geoid);

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  RSTranformType::Pointer fwd2dTransform = RSTranformType::New();
  fwd2dTransform->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  fwd2dTransform->InstantiateTransform();

  ImageType::SizeType size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();

  unsigned int stepx = size[0] / gridSize;
  unsigned int stepy = size[1] / gridSize;

  otb::RPCSolverAdapter::GCPsContainerType gcps;

  // Generate gcps
  for (unsigned int i = 0; i < gridSize; ++i)
  {
    for (unsigned int j = 0; j < gridSize; ++j)
    {
      ImageType::IndexType currentIndex;
      currentIndex[0] = i * stepx;
      currentIndex[1] = j * stepy;

      Point2DType currentPoint, currentWgs84Point;
      reader->GetOutput()->TransformIndexToPhysicalPoint(currentIndex, currentPoint);

      currentWgs84Point = fwd2dTransform->TransformPoint(currentPoint);

      double height = otb::DEMHandler::Instance()->GetHeightAboveEllipsoid(currentWgs84Point);

      Point3DType current3DWgs84Point;
      current3DWgs84Point[0] = currentWgs84Point[0];
      current3DWgs84Point[1] = currentWgs84Point[1];
      current3DWgs84Point[2] = height;

      gcps.push_back(std::make_pair(currentPoint, current3DWgs84Point));

      std::cout << currentPoint[0] << " " << currentPoint[1] << " " << current3DWgs84Point[0] << " " << current3DWgs84Point[1] << " " << current3DWgs84Point[2]
                << std::endl;
    }
  }

  // Solve rpc
  otb::ImageKeywordlist rpcKwl;
  double                rmse;

  // Call solver: either write geom and exit, or evaluate model precision
  if (argc == 8)
  {
    const std::string outgeom = argv[7];
    bool              success = otb::RPCSolverAdapter::Solve(gcps, rmse, outgeom);
    if (success)
      return EXIT_SUCCESS;
    else
      return EXIT_FAILURE;
  }

  otb::RPCSolverAdapter::Solve(gcps, rmse, rpcKwl);

  std::cout << "Optimization done, RMSE=" << rmse << std::endl;

  // Build forward and inverse rpc transform
  RSTranform3dType::Pointer rpcFwdTransform = RSTranform3dType::New();
  rpcFwdTransform->SetInputKeywordList(rpcKwl);
  rpcFwdTransform->InstantiateTransform();
  RSTranformType::Pointer rpcInvTransform = RSTranformType::New();
  rpcInvTransform->SetOutputKeywordList(rpcKwl);
  rpcInvTransform->InstantiateTransform();

  EuclideanDistanceMetricType::Pointer euclideanDistanceMetric = EuclideanDistanceMetricType::New();
  GeoDistanceType::Pointer             geoDistance             = GeoDistanceType::New();

  bool fail = false;

  for (otb::RPCSolverAdapter::GCPsContainerType::iterator it = gcps.begin(); it != gcps.end(); ++it)
  {
    Point2DType imgPoint, groundPoint, groundPoint2dRef;
    Point3DType imgPoint3D, groundPoint3D;

    groundPoint2dRef[0] = it->second[0];
    groundPoint2dRef[1] = it->second[1];

    // Check forward transform
    imgPoint3D[0] = it->first[0];
    imgPoint3D[1] = it->first[1];
    imgPoint3D[2] = it->second[2];

    groundPoint3D = rpcFwdTransform->TransformPoint(imgPoint3D);

    groundPoint[0] = groundPoint3D[0];
    groundPoint[1] = groundPoint3D[1];

    double groundRes = geoDistance->Evaluate(groundPoint, groundPoint2dRef);

    if (groundRes > geoTol)
    {
      fail = true;
      std::cerr << "Imprecise result with forward estimated model: fwd(" << it->first << ") = " << groundPoint << ", but reference is " << groundPoint2dRef
                << " error: " << groundRes << " meters)" << std::endl;
    }

    // Check inverse transform
    imgPoint = rpcInvTransform->TransformPoint(groundPoint2dRef);

    double imgRes = euclideanDistanceMetric->Evaluate(imgPoint, it->first);

    if (imgRes > imgTol)
    {
      fail = true;
      std::cerr << "Imprecise result with inverse estimated model: inv(" << groundPoint2dRef << ") = " << imgPoint << ", but reference is " << it->first
                << " error: " << imgRes << " pixels)" << std::endl;
    }
  }


  if (fail)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
