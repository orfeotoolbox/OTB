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
./FineRegistrationImageFilterExample Input/StereoFixed.png \
                                     Input/StereoMoving.png \
                                     Output/fcDisplacementFieldOutput-horizontal.png \
                                     Output/fcDisplacementFieldOutput-vertical.png \
                                     Output/fcCorrelFieldOutput.png \
                                     Output/fcDResampledOutput2.png \
                                     1.0 \
                                     5 \
                                     3 \
                                     0.1
*/


/* Example usage:
./FineRegistrationImageFilterExample Input/StereoFixed.png \
                                     Input/StereoMoving.png \
                                     Output/fcMRSDDisplacementFieldOutput-horizontal.png \
                                     Output/fcMRSDDisplacementFieldOutput-vertical.png \
                                     Output/fcMRSDCorrelFieldOutput.png \
                                     Output/fcMRSDDResampledOutput2.png \
                                     1.0 \
                                     5 \
                                     3 \
                                     0.1 \
                                     mrsd
*/


// This example demonstrates the use of the \doxygen{otb}{FineRegistrationImageFilter}. This filter performs deformation estimation
// using the classical extrema of image-to-image metric look-up in a search window.
//
// The first step toward the use of these filters is to include the proper header files.

#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkWarpImageFilter.h"
#include "itkMeanReciprocalSquareDifferenceImageToImageMetric.h"

#include "otbFineRegistrationImageFilter.h"

#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

#include <iostream>

int main(int argc, char** argv)
{

  if (argc < 11)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedFileName movingFileName fieldOutNameHorizontal fieldOutNameVertical fieldMetric warped ";
    std::cerr << "smoothingSigma metricRadius explorationRadius subpixelPrecision";

    return EXIT_FAILURE;
  }

  const unsigned int ImageDimension = 2;

  using PixelType             = double;
  using DisplacementPixelType = itk::Vector<double, ImageDimension>;

  using OutputPixelType = unsigned char;
  using OutputImageType = otb::Image<OutputPixelType, ImageDimension>;

  // Several type of \doxygen{otb}{Image} are required to represent the input image, the metric field,
  // and the deformation field.

  // Allocate Images
  using InputImageType        = otb::Image<PixelType, ImageDimension>;
  using MetricImageType       = otb::Image<PixelType, ImageDimension>;
  using DisplacementFieldType = otb::Image<DisplacementPixelType, ImageDimension>;

  using InputReaderType            = otb::ImageFileReader<InputImageType>;
  InputReaderType::Pointer fReader = InputReaderType::New();
  fReader->SetFileName(argv[1]);

  InputReaderType::Pointer mReader = InputReaderType::New();
  mReader->SetFileName(argv[2]);

  // To make the metric estimation more robust, the first
  // required step is to blur the input images. This is done using the
  // \doxygen{itk}{RecursiveGaussianImageFilter}:

  // Blur input images
  using InputBlurType = itk::RecursiveGaussianImageFilter<InputImageType, InputImageType>;

  InputBlurType::Pointer fBlur = InputBlurType::New();
  fBlur->SetInput(fReader->GetOutput());
  fBlur->SetSigma(atof(argv[7]));

  InputBlurType::Pointer mBlur = InputBlurType::New();
  mBlur->SetInput(mReader->GetOutput());
  mBlur->SetSigma(atof(argv[7]));

  // Now, we declare and instantiate the \doxygen{otb}{FineRegistrationImageFilter} which is going to perform the registration:

  // Create the filter
  using RegistrationFilterType = otb::FineRegistrationImageFilter<InputImageType, MetricImageType, DisplacementFieldType>;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingInput(mBlur->GetOutput());
  registrator->SetFixedInput(fBlur->GetOutput());

  // Some parameters need to be specified to the filter:
  // \begin{itemize}
  // \item The area where the search is performed. This area is defined by its radius:

  using RadiusType = RegistrationFilterType::SizeType;

  RadiusType searchRadius;

  searchRadius[0] = atoi(argv[8]);
  searchRadius[1] = atoi(argv[8]);

  registrator->SetSearchRadius(searchRadius);

  std::cout << "Exploration radius " << registrator->GetSearchRadius() << std::endl;

  // \item The window used to compute the local metric. This window is also defined by its radius:

  RadiusType metricRadius;
  metricRadius[0] = atoi(argv[9]);
  metricRadius[1] = atoi(argv[9]);

  registrator->SetRadius(metricRadius);

  std::cout << "Metric radius " << registrator->GetRadius() << std::endl;

  // We need to set the sub-pixel accuracy we want to obtain:
  registrator->SetConvergenceAccuracy(atof(argv[10]));

  // The default matching metric used by the \doxygen{otb}{FineRegistrationImageFilter} is standard correlation.
  // However, we may also use any other image-to-image metric provided by ITK. For instance, here is how we
  // would use the \doxygen{itk}{MutualInformationImageToImageMetric} (do not forget to include the proper header).

  if (argc > 11)
  {
    using MRSDMetricType               = itk::MeanReciprocalSquareDifferenceImageToImageMetric<InputImageType, InputImageType>;
    MRSDMetricType::Pointer mrsdMetric = MRSDMetricType::New();
    registrator->SetMetric(mrsdMetric);

    // The \doxygen{itk}{MutualInformationImageToImageMetric} produces low value for poor matches, therefore, the filter has
    // to maximize the metric :

    registrator->MinimizeOff();
  }


  // \end{itemize}
  // The execution of the \doxygen{otb}{FineRegistrationImageFilter} will be triggered by
  // the \code{Update()} call on the writer at the end of the
  // pipeline. Make sure to use a
  // \doxygen{otb}{ImageFileWriter} if you want to benefit
  // from the streaming features.

  using ChannelExtractionFilterType                     = otb::ImageOfVectorsToMonoChannelExtractROI<DisplacementFieldType, InputImageType>;
  ChannelExtractionFilterType::Pointer channelExtractor = ChannelExtractionFilterType::New();

  channelExtractor->SetInput(registrator->GetOutputDisplacementField());
  channelExtractor->SetChannel(1);

  using RescalerType                  = itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>;
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

  using WarperType           = itk::WarpImageFilter<InputImageType, InputImageType, DisplacementFieldType>;
  WarperType::Pointer warper = WarperType::New();

  InputImageType::PixelType padValue = 4.0;

  warper->SetInput(mReader->GetOutput());
  warper->SetDisplacementField(registrator->GetOutputDisplacementField());
  warper->SetEdgePaddingValue(padValue);

  using MetricRescalerType = itk::RescaleIntensityImageFilter<MetricImageType, OutputImageType>;

  MetricRescalerType::Pointer metricRescaler = MetricRescalerType::New();
  metricRescaler->SetInput(registrator->GetOutput());
  metricRescaler->SetOutputMinimum(0);
  metricRescaler->SetOutputMaximum(255);

  using WriterType = otb::ImageFileWriter<OutputImageType>;

  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetInput(metricRescaler->GetOutput());
  writer1->SetFileName(argv[5]);
  writer1->Update();

  using CastFilterType           = itk::CastImageFilter<InputImageType, OutputImageType>;
  CastFilterType::Pointer caster = CastFilterType::New();
  caster->SetInput(warper->GetOutput());


  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(argv[6]);
  writer2->SetInput(caster->GetOutput());
  writer2->Update();

  // Figure~\ref{fig:FineCorrelationImageFilterOUTPUT} shows the result of
  // applying the \doxygen{otb}{FineRegistrationImageFilter}.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.2\textwidth]{StereoFixed.eps}
  // \includegraphics[width=0.2\textwidth]{StereoMoving.eps}
  // \includegraphics[width=0.2\textwidth]{fcCorrelFieldOutput.eps}
  // \includegraphics[width=0.2\textwidth]{fcMRSDCorrelFieldOutput.eps}
  // \includegraphics[width=0.2\textwidth]{fcDResampledOutput2.eps}
  // \includegraphics[width=0.2\textwidth]{fcMRSDDResampledOutput2.eps}
  // \includegraphics[width=0.2\textwidth]{fcDisplacementFieldOutput-horizontal.eps}
  // \includegraphics[width=0.2\textwidth]{fcMRSDDisplacementFieldOutput-horizontal.eps}
  // \itkcaption[Displacement field and resampling from fine registration]{From left
  // to right and top to bottom: fixed input image, moving image with a low stereo angle,
  // local correlation field, local mean reciprocal square difference field,
  // resampled image based on correlation, resampled image based on mean reciprocal square difference,
  //  estimated epipolar deformation using on correlation,
  // estimated epipolar deformation using mean reciprocal square difference.
  // }
  // \label{fig:FineCorrelationImageFilterOUTPUT}
  // \end{figure}

  return EXIT_SUCCESS;
}
