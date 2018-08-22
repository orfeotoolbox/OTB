/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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



#include <iostream>
#include <typeinfo>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSinclairImageFilter.h"
#include "otbSinclairToCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToCoherencyMatrixFunctor.h"
#include "otbSinclairToMuellerMatrixFunctor.h"
#include "otbMultiChannelExtractROI.h"




template<class TInputPixel, class TOutputPixel, class TFunction>
int generic_SinclairImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * outputFilename = argv[4];

  typedef otb::VectorImage<TOutputPixel> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * inputFilename3  = argv[3];

  typedef otb::Image<TInputPixel> InputImageType;
  typedef otb::VectorImage<TOutputPixel> OutputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::MultiChannelExtractROI<TOutputPixel, TOutputPixel > ExtractROIType;
  typedef otb::SinclairImageFilter<InputImageType, InputImageType, InputImageType, InputImageType, OutputImageType, TFunction> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  typename ReaderType::Pointer reader1 = ReaderType::New();
  typename ReaderType::Pointer reader2 = ReaderType::New();
  typename ReaderType::Pointer reader3 = ReaderType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);
  filter->SetInputHH(reader1->GetOutput());
  filter->SetInputHV(reader2->GetOutput());
  filter->SetInputVH(reader2->GetOutput());
  filter->SetInputVV(reader3->GetOutput());

  filter->UpdateOutputInformation();

  typename ExtractROIType::Pointer  extract = ExtractROIType::New();
  extract->SetStartX(10);
  extract->SetStartY(10);
  extract->SetSizeX(30);
  extract->SetSizeY(30);
  extract->SetInput(filter->GetOutput());

  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);

  writer->SetInput( extract->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}

int otbSinclairImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef std::complex <double> InputPixelType;
  typedef std::complex <double> OutputPixelType;
  typedef double                OutputRealPixelType;

  typedef otb::Image<InputPixelType,  Dimension>       InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;
  typedef otb::VectorImage<OutputRealPixelType, Dimension> OutputRealImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "SinclairToCovarianceMatrix")
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCircularCovarianceMatrix")
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCircularCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCoherencyMatrix")
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCoherencyMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToMuellerMatrix")
    return (generic_SinclairImageFilter<InputPixelType, OutputRealPixelType,
                otb::Functor::SinclairToMuellerMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputRealImageType::PixelType> >
                  (argc, argv));
  else return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
