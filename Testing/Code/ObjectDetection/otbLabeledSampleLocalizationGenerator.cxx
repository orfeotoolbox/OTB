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


#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbLabeledSampleLocalizationGenerator.h"

int otbLabeledSampleLocalizationGeneratorNew(int argc, char* argv[])
{
  typedef otb::VectorData<>                                        VectorDataType;
  typedef otb::LabeledSampleLocalizationGenerator<VectorDataType>  GeneratorType;

  // instantiation
  GeneratorType::Pointer generator = GeneratorType::New();

  std::cout << generator << std::endl;

  return EXIT_SUCCESS;
}

int otbLabeledSampleLocalizationGenerator(int argc, char* argv[])
{
  const char * inputVD1  = argv[1];
  const char * inputVD2  = argv[2];
  const char * outputVD  = argv[3];

  typedef otb::VectorData<>                                        VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>                VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>                VectorDataWriterType;
  typedef otb::LabeledSampleLocalizationGenerator<VectorDataType>  GeneratorType;

  // instantiation
  VectorDataReaderType::Pointer  reader1    = VectorDataReaderType::New();
  VectorDataReaderType::Pointer  reader2    = VectorDataReaderType::New();
  VectorDataWriterType::Pointer  writer     = VectorDataWriterType::New();
  GeneratorType::Pointer         generator  = GeneratorType::New();

  reader1->SetFileName(inputVD1);
  //reader1->Update();

  reader2->SetFileName(inputVD2);
  //reader2->Update();

  generator->PushBackInput(reader1->GetOutput());
  generator->PushBackInput(reader2->GetOutput());

  generator->SetSeed(0); // enable reproducible random number sequence
  generator->SetClassKey("Class");
  generator->SetNoClassIdentifier(0);
  generator->SetInhibitionRadius(5);
  generator->SetRandomLocalizationDensity(0.004);
  generator->SetNbMaxIteration(1000);
  generator->SetNumberOfPositiveSamplesPerPoint(2);

  //generator->Update();

  writer->SetFileName(outputVD);
  writer->SetInput(generator->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
