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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageRegionIterator.h"
#include "otbCLHistogramEqualizationFilter.h"

int otbHelperCLAHE(int itkNotUsed(argc), char * argv [])
{
  typedef int InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image< InputPixelType ,  Dimension > InputImageType;
  typedef otb::ImageFileReader< InputImageType > ReaderType; 
  typedef otb::ImageFileWriter< InputImageType > WriterType;
  ReaderType::Pointer reader ( ReaderType::New() );
  WriterType::Pointer writer ( WriterType::New() );
  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );
  reader->UpdateOutputInformation();

  typedef otb::VectorImage< int , 2 > HistogramType;
  typedef otb::VectorImage< double , 2 > LutType;

  typedef itk::StreamingImageFilter< LutType , LutType >
    StreamingImageFilter;

  typedef otb::InPlacePassFilter < InputImageType > BufferFilter;

  typedef otb::ComputeHistoFilter< InputImageType , HistogramType >
    HistoFilter;

  typedef otb::ComputeGainLutFilter< HistogramType , LutType >
    GainLutFilter;

  typedef otb::ApplyGainFilter< InputImageType , LutType , InputImageType >
    ApplyGainFilter;

  HistoFilter::Pointer histoFilter( HistoFilter::New() );
  GainLutFilter::Pointer lutFilter( GainLutFilter::New() );
  ApplyGainFilter::Pointer applyFilter( ApplyGainFilter::New() );
  BufferFilter::Pointer buffer( BufferFilter::New() );
  StreamingImageFilter::Pointer streamFilter( StreamingImageFilter::New() );
  InputImageType::SizeType size;
  size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  // histoEqualize->SetThreshold(100);

  histoFilter->SetInput( reader->GetOutput() );
  buffer->SetInput( reader->GetOutput() );
  lutFilter->SetInput( histoFilter->GetHistoOutput() );
  streamFilter->SetInput( lutFilter->GetOutput() );
  applyFilter->SetInputLut( streamFilter->GetOutput() );
  applyFilter->SetInputImage( buffer->GetOutput() );

  histoFilter->SetMin(0);
  histoFilter->SetMax(255);
  lutFilter->SetMin(0);
  lutFilter->SetMax(255);
  applyFilter->SetMin(0);
  applyFilter->SetMax(255);

  histoFilter->SetThumbSize(size);
  applyFilter->SetThumbSize(size);
  lutFilter->SetNbPixel(size[0]*size[1]);



  writer->SetInput(applyFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
