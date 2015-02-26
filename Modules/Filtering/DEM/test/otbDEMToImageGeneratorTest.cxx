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
#include "itkMacro.h"

#include "otbDEMToImageGenerator.h"
#include "otbImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

int otbDEMToImageGeneratorTest(int argc, char * argv[])
{
  if (argc < 9)
    {
    std::cout << argv[0] <<
    " folder path , output filename , Longitude Output Orign point , Latitude Output Origin point , X Output Size, Y Output size , X Spacing , Y Spacing"
              << std::endl;
    return EXIT_FAILURE;
    }

  char * folderPath = argv[1];
  char * outputName = argv[2];

  const unsigned int Dimension = 2;
  typedef otb::Image<double, Dimension>            ImageType;
  typedef otb::DEMToImageGenerator<ImageType>      DEMToImageGeneratorType;
  typedef DEMToImageGeneratorType::PointType       PointType;
  typedef DEMToImageGeneratorType::SizeType        SizeType;
  typedef DEMToImageGeneratorType::SpacingType     SpacingType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  // Instantiating object
  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();
  WriterType::Pointer              writer = WriterType::New();

  PointType origin;
  origin[0] = ::atof(argv[3]);
  origin[1] = ::atof(argv[4]);

  SizeType size;
  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);

  SpacingType spacing;
  spacing[0] = ::atof(argv[7]);
  spacing[1] = ::atof(argv[8]);

  otb::DEMHandler::Instance()->OpenDEMDirectory(folderPath);

  object->SetOutputOrigin(origin);
  object->SetOutputSize(size);
  object->SetOutputSpacing(spacing);

  std::cout << object << std::endl;

  otb::StandardFilterWatcher watcher(object, "DEM to image generator");

  writer->SetFileName(outputName);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
