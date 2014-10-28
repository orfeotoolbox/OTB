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

#include "itkListSample.h"

#include "otbMath.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"

#include "otbAngularProjectionSetImageFilter.h"

int otbAngularProjectionSetImageFilterTest ( int argc, char * argv[] )
{
  const unsigned int Dimension = 2;
  const unsigned int SpaceDimension = 3;
  const unsigned int nbInputImages = SpaceDimension+1;

  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddOption( "--InputImages", "Input Images", "-in", nbInputImages, true );
  parser->AddOption( "--OutputImages", "Generic name for output Images (_#.hdr will be added)", "-out", 1, true );

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << argv[0] << " performs otbAngularProjectionSetImageFilterTest with " << nbInputImages << " images\n";
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
  const char * outputImageName = parseResult->GetParameterString("--OutputImages").c_str();

  // Main type definition
  typedef float  PixelType;
  typedef double PrecisionType;
  typedef itk::FixedArray< PrecisionType, SpaceDimension > AngleType;
  typedef itk::Statistics::ListSample< AngleType >         AngleListType;
  typedef otb::Image< PixelType, Dimension >               ImageType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ObjectList< ReaderType > ReaderListType;
  ReaderListType::Pointer reader = ReaderListType::New();
  reader->Resize( nbInputImages );
  for ( unsigned int i = 0; i < nbInputImages; i++ )
  {
    reader->SetNthElement(i, ReaderType::New());
    reader->GetNthElement(i)->SetFileName( inputImageName[i].c_str() );
    reader->GetNthElement(i)->Update();
  }

  // Parameter (to be changed if necessary)
  // Here, we will have one output only
  AngleListType::Pointer angleList = AngleListType::New();
  AngleType angle;
  for ( unsigned int i = 0; i < SpaceDimension; i++ )
  {
    angle[i] = otb::CONST_PI / static_cast<double>( SpaceDimension );
  }
  angleList->PushBack( angle );

  // Filter
  typedef otb::AngularProjectionSetImageFilter<
    ImageType, ImageType, AngleListType, PrecisionType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  for ( unsigned int i = 0; i < nbInputImages; i++ )
  {
    filter->SetInput( i, reader->GetNthElement(i)->GetOutput() );
  }
  filter->SetAngleList( angleList );

  // Saving
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
    writer->SetFileName( title.str().c_str() );
    writer->SetInput( filter->GetOutput()->GetNthElement(i) );
    writer->Update();
  }

  return EXIT_SUCCESS;
}

