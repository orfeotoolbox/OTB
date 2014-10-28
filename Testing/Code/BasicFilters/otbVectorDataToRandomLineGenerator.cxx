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


#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataToRandomLineGenerator.h"

int otbVectorDataToRandomLineGeneratorNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::VectorData<>                                        VectorDataType;
  typedef otb::VectorDataToRandomLineGenerator<VectorDataType>     GeneratorType;

  // instantiation
  GeneratorType::Pointer generator = GeneratorType::New();

  std::cout << generator << std::endl;

  return EXIT_SUCCESS;
}

int otbVectorDataToRandomLineGenerator(int itkNotUsed(argc), char* argv[])
{
  const char * inputVD  = argv[1];
  const char * outputVD  = argv[2];

  typedef otb::VectorData<>                                        VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>                VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>                VectorDataWriterType;
  typedef otb::VectorDataToRandomLineGenerator<VectorDataType>     GeneratorType;

  // instantiation
  VectorDataReaderType::Pointer  reader     = VectorDataReaderType::New();
  VectorDataWriterType::Pointer  writer     = VectorDataWriterType::New();
  GeneratorType::Pointer         generator  = GeneratorType::New();

  reader->SetFileName(inputVD);
  reader->Update();

  generator->SetInput(reader->GetOutput());
  generator->SetSeed(0); // enable reproducible random number sequence
  generator->SetNumberOfOutputLine(20);
  generator->SetMinLineSize(5);
  generator->SetMaxLineSize(20);

  generator->Update();

  writer->SetFileName(outputVD);
  writer->SetInput(generator->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
