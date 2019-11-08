/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
#include "otbGenericRSTransform.h"
#include "otbGeographicalDistance.h"
#include "otbMacro.h"
#include <string>

int otbGCPsToRPCSensorModelImageFilterCheckRpcModel(int argc, char* argv[])
{
  // Look for tolerance value
  std::string  s_tol    = argv[argc - 1]; // last argument
  int          tol      = 0;
  unsigned int nbPoints = argc - 3;
  if (s_tol.find("err=") != std::string::npos)
  {
    nbPoints--; // last argument in not a gcp pairs point
    tol = stoi(s_tol.substr(s_tol.find("=") + 1));
  }
  // Check if the number of gcp pairs point is consistent


  if (nbPoints % 5 != 0)
  {
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
  }

  typedef otb::VectorImage<float, 2> ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPsToSensorModelFilterType;
  typedef GCPsToSensorModelFilterType::Point2DType        Point2DType;
  typedef GCPsToSensorModelFilterType::Point3DType        Point3DType;
  typedef otb::GenericRSTransform<double, 3, 3> GenericRSTransformType;
  typedef otb::GeographicalDistance<ImageType::PointType> GeoDistanceType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  unsigned int nbGCPs = nbPoints / 5;
  otbLogMacro(Info, << "Receiving " << nbPoints << " from command line.");

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
  {
    Point2DType sensorPoint;
    sensorPoint[0] = std::stof(argv[3 + gcpId * 5]);
    sensorPoint[1] = std::stof(argv[4 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = std::stof(argv[5 + gcpId * 5]);
    geoPoint[1] = std::stof(argv[6 + gcpId * 5]);
    geoPoint[2] = std::stof(argv[7 + gcpId * 5]);

    otbLogMacro(Debug, << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint);

    rpcEstimator->AddGCP(sensorPoint, geoPoint);
  }

  // Estimate the rpc model
  rpcEstimator->GetOutput()->UpdateOutputInformation();

  // Instancicate a GenericRSTransform in order to transform the
  // indexes, using the rpcModel estimated, into geographical
  // coordiantes.
  // The test will check for nan coordinates, and the distance between
  // geographical coordinates.

  GenericRSTransformType::Pointer grsTrasnform = GenericRSTransformType::New();
  grsTrasnform->SetInputKeywordList(rpcEstimator->GetKeywordlist());
  otbLogMacro(Debug, << rpcEstimator->GetKeywordlist());
  grsTrasnform->SetOutputProjectionRef("EPSG:4326");

  // Set the DEM Directory
  if (std::string(argv[2]).compare("no_output") != 0)
    otb::DEMHandler::Instance()->OpenDEMDirectory(argv[2]);

  grsTrasnform->InstantiateTransform();

  // Test
  GeoDistanceType::Pointer geoDistance     = GeoDistanceType::New();
  bool                     isErrorDetected = false;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
  {
    Point3DType point;
    point[0] = std::stof(argv[3 + gcpId * 5]);
    point[1] = std::stof(argv[4 + gcpId * 5]);
    point[2] = std::stof(argv[7 + gcpId * 5]);

    Point3DType transformedPoint;
    transformedPoint = grsTrasnform->TransformPoint(point);

    Point2DType transformedPoint2D;
    transformedPoint2D[0] = transformedPoint[0];
    transformedPoint2D[1] = transformedPoint[1];

    // reference point
    Point2DType geoPoint;
    geoPoint[0] = std::stof(argv[5 + gcpId * 5]);
    geoPoint[1] = std::stof(argv[6 + gcpId * 5]);

    // Search for nans
    if (vnl_math_isnan(transformedPoint2D[0]) || vnl_math_isnan(transformedPoint2D[1]))
    {
      otbLogMacro(Warning, << "Reference : " << geoPoint << " --> Result of the reprojection using the estimated RpcModel " << transformedPoint2D);
      otbLogMacro(Warning, << "The result of the projection is nan, there is a problem with the estimated RpcModel");
      isErrorDetected = true;
    }

    // Search for wrong projection results
    double residual = geoDistance->Evaluate(geoPoint, transformedPoint2D);
    if (residual > tol)
    {
      otbLogMacro(Warning, << "Reference : " << geoPoint << " --> Result of the reprojection using the estimated RpcModel " << transformedPoint2D << std::endl
                           << " Residual [" << residual << "] is higher than the tolerance [" << tol << "], there is a problem with the estimated RpcModel");
      isErrorDetected = true;
    }
  }

  // Is there an error
  if (isErrorDetected)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
