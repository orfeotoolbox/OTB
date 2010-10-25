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

#include "itkExceptionObject.h"
#include "otbDEMToImageGenerator.h"
#include "otbMapProjections.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImage.h"
#include "otbStandardFilterWatcher.h"

int otbDEMToOrthoImageGeneratorTest2(int argc, char * argv[])
{
  if (argc < 9)
    {
    std::cout << argv[0] <<
    " DEM folder path , output filename , Easting Output Orign point , Northing Output Origin point , X Output Size, Y Output size , X Spacing , Y Spacing, Zone, Hemisphere"
              << std::endl;
    return EXIT_FAILURE;
    }

  char * folderPath = argv[1];
  char * outputName = argv[2];

  const unsigned int Dimension = 2;
  typedef otb::Image<double, Dimension>                               ImageType;
  typedef otb::UtmInverseProjection                                   MapProjectionType;
  typedef otb::DEMToImageGenerator<ImageType>                         DEMToImageGeneratorType;
  typedef DEMToImageGeneratorType::DEMHandlerType                     DEMHandlerType;
  typedef DEMHandlerType::PointType                                   PointType;
  typedef DEMToImageGeneratorType::SizeType                           SizeType;
  typedef DEMToImageGeneratorType::SpacingType                        SpacingType;
  typedef otb::StreamingImageFileWriter<ImageType>                    WriterType;

  // Instantiating object
  MapProjectionType::Pointer       mapProj = MapProjectionType::New();
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
  std::cout << atoi(argv[9]) << std::endl;

  mapProj->SetZone(::atoi(argv[9]));
  mapProj->SetHemisphere(argv[10][0]);
  std::string projectionRef = mapProj->GetWkt();

  object->SetDEMDirectoryPath(folderPath);
  object->SetOutputOrigin(origin);
  object->SetOutputSize(size);
  object->SetOutputSpacing(spacing);
  object->SetOutputProjectionRef(projectionRef);

  std::cout << object << std::endl;

  otb::StandardFilterWatcher watcher(object, "DEM to ortho image generator");

  writer->SetFileName(outputName);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
