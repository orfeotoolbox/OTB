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

#include "otbRadianceToImageImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVariableLengthVector.h"

int otbRadianceToImageImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                            PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                            InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                            OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                              ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                             WriterType;
  typedef otb::RadianceToImageImageFilter<InputImageType, OutputImageType> RadianceToImageImageFilterType;
  typedef RadianceToImageImageFilterType::VectorType                       VectorType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  VectorType alpha(nbOfComponent);
  VectorType beta(nbOfComponent);
  alpha.Fill(0);
  beta.Fill(0);

  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    alpha[i] = static_cast<double>(atof(argv[i + 3]));
    beta[i] = static_cast<double>(atof(argv[i + 7]));
    }

  // Instantiating object
  RadianceToImageImageFilterType::Pointer filter = RadianceToImageImageFilterType::New();
  filter->SetAlpha(alpha);
  filter->SetBeta(beta);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
