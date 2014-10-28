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

#include "otbImage.h"
#include "otbImageFileReader.h"

int otbVectorDataProjectionFilterFromMapToImage(int argc, char * argv[])
{

  if (argc < 4)
    {
    std::cout << argv[0] << " <input vector filename> <input image filename>"
              << " <output vector filename> "  << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::VectorData<double> InputVectorDataType;
  typedef otb::VectorData<double> OutputVectorDataType;

  typedef otb::VectorDataFileReader<InputVectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  typedef otb::Image<unsigned short int, 2> ImageType;
  typedef otb::ImageFileReader<ImageType>   ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(argv[2]);
  imageReader->UpdateOutputInformation();

  typedef otb::VectorDataProjectionFilter<InputVectorDataType, OutputVectorDataType> VectorDataFilterType;

  VectorDataFilterType::Pointer vectorDataProjection = VectorDataFilterType::New();

  vectorDataProjection->SetInput(reader->GetOutput());

  vectorDataProjection->SetOutputProjectionRef(imageReader->GetOutput()->GetProjectionRef());
  vectorDataProjection->SetOutputOrigin(imageReader->GetOutput()->GetOrigin());
  vectorDataProjection->SetOutputSpacing(imageReader->GetOutput()->GetSpacing());

  typedef otb::VectorDataFileWriter<OutputVectorDataType> VectorDataFileWriterType;
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(vectorDataProjection->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
