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

#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "otbEnergyTextureImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"


int otbEnergyTextureImageFunction(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> ImageType;
  typedef ImageType::SizeType SizeType;
  typedef ImageType::OffsetType OffsetType;

  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;

  typedef otb::EnergyTextureImageFunction<ImageType> FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<ImageType, ImageType, FunctionType> FilterType;

  FunctionType::Pointer energyFunction = FunctionType::New();
  FilterType::Pointer filter = FilterType::New();
  

  // Instantiating object
  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  filter->SetInput(reader->GetOutput());

  SizeType radius;
  radius[0] = atoi(argv[3]);
  radius[1] = atoi(argv[4]);
  energyFunction->SetRadius(radius);
  OffsetType offset;
  offset[0] =  atoi(argv[5]);
  offset[1] =  atoi(argv[6]);
  energyFunction->SetOffset(offset);

  filter->SetFunction(energyFunction);
  writer->SetInput(filter->GetOutput());
  writer->SetNumberOfStreamDivisions(1);
  writer->Update();
  

  return EXIT_SUCCESS;
}
