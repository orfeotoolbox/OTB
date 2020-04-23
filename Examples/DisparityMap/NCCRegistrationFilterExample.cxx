/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/* Example usage:
./NCCRegistrationFilterExample Input/StereoFixed.png \
                               Input/StereoMoving.png \
                               Output/deformationFieldOutput-horizontal.png \
                               Output/deformationFieldOutput-vertical.png \
                               Output/resampledOutput2.png \
                               5 \
                               1.0 \
                               2
*/


// This example demonstrates the use of the \doxygen{otb}{NCCRegistrationFilter}. This filter performs deformation estimation
// by optimising a PDE based on the normalized correlation coefficient. It uses the finite difference solver hierarchy.
//
// The first step toward the use of these filters is to include the proper header files.

#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

#include "otbNCCRegistrationFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkWarpImageFilter.h"

#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

#include <iostream>

int main(int argc, char** argv)
{

  if (argc != 9)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedFileName movingFileName fieldOutNameHorizontal fieldOutNameVertical imageOutName ";
    std::cerr << "explorationSize bluringSigma nbIterations ";

    return EXIT_FAILURE;
  }

  const unsigned int ImageDimension = 2;

  using PixelType             = double;
  using DisplacementPixelType = itk::Vector<double, ImageDimension>;

  using OutputPixelType = unsigned char;
  using OutputImageType = otb::Image<OutputPixelType, ImageDimension>;

  // Several type of \doxygen{otb}{Image} are required to represent the reference image (fixed)
  // the image we want to register (moving) and the deformation field.

  // Allocate Images
  using MovingImageType       = otb::Image<PixelType, ImageDimension>;
  using FixedImageType        = otb::Image<PixelType, ImageDimension>;
  using DisplacementFieldType = otb::Image<DisplacementPixelType, ImageDimension>;

  using FixedReaderType            = otb::ImageFileReader<FixedImageType>;
  FixedReaderType::Pointer fReader = FixedReaderType::New();
  fReader->SetFileName(argv[1]);

  using MovingReaderType            = otb::ImageFileReader<MovingImageType>;
  MovingReaderType::Pointer mReader = MovingReaderType::New();
  mReader->SetFileName(argv[2]);

  // To make the correlation estimation more robust, the first
  // required step is to blur the input images. This is done using the
  // \doxygen{itk}{RecursiveGaussianImageFilter}:

  // Blur input images
  using FixedBlurType = itk::RecursiveGaussianImageFilter<FixedImageType, FixedImageType>;

  FixedBlurType::Pointer fBlur = FixedBlurType::New();
  fBlur->SetInput(fReader->GetOutput());
  fBlur->SetSigma(std::stof(argv[7]));

  using MovingBlurType = itk::RecursiveGaussianImageFilter<MovingImageType, MovingImageType>;

  MovingBlurType::Pointer mBlur = MovingBlurType::New();
  mBlur->SetInput(mReader->GetOutput());
  mBlur->SetSigma(std::stof(argv[7]));

  // Now, we need to instantiate the NCCRegistrationFilter which is going to perform the registration:

  // Create the filter
  using RegistrationFilterType = otb::NCCRegistrationFilter<FixedImageType, MovingImageType, DisplacementFieldType>;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingImage(mBlur->GetOutput());
  registrator->SetFixedImage(fBlur->GetOutput());

  // Some parameters need to be specified to the NCCRegistrationFilter:
  // \begin{itemize}
  // \item The area where the search is performed. This area is defined by its radius:

  using RadiusType = RegistrationFilterType::RadiusType;

  RadiusType radius;

  radius[0] = std::stoi(argv[6]);
  radius[1] = std::stoi(argv[6]);

  registrator->SetNCCRadius(radius);

  std::cout << "NCC radius " << registrator->GetNCCRadius() << std::endl;

  // \item The number of iterations for the PDE resolution:

  registrator->SetNumberOfIterations(std::stoi(argv[8]));
  // registrator->GetDisplacementField();

  // \end{itemize}
  // The execution of the NCCRegistrationFilter will be triggered by
  // the \code{Update()} call on the writer at the end of the
  // pipeline. Make sure to use a
  // \doxygen{otb}{ImageFileWriter} if you want to benefit
  // from the streaming features.

  using ChannelExtractionFilterType                     = otb::ImageOfVectorsToMonoChannelExtractROI<DisplacementFieldType, MovingImageType>;
  ChannelExtractionFilterType::Pointer channelExtractor = ChannelExtractionFilterType::New();

  channelExtractor->SetInput(registrator->GetOutput());
  channelExtractor->SetChannel(1);

  using RescalerType                  = itk::RescaleIntensityImageFilter<MovingImageType, OutputImageType>;
  RescalerType::Pointer fieldRescaler = RescalerType::New();

  fieldRescaler->SetInput(channelExtractor->GetOutput());
  fieldRescaler->SetOutputMaximum(255);
  fieldRescaler->SetOutputMinimum(0);

  using DFWriterType = otb::ImageFileWriter<OutputImageType>;

  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput(fieldRescaler->GetOutput());
  dfWriter->Update();

  channelExtractor->SetChannel(2);
  dfWriter->SetFileName(argv[4]);
  dfWriter->Update();

  using WarperType           = itk::WarpImageFilter<MovingImageType, MovingImageType, DisplacementFieldType>;
  WarperType::Pointer warper = WarperType::New();

  MovingImageType::PixelType padValue = 4.0;

  warper->SetInput(mReader->GetOutput());
  warper->SetDisplacementField(registrator->GetOutput());
  warper->SetEdgePaddingValue(padValue);

  using CastFilterType           = itk::CastImageFilter<MovingImageType, OutputImageType>;
  CastFilterType::Pointer caster = CastFilterType::New();
  caster->SetInput(warper->GetOutput());

  using WriterType = otb::ImageFileWriter<OutputImageType>;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[5]);
  writer->SetInput(caster->GetOutput());
  writer->Update();

  // Figure~\ref{fig:NCCRegistrationFilterOUTPUT} shows the result of
  // applying the disparity map estimation.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{StereoFixed.eps}
  // \includegraphics[width=0.40\textwidth]{StereoMoving.eps}
  // \includegraphics[width=0.40\textwidth]{deformationFieldOutput-horizontal.eps}
  // \includegraphics[width=0.40\textwidth]{deformationFieldOutput-vertical.eps}
  // \itkcaption[Displacement field and resampling from NCC registration]{From left
  // to right and top to bottom: fixed input image, moving image with a low stereo angle,
  // estimated deformation field in the horizontal direction, estimated deformation field in the vertical direction.}
  // \label{fig:NCCRegistrationFilterOUTPUT}
  // \end{figure}

  return EXIT_SUCCESS;
}
