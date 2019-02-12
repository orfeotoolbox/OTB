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

#include "itkMacro.h"

#include "otbSpatialObjectToImageDrawingFilter.h"
#include "itkEllipseSpatialObject.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbSpatialObjectToImageDrawingFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       outputFilename = argv[1];
  const double       radiusx = atof(argv[2]);
  const double       radiusy = atof(argv[3]);
  const unsigned int Dimension = 2;
  typedef unsigned char                                                        PixelType;
  typedef otb::Image<PixelType, Dimension>                                     ImageType;
  typedef otb::ImageFileWriter<ImageType>                                      WriterType;
  typedef itk::EllipseSpatialObject<Dimension>                                 SpatialObjectType;
  typedef otb::SpatialObjectToImageDrawingFilter<SpatialObjectType, ImageType> SpatialObjectToImageDrawingFilterType;
  // Instantiating object
  SpatialObjectToImageDrawingFilterType::Pointer filter = SpatialObjectToImageDrawingFilterType::New();
  WriterType::Pointer                            writer = WriterType::New();

  SpatialObjectType::Pointer   ellipse = SpatialObjectType::New();
  SpatialObjectType::ArrayType radius;
  radius[0] = radiusx;
  radius[1] = radiusy;

  ellipse->SetRadius(radius);

  ImageType::PointType origin;
  origin[0] = -radiusx;
  origin[1] = -radiusy;
  filter->SetInput(ellipse);
  filter->SetOrigin(origin);
  filter->SetInsideValue(255);
  filter->SetOutsideValue(0);

  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outputFilename);
  writer->Update();

  return EXIT_SUCCESS;
}
