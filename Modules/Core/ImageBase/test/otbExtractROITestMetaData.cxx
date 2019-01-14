/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <fstream>

#include "itkExtractImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbExtractROI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otb_boost_string_header.h"

int otbExtractROITestMetaData(int itkNotUsed(argc), char * argv[])
{
  typedef float PixelType;

  typedef otb::Image<PixelType>                    ImageType;

  typedef otb::ImageFileReader<ImageType>          ImageFileReaderType;
  typedef otb::ImageFileWriter<ImageType>          ImageFileWriterType;

  typedef otb::ExtractROI<PixelType, PixelType>    ExtractROIType;

  ImageFileReaderType::Pointer readerInput = ImageFileReaderType::New();
  ImageFileReaderType::Pointer reader00 = ImageFileReaderType::New();
  ImageFileReaderType::Pointer reader57 = ImageFileReaderType::New();
  ImageFileWriterType::Pointer writer00 = ImageFileWriterType::New();
  ImageFileWriterType::Pointer writer57 = ImageFileWriterType::New();

  ExtractROIType::Pointer extract00 = ExtractROIType::New();
  ExtractROIType::Pointer extract57 = ExtractROIType::New();

  // Read input file
  readerInput->SetFileName(argv[1]);
  readerInput->GenerateOutputInformation();

  // Extract left up image
  extract00->SetInput(readerInput->GetOutput());
  extract00->SetSizeX(50);
  extract00->SetSizeY(25);
  extract00->SetStartX(0);
  extract00->SetStartY(0);
  extract00->UpdateOutputInformation();

  // Save left up image information
  std::ofstream file00;
  file00.open(argv[4]);
  file00 << static_cast<ImageType::Pointer>(extract00->GetOutput()) << std::endl;
  file00.close();

  // Extract image with non-zero index
  extract57->SetInput(readerInput->GetOutput());
  extract57->SetSizeX(50);
  extract57->SetSizeY(25);
  extract57->SetStartX(5);
  extract57->SetStartY(7);
  extract57->UpdateOutputInformation();

  // Save extract image information
  std::ofstream file57;
  file57.open(argv[5]);
  file57 << static_cast<ImageType::Pointer>(extract57->GetOutput()) << std::endl;
  file57.close();

  // Write left up image
  writer00->SetFileName(argv[2]);
  writer00->SetInput(extract00->GetOutput());
  //writer00->SetWriteGeomFile(true);
  writer00->Update();

  // Write image with non zero index
  writer57->SetFileName(argv[3]);
  writer57->SetInput(extract57->GetOutput());
  //writer57->SetWriteGeomFile(true);
  writer57->Update();

  // Reading image with left up index
  reader00->SetFileName(argv[2]);
  reader00->GenerateOutputInformation();

  if( reader00->GetOutput()->GetProjectionRef() != "" || boost::algorithm::istarts_with(reader00->GetOutput()->GetProjectionRef(), "LOCAL_CS") )
    {
    std::cout<<"The read generated extract from index (0, 0) must NOT contain a ProjectionReference."<<std::endl;
    std::cout<<"Found ProjectionReference: "<<reader00->GetOutput()->GetProjectionRef()<<std::endl;

    return EXIT_FAILURE;
    }

  if( reader00->GetOutput()->GetGCPCount() == 0)
    {
    std::cout<<"The read generated extract from index (0, 0) must contain a list a GCPs.."<<std::endl;
    return EXIT_FAILURE;
    }

  // Reading image with non-zero index
  reader57->SetFileName(argv[3]);
  reader57->GenerateOutputInformation();

  if( reader00->GetOutput()->GetProjectionRef() != "" || boost::algorithm::istarts_with(reader00->GetOutput()->GetProjectionRef(), "LOCAL_CS") )
    {
    std::cout<<"The read generated extract from index (x, y) must NOT contain a ProjectionReference."<<std::endl;
    std::cout<<"Found ProjectionReference: "<<reader57->GetOutput()->GetProjectionRef()<<std::endl;
    return EXIT_FAILURE;
    }

  if( reader57->GetOutput()->GetGCPCount() != 0)
    {
    std::cout<<"The read generated extract from index (x, y) must NOT contain a list a GCPs.."<<std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
