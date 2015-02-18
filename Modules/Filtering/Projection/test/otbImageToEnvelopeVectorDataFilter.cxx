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

#include "otbImage.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"

typedef unsigned short                            PixelType;
typedef otb::Image<PixelType, 2>                   ImageType;
typedef otb::ImageFileReader<ImageType>           ReaderType;
typedef otb::VectorData<>                         VectorDataType;
typedef otb::VectorDataFileWriter<VectorDataType> WriterType;
typedef otb::ImageToEnvelopeVectorDataFilter
    <ImageType, VectorDataType>                    FilterType;

int otbImageToEnvelopeVectorDataFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  // Instantiation
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}

int otbImageToEnvelopeVectorDataFilter(int itkNotUsed(argc), char* argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
