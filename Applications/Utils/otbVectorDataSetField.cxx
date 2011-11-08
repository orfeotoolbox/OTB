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

#include "otbVectorDataSetField.h"

#include <iostream>

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"

namespace otb
{

int VectorDataSetField::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("VectorDataSetField");
  descriptor->SetDescription("Set a specified field to a specified value on all features of a vector data");
  descriptor->AddOption("Input","Input vector data","in", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Output","Output vector data","out", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Field","Field name","fn", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("Value","Field value","fv", 1, true, ApplicationDescriptor::String);
  
  return EXIT_SUCCESS;
}

int VectorDataSetField::Execute(otb::ApplicationOptionsResult* parseResult)
{
  try
    {
    typedef otb::VectorData<> VectorDataType;
    typedef otb::VectorDataFileReader<VectorDataType> ReaderVectorType;
    ReaderVectorType::Pointer reader = ReaderVectorType::New();
    reader->SetFileName(parseResult->GetParameterString("Input"));
    reader->Update();

    typedef VectorDataType::DataTreeType            DataTreeType;
    typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;
    TreeIteratorType it(reader->GetOutput()->GetDataTree());

    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
      {
      it.Get()->SetFieldAsString(parseResult->GetParameterString("Field"), parseResult->GetParameterString("Value"));
      }

    typedef otb::VectorDataFileWriter<VectorDataType> WriterVectorType;
    WriterVectorType::Pointer writerVector = WriterVectorType::New();
    writerVector->SetFileName(parseResult->GetParameterString("Output"));
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
}
