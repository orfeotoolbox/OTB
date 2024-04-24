/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbSarForwardTransform.h"
#include "otbSarInverseTransform.h"
#include "otbGenericRSTransform.h"
#include "otbGeographicalDistance.h"
#include "itkPoint.h"
#include "itkEuclideanDistanceMetric.h"
#include <string>
#include <vector>
#include <iomanip>

using Point2DType = itk::Point<double, 2>;
using Point3DType = itk::Point<double, 3>;
using Points2DContainerType = std::vector<Point2DType>;
using Points3DContainerType = std::vector<Point3DType>;
using ForwardTransformType = otb::SarForwardTransform<double, 2, 3>;
using InverseTransformType = otb::SarInverseTransform<double, 3, 2>;
using GenericRSTransformType = otb::GenericRSTransform<double, 3, 3>;
using DistanceType = itk::Statistics::EuclideanDistanceMetric<Point2DType>;
using GeographicalDistanceType = otb::GeographicalDistance<Point3DType>;

int otbSarTransformTest(int itkNotUsed(argc), char* argv[])
{
  bool success = true;
  Point2DType imagePoint;
  Point3DType geo3dPoint;

  // Inputs
  std::string sarFile(argv[1]);
  std::string gcpFileName(argv[2]);
  double geoTol(atof(argv[3]));
  double imgTol(atof(argv[4]));

  using ImageType = otb::Image<double, 2>;
  using ImageFileReaderType = otb::ImageFileReader<ImageType>;
  auto reader = ImageFileReaderType::New();
  reader->SetFileName(sarFile);
  reader->UpdateOutputInformation();
  auto imd = reader->GetOutput()->GetImageMetadata();
  imd.Remove(otb::MDGeom::ProjectionWKT);

  // Tools
  auto imgDistance = DistanceType::New();
  auto geoDistance = GeographicalDistanceType::New();
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char* wgsRef = nullptr;
  oSRS.exportToWkt(&wgsRef);

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
  Points2DContainerType pointsContainer;
  Points3DContainerType geo3dPointsContainer;
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

        pointsContainer.push_back(imagePoint);
        geo3dPointsContainer.push_back(geo3dPoint);
      }
    }
    file.close();
  }

  // For each CGP
  double distance;
  auto pointsIt = pointsContainer.begin();
  auto geo3dPointsIt = geo3dPointsContainer.begin();
  for ( ;
       (pointsIt != pointsContainer.end()) && (geo3dPointsIt != geo3dPointsContainer.end()) ;
       ++pointsIt, ++geo3dPointsIt)
  {
    // Testing forward transform
     geo3dPoint = ForwardTransform->TransformPoint(*pointsIt);
     distance = geoDistance->Evaluate(geo3dPoint, *geo3dPointsIt);
     if (distance > geoTol)
     {
       std::cerr << std::fixed << std::setw(15) << std::setprecision(15)
                 << "Geo distance between ForwardTransform->TransformPoint and GCP too high :\n"
                 << "GCP: " << *geo3dPointsIt << " / computed: " << geo3dPoint << "\n"
                 << "dist = " << distance << " (tol = " << geoTol << ")" << std::endl;
       success = false;
     }

     // Testing inverse transform
     imagePoint = InverseTransform->TransformPoint(*geo3dPointsIt);
     distance = imgDistance->Evaluate(imagePoint, *pointsIt);
     if (distance > imgTol)
     {
       std::cerr << std::fixed << std::setw(15) << std::setprecision(15)
                 << "Distance between InverseTransform->TransformPoint and GCP too high :\n"
                 << "GCP: " << *pointsIt << " / computed: " << imagePoint << "\n"
                 << "dist = " << distance << " (tol = " << imgTol << ")" << std::endl;
       success = false;
     }

    // Testing image to wgs transform
    Point3DType gcpPoint;
    gcpPoint[0] = (*pointsIt)[0];
    gcpPoint[1] = (*pointsIt)[1];
    gcpPoint[2] = 0;

    geo3dPoint = GenericRSTransform_img2wgs->TransformPoint(gcpPoint);
    distance = geoDistance->Evaluate(geo3dPoint, *geo3dPointsIt);
    if (distance > geoTol)
    {
      std::cerr << "Geo distance between GenericRSTransform_img2wgs->TransformPoint and GCP too high :\n"
    << "GCP: " << *geo3dPointsIt << " / computed: " << geo3dPoint << "\n"
                << "dist = " << distance << " (tol = " << geoTol << ")" << std::endl;
      success = false;
    }

    // Testing wgs to image transform
     Point3DType image3dPoint = GenericRSTransform_wgs2img->TransformPoint(*geo3dPointsIt);
     using Distance3dType = itk::Statistics::EuclideanDistanceMetric<Point3DType>;
     auto imgDistance3d = Distance3dType::New();
     distance = imgDistance3d->Evaluate(image3dPoint, gcpPoint);
     if (distance > imgTol)
     {
       std::cerr << "Distance between GenericRSTransform_wgs2img->TransformPoint and GCP too high :\n"
        << "GCP: " << *pointsIt << " / computed: " << image3dPoint << "\n"
                 << "dist = " << distance << " (tol = " << imgTol << ")" << std::endl;
       success = false;
     }
  }

  if (success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
