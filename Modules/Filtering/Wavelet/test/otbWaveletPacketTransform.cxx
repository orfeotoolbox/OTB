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
#include "otbWaveletPacketTransform.h"

template<otb::Wavelet::Wavelet TWavelet>
int otbWaveletPacketTransform_generic(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFileName = argv[1];
  const char *       outputFileName = argv[2];
  const unsigned int level = atoi(argv[3]);
  const unsigned int decimFactor = atoi(argv[4]);

  const int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  /* Wavelet choice */
  const otb::Wavelet::Wavelet wvltID = TWavelet;

  /* Wavelet packet configuration */
  typedef otb::FullyDecomposedWaveletPacketCost<ImageType> CostType;
  CostType::NumberOfAllowedDecompositions = level;

  /* Forward Transformation */
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::FORWARD, PixelType, Dimension>            WaveletOperator;
  typedef otb::WaveletFilterBank<ImageType, ImageType, WaveletOperator, otb::Wavelet::FORWARD> ForwardFilterBank;
  typedef otb::WaveletPacketTransform<ImageType, ImageType, ForwardFilterBank,
                                      otb::Wavelet::FORWARD, CostType>                         FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetSubsampleImageFactor(decimFactor);
  filter->Update();

  /* Inverse Transformation */
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::INVERSE, PixelType, Dimension>               InvWaveletOperator;
  typedef otb::WaveletFilterBank<ImageType, ImageType, InvWaveletOperator, otb::Wavelet::INVERSE> InvFilterBank;
  typedef otb::WaveletPacketTransform<ImageType, ImageType, InvFilterBank, otb::Wavelet::INVERSE> InvFilterType;

  typename InvFilterType::Pointer invFilter = InvFilterType::New();
  invFilter->SetInput(filter->GetOutput());
  invFilter->SetWaveletPacketRule(filter->GetWaveletPacketRule());
  invFilter->SetSubsampleImageFactor(decimFactor);
  invFilter->Update();

  /* Writing the output */
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(invFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbWaveletPacketTransform(int argc, char * argv[])
{
  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0]
              << "<InputImage> <OutputImage> <level> <decimFactor> <waveletType>" << std::endl;
    return EXIT_FAILURE;
    }
  int waveletType = atoi(argv[5]);

  switch (waveletType)
    {
    case 0:
      return otbWaveletPacketTransform_generic<otb::Wavelet::HAAR> (argc, argv);
      break;
    case 1:
      return otbWaveletPacketTransform_generic<otb::Wavelet::DB4> (argc, argv);
      break;
    case 2:
      return otbWaveletPacketTransform_generic<otb::Wavelet::DB6> (argc, argv);
      break;
    case 3:
      return otbWaveletPacketTransform_generic<otb::Wavelet::DB8> (argc, argv);
      break;
    case 4:
      return otbWaveletPacketTransform_generic<otb::Wavelet::DB12> (argc, argv);
      break;
    case 5:
      return otbWaveletPacketTransform_generic<otb::Wavelet::DB20> (argc, argv);
      break;
    case 6:
      return  otbWaveletPacketTransform_generic<otb::Wavelet::SPLINE_BIORTHOGONAL_2_4> (argc, argv);
      break;
    case 7:
      return otbWaveletPacketTransform_generic<otb::Wavelet::SPLINE_BIORTHOGONAL_4_4> (argc, argv);
      break;
    case 8:
      return otbWaveletPacketTransform_generic<otb::Wavelet::SYMLET8> (argc, argv);
      break;
    default:
      std::cerr << "No more wavelet available\n";
      return EXIT_FAILURE;
    }
}
