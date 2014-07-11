/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataProjectionFilter.h"

#include "otbVectorDataToLabelImageFilter.h"
#include "otbStandardOneLineFilterWatcher.h"

typedef otb::Image<unsigned int, 2>                           ImageType;
typedef otb::VectorData<>                                     VectorDataType;
typedef otb::ImageFileReader<ImageType>                       ReaderType;
typedef otb::VectorDataFileReader<VectorDataType>             VDReaderType;
typedef otb::VectorDataProjectionFilter<VectorDataType,
                                        VectorDataType>       VDProjectionType;
typedef otb::ImageFileWriter<ImageType>              WriterType;
typedef otb::VectorDataToLabelImageFilter<VectorDataType,
                                          ImageType>          RasterizationFilterType;

int otbVectorDataToLabelImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  RasterizationFilterType::Pointer  rasterization = RasterizationFilterType::New();
  return EXIT_SUCCESS;
}

int otbVectorDataToLabelImageFilter(int itkNotUsed(argc), char * argv[])
{

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // Read and project the vd into the raster coordinates images
  VDReaderType::Pointer vdreader = VDReaderType::New();
  vdreader->SetFileName(argv[2]);

  VDProjectionType::Pointer vdproj = VDProjectionType::New();
  vdproj->SetInput(vdreader->GetOutput());
  vdproj->SetInputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
  vdproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  vdproj->Update();

  // rasterize
  RasterizationFilterType::Pointer  rasterization = RasterizationFilterType::New();
  rasterization->AddVectorData(vdproj->GetOutput());
  rasterization->SetOutputParametersFromImage(reader->GetOutput());
  rasterization->SetBurnAttribute("DN");

  WriterType::Pointer writer  = WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(rasterization->GetOutput());
  writer->Update();

return EXIT_SUCCESS;
}
