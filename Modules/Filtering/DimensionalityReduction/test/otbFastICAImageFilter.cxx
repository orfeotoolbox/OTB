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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbFastICAImageFilter.h"


int otbFastICAImageFilterTest ( int , char* argv[] )
{

  std::string inputImageName = argv[1];
  std::string outputImageName = argv[2];
  std::string outputInvImageName = argv[3];

  const unsigned int nbComponents =  0;
  const unsigned int nbIterations =  20;
  const double mu = 1.;

  // Main type definition
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage< PixelType, Dimension > ImageType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageName);

  // Image filtering
  typedef otb::FastICAImageFilter< ImageType, ImageType, otb::Transform::FORWARD > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetNumberOfPrincipalComponentsRequired( nbComponents );
  filter->SetNumberOfIterations( nbIterations );
  filter->SetMu( mu );

  typedef otb::CommandProgressUpdate< FilterType > CommandType;
  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver( itk::ProgressEvent(), observer );

  std::cerr << "Decomposition\n";

  // Writing
  typedef otb::ImageFileWriter< ImageType > ImageWriterType;
  ImageWriterType::Pointer writer = ImageWriterType::New();
  writer->SetFileName( outputImageName );
  writer->SetInput( filter->GetOutput() );
  writer->Update();

  // std::cerr << filter << "\n";

  if ( true )
  {
    typedef otb::FastICAImageFilter< ImageType, ImageType, otb::Transform::INVERSE > InvFilterType;
    InvFilterType::Pointer invFilter = InvFilterType::New();
    invFilter->SetInput( filter->GetOutput() );
    invFilter->SetMeanValues( filter->GetMeanValues() );
    invFilter->SetStdDevValues( filter->GetStdDevValues() );
    invFilter->SetPCATransformationMatrix( filter->GetPCATransformationMatrix() );
    invFilter->SetTransformationMatrix( filter->GetTransformationMatrix() );

    typedef otb::CommandProgressUpdate< InvFilterType > CommandType2;
    CommandType2::Pointer invObserver = CommandType2::New();
    invFilter->AddObserver( itk::ProgressEvent(), invObserver );

    std::cerr << "Reconstruction\n";

    ImageWriterType::Pointer invWriter = ImageWriterType::New();
    invWriter->SetFileName(outputInvImageName);
    invWriter->SetInput( invFilter->GetOutput() );
    invWriter->Update();
  }

  return EXIT_SUCCESS;
}
