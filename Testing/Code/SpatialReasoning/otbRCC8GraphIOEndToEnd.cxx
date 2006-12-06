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
#include "itkExceptionObject.h"
#include "otbRCC8Graph.h"
#include "otbRCC8VertexBase.h"
#include "otbRCC8GraphFileReader.h"
#include "otbRCC8GraphFileWriter.h"

int otbRCC8GraphIOEndToEnd(int argc, char* argv[])
{
try
  {
    char * inputFilename = argv[1];
    char * outputFilename = argv[2];
    typedef unsigned int  LabelType;
    typedef otb::RCC8VertexBase<LabelType> VertexType;
    typedef otb::RCC8Graph<VertexType> RCC8GraphType;
    typedef otb::RCC8GraphFileReader<RCC8GraphType> RCC8GraphFileReaderType;
    typedef otb::RCC8GraphFileWriter<RCC8GraphType> RCC8GraphFileWriterType;
    
    // End to end test
    RCC8GraphFileReaderType::Pointer rcc8GraphReader = RCC8GraphFileReaderType::New();
    rcc8GraphReader->SetFileName(inputFilename);
    RCC8GraphFileWriterType::Pointer rcc8GraphWriter = RCC8GraphFileWriterType::New();
    rcc8GraphWriter->SetInput(rcc8GraphReader->GetOutput());
    rcc8GraphWriter->SetFileName(outputFilename);
    rcc8GraphWriter->Update();
  }
catch( itk::ExceptionObject & err ) 
  { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
  } 
catch( ... ) 
  { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
  } 
 return EXIT_SUCCESS;
}
