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



#include "itkMacro.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkMeanImageFilter.h"

#include "otbPerBandVectorImageFilter.h"

#include "otbReciprocalHuynenDecompImageFilter.h"
#include "otbSinclairToReciprocalCovarianceMatrixImageFilter.h"

int otbReciprocalHuynenDecompImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilenameHH = argv[1];
  const char * inputFilenameHV = argv[2];
  const char * inputFilenameVV = argv[3];
  int size = atoi(argv[4]);
  const char * outputFilename = argv[5];


  typedef std::complex<double>  ComplexPixelType;
  const unsigned int Dimension = 2;


  typedef otb::Image<ComplexPixelType, Dimension>  	   ComplexImageType;
  typedef otb::VectorImage<ComplexPixelType, Dimension>  ComplexVectorImageType;


  typedef otb::ImageFileReader<ComplexImageType>  ReaderType;
  typedef otb::ImageFileWriter<ComplexVectorImageType> WriterType;

  using SinclairToCovFilterType = otb::SinclairToReciprocalCovarianceMatrixImageFilter<ComplexImageType, ComplexVectorImageType>;

  typedef itk::MeanImageFilter<ComplexImageType, ComplexImageType>         MeanFilterType;
  typedef otb::PerBandVectorImageFilter<ComplexVectorImageType, ComplexVectorImageType, MeanFilterType> PerBandMeanFilterType;

  typedef otb::ReciprocalHuynenDecompImageFilter<ComplexVectorImageType, ComplexVectorImageType> FilterType;



  ReaderType::Pointer readerHH = ReaderType::New();
  ReaderType::Pointer readerHV = ReaderType::New();
  ReaderType::Pointer readerVV = ReaderType::New();

  WriterType::Pointer writer = WriterType::New();

  SinclairToCovFilterType::Pointer sinclairtocov = SinclairToCovFilterType::New();
  PerBandMeanFilterType::Pointer perBand = PerBandMeanFilterType::New();
  FilterType::Pointer huynenfilter = FilterType::New();

  MeanFilterType::InputSizeType radius;
  radius.Fill( size );
  perBand->GetFilter()->SetRadius(radius);

  readerHH->SetFileName(inputFilenameHH);
  readerHV->SetFileName(inputFilenameHV);
  readerVV->SetFileName(inputFilenameVV);

  sinclairtocov->SetVariadicNamedInput(otb::polarimetry_tags::hh{},readerHH->GetOutput());
  sinclairtocov->SetVariadicNamedInput(otb::polarimetry_tags::hv_or_vh{},readerHV->GetOutput());
  sinclairtocov->SetVariadicNamedInput(otb::polarimetry_tags::vv{},readerVV->GetOutput());

  perBand->SetInput(sinclairtocov->GetOutput());

  huynenfilter->SetInput1(perBand->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(huynenfilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
