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

#include "otbWaterSqrtSpectralAngleImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbWaterSqrtSpectralAngleImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension> InputImageType;
  typedef otb::Image<double, Dimension>       OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::WaterSqrtSpectralAngleImageFilter<InputImageType, OutputImageType>
  WaterSqrtSpectralAngleImageFilterType;

  // Instantiating objects
  WaterSqrtSpectralAngleImageFilterType::Pointer filter = WaterSqrtSpectralAngleImageFilterType::New();
  ReaderType::Pointer                            reader = ReaderType::New();
  WriterType::Pointer                            writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int blueChannel(::atoi(argv[3]));
  unsigned int greenChannel(::atoi(argv[4]));
  unsigned int redChannel(::atoi(argv[5]));
  unsigned int nirChannel(::atoi(argv[6]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filter->GetFunctor().SetBlueChannel(blueChannel);
  filter->GetFunctor().SetGreenChannel(greenChannel);
  filter->GetFunctor().SetRedChannel(redChannel);
  filter->GetFunctor().SetNIRChannel(nirChannel);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;

}
