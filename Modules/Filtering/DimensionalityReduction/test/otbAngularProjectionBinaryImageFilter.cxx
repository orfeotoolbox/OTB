/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbMath.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"

#include "otbAngularProjectionBinaryImageFilter.h"

int otbAngularProjectionBinaryImageFilterTest ( int , char * [] )
{
  std::string inputImageName1("");
  std::string inputImageName2("");
  std::string outputImageName("");

  // Main type definition
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension > ImageType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(inputImageName1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(inputImageName2);

  // Image filtering
  typedef otb::AngularProjectionBinaryImageFilter< ImageType, ImageType, PixelType >
    FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput1( reader1->GetOutput() );
  filter->SetInput2( reader2->GetOutput() );

  std::vector< PixelType > angle;
  angle.push_back( otb::CONST_PI_2 );
  angle.push_back( 0. );

  filter->SetAngleSet( angle );

  typedef otb::CommandProgressUpdate< FilterType > CommandType;
  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver( itk::ProgressEvent(), observer );

  filter->Update();

  typedef otb::ImageFileWriter< ImageType > WriterType;
  std::vector< WriterType::Pointer > writers;
  writers.resize( filter->GetNumberOfOutputs() );

  for ( unsigned int i = 0; i < filter->GetNumberOfOutputs(); ++i )
  {
    std::stringstream title;
    title << outputImageName << "_" << i << ".hdr";

    writers[i] = WriterType::New();
    writers[i]->SetFileName( title.str() );
    writers[i]->SetInput( filter->GetOutput(i) );
    writers[i]->Update();
  }
  return EXIT_SUCCESS;
}
