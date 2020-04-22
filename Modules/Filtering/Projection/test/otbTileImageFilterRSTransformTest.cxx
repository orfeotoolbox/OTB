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

#include "otbImage.h"
#include "otbTileImageFilter.h"
#include "otbImageFileReader.h"
#include "otbGeographicalDistance.h"
#include "otbGenericRSTransform.h"
#include <iomanip>

typedef otb::Image<unsigned int>             ImageType;
typedef otb::TileImageFilter<ImageType>      TileImageFilterType;
typedef otb::ImageFileReader<ImageType>      ImageReaderType;
typedef otb::GenericRSTransform<>            RSTransformType;
typedef ImageType::PointType                 PointType;
typedef otb::GeographicalDistance<PointType> GeographicalDistanceType;

int otbTileImageFilterRSTransformTest(int argc, char* argv[])
{
  TileImageFilterType::SizeType layout;
  layout[0] = atoi(argv[1]);
  layout[1] = atoi(argv[2]);

  double threshold = 1e-20;

  unsigned int numberOfImages = layout[0] * layout[1];

  std::vector<ImageReaderType::Pointer> readers;
  std::vector<PointType>                points;
  std::vector<RSTransformType::Pointer> transforms;

  TileImageFilterType::Pointer tileFilter = TileImageFilterType::New();
  tileFilter->SetLayout(layout);

  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(0);

  for (unsigned int i = 0; i < numberOfImages; ++i)
  {
    if (i + 3 > (unsigned int)argc)
    {
      std::cerr << "Not enough images to support layout!" << std::endl;
      return EXIT_FAILURE;
    }

    // Read tile
    ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName(argv[i + 3]);
    reader->UpdateOutputInformation();
    readers.push_back(reader);

    // Compute tile center
    PointType center = reader->GetOutput()->GetOrigin();
    center[0] += reader->GetOutput()->GetSignedSpacing()[0] * reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0] / 2;
    center[1] += reader->GetOutput()->GetSignedSpacing()[1] * reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1] / 2;
    points.push_back(center);

    // Set-up transform
    RSTransformType::Pointer rsTransform = RSTransformType::New();
    rsTransform->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
    rsTransform->InstantiateTransform();
    transforms.push_back(rsTransform);

    tileFilter->SetInput(i, reader->GetOutput());
  }

  tileFilter->GetOutput()->UpdateOutputInformation();

  // Build RS transform for tiled image
  RSTransformType::Pointer mosaicRsTransform = RSTransformType::New();
  mosaicRsTransform->SetInputKeywordList(tileFilter->GetOutput()->GetImageKeywordlist());
  mosaicRsTransform->InstantiateTransform();

  // Check that individual RSTransform gives the same result as tiled
  // transform

  GeographicalDistanceType::Pointer distance = GeographicalDistanceType::New();

  std::cout << std::setprecision(10) << std::endl;

  for (unsigned int i = 0; i < numberOfImages; ++i)
  {
    PointType tilePoint   = transforms.at(i)->TransformPoint(points.at(i));
    PointType mosaicPoint = mosaicRsTransform->TransformPoint(points.at(i));

    double distValue = distance->Evaluate(tilePoint, mosaicPoint);

    std::cout << "Tile " << i << ":" << std::endl;
    std::cout << "Image point: " << points.at(i) << std::endl;
    std::cout << "Ground point from tile:   " << tilePoint << std::endl;
    std::cout << "Ground point from mosaic: " << mosaicPoint << std::endl;
    std::cout << "Distance: " << distValue << std::endl;

    if (distValue > threshold)
    {

      std::cerr << "Projection consistency problem in tile " << argv[i + 3] << std::endl;
      return EXIT_FAILURE;
    }
  }


  return EXIT_SUCCESS;
}
