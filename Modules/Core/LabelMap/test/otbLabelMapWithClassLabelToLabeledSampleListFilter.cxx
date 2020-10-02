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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkBinaryImageToLabelMapFilter.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "otbShapeAttributesLabelMapFilter.h"

#include "itkVariableLengthVector.h"
#include "itkListSample.h"

#include "otbLabelMapWithClassLabelToLabeledSampleListFilter.h"

int otbLabelMapWithClassLabelToLabeledSampleListFilter(int argc, char* argv[])
{
  const char* infname = argv[1];

  // Labeled image type
  const unsigned int     Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> LabeledImageType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledReaderType;
  typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, Dimension, double, LabelType> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType> LabelMapType;
  typedef itk::BinaryImageToLabelMapFilter<LabeledImageType, LabelMapType> LabelMapFilterType;
  typedef otb::ShapeAttributesLabelMapFilter<LabelMapType> ShapeLabelMapFilterType;


  typedef itk::VariableLengthVector<double> VectorType;
  typedef itk::FixedArray<LabelType, 1> TrainingVectorType;
  typedef itk::Statistics::ListSample<VectorType>         ListSampleType;
  typedef itk::Statistics::ListSample<TrainingVectorType> TrainingListSampleType;
  typedef otb::LabelMapWithClassLabelToLabeledSampleListFilter<LabelMapType, ListSampleType, TrainingListSampleType> LabelMap2ListSampleFilterType;

  // instantiation
  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  lreader->SetFileName(infname);

  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(lreader->GetOutput());
  labelMapFilter->SetInputForegroundValue(1);

  ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(labelMapFilter->GetOutput());
  shapeLabelMapFilter->Update();

  LabelMap2ListSampleFilterType::Pointer filter = LabelMap2ListSampleFilterType::New();

  filter->SetInputLabelMap(shapeLabelMapFilter->GetOutput());

  filter->GetMeasurementFunctor().AddAttribute("test");
  filter->GetMeasurementFunctor().RemoveAttribute("test");
  filter->GetMeasurementFunctor().AddAttribute("test");
  filter->GetMeasurementFunctor().ClearAttributes();

  for (int i = 2; i < argc; ++i)
  {
    filter->GetMeasurementFunctor().AddAttribute(argv[i]);
  }

  std::cout << "Number of attributes: " << filter->GetMeasurementFunctor().GetNumberOfAttributes() << std::endl;

  shapeLabelMapFilter->GetOutput()->GetLabelObject(1)->SetClassLabel(1);

  filter->Update();

  return EXIT_SUCCESS;
}
