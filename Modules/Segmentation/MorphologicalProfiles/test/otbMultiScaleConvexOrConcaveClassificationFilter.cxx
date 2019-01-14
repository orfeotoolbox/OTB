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

#include "otbMorphologicalOpeningProfileFilter.h"
#include "otbMorphologicalClosingProfileFilter.h"
#include "otbProfileToProfileDerivativeFilter.h"
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.h"
#include "otbMultiScaleConvexOrConcaveClassificationFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbMultiScaleConvexOrConcaveClassificationFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilename = argv[2];
  const unsigned int profileSize = atoi(argv[3]);
  const unsigned int initialValue = atoi(argv[4]);
  const unsigned int step = atoi(argv[5]);
  const double       sigma = atof(argv[6]);

  const unsigned int Dimension = 2;
  typedef double         InputPixelType;
  typedef double         OutputPixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<LabeledPixelType, 2>        LabeledImageType;

  typedef otb::ImageFileReader<InputImageType>            ReaderType;
  typedef otb::ImageFileWriter<LabeledImageType> LabeledWriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningProfileFilterType;
  typedef otb::MorphologicalClosingProfileFilter<InputImageType, InputImageType, StructuringElementType>
  ClosingProfileFilterType;
  typedef otb::ProfileToProfileDerivativeFilter<InputImageType, InputImageType> DerivativeFilterType;
  typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<InputImageType, OutputImageType, LabeledImageType>
  MultiScaleCharacteristicsFilterType;
  typedef otb::MultiScaleConvexOrConcaveClassificationFilter<InputImageType,
      LabeledImageType> MultiScaleClassificationFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  OpeningProfileFilterType::Pointer oprofileFilter = OpeningProfileFilterType::New();
  oprofileFilter->SetInput(reader->GetOutput());
  oprofileFilter->SetProfileSize(profileSize);
  oprofileFilter->SetInitialValue(initialValue);
  oprofileFilter->SetStep(step);

  ClosingProfileFilterType::Pointer cprofileFilter = ClosingProfileFilterType::New();
  cprofileFilter->SetInput(reader->GetOutput());
  cprofileFilter->SetProfileSize(profileSize);
  cprofileFilter->SetInitialValue(initialValue);
  cprofileFilter->SetStep(step);

  DerivativeFilterType::Pointer oderivativeFilter = DerivativeFilterType::New();
  oderivativeFilter->SetInput(oprofileFilter->GetOutput());

  DerivativeFilterType::Pointer cderivativeFilter = DerivativeFilterType::New();
  cderivativeFilter->SetInput(cprofileFilter->GetOutput());

  MultiScaleCharacteristicsFilterType::Pointer omsCharFilter = MultiScaleCharacteristicsFilterType::New();
  omsCharFilter->SetInput(oderivativeFilter->GetOutput());
  omsCharFilter->SetInitialValue(initialValue);
  omsCharFilter->SetStep(step);

  MultiScaleCharacteristicsFilterType::Pointer cmsCharFilter = MultiScaleCharacteristicsFilterType::New();
  cmsCharFilter->SetInput(cderivativeFilter->GetOutput());
  cmsCharFilter->SetInitialValue(initialValue);
  cmsCharFilter->SetStep(step);

  MultiScaleClassificationFilterType::Pointer classificationFilter = MultiScaleClassificationFilterType::New();
  using namespace otb::Functor::MultiScaleConvexOrConcaveDecisionRule_tags;
  classificationFilter->SetVariadicNamedInput<max_opening_profile_derivative>(omsCharFilter->GetOutput());
  classificationFilter->SetVariadicNamedInput<opening_profile_characteristics>(omsCharFilter->GetOutputCharacteristics());
  classificationFilter->SetVariadicNamedInput<max_closing_profile_derivative>(cmsCharFilter->GetOutput());
  classificationFilter->SetVariadicNamedInput<closing_profile_characteristics>(cmsCharFilter->GetOutputCharacteristics());
  classificationFilter->GetModifiableFunctor().SetSigma(sigma);
  classificationFilter->GetModifiableFunctor().SetLabelSeparator(initialValue + profileSize * step);

  LabeledWriterType::Pointer labeledWriter = LabeledWriterType::New();

  labeledWriter->SetFileName(outputFilename);
  labeledWriter->SetInput(classificationFilter->GetOutput());
  labeledWriter->Update();

  return EXIT_SUCCESS;
}
