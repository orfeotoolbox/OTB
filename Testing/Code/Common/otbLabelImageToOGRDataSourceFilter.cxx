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


#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"

int otbLabelImageToOGRDataSourceFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> InputLabelImageType;

  typedef otb::LabelImageToOGRDataSourceFilter<InputLabelImageType>  FilterType;

  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;
}


int otbLabelImageToOGRDataSourceFilter(int argc, char * argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputLabelImageFile " << std::endl;
    return EXIT_FAILURE;
    }
  const char * infname = argv[1];

  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> InputLabelImageType;

  typedef otb::LabelImageToOGRDataSourceFilter<InputLabelImageType>   FilterType;
  typedef otb::ImageFileReader<InputLabelImageType>                LabelImageReaderType;


  FilterType::Pointer filter = FilterType::New();
  LabelImageReaderType::Pointer reader = LabelImageReaderType::New();

  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());
  filter->Update();


  return EXIT_SUCCESS;
}
