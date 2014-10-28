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
#include "otbRCC8VertexBase.h"
#include "otbRCC8GraphFileReader.h"
#include "otbRCC8GraphFileWriter.h"
#include "otbPolygon.h"

int otbRCC8GraphIOEndToEnd(int itkNotUsed(argc), char* argv[])
{
  char * inputFilename = argv[1];
  char * outputFilename = argv[2];
  typedef otb::Polygon<>                          PathType;
  typedef otb::RCC8VertexBase<PathType>           VertexType;
  typedef otb::RCC8Graph<VertexType>              RCC8GraphType;
  typedef otb::RCC8GraphFileReader<RCC8GraphType> RCC8GraphFileReaderType;
  typedef otb::RCC8GraphFileWriter<RCC8GraphType> RCC8GraphFileWriterType;

  // End to end test
  RCC8GraphFileReaderType::Pointer rcc8GraphReader = RCC8GraphFileReaderType::New();
  rcc8GraphReader->SetFileName(inputFilename);
  RCC8GraphFileWriterType::Pointer rcc8GraphWriter = RCC8GraphFileWriterType::New();
  rcc8GraphWriter->SetInput(rcc8GraphReader->GetOutput());
  rcc8GraphWriter->SetFileName(outputFilename);
  rcc8GraphWriter->Update();

  return EXIT_SUCCESS;
}
