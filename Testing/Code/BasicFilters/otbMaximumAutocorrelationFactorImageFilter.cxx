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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbMaximumAutocorrelationFactorImageFilter.h"

typedef otb::VectorImage<float,2> ImageType;
typedef otb::VectorImage<float,2>         OutputImageType;
typedef otb::ImageFileReader<ImageType>    ReaderType;
typedef otb::StreamingImageFileWriter<OutputImageType> WriterType;
typedef otb::MaximumAutocorrelationFactorImageFilter<ImageType,OutputImageType> MADFilterType;

int otbMaximumAutocorrelationFactorImageFilterNew(int argc, char* argv[])
{
  MADFilterType::Pointer  madFilter = MADFilterType::New();

  return EXIT_SUCCESS;
}


int otbMaximumAutocorrelationFactorImageFilter(int argc, char* argv[])
{
  char * infname = argv[1];
  char * outfname = argv[2];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  MADFilterType::Pointer  madFilter = MADFilterType::New();
  madFilter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(madFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
