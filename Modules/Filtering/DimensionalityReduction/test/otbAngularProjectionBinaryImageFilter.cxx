/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "otbCommandLineArgumentParser.h"

#include "otbAngularProjectionBinaryImageFilter.h"

int otbAngularProjectionBinaryImageFilterTest ( int argc, char * argv[] )
{
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddOption( "--InputImages", "Input Images", "-in", 2, true );
  parser->AddOption( "--OutputImages", "Generic name for output Images (_#.hdr will be added)", "-out", 1, true );

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << argv[0] << " performs angular projection on 2 images\n";
    std::string descriptionException = err.GetDescription();
    if ( descriptionException.find("ParseCommandLine(): Help Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    if(descriptionException.find("ParseCommandLine(): Version Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    return EXIT_FAILURE;
  }

  std::string inputImageName1(parseResult->GetParameterString("--InputImages", 0));
  std::string inputImageName2(parseResult->GetParameterString("--InputImages", 1));
  std::string outputImageName(parseResult->GetParameterString("--OutputImages"));

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
