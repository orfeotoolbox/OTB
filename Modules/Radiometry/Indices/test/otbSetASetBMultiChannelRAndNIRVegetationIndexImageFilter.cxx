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

#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

template<class TInputImage, class TOutputImage, class TFunction>
void generic_SetASetBMultiChannelRAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputImage>  ReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;

  typedef otb::MultiChannelRAndNIRIndexImageFilter<TInputImage, TOutputImage, TFunction>
  MultiChannelRAndNIRIndexImageFilterType;

  // Instantiating object
  typename MultiChannelRAndNIRIndexImageFilterType::Pointer filter = MultiChannelRAndNIRIndexImageFilterType::New();
  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();
  double       a(::atof(argv[1]));
  double       b(::atof(argv[2]));
  const char * inputFilename  = argv[3];
  const char * outputFilename = argv[4];
  unsigned int redChannel(::atoi(argv[5]));
  unsigned int nirChannel(::atoi(argv[6]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  filter->SetInput(reader->GetOutput());
  filter->SetRedIndex(redChannel);
  filter->SetNIRIndex(nirChannel);
  filter->GetFunctor().SetA(a);
  filter->GetFunctor().SetB(b);
  writer->SetInput(filter->GetOutput());
  writer->Update();
}

int otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<unsigned char, Dimension> InputImageType;
  typedef otb::Image<double, Dimension>              OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "PVI")
    generic_SetASetBMultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
        otb::Functor::PVI<InputImageType::InternalPixelType,
            InputImageType::InternalPixelType,
            OutputImageType::PixelType> >
      (argc, argv);

  return EXIT_SUCCESS;
}
