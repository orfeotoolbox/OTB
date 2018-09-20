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
#include "otbSinclairReciprocalImageFilter.h"
#include "otbSinclairToReciprocalCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToReciprocalCoherencyMatrixFunctor.h"
#include "otbMultiChannelExtractROI.h"




template<class TInputPixel, class TOutputPixel, class TFunction>
int generic_SinclairReciprocalImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename1 = argv[1];
  const char * inputFilename2 = argv[2];
  const char * inputFilename3 = argv[3];
  const char * outputFilename = argv[4];

  typedef otb::Image<TInputPixel> InputImageType;
  typedef otb::VectorImage<TOutputPixel> OutputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::MultiChannelExtractROI<TOutputPixel, TOutputPixel > ExtractROIType;
  typedef otb::SinclairReciprocalImageFilter<InputImageType, InputImageType, InputImageType, OutputImageType, TFunction> FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  typename ReaderType::Pointer reader1 = ReaderType::New();
  typename ReaderType::Pointer reader2 = ReaderType::New();
  typename ReaderType::Pointer reader3 = ReaderType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);

  filter->SetInputHH(reader1->GetOutput());
  filter->SetInputHV_VH(reader2->GetOutput());
  filter->SetInputVV(reader3->GetOutput());

  typename ExtractROIType::Pointer  extract = ExtractROIType::New();
  extract->SetStartX(10);
  extract->SetStartY(10);
  extract->SetSizeX(30);
  extract->SetSizeY(30);
  extract->SetInput( filter->GetOutput() );

  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);

  writer->SetInput( extract->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}


int otbSinclairReciprocalImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef std::complex <double> InputPixelType;
  typedef std::complex <double> OutputPixelType;

  typedef otb::Image<InputPixelType,  Dimension>       InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "SinclairToReciprocalCovarianceMatrix")
    return (generic_SinclairReciprocalImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToReciprocalCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToReciprocalCircularCovarianceMatrix")
    return (generic_SinclairReciprocalImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToReciprocalCoherencyMatrix")
    return (generic_SinclairReciprocalImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
