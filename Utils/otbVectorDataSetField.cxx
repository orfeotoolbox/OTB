/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

#include "otbCommandLineArgumentParser.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"

int main(int argc, char* argv[] )
{

  try
  {
    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    parser->SetProgramDescription("Set a specified field to a specified value on all features of a vector data");
    parser->AddOption("--Input","Input vector data","-in",1,true);
    parser->AddOption("--Output","Output vector data","-out",1,true);
    parser->AddOption("--Field","Field name","-fn",1,true);
    parser->AddOption("--Value","Field value","-fv",1,true);

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer  parseResult = ParserResultType::New();

    try
    {
      parser->ParseCommandLine(argc,argv,parseResult);
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

    typedef otb::VectorData<> VectorDataType;
    typedef otb::VectorDataFileReader<VectorDataType> ReaderVectorType;
    ReaderVectorType::Pointer reader = ReaderVectorType::New();
    reader->SetFileName(parseResult->GetParameterString("--Input"));
    reader->Update();

    typedef VectorDataType::DataTreeType            DataTreeType;
    typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;
    TreeIteratorType it(reader->GetOutput()->GetDataTree());

    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
      {
      it.Get()->SetFieldAsString(parseResult->GetParameterString("--Field"), parseResult->GetParameterString("--Value"));
      }

    typedef otb::VectorDataFileWriter<VectorDataType> WriterVectorType;
    WriterVectorType::Pointer writerVector = WriterVectorType::New();
    writerVector->SetFileName(parseResult->GetParameterString("--Output"));
    writerVector->SetInput(reader->GetOutput());
    writerVector->Update();

  }
  catch ( itk::ExceptionObject & err )
  {
    std::cout << "Following otbException caught :" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }
  catch ( std::bad_alloc & err )
  {
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl;
    return EXIT_FAILURE;
  }
  catch ( ... )
  {
    std::cout << "Unknown Exception found !" << std::endl;
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;

}
