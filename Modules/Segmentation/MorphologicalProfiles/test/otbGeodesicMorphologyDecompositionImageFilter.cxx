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

#include "otbGeodesicMorphologyDecompositionImageFilter.h"
#include "otbGeodesicMorphologyDecompositionImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkBinaryBallStructuringElement.h"

int otbGeodesicMorphologyDecompositionImageFilter(int itkNotUsed(argc), char * argv[])
{

  const char *       infname = argv[1];
  const char *       convexfname = argv[2];
  const char *       concavefname = argv[3];
  const char *       levelingfname = argv[4];
  const unsigned int sesize = atoi(argv[5]);

  typedef otb::Image<double, 2> ImageType;
  typedef itk::BinaryBallStructuringElement<double,
      2>                                                  StructuringElementType;
  typedef StructuringElementType::RadiusType       RadiusType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::GeodesicMorphologyDecompositionImageFilter<ImageType, ImageType,
      StructuringElementType>
  GeodesicMorphologyDecompositionImageFilterType;

  // Instantiating object
  GeodesicMorphologyDecompositionImageFilterType::Pointer filter = GeodesicMorphologyDecompositionImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());

  RadiusType radius;
  radius.Fill(sesize);

  filter->SetRadius(radius);

  // writer the leveling map
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(levelingfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  // write the convex map
  writer = WriterType::New();
  writer->SetFileName(convexfname);
  writer->SetInput(filter->GetConvexMap());
  writer->Update();

  // write the concave map
  writer = WriterType::New();
  writer->SetFileName(concavefname);
  writer->SetInput(filter->GetConcaveMap());
  writer->Update();

  return EXIT_SUCCESS;
}
