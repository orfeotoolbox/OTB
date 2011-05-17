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
#include <fstream>

#include "otbImage.h"
#include "otbVectorImage.h"

#include "itkExtractImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbExtractROI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbExtractROITestMetaData(int argc, char * argv[])
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
  writer00->Update();

  // Write image with non zero index
  writer57->SetFileName(argv[3]);
  writer57->SetInput(extract57->GetOutput());
  writer57->Update();

  // Reading image with left up index
  reader00->SetFileName(argv[2]);
  reader00->GenerateOutputInformation();

  // Save extract image information
  std::ofstream file00_read;
  file00_read.open(argv[6]);
  file00_read << static_cast<ImageType::Pointer>(reader00->GetOutput()) << std::endl;
  file00_read.close();

  // Reading image with non-zero index
  reader57->SetFileName(argv[3]);
  reader57->GenerateOutputInformation();

  // Save extract image information
  std::ofstream file57_read;
  file57_read.open(argv[7]);
  file57_read << static_cast<ImageType::Pointer>(reader57->GetOutput()) << std::endl;
  file57_read.close();

  return EXIT_SUCCESS;
}
