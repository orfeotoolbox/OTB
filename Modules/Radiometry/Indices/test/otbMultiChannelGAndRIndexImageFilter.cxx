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

#include "otbMultiChannelGAndRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndicesFunctor.h"

template<class TInputImage, class TOutputImage, class TFunction>
int generic_MultiChannelGAndRIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputImage>  ReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;

  typedef otb::MultiChannelGAndRIndexImageFilter<TInputImage, TOutputImage, TFunction>
  MultiChannelGAndRIndexImageFilterType;

  // Instantiating object
  typename MultiChannelGAndRIndexImageFilterType::Pointer filter = MultiChannelGAndRIndexImageFilterType::New();
  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  unsigned int greenChannel(::atoi(argv[3]));
  unsigned int redChannel(::atoi(argv[4]));
  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  filter->SetGreenIndex(greenChannel);
  filter->SetRedIndex(redChannel);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbMultiChannelGAndRIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<unsigned char, Dimension> InputImageType;
  typedef otb::Image<double, Dimension>              OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "IR")
    return (generic_MultiChannelGAndRIndexImageFilter<InputImageType, OutputImageType,
                otb::Functor::IR<InputImageType::InternalPixelType,
                    InputImageType::InternalPixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else if (strArgv == "IC")
    return (generic_MultiChannelGAndRIndexImageFilter<InputImageType, OutputImageType,
                otb::Functor::IC<InputImageType::InternalPixelType,
                    InputImageType::InternalPixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else if (strArgv == "IB")
    return (generic_MultiChannelGAndRIndexImageFilter<InputImageType, OutputImageType,
                otb::Functor::IB<InputImageType::InternalPixelType,
                    InputImageType::InternalPixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
