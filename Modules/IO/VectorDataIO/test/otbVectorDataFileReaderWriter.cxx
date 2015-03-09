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

#include "otbMacro.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"

int otbVectorDataFileReaderWriter(int itkNotUsed(argc), char * argv[])
{

  typedef otb::VectorData<double, 2>                VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> ReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;

  //Instantiation
  ReaderType::Pointer     reader = ReaderType::New();
  WriterType::Pointer     writer = WriterType::New();
  VectorDataType::Pointer data = VectorDataType::New();

  reader->SetFileName(argv[1]);

  writer->SetFileName(argv[2]);
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
