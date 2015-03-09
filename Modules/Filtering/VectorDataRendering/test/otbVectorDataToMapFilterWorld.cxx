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

#include "otbVectorDataFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include <fstream>
#include <iostream>

#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorDataToMapFilter.h"

int otbVectorDataToMapFilterWorld(int argc, char * argv [])
{

  if (argc < 11)
    {
    std::cout << argv[0] << " <input vector filename> <input image filename>"
              << " <output vector filename> "
              << " <sizeX> <sizeY> "
              << " <origin lon> <origin lat> "
              << " <spacing lon> <spacing lat> "
              << " <font filename>"
              << std::endl;
    return EXIT_FAILURE;
    }

  //Read the vector data
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;

  //Reproject the vector data in the proper projection
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;

  VectorDataFileReaderType::Pointer reader0 = VectorDataFileReaderType::New();
  reader0->SetFileName(argv[1]);
  ProjectionFilterType::Pointer projection0 = ProjectionFilterType::New();
  projection0->SetInput(reader0->GetOutput());

  VectorDataFileReaderType::Pointer reader1 = VectorDataFileReaderType::New();
  reader1->SetFileName(argv[2]);
  ProjectionFilterType::Pointer projection1 = ProjectionFilterType::New();
  projection1->SetInput(reader1->GetOutput());

  //Convert the vector data into an image
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

  ImageType::SizeType size;
  size[0] = atoi(argv[4]);
  size[1] = atoi(argv[5]);

  ImageType::PointType origin;
  origin[0] = atof(argv[6]);
  origin[1] = atof(argv[7]);

  ImageType::SpacingType spacing;
  spacing[0] = atof(argv[8]);
  spacing[1] = atof(argv[9]);

  typedef otb::VectorDataToMapFilter<VectorDataType, ImageType> VectorDataToMapFilterType;
  VectorDataToMapFilterType::Pointer vectorDataRendering = VectorDataToMapFilterType::New();
  vectorDataRendering->SetInput(0, projection0->GetOutput());
  vectorDataRendering->SetInput(1, projection1->GetOutput());

  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  vectorDataRendering->SetFontFileName(argv[10]);
  vectorDataRendering->AddStyle("world");
  vectorDataRendering->AddStyle("city");
  vectorDataRendering->UseAsOverlayOff();

  //Save the image in a file
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(vectorDataRendering->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  return EXIT_SUCCESS;
}
