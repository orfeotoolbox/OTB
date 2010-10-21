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
#include "otbImageFileReader.h"
#include "otbExtractROI.h"

int otbDEMToOrthoImageGeneratorWithInputImageTest(int argc, char * argv[])
{
  if (argc < 4)
    {
    std::cout << argv[0] <<
    " DEM folder path , input filename, output filename "
              << std::endl;
    return EXIT_FAILURE;
    }

  char * folderPath = argv[1];
  char * inputName  = argv[2];
  char * outputName = argv[3];

  const unsigned int Dimension = 2;
  typedef double                                                      PixelType;
  typedef otb::Image<PixelType, Dimension>                            ImageType;
  typedef otb::DEMToImageGenerator<ImageType>                         DEMToImageGeneratorType;
  typedef DEMToImageGeneratorType::SizeType                           SizeType;
  typedef otb::StreamingImageFileWriter<ImageType>                    WriterType;
  typedef otb::ImageFileReader<ImageType>                             ReaderType;

  // Instantiating object
  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();
  ReaderType::Pointer              reader = ReaderType::New();
  WriterType::Pointer              writer = WriterType::New();

  reader->SetFileName(inputName);
  reader->UpdateOutputInformation();
  unsigned int startX = 10;
  unsigned int startY = 10;
  unsigned int sizeX = 500;
  unsigned int sizeY = 500;

  typedef otb::ExtractROI<PixelType, PixelType> ExtractROIType;
  ExtractROIType::Pointer filterRoi = ExtractROIType::New();

  filterRoi->SetStartX(startX);
  filterRoi->SetStartY(startY);
  filterRoi->SetSizeX(sizeX);
  filterRoi->SetSizeY(sizeY);

  filterRoi->SetInput(reader->GetOutput());
  filterRoi->UpdateOutputInformation();

  object->SetDEMDirectoryPath(folderPath);
  object->SetOutputParametersFromImage(filterRoi->GetOutput());

  std::cout << object << std::endl;

  otb::StandardFilterWatcher watcher(object, "DEM to ortho image generator");

  writer->SetFileName(outputName);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
