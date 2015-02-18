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


#include "otbLabelImageToVectorDataFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"

int otbLabelImageToVectorDataFilter(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputLabelImageFile outputVectorfile(shp)" << std::endl;
    return EXIT_FAILURE;
    }
  const char * infname = argv[1];
  const char * outfname = argv[2];


  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> InputLabelImageType;

  typedef otb::LabelImageToVectorDataFilter<InputLabelImageType>   FilterType;
  typedef otb::ImageFileReader<InputLabelImageType>                LabelImageReaderType;
  typedef FilterType::VectorDataType                               VectorDataType;
  typedef otb::VectorDataFileWriter<VectorDataType>                WriterType;

  FilterType::Pointer filter = FilterType::New();
  LabelImageReaderType::Pointer reader = LabelImageReaderType::New();
  WriterType::Pointer  writer = WriterType::New();

  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());
  filter->SetUse8Connected(false);

  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
