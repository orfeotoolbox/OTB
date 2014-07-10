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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbCommandLineArgumentParser.h"

#include "otbLocalActivityVectorImageFilter.h"

int otbLocalActivityVectorImageFilterNewTest ( int itkNotUsed(argc), char** itkNotUsed(argv) )
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage< PixelType, Dimension > ImageType;
  typedef otb::LocalActivityVectorImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  return EXIT_SUCCESS;
}

int otbLocalActivityVectorImageFilterTest ( int argc, char* argv[] )
{
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddInputImage();
  parser->AddOption( "--Radius", "Set the radius of the sliding window (def.1)", "-r", 2, false );
  parser->AddOutputImage();

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << argv[0] << " performs the local activity estimation on a vector image\n";
    std::cerr << "Local activity is defined here a the difference between pixel and mean of surrounding\n";
    std::string descriptionException = err.GetDescription();
    if ( descriptionException.find("ParseCommandLine(): Help Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    if(descriptionException.find("ParseCommandLine(): Version Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    return EXIT_FAILURE;
  }

  std::string inputImageName = parseResult->GetInputImage();
  std::string outputImageName = parseResult->GetOutputImage();
  unsigned int radiusX = 1;
  unsigned int radiusY = 1;
  if ( parseResult->IsOptionPresent("--Radius") )
  {
    radiusX = parseResult->GetParameterUInt("--Radius", 0);
    radiusY = parseResult->GetParameterUInt("--Radius", 1);
  }

  // Main type definition
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage< PixelType, Dimension > ImageType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageName);

  // Image filtering
  typedef otb::LocalActivityVectorImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  FilterType::RadiusType radius = {{ radiusX, radiusY }};
  filter->SetRadius( radius );

  typedef otb::CommandProgressUpdate< FilterType > CommandType;
  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver( itk::ProgressEvent(), observer );

  typedef otb::ImageFileWriter< ImageType > ImageWriterType;
  ImageWriterType::Pointer writer = ImageWriterType::New();
  writer->SetFileName( outputImageName );
  writer->SetInput( filter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
