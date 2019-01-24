/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"

int otbWaveletFilterBank(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFileName = argv[1];
  const unsigned int decimFactor = atoi(argv[2]); // 1 for multiscale, 2 for multiresolution
  const char *       outputFileName = argv[3];

  const int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  /* Wavelet choice */
  //const otb::Wavelet::Wavelet wvltID = otb::Wavelet::HAAR;
  const otb::Wavelet::Wavelet wvltID = otb::Wavelet::SYMLET8;

  /* Forward Transformation */
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::FORWARD, PixelType, Dimension> WaveletOperator;
  typedef otb::WaveletFilterBank<ImageType, ImageType, WaveletOperator, otb::Wavelet::FORWARD>
  FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetSubsampleImageFactor(decimFactor);

  /* Inverse Transformation */
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::INVERSE, PixelType, Dimension> InvWaveletOperator;
  typedef otb::WaveletFilterBank<ImageType, ImageType, InvWaveletOperator, otb::Wavelet::INVERSE>
  InvFilterType;
  InvFilterType::Pointer invFilter = InvFilterType::New();
  for (unsigned int i = 0; i < filter->GetNumberOfOutputs(); ++i)
    invFilter->SetInput(i, filter->GetOutput(i));
  invFilter->SetSubsampleImageFactor(filter->GetSubsampleImageFactor());

  /* Saving output */
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(invFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
