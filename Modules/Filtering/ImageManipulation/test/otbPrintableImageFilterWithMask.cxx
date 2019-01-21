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



#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"

#include "otbPrintableImageFilter.h"

int otbPrintableImageFilterWithMask(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * masktFilename  = argv[2];
  const char * outputFilename = argv[3];

  typedef double InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::PrintableImageFilter<InputImageType>    FilterType;
  typedef FilterType::OutputImageType                  OutputImageType;
  typedef OutputImageType::PixelType                   OutputPixelType;
  typedef FilterType::MaskImageType                    MaskImageType;
  typedef FilterType::MaskPixelType                    MaskPixelType;

  typedef otb::ImageFileReader<InputImageType>           InputReaderType;
  typedef otb::ImageFileReader<MaskImageType>            MaskReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  FilterType::Pointer      printableImageFilter = FilterType::New();
  InputReaderType::Pointer inputReader          = InputReaderType::New();
  MaskReaderType::Pointer  maskReader           = MaskReaderType::New();
  WriterType::Pointer      writer               = WriterType::New();

  inputReader->SetFileName(inputFilename);
  maskReader->SetFileName(masktFilename);
  maskReader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);

  printableImageFilter->SetInput(inputReader->GetOutput());
  printableImageFilter->SetInputMask(maskReader->GetOutput());

  FilterType::ChannelsType chList;
  chList.push_back(3);
  chList.push_back(2);
  chList.push_back(1);
  printableImageFilter->SetChannelList(chList);

  OutputPixelType objectColor;
  objectColor.SetSize(3);
  objectColor[0] = 0;
  objectColor[1] = 255;
  objectColor[2] = 0;
  printableImageFilter->SetObjectColor(objectColor);
  printableImageFilter->SetBackgroundMaskValue(static_cast<MaskPixelType>(atof(argv[4])));

  writer->SetInput(printableImageFilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
