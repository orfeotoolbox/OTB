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

#include "itkScalarToRGBPixelFunctor.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbLabelizeConfidenceConnectedImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbLabelizeConfidenceConnectedImageFilter(int itkNotUsed(argc), char * argv[])
{
  // Arguments
  char* inputImageName = argv[1];
  char* outputImageName = argv[2];

  typedef unsigned char                InputPixelType;
  typedef unsigned char                OutputPixelType;
  typedef itk::RGBPixel<unsigned char> ColorPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<ColorPixelType, Dimension>  RGBImageType;

  InputPixelType lowerThreshold((InputPixelType) ::atoi(argv[3]));
  InputPixelType upperThreshold((InputPixelType) ::atoi(argv[4]));

  double       multiplier = (double) (atof(argv[5]));
  unsigned int numberOfIterations = (unsigned int) (atoi(argv[6]));
  unsigned int initialRadius = (unsigned int) (atoi(argv[7]));

  // Reader
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageName);

  // Writer
  typedef otb::ImageFileWriter<RGBImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageName);

  // Labelize filter
  typedef otb::LabelizeConfidenceConnectedImageFilter<InputImageType, OutputImageType> LabelizeFilterType;
  LabelizeFilterType::Pointer filter = LabelizeFilterType::New();

  filter->SetLowerThreshold(lowerThreshold);
  filter->SetUpperThreshold(upperThreshold);
  filter->SetMultiplier(multiplier);
  filter->SetNumberOfIterations(numberOfIterations);
  filter->SetInitialNeighborhoodRadius(initialRadius);

  // Rescale intensity
  typedef itk::RescaleIntensityImageFilter<OutputImageType, OutputImageType> RescaleFilterType;
  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  // Label to RGB image
  typedef itk::Functor::ScalarToRGBPixelFunctor<OutputPixelType>                   FunctorType;
  typedef itk::UnaryFunctorImageFilter<OutputImageType, RGBImageType, FunctorType> ColorLabelFilterType;
  ColorLabelFilterType::Pointer labelToRGB = ColorLabelFilterType::New();

  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  labelToRGB->SetInput(rescaler->GetOutput());
  writer->SetInput(labelToRGB->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
