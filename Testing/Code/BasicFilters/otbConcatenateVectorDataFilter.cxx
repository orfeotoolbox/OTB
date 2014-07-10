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

#include "otbCommandLineArgumentParser.h"

// Images
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

// Concatener
#include "otbConcatenateVectorDataFilter.h"


typedef otb::VectorData<>                                 VectorDataType;
typedef otb::VectorDataFileReader<VectorDataType>         ReaderType;
typedef otb::VectorDataFileWriter<VectorDataType>         WriterType;
typedef otb::ConcatenateVectorDataFilter<VectorDataType>  ConcatenateFilterType;

int otbConcatenateVectorDataFilterNew (int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  ConcatenateFilterType::Pointer concatenate = ConcatenateFilterType::New();
  return EXIT_SUCCESS;
}

int otbConcatenateVectorDataFilter (int argc, char * argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();
  parser->AddOptionNParams("--InputVectorDatas","Input VectorDatas to concatenate ", "-in", true);
  parser->AddOption("--OutputVectorData","Output concatenated VectorData","-out", true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();
  parser->ParseCommandLine(argc, argv, parseResult);

  // Get number of input vectorDatas
  unsigned int nbInputs = parseResult->GetNumberOfParameters("--InputVectorDatas");

  // Instanciate a concatenate filter
    // Concatenate the vector datas
  ConcatenateFilterType::Pointer concatenate = ConcatenateFilterType::New();

  for (unsigned int idx = 0; idx < nbInputs; ++idx)
    {
    // Reader object
    ReaderType::Pointer reader = ReaderType::New();
    //std::cout << << std::endl;
    reader->SetFileName(parseResult->GetParameterString("--InputVectorDatas", idx ));
    reader->Update();

    concatenate->AddInput(reader->GetOutput());
    }

  // Write the output
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(parseResult->GetParameterString("--OutputVectorData"));
  writer->SetInput(concatenate->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
