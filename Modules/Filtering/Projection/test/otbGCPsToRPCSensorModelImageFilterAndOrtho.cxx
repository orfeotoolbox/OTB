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

#include "otbImageFileReader.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
#include "otbImageFileWriter.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbMacro.h"

int otbGCPsToRPCSensorModelImageFilterAndOrtho(int argc, char* argv[])
{
  if (argc < 11)
  {
    std::cerr << "Usage: " << argv[0] << " infname outfname <origin easting> <origin northing> <x size> <y size> <x spacing> <y spacing> <UTM zone> <UTM "
                                         "hemisphere> a1x a1y b1x b1y b1z ... aNx aNy bNx bNy bNz "
              << std::endl;
    return EXIT_FAILURE;
  }
  else if ((argc - 11) % 5 != 0)
  {
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
  }

  const char* infname  = argv[1];
  const char* outfname = argv[2];

  typedef otb::VectorImage<float, 2> ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPsToSensorModelFilterType;
  typedef GCPsToSensorModelFilterType::Point2DType        Point2DType;
  typedef GCPsToSensorModelFilterType::Point3DType        Point3DType;
  typedef otb::ImageFileWriter<ImageType>                 WriterType;
  typedef otb::GenericRSResampleImageFilter<ImageType, ImageType> OrthoRectifFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());

  unsigned int nbGCPs = (argc - 11) / 5;

  otbLogMacro(Info, << "Receiving " << nbGCPs << " from command line.");

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
  {
    Point2DType sensorPoint;
    sensorPoint[0] = atof(argv[11 + gcpId * 5]);
    sensorPoint[1] = atof(argv[12 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = atof(argv[13 + 5 * gcpId]);
    geoPoint[1] = atof(argv[14 + 5 * gcpId]);
    geoPoint[2] = atof(argv[15 + 5 * gcpId]);

    otbLogMacro(Debug, << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint);

    rpcEstimator->AddGCP(sensorPoint, geoPoint);
  }

  rpcEstimator->GetOutput()->UpdateOutputInformation();

  // Set floatfield to format properly
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(10);

  otbLogMacro(Debug, << rpcEstimator->GetOutput()->GetImageKeywordlist());
  otbLogMacro(Info, << "Residual ground error: " << rpcEstimator->GetRMSGroundError());

  // Orthorectify the output image
  WriterType::Pointer writer = WriterType::New();

  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();

  orthoRectifFilter->SetInput(rpcEstimator->GetOutput());

  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  ImageType::SizeType size;
  size[0] = atoi(argv[5]); // X size
  size[1] = atoi(argv[6]); // Y size
  orthoRectifFilter->SetOutputSize(size);

  ImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  ImageType::PointType origin;
  origin[0] = strtod(argv[3], nullptr); // Origin easting
  origin[1] = strtod(argv[4], nullptr); // Origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  std::string wkt =
      otb::SpatialReference::FromUTM(atoi(argv[9]), argv[10][0] == 'N' ? otb::SpatialReference::hemisphere::north : otb::SpatialReference::hemisphere::south)
          .ToWkt();

  orthoRectifFilter->SetOutputProjectionRef(wkt);

  ImageType::PixelType no_data(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  no_data.Fill(0.0);
  orthoRectifFilter->SetEdgePaddingValue(no_data);

  // Displacement Field spacing
  ImageType::SpacingType gridSpacing;
  gridSpacing[0] = 16.0 * atof(argv[7]);
  gridSpacing[1] = 16.0 * atof(argv[8]);

  orthoRectifFilter->SetDisplacementFieldSpacing(gridSpacing);

  writer->SetInput(orthoRectifFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->Update();

  return EXIT_SUCCESS;
}
