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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbCommandLineArgumentParser.h"

#include "otbFastICAImageFilter.h"
#include "otbFastICAInternalOptimizerVectorImageFilter.h"

int otbFastICAInternalOptimizerVectorImageFilterNewTest ( int argc, char* argv[] )
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage< PixelType, Dimension > ImageType;
  typedef otb::FastICAInternalOptimizerVectorImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  return EXIT_SUCCESS;
}

int otbFastICAImageFilterNewTest ( int argc, char* argv[] )
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage< PixelType, Dimension > ImageType;
  typedef otb::FastICAImageFilter< ImageType, ImageType, otb::Transform::FORWARD > FilterType;
  FilterType::Pointer filter = FilterType::New();
  return EXIT_SUCCESS;
}

int otbFastICAImageFilterTest ( int argc, char* argv[] )
{
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddInputImage();
  parser->AddOption( "--NumComponents", "Number of components to keep for output", "-n", 1, false );
  parser->AddOption( "--Inverse", "Performs also the inverse transformation (give the output name)", "-inv", 1, false );
  parser->AddOption( "--NumIterations", "number of iterations (def.20)", "-iter", 1, false );
  parser->AddOption( "--Mu", "Give the increment weight of W in [0, 1] (def. 1)", "-mu", 1, false );
  parser->AddOutputImage();

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();
    
  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << argv[0] << " applies FastICA transformations\n";
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
  const unsigned int nbComponents = parseResult->IsOptionPresent("--NumComponents") ?
    parseResult->GetParameterUInt("--NumComponents") : 0;
  const unsigned int nbIterations = parseResult->IsOptionPresent("--NumIterations") ?
    parseResult->GetParameterUInt("--NumIterations") : 20;
  const double mu = parseResult->IsOptionPresent("--Mu" ) ?
    parseResult->GetParameterDouble("--Mu") : 1.;

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

  if ( parseResult->IsOptionPresent("--Inverse") )
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
    invWriter->SetFileName( parseResult->GetParameterString("--Inverse") );
    invWriter->SetInput( invFilter->GetOutput() );
    invWriter->Update();
  }

  return EXIT_SUCCESS;
}


