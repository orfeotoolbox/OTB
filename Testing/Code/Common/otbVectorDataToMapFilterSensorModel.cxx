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

#include <fstream>
#include <iostream>

#include "otbVectorDataFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"

#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataToMapFilter.h"

int otbVectorDataToMapFilterSensorModel(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  if (argc < 5)
    {
    std::cout << argv[0] << " <input vector filename> <input image filename>"
              << " <output vector filename> "
              << "<font filename>"
              << std::endl;

    return EXIT_FAILURE;
    }

  //Read the vector data
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  reader->SetFileName(argv[1]);

  //Read the image (only for the information)
  typedef otb::Image<unsigned short int, 2>     SensorImageType;
  typedef otb::ImageFileReader<SensorImageType> ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(argv[2]);
  imageReader->UpdateOutputInformation();

  //Reproject the vector data in the proper projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(reader->GetOutput());

  projection->SetOutputKeywordList(imageReader->GetOutput()->GetImageKeywordlist());
  projection->SetOutputOrigin(imageReader->GetOutput()->GetOrigin());
  projection->SetOutputSpacing(imageReader->GetOutput()->GetSpacing());

  //Convert the vector data into an image
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

  ImageType::SizeType size;
  size[0] = 500;
  size[1] = 500;

  ImageType::PointType origin;
//   origin[0] = imageReader->GetOutput()->GetOrigin()[0];
//   origin[1] = imageReader->GetOutput()->GetOrigin()[1];
  origin[0] = 11052;
  origin[1] = 7585;

  ImageType::SpacingType spacing;
//   spacing[0] = imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]/static_cast<double>(size[0]);
//   spacing[1] = imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]/static_cast<double>(size[1]);
  spacing[0] = 500 / static_cast<double>(size[0]);
  spacing[1] = 500 / static_cast<double>(size[1]);

  std::cout << "Size: " << size << std::endl;
  std::cout << "Origin: " << origin << std::endl;
  std::cout << "Spacing: " << spacing << std::endl;

  typedef otb::VectorDataToMapFilter<VectorDataType, ImageType> VectorDataToMapFilterType;
  VectorDataToMapFilterType::Pointer vectorDataRendering = VectorDataToMapFilterType::New();
  vectorDataRendering->SetInput(projection->GetOutput());

  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetFontFileName(argv[4]);
  vectorDataRendering->AddStyle("minor-roads-casing");
  vectorDataRendering->AddStyle("roads-text");

  //Save the image in a file
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(vectorDataRendering->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  return EXIT_SUCCESS;
}
