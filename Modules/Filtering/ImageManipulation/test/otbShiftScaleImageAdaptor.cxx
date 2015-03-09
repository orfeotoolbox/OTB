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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbShiftScaleImageAdaptor.h"

int otbShiftScaleImageAdaptor(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double                                                        PixelType;
  typedef unsigned char                                                 OutputPixelType;
  typedef otb::Image<PixelType, 2>                                      InputImageType;
  typedef otb::Image<OutputPixelType, 2>                                OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                          ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                         WriterType;
  typedef otb::ShiftScaleImageAdaptor<InputImageType, PixelType>        FilterType;
  typedef itk::RescaleIntensityImageFilter<FilterType, OutputImageType> RescalerType;

  FilterType::Pointer   filter   = FilterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();
  ReaderType::Pointer   reader   = ReaderType::New();
  WriterType::Pointer   writer   = WriterType::New();

  filter->SetScale(3.);
  filter->SetShift(10.);

  reader->SetFileName(inputFilename);

  filter->SetImage(reader->GetOutput());

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(filter);

  writer->SetFileName(outputFilename);
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;

}
