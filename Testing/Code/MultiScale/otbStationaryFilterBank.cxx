/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbStationaryFilterBank.h"

#include "otbHaarOperator.h"
#include "otb_9_7_Operator.h"

#include "otbCommandLineArgumentParser.h"
#include "otbCommandProgressUpdate.h"

int otbStationaryFilterBank( int argc, char * argv[] )
{
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();
  parser->AddInputImage() ;
  //parser->AddOption( "--Level", "Decomposition level through the low pass branch (def. 1)", "-l", 1, false );
        //parser->AddOutputImage();
  parser->AddOption( "--Output", "Output filename prefix", "-out", 1, true );
  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << "Test undecimated wavelet transform through inner products\n";
    std::string descriptionException = err.GetDescription();
    if ( descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos )
      return EXIT_SUCCESS;
    if ( descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos )
      return EXIT_SUCCESS;
    return EXIT_FAILURE;
  }

  const char * inputFileName = parseResult->GetInputImage().c_str();
        //const char * outputFileName = parseResult->GetOutputImage().c_str();
        //unsigned int level = parseResult->IsOptionPresent("--Level" ) ?
        //        parseResult->GetParameterUInt("--Level") : 1;

  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::ImageFileReader< ImageType > ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );

  /* Transformation */
  //typedef otb::LowPassHaarOperator< PixelType, Dimension > LowPassOperator;
  //typedef otb::HighPassHaarOperator< PixelType, Dimension > HighPassOperator;
  typedef otb::LowPass_9_7_Operator< PixelType, Dimension > LowPassOperator;
  typedef otb::HighPass_9_7_Operator< PixelType, Dimension > HighPassOperator;

  typedef otb::StationaryFilterBank< ImageType, ImageType, LowPassOperator, HighPassOperator >
      FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  /* Observer */
  typedef otb::CommandProgressUpdate< FilterType > CommandType;
  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver( itk::ProgressEvent(), observer );

  filter->Update();


  std::string prefix = parseResult->GetParameterString("--Output");
  for ( unsigned int i = 0; i < filter->GetNumberOfOutputs(); i++ )
  {
    std::stringstream filename;
    filename << prefix;
    filename << "-" << i << ".tif";

    typedef otb::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(filename.str());
    writer->SetInput(filter->GetOutput(i));
    writer->Update();

  }
  return EXIT_SUCCESS;
}






