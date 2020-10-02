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


#include "otbOGRDataToClassStatisticsFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include <fstream>


int otbOGRDataToClassStatisticsFilter(int argc, char* argv[])
{
  typedef otb::VectorImage<float>   InputImageType;
  typedef otb::Image<unsigned char> MaskImageType;
  typedef otb::OGRDataToClassStatisticsFilter<InputImageType, MaskImageType> FilterType;

  if (argc < 3)
  {
    std::cout << "Usage : " << argv[0] << " input_vector  output_stats" << std::endl;
  }

  std::string vectorPath(argv[1]);
  std::string outputPath(argv[2]);

  otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(vectorPath);

  InputImageType::RegionType region;
  region.SetSize(0, 99);
  region.SetSize(1, 50);

  InputImageType::PointType origin;
  origin.Fill(0.5);

  InputImageType::SpacingType spacing;
  spacing[0] = 1.0;
  spacing[1] = -1.0;

  InputImageType::PixelType pixel(3);
  pixel.Fill(1);

  InputImageType::Pointer inputImage = InputImageType::New();
  inputImage->SetNumberOfComponentsPerPixel(3);
  inputImage->SetLargestPossibleRegion(region);
  inputImage->SetOrigin(origin);
  inputImage->SetSignedSpacing(spacing);
  // Don't allocate the input image, the filter should not need it
  // inputImage->Allocate();
  // inputImage->FillBuffer(pixel);

  MaskImageType::Pointer mask = MaskImageType::New();
  mask->SetRegions(region);
  mask->SetOrigin(origin);
  mask->SetSignedSpacing(spacing);
  mask->Allocate();
  itk::ImageRegionIterator<MaskImageType> it(mask, region);
  unsigned int                            count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it, ++count)
  {
    it.Set(count % 2);
  }

  std::string fieldName("Label");

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(inputImage);
  filter->SetMask(mask);
  filter->SetOGRData(vectors);
  filter->SetFieldName(fieldName);
  filter->SetLayerIndex(0);

  filter->Update();

  FilterType::ClassCountMapType&                 classCount = filter->GetClassCountOutput()->Get();
  FilterType::PolygonSizeMapType&                polySize   = filter->GetPolygonSizeOutput()->Get();
  FilterType::ClassCountMapType::const_iterator  itClass;
  FilterType::PolygonSizeMapType::const_iterator itPoly;

  std::ofstream ofs;
  ofs.open(outputPath);
  ofs << "# Layer 0 : polygons" << std::endl;
  ofs << "# Class sample counts" << std::endl;
  for (itClass = classCount.begin(); itClass != classCount.end(); ++itClass)
  {
    ofs << "class " << itClass->first << " = " << itClass->second << std::endl;
  }
  ofs << "# Vector sizes" << std::endl;
  for (itPoly = polySize.begin(); itPoly != polySize.end(); ++itPoly)
  {
    ofs << "feature " << itPoly->first << " = " << itPoly->second << std::endl;
  }

  filter->SetLayerIndex(1);
  filter->Update();

  ofs << "# Layer 1 : lines" << std::endl;
  ofs << "# Class sample counts" << std::endl;
  for (itClass = classCount.begin(); itClass != classCount.end(); ++itClass)
  {
    ofs << "class " << itClass->first << " = " << itClass->second << std::endl;
  }
  ofs << "# Vector sizes" << std::endl;
  for (itPoly = polySize.begin(); itPoly != polySize.end(); ++itPoly)
  {
    ofs << "feature " << itPoly->first << " = " << itPoly->second << std::endl;
  }

  filter->SetLayerIndex(2);
  filter->Update();

  ofs << "# Layer 2 : points" << std::endl;
  ofs << "# Class sample counts" << std::endl;
  for (itClass = classCount.begin(); itClass != classCount.end(); ++itClass)
  {
    ofs << "class " << itClass->first << " = " << itClass->second << std::endl;
  }
  ofs << "# Vector sizes" << std::endl;
  for (itPoly = polySize.begin(); itPoly != polySize.end(); ++itPoly)
  {
    ofs << "feature " << itPoly->first << " = " << itPoly->second << std::endl;
  }

  ofs.close();
  return EXIT_SUCCESS;
}
