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


#include <fstream>
#include <iomanip>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbGenericRSTransform.h"
#include <ogr_spatialref.h>
#include "itkEuclideanDistanceMetric.h"
#include "otbGeographicalDistance.h"
#include "otbDEMHandler.h"

typedef otb::VectorImage<double, 2> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::GenericRSTransform<>       TransformType;
typedef TransformType::InputPointType   PointType;

typedef otb::GenericRSTransform<double, 3, 3> Transform3DType;
typedef Transform3DType::InputPointType Point3DType;

typedef itk::Statistics::EuclideanDistanceMetric<ImageType::PointType> DistanceType;
typedef itk::Statistics::EuclideanDistanceMetric<Point3DType>          Distance3DType;
typedef otb::GeographicalDistance<ImageType::PointType>                GeoDistanceType;
typedef otb::GeographicalDistance<Point3DType>                         GeoDistance3DType;

int otbGenericRSTransformFromImage(int itkNotUsed(argc), char* argv[])
{
  /*
   * This test checks if we can instantiate a GenericRSTransform from the image information
   * without throwing an exception
   */

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char* wgsRef = nullptr;
  oSRS.exportToWkt(&wgsRef);

  // Instantiate WGS->Image transform
  TransformType::Pointer wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->InstantiateTransform();

  // Instantiate Image->WGS transform
  TransformType::Pointer img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->InstantiateTransform();

  return EXIT_SUCCESS;
}

int otbGenericRSTransformImageAndMNTToWGS84ConversionChecking(int itkNotUsed(argc), char* argv[])
{
  std::string infname = argv[1];
  std::string demdir  = argv[2];

  ImageType::PointType refImgPt, refGeoPt, estimatedImgPt, estimatedGeoPt;
  refImgPt[0]      = atof(argv[4]);
  refImgPt[1]      = atof(argv[5]);
  refGeoPt[0]      = atof(argv[6]);
  refGeoPt[1]      = atof(argv[7]);
  double refHeight = atof(argv[8]);
  double imgTol    = atof(argv[9]);
  double geoTol    = atof(argv[10]);

  Point3DType refImgPt3d, refGeoPt3d, estimatedImgPt3d, estimatedGeoPt3d;
  refImgPt3d[0] = refImgPt[0];
  refImgPt3d[1] = refImgPt[1];
  refGeoPt3d[0] = refGeoPt[0];
  refGeoPt3d[1] = refGeoPt[1];

  bool pass = true;

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char* wgsRef = nullptr;
  oSRS.exportToWkt(&wgsRef);

  DistanceType::Pointer   distance   = DistanceType::New();
  Distance3DType::Pointer distance3d = Distance3DType::New();

  GeoDistanceType::Pointer   geoDistance   = GeoDistanceType::New();
  GeoDistance3DType::Pointer geoDistance3d = GeoDistance3DType::New();

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  demHandler->OpenDEMDirectory(argv[2]);
  demHandler->OpenGeoidFile(argv[3]);
  double heightAboveEllipsoid = demHandler->GetHeightAboveEllipsoid(refGeoPt);

  // Instantiate WGS->Image transform
  TransformType::Pointer wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->InstantiateTransform();

  // Instantiate Image->WGS transform
  TransformType::Pointer img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->InstantiateTransform();


  // Instantiate WGS->Image transform 3D
  Transform3DType::Pointer wgs2img3d = Transform3DType::New();
  wgs2img3d->SetInputProjectionRef(wgsRef);
  wgs2img3d->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img3d->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img3d->InstantiateTransform();

  // Instantiate Image->WGS transform 3D
  Transform3DType::Pointer img2wgs3d = Transform3DType::New();
  img2wgs3d->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs3d->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs3d->SetOutputProjectionRef(wgsRef);
  img2wgs3d->InstantiateTransform();


  std::cout << std::setprecision(8) << std::endl;

  std::cout << "References: " << std::endl;
  std::cout << "Geo (wgs84): " << refGeoPt << std::endl;
  std::cout << "Image: " << refImgPt << std::endl;
  std::cout << "Height: " << refHeight << std::endl;

  std::cout << "-------------------- " << std::endl;
  std::cout << "Using geoid and height above ellipsoid from " << demdir << std::endl;

  estimatedImgPt = wgs2img->TransformPoint(refGeoPt);
  std::cout << "Inverse(refGeo): " << refGeoPt << " -> " << estimatedImgPt << std::endl;

  double imgRes = distance->Evaluate(refImgPt, estimatedImgPt);
  std::cout << "Residual: " << imgRes << " pixels" << std::endl;

  if (imgRes > imgTol)
  {
    pass = false;
    std::cerr << "Image residual is too high: " << imgRes << " pixels" << std::endl;
  }


  estimatedGeoPt = img2wgs->TransformPoint(refImgPt);
  std::cout << "Forward(refImg): " << refImgPt << " -> " << estimatedGeoPt << std::endl;

  double geoRes = geoDistance->Evaluate(refGeoPt, estimatedGeoPt);
  std::cout << "Residual: " << geoRes << " meters" << std::endl;

  if (geoRes > geoTol)
  {
    pass = false;
    std::cerr << "Geographic (WGS84) residual is too high: " << geoRes << " meters" << std::endl;
  }

  std::cout << "-------------------- " << std::endl;
  std::cout << "Using reference elevation: " << std::endl;

  refImgPt3d[2] = refHeight;
  refGeoPt3d[2] = refHeight;

  estimatedGeoPt3d = img2wgs3d->TransformPoint(refImgPt3d);
  std::cout << "Forward(refImg): " << refImgPt3d << " -> " << estimatedGeoPt3d << std::endl;
  geoRes = geoDistance3d->Evaluate(refGeoPt3d, estimatedGeoPt3d);
  std::cout << "Residual: " << geoRes << " meters" << std::endl;

  if (geoRes > geoTol)
  {
    pass = false;
    std::cerr << "Geographic (WGS84) residual is too high: " << geoRes << " meters" << std::endl;
  }

  estimatedImgPt3d = wgs2img3d->TransformPoint(refGeoPt3d);
  std::cout << "Inverse(refGeo): " << refGeoPt3d << " -> " << estimatedImgPt3d << std::endl;
  imgRes = distance3d->Evaluate(refImgPt3d, estimatedImgPt3d);
  std::cout << "Residual: " << imgRes << " pixels" << std::endl;

  if (imgRes > imgTol)
  {
    pass = false;
    std::cerr << "Image residual is too high: " << imgRes << " pixels" << std::endl;
  }

  std::cout << "-------------------- " << std::endl;
  std::cout << "Using fixed height above ellipsoid: " << heightAboveEllipsoid << std::endl;

  refImgPt3d[2] = heightAboveEllipsoid;
  refGeoPt3d[2] = heightAboveEllipsoid;

  estimatedGeoPt3d = img2wgs3d->TransformPoint(refImgPt3d);
  std::cout << "Forward(refImg): " << refImgPt3d << " -> " << estimatedGeoPt3d << std::endl;
  geoRes = geoDistance3d->Evaluate(refGeoPt3d, estimatedGeoPt3d);
  std::cout << "Residual: " << geoRes << " meters" << std::endl;

  if (geoRes > geoTol)
  {
    pass = false;
    std::cerr << "Geographic (WGS84) residual is too high: " << geoRes << " meters" << std::endl;
  }

  estimatedImgPt3d = wgs2img3d->TransformPoint(refGeoPt3d);
  std::cout << "Inverse(refGeo): " << refGeoPt << " -> " << estimatedImgPt << std::endl;
  imgRes = distance3d->Evaluate(refImgPt3d, estimatedImgPt3d);
  std::cout << "Residual: " << imgRes << " pixels" << std::endl;

  if (imgRes > imgTol)
  {
    pass = false;
    std::cerr << "Image residual is too high: " << imgRes << " pixels" << std::endl;
  }

  if (pass)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    std::cerr << "There were imprecise results." << std::endl;
    return EXIT_FAILURE;
  }
}
