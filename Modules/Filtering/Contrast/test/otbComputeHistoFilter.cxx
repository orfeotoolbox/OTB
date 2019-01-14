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

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbComputeHistoFilter.h"

int otbComputeHistoFilter(int itkNotUsed(argc), char * argv [])
{
  typedef int InputPixelType;
  typedef int OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image< InputPixelType ,  Dimension > InputImageType;
  typedef otb::VectorImage< OutputPixelType , Dimension > HistoImageType;
  typedef otb::ComputeHistoFilter< InputImageType , HistoImageType > FilterType;

  typedef otb::ImageFileReader< InputImageType > ReaderType; 
  typedef otb::ImageFileWriter< HistoImageType > WriterType;
  ReaderType::Pointer reader ( ReaderType::New() );
  WriterType::Pointer writer ( WriterType::New() );
  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );
  reader->UpdateOutputInformation();

  FilterType::Pointer computeHisto ( FilterType::New() );
  
  computeHisto->SetInput( reader->GetOutput() );
  computeHisto->SetMin(0);
  computeHisto->SetMax(255);
  computeHisto->SetNbBin(256);
  auto size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  size[0] /= 4;
  size[1] /= 4;
  computeHisto->SetThumbSize( size );

  writer->SetInput( computeHisto->GetHistoOutput() );
  writer->Update();
  return EXIT_SUCCESS;
}