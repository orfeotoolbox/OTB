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

#include "otbPhysicalToRPCSensorModelImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbPhysicalToRPCSensorModelImageFilter(int argc, char* argv[])
{
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef otb::PhysicalToRPCSensorModelImageFilter<ImageType> PhysicalToSensorModelType;
  typedef PhysicalToSensorModelType::SizeType         SizeType;

  // Object instanciation
  PhysicalToSensorModelType::Pointer estimator = PhysicalToSensorModelType::New();

  if(argc == 4)
    {
    const char * infname  = argv[1];
    unsigned int gridSize = atoi(argv[2]);
    const char * outfname  = argv[3];
    
    // Reader
    typedef otb::ImageFileReader<ImageType>             ReaderType;
    ReaderType::Pointer                reader    = ReaderType::New();
  
    // Set the fileName
    reader->SetFileName(infname);
    reader->UpdateOutputInformation();
    
    // wire the filter
    estimator->SetInput(reader->GetOutput());
    estimator->SetGridSize(gridSize);

    // Write the resampled image
    typedef otb::ImageFileWriter<ImageType>    WriterType;
    WriterType::Pointer writer= WriterType::New();
    writer->SetAutomaticTiledStreaming();
    writer->SetFileName(outfname);
    writer->SetInput(estimator->GetOutput());
    writer->Update();
    }
  else
    {
    std::cout <<"Unit Test" << std::endl;
    }
  
  return 0;
}
