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

#include <iostream>


#include "otbAttributesMapLabelObject.h"
#include "otbLabelObjectOpeningMuParserFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"

int otbLabelObjectOpeningMuParserFilterTest(int itkNotUsed(argc), char * argv[])
{

  const char * imageInputFilename  = argv[1];
  const char * labelImageInputFilename  = argv[2];
  const char * outputFilename = argv[3];
  const char * expression =     argv[4];

  typedef float InputPixelType;
  const unsigned int     Dimension = 2;

  typedef unsigned int LabelType;
  typedef otb::Image<LabelType, Dimension>              LabelImageType;
  typedef otb::VectorImage<InputPixelType,  Dimension>  InputVectorImageType;

  typedef otb::ImageFileReader<InputVectorImageType>  ImageReaderType;
  typedef otb::ImageFileReader<LabelImageType>        LabelImageReaderType;
  typedef otb::ImageFileWriter<LabelImageType>        WriterType;

  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>    AttributesMapLabelObjectType;
  typedef itk::LabelMap<AttributesMapLabelObjectType>                    AttributesLabelMapType;
  typedef otb::LabelObjectOpeningMuParserFilter<AttributesLabelMapType>  FilterType;

  typedef itk::LabelImageToLabelMapFilter<LabelImageType, AttributesLabelMapType> LabelImageToLabelMapFilterType;

  typedef otb::ShapeAttributesLabelMapFilter<AttributesLabelMapType>  ShapeLabelMapFilterType;

  typedef otb::BandsStatisticsAttributesLabelMapFilter<AttributesLabelMapType, InputVectorImageType> RadiometricLabelMapFilterType;

  typedef itk::LabelMapToLabelImageFilter<AttributesLabelMapType, LabelImageType> LabelMapToLabelImageFilterType;


  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  LabelImageReaderType::Pointer labelImageReader = LabelImageReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  labelImageReader->SetFileName(labelImageInputFilename);
  labelImageReader->GenerateOutputInformation();

  imageReader->SetFileName(imageInputFilename);
  imageReader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);

  LabelImageToLabelMapFilterType::Pointer labelImageToLabelMap = LabelImageToLabelMapFilterType::New();
  labelImageToLabelMap->SetInput(labelImageReader->GetOutput());
  labelImageToLabelMap->SetBackgroundValue(0);

  // shape attributes computation
  ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(labelImageToLabelMap->GetOutput());
  shapeLabelMapFilter->SetReducedAttributeSet(false);

  // band stat attributes computation
  RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter
  = RadiometricLabelMapFilterType::New();

  radiometricLabelMapFilter->SetInput(shapeLabelMapFilter->GetOutput());

  radiometricLabelMapFilter->SetFeatureImage(imageReader->GetOutput());
  radiometricLabelMapFilter->SetReducedAttributeSet(false);
  radiometricLabelMapFilter->Update();

  AttributesLabelMapType::Pointer AttributesLabelMap;

  AttributesLabelMap=radiometricLabelMapFilter->GetOutput();

  std::string stringExpression(expression);


  filter->SetExpression(stringExpression);
  filter->SetInput(radiometricLabelMapFilter->GetOutput());
  filter->Update();

  LabelMapToLabelImageFilterType::Pointer labelMapToLabelImage = LabelMapToLabelImageFilterType::New();
  labelMapToLabelImage->SetInput(filter->GetOutput());
  labelMapToLabelImage->Update();



  writer->SetInput(labelMapToLabelImage->GetOutput());
  writer->Update();


  return EXIT_SUCCESS;
}
