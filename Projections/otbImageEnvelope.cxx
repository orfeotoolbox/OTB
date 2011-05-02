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

#include "otbImage.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbCommandLineArgumentParser.h"

typedef unsigned short                            PixelType;
typedef otb::Image<PixelType, 2>                   ImageType;
typedef otb::ImageFileReader<ImageType>           ReaderType;
typedef otb::VectorData<>                         VectorDataType;
typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
typedef otb::ImageToEnvelopeVectorDataFilter
    <ImageType, VectorDataType>                    FilterType;

int main(int argc, char* argv[])
{

  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Write a vector file containing a polygon corresponding to the image envelope.");
  parser->AddInputImage();
  parser->AddOption("--OutputVectorData","Vector Data file containg the envelope","-out", 1, true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine(argc, argv, parseResult);
  }
  catch ( itk::ExceptionObject & err )
  {
    std::string descriptionException = err.GetDescription();
    if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
      {
      return EXIT_SUCCESS;
      }
    if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
      {
      return EXIT_SUCCESS;
      }
    return EXIT_FAILURE;
  }

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(parseResult->GetParameterString("--OutputVectorData"));
  writer->Update();

  return EXIT_SUCCESS;
}
