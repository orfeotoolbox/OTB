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
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

int otbVectorDataProjectionFilterFromMapToGeo(int argc, char * argv[])
{

  if (argc < 2)
    {
    std::cout << argv[0] << " <input vector filename> <output vector filename> "  << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::VectorData<double> InputVectorDataType;
  typedef otb::VectorData<double> OutputVectorDataType;

  typedef otb::VectorDataFileReader<InputVectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  typedef otb::VectorDataProjectionFilter<InputVectorDataType, OutputVectorDataType> VectorDataFilterType;
  VectorDataFilterType::Pointer vectorDataProjection = VectorDataFilterType::New();

  vectorDataProjection->SetInput(reader->GetOutput());

  typedef otb::VectorDataFileWriter<OutputVectorDataType> VectorDataFileWriterType;
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(vectorDataProjection->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
