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

int otbDEMToOrthoImageGeneratorWithInputImageTest(int argc, char * argv[])
{
  if (argc < 6)
    {
    std::cout << argv[0] <<
    " DEM folder path , input filename, output filename , X Output Size, Y Output size "
              << std::endl;
    return EXIT_FAILURE;
    }

  char * folderPath = argv[1];
  char * inputName  = argv[2];
  char * outputName = argv[3];

  const unsigned int Dimension = 2;
  typedef otb::Image<double, Dimension>                               ImageType;
  typedef otb::DEMToImageGenerator<ImageType>                         DEMToImageGeneratorType;
  typedef DEMToImageGeneratorType::SizeType                           SizeType;
  typedef otb::StreamingImageFileWriter<ImageType>                    WriterType;
  typedef otb::ImageFileReader<ImageType>                             ReaderType;

  // Instantiating object
  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();
  ReaderType::Pointer              reader = ReaderType::New();
  WriterType::Pointer              writer = WriterType::New();

  SizeType size;
  size[0] = ::atoi(argv[4]);
  size[1] = ::atoi(argv[5]);

  reader->SetFileName(inputName);
  reader->UpdateOutputInformation();

  object->SetDEMDirectoryPath(folderPath);
//  object->SetOutputOrigin(origin);
//  object->SetOutputSize(size);
//  object->SetOutputSpacing(spacing);
  object->SetOutputParametersFromImage(reader->GetOutput());

  std::cout << object << std::endl;

  otb::StandardFilterWatcher watcher(object, "DEM to ortho image generator");

  writer->SetFileName(outputName);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
