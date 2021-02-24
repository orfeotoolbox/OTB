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
#include "itkEuclideanDistanceMetric.h"
#include "otbGeographicalDistance.h"
#include "otbImageMetadata.h"
#include "otbMetaDataKey.h"
#include "otbGeomMetadataSupplier.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbMacro.h"
#include "otbDEMHandler.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbRPCForwardTransform.h"
#include "otbRPCInverseTransform.h"
#include "otbGenericRSTransform.h"

using PointType = itk::Point<double, 3>;
using PointsContainerType = std::vector<PointType>;
using ForwardTransformType = otb::RPCForwardTransform<double, 3, 3>;
using InverseTransformType = otb::RPCInverseTransform<double, 3, 3>;
using GenericRSTransformType = otb::GenericRSTransform<double, 3, 3>;
using DistanceType = itk::Statistics::EuclideanDistanceMetric<PointType>;
using GeographicalDistanceType = otb::GeographicalDistance<PointType>;

int otbRPCTransformTest(int itkNotUsed(argc), char* argv[])
{
  bool success = true;
  PointType imagePoint;
  PointType geo3dPoint;

  // Inputs
  std::string rpcFile(argv[1]);
  std::string gcpFileName(argv[2]);
  double geoTol(atof(argv[3]));
  // double imgTol(atof(argv[4]));

  // Tools
  auto imgDistance = DistanceType::New();
  auto geoDistance = GeographicalDistanceType::New();
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char* wgsRef = nullptr;
  oSRS.exportToWkt(&wgsRef);
   
  otb::ImageMetadata imd;
  if (0 == rpcFile.compare(rpcFile.length() - 4, 4, "geom"))
  {
    // Fetching the RPC model from a GEOM file
    otb::GeomMetadataSupplier geomSupplier(rpcFile);
    for (int loop = 0 ; loop < geomSupplier.GetNbBands() ; ++loop)
      imd.Bands.emplace_back();
    auto imi = otb::ImageMetadataInterfaceFactory::CreateIMI(imd, geomSupplier);
    imd = imi->GetImageMetadata();
    geomSupplier.FetchRPC(imd);
  }
  else
  {
    // Fetching the RPC model from a product
    using ImageType = otb::Image<double, 2>;
    using ImageFileReaderType = otb::ImageFileReader<ImageType>;
    auto reader = ImageFileReaderType::New();
    reader->SetFileName(rpcFile);
    reader->UpdateOutputInformation();
    imd = reader->GetOutput()->GetImageMetadata();
    imd.Remove(otb::MDGeom::ProjectionWKT);
  }

  // Setting the transforms
  auto ForwardTransform = ForwardTransformType::New();
  ForwardTransform->SetMetadata(imd);
  
  auto InverseTransform = InverseTransformType::New();
  InverseTransform->SetMetadata(imd);
  
  auto GenericRSTransform_img2wgs = GenericRSTransformType::New();
  GenericRSTransform_img2wgs->SetInputProjectionRef("");
  GenericRSTransform_img2wgs->SetOutputProjectionRef(wgsRef);
  GenericRSTransform_img2wgs->SetInputImageMetadata(&imd);
  GenericRSTransform_img2wgs->InstantiateTransform();

  auto GenericRSTransform_wgs2img = GenericRSTransformType::New();
  GenericRSTransform_wgs2img->SetInputProjectionRef(wgsRef);
  GenericRSTransform_wgs2img->SetOutputProjectionRef("");
  GenericRSTransform_wgs2img->SetOutputImageMetadata(&imd);
  GenericRSTransform_wgs2img->InstantiateTransform();
  
  // Loading the GCP
  PointsContainerType pointsContainer;
  PointsContainerType geo3dPointsContainer;
  std::ifstream file(gcpFileName, std::ios::in);
  if (file)
  {
    std::string line;
    while (getline(file, line))
    {
      if (line.find_first_of("#") != 0)
      {
        std::istringstream iss(line);

        iss >> imagePoint[0] >> imagePoint[1] >> geo3dPoint[0] >> geo3dPoint[1] >> geo3dPoint[2];
	imagePoint[2] = geo3dPoint[2];

        pointsContainer.push_back(imagePoint);
        geo3dPointsContainer.push_back(geo3dPoint);
      }
    }
    file.close();
  }

  // For each CGP
  double distance;
  for (PointsContainerType::iterator pointsIt = pointsContainer.begin(), geo3dPointsIt = geo3dPointsContainer.begin() ;
       (pointsIt != pointsContainer.end()) && (geo3dPointsIt != geo3dPointsContainer.end()) ;
       ++pointsIt, ++geo3dPointsIt)
  {
    // Testing forward transform
    // geo3dPoint = ForwardTransform->TransformPoint(*pointsIt);
    // distance = geoDistance->Evaluate(geo3dPoint, *geo3dPointsIt);
    // if (distance > geoTol)
    // {
    //   std::cerr << "Geo distance between ForwardTransform->TransformPoint and GCP too high :\n"
    // 		<< "GCP: " << *geo3dPointsIt << " / computed: " << geo3dPoint << "\n"
    //             << "dist = " << distance << " (tol = " << geoTol << ")" << std::endl;
    //   success = false;
    // }

    // // Testing inverse transform
    // imagePoint = InverseTransform->TransformPoint(*geo3dPointsIt);
    // distance = imgDistance->Evaluate(imagePoint, *pointsIt);
    // if (distance > imgTol)
    // {
    //   std::cerr << "Distance between InverseTransform->TransformPoint and GCP too high :\n"
    // 		<< "GCP: " << *pointsIt << " / computed: " << imagePoint << "\n"
    //             << "dist = " << distance << " (tol = " << imgTol << ")" << std::endl;
    //   success = false;
    // }

    // Testing image to wgs transform
    geo3dPoint = GenericRSTransform_img2wgs->TransformPoint(*pointsIt);
    distance = geoDistance->Evaluate(geo3dPoint, *geo3dPointsIt);
    if (distance > geoTol)
    {
      std::cerr << "Geo distance between GenericRSTransform_img2wgs->TransformPoint and GCP too high :\n"
		<< "GCP: " << *geo3dPointsIt << " / computed: " << geo3dPoint << "\n"
                << "dist = " << distance << " (tol = " << geoTol << ")" << std::endl;
      success = false;
    }

    // Testing wgs to image transform
    // imagePoint = GenericRSTransform_wgs2img->TransformPoint(*geo3dPointsIt);
    // distance = imgDistance->Evaluate(imagePoint, *pointsIt);
    // if (distance > imgTol)
    // {
    //   std::cerr << "Distance between GenericRSTransform_wgs2img->TransformPoint and GCP too high :\n"
    // 		<< "GCP: " << *pointsIt << " / computed: " << imagePoint << "\n"
    //             << "dist = " << distance << " (tol = " << imgTol << ")" << std::endl;
    //   success = false;
    // }
  }
  
  if (success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
