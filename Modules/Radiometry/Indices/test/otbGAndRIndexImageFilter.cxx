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

#include "otbGAndRIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

template<class TInputGImage, class TInputRImage, class TOutputImage, class TFunction>
int generic_GAndRIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputGImage> GReaderType;
  typedef otb::ImageFileReader<TInputRImage> RReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;

  typedef otb::GAndRIndexImageFilter<TInputGImage, TInputRImage, TOutputImage, TFunction>
  GAndRIndexImageFilterType;

  // Instantiating object
  typename GAndRIndexImageFilterType::Pointer filter = GAndRIndexImageFilterType::New();
  typename GReaderType::Pointer readerG = GReaderType::New();
  typename RReaderType::Pointer readerR = RReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  const char * inputFilenameG  = argv[1];
  const char * inputFilenameR  = argv[2];
  const char * outputFilename = argv[3];

  readerG->SetFileName(inputFilenameG);
  readerR->SetFileName(inputFilenameR);
  writer->SetFileName(outputFilename);
  filter->SetInputG(readerG->GetOutput());
  filter->SetInputR(readerR->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbGAndRIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                    PixelType;
  typedef otb::Image<PixelType, Dimension> InputGImageType;
  typedef otb::Image<PixelType, Dimension> InputRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "IR")
    return (generic_GAndRIndexImageFilter<InputGImageType, InputRImageType, OutputImageType,
                otb::Functor::IR<InputRImageType::PixelType,
                    InputRImageType::PixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else if (strArgv == "IC")
    return (generic_GAndRIndexImageFilter<InputGImageType, InputRImageType, OutputImageType,
                otb::Functor::IC<InputGImageType::PixelType,
                    InputRImageType::PixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else if (strArgv == "IB")
    return (generic_GAndRIndexImageFilter<InputGImageType, InputRImageType, OutputImageType,
                otb::Functor::IB<InputGImageType::PixelType,
                    InputRImageType::PixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
