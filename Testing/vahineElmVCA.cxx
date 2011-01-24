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

#include <iostream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "VcaFilter.h"
#include "ElmVcaFilter.h"

typedef float PixelType;
const unsigned int Dimension = 2;

typedef otb::VectorImage< PixelType, Dimension > VectorImageType;
typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef otb::ImageFileWriter<VectorImageType> WriterType;

typedef otb::VahineElmVcaFilter<VectorImageType> ElmVcaFilterType;
typedef otb::VahineVCAFilter<VectorImageType> VcaFilterType;


int vahineVCA(int argc, char * argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  ElmVcaFilterType::Pointer elmVcaFilter = ElmVcaFilterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);

  elmVcaFilter->SetNbComponents(atoi(argv[3]));
  elmVcaFilter->SetInput(reader->GetOutput());

  writer->SetFileName(argv[2]);
  writer->SetInput(elmVcaFilter->GetOutput());

  writer->Update();

  return 0;
}

int vahineElmVCA(int argc, char * argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  ElmVcaFilterType::Pointer elmVcaFilter = ElmVcaFilterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);

  elmVcaFilter->SetInput(reader->GetOutput());

  writer->SetFileName(argv[2]);
  writer->SetInput(elmVcaFilter->GetOutput());

  writer->Update();

  return 0;
}


