/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbMath.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbCommandLineArgumentParser.h"

#include "otbAngularProjectionBinaryImageFilter.h"

int otbAngularProjectionBinaryImageFilterNewTest ( int argc, char * argv[] )
{
  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::Image< PixelType, Dimension > ImageType;
  typedef double PrecisionType;

  typedef otb::AngularProjectionBinaryImageFilter< ImageType, ImageType, PrecisionType >
    FilterType;
  FilterType::Pointer filter = FilterType::New();
  return EXIT_SUCCESS;
}

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

  const char * inputImageName1 = parseResult->GetParameterString("--InputImages", 0).c_str();
  const char * inputImageName2 = parseResult->GetParameterString("--InputImages", 1).c_str();
  const char * outputImageName = parseResult->GetParameterString("--OutputImages").c_str();

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
    writers[i]->SetFileName( title.str().c_str() );
    writers[i]->SetInput( filter->GetOutput(i) );
    writers[i]->Update();
  }
  return EXIT_SUCCESS;
}



