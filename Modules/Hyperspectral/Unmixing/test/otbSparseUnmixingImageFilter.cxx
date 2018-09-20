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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbCommandLineArgumentParser.h"

#include "otbSparseUnmixingImageFilter.h"

int otbSparseUnmixingImageFilterTest ( int argc, char * argv[] )
{
  // number of images to consider
  const unsigned int nbInputImages = 2;

  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddOption( "--InputImages", "Input Images", "-in", nbInputImages, true );
  parser->AddOption( "--OutputImages", "Generic name for output Images (_#.hdr will be added)", "-out", 1, true );
  parser->AddOption( "--Threshold", "Lower threshold for accounting the waBinaryFunctorImageListToSampleListFiltervelet coeffs (def. 10)", "-th", 1, false );

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << argv[0] << " performs Wvlt unmixing on " << nbInputImages << " images\n";
    std::string descriptionException = err.GetDescription();
    if ( descriptionException.find("ParseCommandLine(): Help Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    if(descriptionException.find("ParseCommandLine(): Version Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    return EXIT_FAILURE;
  }

  std::string inputImageName [ nbInputImages ];
  for ( unsigned int i = 0; i < nbInputImages; i++ )
    inputImageName[i] = parseResult->GetParameterString("--InputImages", i);
  std::string outputImageName = parseResult->GetParameterString("--OutputImages");
  const double threshold = parseResult->IsOptionPresent("--Threshold") ?
    parseResult->GetParameterDouble("--Threshold") : 10.;

  // Main type definition
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension > ImageType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ObjectList< ReaderType > ReaderListType;
  ReaderListType::Pointer reader = ReaderListType::New();
  reader->Resize( nbInputImages );
  for ( unsigned int i = 0; i < nbInputImages; i++ )
  {
    reader->SetNthElement(i, ReaderType::New());
    reader->GetNthElement(i)->SetFileName( inputImageName[i] );
    reader->GetNthElement(i)->Update();
  }

  // Image filtering
  typedef otb::SparseUnmixingImageFilter< ImageType, ImageType, nbInputImages > FilterType;
  FilterType::Pointer filter = FilterType::New();
  for ( unsigned int i = 0; i < nbInputImages; i++ )
    filter->SetInput( i, reader->GetNthElement(i)->GetOutput() );
  filter->SetThresholdValue( threshold );

  typedef otb::CommandProgressUpdate< FilterType > CommandType;
  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver( itk::ProgressEvent(), observer );

  filter->Update();

  typedef otb::ImageFileWriter< ImageType > WriterType;
  typedef otb::ObjectList< WriterType > WriterListType;
  WriterListType::Pointer writers = WriterListType::New();
  writers->Resize( filter->GetOutput()->Size() );

  for ( unsigned int i = 0; i < writers->Size(); i++ )
  {
    std::ostringstream title;
    title << outputImageName << "_" << i << ".hdr";

    writers->SetNthElement(i, WriterType::New());
    WriterType::Pointer writer = writers->GetNthElement(i);
    
    std::string stitle = title.str();
    writer->SetFileName( stitle );
    writer->SetInput( filter->GetOutput()->GetNthElement(i) );
    writer->Update();
  }

  return EXIT_SUCCESS;
}
