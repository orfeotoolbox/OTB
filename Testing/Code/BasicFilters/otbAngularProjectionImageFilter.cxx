/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkFixedArray.h"

#include "otbMath.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"

#include "otbAngularProjectionImageFilter.h"

int otbAngularProjectionImageFilterTest ( int argc, char * argv[] )
{
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddOption( "--InputImages", "Input Images", "-in", 2, true );
  parser->AddOutputImage();

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << argv[0] << " performs otbAngularProjectionImageFilterTest\n";
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
  const char * outputImageName = parseResult->GetOutputImage().c_str();

  // Main type definition
  const unsigned int Dimension = 2;

  typedef float  PixelType;
  typedef double PrecisionType;
  typedef itk::FixedArray< PrecisionType, 1 > AngleType;
  typedef otb::Image< PixelType, Dimension >  ImageType;

  // Reading images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(inputImageName1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(inputImageName2);

  // Parameter (to be changed if necessary)
  AngleType angle;
  angle[0] = otb::CONST_PI_2;

  // Filtering
  typedef otb::AngularProjectionImageFilter<
    ImageType, ImageType, AngleType, PrecisionType > FilterType;

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( 0, reader1->GetOutput() );
  filter->SetInput( 1, reader2->GetOutput() );
  filter->SetAngleArray( angle );

  // Saving
  typedef otb::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputImageName );
  writer->SetInput( filter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}


