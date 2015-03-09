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


//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {QB_Suburb.png}
//    OUTPUTS: {LabelizeNeighborhoodConnectedImageFilterOutput.png}
//    170 170
//    80 80
//  Software Guide : EndCommandLineArgs

#include <iostream>

#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

// Software Guide : BeginCodeSnippet
#include "otbLabelizeNeighborhoodConnectedImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{
  if (argc < 7)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] \
              << "  inputImageFile  outputImageFile  " \
              << "seedLowerThreshold seedUpperThreshold  " \
              << "deltaLower  deltaUpper  " << std::endl;

    return EXIT_FAILURE;
    }

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
  InputPixelType deltaLower((InputPixelType) ::atoi(argv[5]));
  InputPixelType deltaUpper((InputPixelType) ::atoi(argv[6]));

  // Reader
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageName);

  // Writer
  typedef otb::ImageFileWriter<RGBImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageName);

  // Labelize filter
  typedef otb::LabelizeNeighborhoodConnectedImageFilter<InputImageType,
      OutputImageType>
  LabelizeFilterType;
  LabelizeFilterType::Pointer filter = LabelizeFilterType::New();

  filter->SetLowerThreshold(lowerThreshold);
  filter->SetUpperThreshold(upperThreshold);
  filter->SetLowerThresholdDelta(deltaLower);
  filter->SetUpperThresholdDelta(deltaUpper);

  // Label to RGB image
  typedef itk::Functor::ScalarToRGBPixelFunctor<OutputPixelType>
  FunctorType;
  typedef itk::UnaryFunctorImageFilter<OutputImageType, RGBImageType,
      FunctorType> ColorLabelFilterType;
  ColorLabelFilterType::Pointer labelToRGB = ColorLabelFilterType::New();

  filter->SetInput(reader->GetOutput());
  labelToRGB->SetInput(filter->GetOutput());
  writer->SetInput(labelToRGB->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
