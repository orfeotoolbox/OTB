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
#include "otbConcatenateVectorData.h"


#include "otbConcatenateVectorDataFilter.h"
#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"

namespace otb
{

int ConcatenateVectorData::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("ConcatenateVectorData");
  descriptor->SetDescription("Concatenate VectorDatas");
  descriptor->AddOptionNParams("InputVectorDatas","Input VectorDatas to concatenate","in", true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputVectorData","Output vector data","out", 1, true, ApplicationDescriptor::FileName);

  return EXIT_SUCCESS;
}

int ConcatenateVectorData::Execute(otb::ApplicationOptionsResult* parseResult)
{
  try
    {
    typedef otb::VectorData<>                                 VectorDataType;
    typedef otb::VectorDataFileReader<VectorDataType>         ReaderType;
    typedef otb::VectorDataFileWriter<VectorDataType>         WriterType;
    typedef otb::ConcatenateVectorDataFilter<VectorDataType>  ConcatenateFilterType;


    // Get number of input vectorDatas
    unsigned int nbInputs = parseResult->GetNumberOfParameters("InputVectorDatas");

    // Instanciate a concatenate filter Concatenate the vector datas
    ConcatenateFilterType::Pointer concatenate = ConcatenateFilterType::New();
  
    for (unsigned int idx = 0; idx < nbInputs; idx++)
      {
      // Reader object
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(parseResult->GetParameterString("InputVectorDatas", idx ).c_str());
      reader->Update();
      concatenate->AddInput(reader->GetOutput());
      }
    
    // Write the output
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetParameterString("OutputVectorData").c_str());
    writer->SetInput(concatenate->GetOutput());
    writer->Update();
    
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
