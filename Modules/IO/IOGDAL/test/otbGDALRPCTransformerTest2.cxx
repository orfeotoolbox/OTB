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

#include "itkPoint.h"
#include "otbGDALRPCTransformer.h"
#include "itkEuclideanDistanceMetric.h"
#include "otbGeographicalDistance.h"
#include "otbImageMetadata.h"
#include "otbMetaDataKey.h"
#include "otbGeomMetadataSupplier.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbMacro.h"

//typedef otb::Image<double> ImageType;
typedef std::list<itk::Point<double, 3>> pointsContainerType;
typedef itk::Statistics::EuclideanDistanceMetric<otb::GDALRPCTransformer::PointType> DistanceType;
typedef otb::GeographicalDistance<otb::GDALRPCTransformer::PointType> GeographicalDistanceType;

int otbGDALRPCTransformerTest2(int itkNotUsed(argc), char* argv[])
{
  bool success = true;

  // Inputs
  std::string inputGeomFile(argv[1]);
  std::string gcpFileName(argv[2]);
  double geoTol(atof(argv[3]));
  double imgTol(atof(argv[4]));

  // Tools
  auto distance = DistanceType::New();
  auto geoDistance = GeographicalDistanceType::New();
  
  // Fetching the RPC model from the product
  otb::ImageMetadata imd;
  otb::GeomMetadataSupplier geomSupplier(inputGeomFile);
  for (int loop = 0 ; loop < geomSupplier.GetNbBands() ; ++loop)
    imd.Bands.emplace_back();
  auto imi = otb::ImageMetadataInterfaceFactory::CreateIMI(imd, geomSupplier);
  imd = imi->GetImageMetadata();
  geomSupplier.FetchRPC(imd);
  auto rpcModel = boost::any_cast<otb::Projection::RPCParam>(imd[otb::MDGeom::RPC]);

  // Setting the RPCTransformer
  otb::GDALRPCTransformer transformer(rpcModel.LineOffset, rpcModel.SampleOffset, rpcModel.LatOffset, rpcModel.LonOffset, rpcModel.HeightOffset,
                                      rpcModel.LineScale, rpcModel.SampleScale, rpcModel.LatScale, rpcModel.LonScale, rpcModel.HeightScale,
                                      rpcModel.LineNum, rpcModel.LineDen, rpcModel.SampleNum, rpcModel.SampleDen, false);

  // Loading the GCP
  pointsContainerType pointsContainer;
  pointsContainerType geo3dPointsContainer;
  std::ifstream file(gcpFileName, std::ios::in);
  if (file)
  {
    otb::GDALRPCTransformer::PointType imagePoint;
    otb::GDALRPCTransformer::PointType geo3dPoint;
    std::string line;
    imagePoint[2] = rpcModel.HeightOffset;
    while (getline(file, line))
    {
      if (line.find_first_of("#") != 0)
      {
        std::istringstream iss(line);

        iss >> imagePoint[0] >> imagePoint[1] >> geo3dPoint[0] >> geo3dPoint[1] >> geo3dPoint[2];

        pointsContainer.push_back(imagePoint);
        geo3dPointsContainer.push_back(geo3dPoint);
      }
    }
    file.close();
  }

  // For each CGP
  pointsContainerType::iterator pointsIt = pointsContainer.begin();
  pointsContainerType::iterator geo3dPointsIt = geo3dPointsContainer.begin();
  while ((pointsIt != pointsContainer.end()) && (geo3dPointsIt != geo3dPointsContainer.end()))
  {
    otbLogMacro(Debug, << "Point: " << *pointsIt << " GeoPoint: " << *geo3dPointsIt);
    // Testing forward transform
    otbLogMacro(Debug, << "ForwardTransform: " << transformer.ForwardTransform(*pointsIt));
    auto forwardPointDistance = geoDistance->Evaluate(transformer.ForwardTransform(*pointsIt), *geo3dPointsIt);
    otbLogMacro(Debug, << "forwardPointDistance: " << forwardPointDistance);
    if (forwardPointDistance > geoTol)
    {
      std::cerr << "Geo distance between otbGDALRPCTransformer->ForwardTransform and GCP too high : "
                << "dist = " << forwardPointDistance << " (tol = " << geoTol << ")" << std::endl;
      success = false;
    }

    // Testing inverse transform
    otbLogMacro(Debug, << "InverseTransform: " << transformer.InverseTransform(*geo3dPointsIt));
    auto inversePointDistance = distance->Evaluate(transformer.InverseTransform(*geo3dPointsIt), *pointsIt);
    otbLogMacro(Debug, << "inversePointDistance: " << inversePointDistance);
    if (inversePointDistance > imgTol)
    {
      std::cerr << "Distance between otbGDALRPCTransformer->InverseTransform and GCP too high : "
                << "dist = " << inversePointDistance << " (tol = " << imgTol << ")" << std::endl;
      success = false;
    }

    ++pointsIt;
    ++geo3dPointsIt;
  }
  
  if (success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
