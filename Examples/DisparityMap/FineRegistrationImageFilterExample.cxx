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
//    INPUTS: {StereoFixed.png}, {StereoMoving.png}
//    OUTPUTS: {fcDeformationFieldOutput-horizontal.png}, {fcDeformationFieldOutput-vertical.png}, {fcCorrelFieldOutput.png}, {fcDResampledOutput2.png}
//    1.0 5 3 0.1
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {StereoFixed.png}, {StereoMoving.png}
//    OUTPUTS: {fcMRSDDeformationFieldOutput-horizontal.png}, {fcMRSDDeformationFieldOutput-vertical.png}, {fcMRSDCorrelFieldOutput.png}, {fcMRSDDResampledOutput2.png}
//    1.0 5 3 0.1 mrsd
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example demonstrates the use of the \doxygen{otb}{FineRegistrationImageFilter}. This filter performs deformation estimation
// using the classical extrema of image-to-image metric look-up in a search window.
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "otbWarpImageFilter.h"
#include "itkMeanReciprocalSquareDifferenceImageToImageMetric.h"

// Software Guide : BeginCodeSnippet
#include "otbFineRegistrationImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

#include <iostream>

int main(int argc, char** argv)
{

  if (argc < 11)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " fixedFileName movingFileName fieldOutNameHorizontal fieldOutNameVertical fieldMetric warped ";
    std::cerr << "smoothingSigma metricRadius explorationRadius subpixelPrecision";

    return EXIT_FAILURE;
    }

  const unsigned int ImageDimension = 2;

  typedef double                              PixelType;
  typedef itk::Vector<double, ImageDimension> DeformationPixelType;

  typedef double CoordinateRepresentationType;

  typedef unsigned char                               OutputPixelType;
  typedef otb::Image<OutputPixelType, ImageDimension> OutputImageType;

  // Software Guide : BeginLatex
  //
  // Several type of \doxygen{otb}{Image} are required to represent the input image, the metric field,
  // and the deformation field.
  //
  // Software Guide : EndLatex

  //Allocate Images
  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType, ImageDimension> InputImageType;
  typedef otb::Image<PixelType, ImageDimension> MetricImageType;
  typedef otb::Image<DeformationPixelType,
      ImageDimension>                           DeformationFieldType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<InputImageType> InputReaderType;
  InputReaderType::Pointer fReader = InputReaderType::New();
  fReader->SetFileName(argv[1]);

  InputReaderType::Pointer mReader = InputReaderType::New();
  mReader->SetFileName(argv[2]);

  // Software Guide : BeginLatex
  //
  // To make the metric estimation more robust, the first
  // required step is to blur the input images. This is done using the
  // \doxygen{itk}{RecursiveGaussianImageFilter}:
  //
  // Software Guide : EndLatex

  //Blur input images
  // Software Guide : BeginCodeSnippet
  typedef itk::RecursiveGaussianImageFilter<InputImageType,
      InputImageType> InputBlurType;

  InputBlurType::Pointer fBlur = InputBlurType::New();
  fBlur->SetInput(fReader->GetOutput());
  fBlur->SetSigma(atof(argv[7]));

  InputBlurType::Pointer mBlur = InputBlurType::New();
  mBlur->SetInput(mReader->GetOutput());
  mBlur->SetSigma(atof(argv[7]));
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now, we declare and instantiate the \doxygen{otb}{FineCorrelationImageFilter} which is going to perform the registration:
  //
  // Software Guide : EndLatex

  //Create the filter
  // Software Guide : BeginCodeSnippet
  typedef otb::FineRegistrationImageFilter<InputImageType,
      MetricImageType,
      DeformationFieldType>
  RegistrationFilterType;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingInput(mBlur->GetOutput());
  registrator->SetFixedInput(fBlur->GetOutput());
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Some parameters need to be specified to the filter:
  // \begin{itemize}
  // \item The area where the search is performed. This area is defined by its radius:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef RegistrationFilterType::SizeType RadiusType;

  RadiusType searchRadius;

  searchRadius[0] = atoi(argv[8]);
  searchRadius[1] = atoi(argv[8]);

  registrator->SetSearchRadius(searchRadius);
// Software Guide : EndCodeSnippet

  std::cout << "Exploration radius " << registrator->GetSearchRadius() << std::endl;

  // Software Guide : BeginLatex
  //
  // \item The window used to compute the local metric. This window is also defined by its radius:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  RadiusType metricRadius;
  metricRadius[0] = atoi(argv[9]);
  metricRadius[1] = atoi(argv[9]);

  registrator->SetRadius(metricRadius);
// Software Guide : EndCodeSnippet

  std::cout << "Metric radius " << registrator->GetRadius() << std::endl;

  // Software Guide : BeginLatex
  //
  // We need to set the sub-pixel accuracy we want to obtain:
  //
  // Software Guide : EndLatex
  registrator->SetSubPixelAccuracy(atof(argv[10]));

  // Software Guide : BeginLatex
  //
  // The default matching metric used by the \doxygen{FineRegistrationImageFilter} is standard correlation.
  // However, we may also use any other image-to-image metric provided by ITK. For instance, here is how we
  // would use the \doxygen{itk}{MutualInformationImageToImageMetric} (do not forget to include the proper header).
  //
  // Software Guide : EndLatex

  if(argc > 11)
      {
      // Software Guide : BeginCodeSnippet
      typedef itk::MeanReciprocalSquareDifferenceImageToImageMetric
        <InputImageType, InputImageType> MRSDMetricType;
      MRSDMetricType::Pointer mrsdMetric = MRSDMetricType::New();
      registrator->SetMetric(mrsdMetric);
      // Software Guide : EndCodeSnippet

      // Software Guide : BeginLatex
      //
      // The \doxygen{itk}{MutualInformationImageToImageMetric} produces low value for poor matches, therefore, the filter has
      // to maximize the metric :
      //
      // Software Guide : EndLatex

      // Software Guide : BeginCodeSnippet
      registrator->MinimizeOff();
      // Software Guide : EndCodeSnippet
      }


  // Software Guide : BeginLatex
  //
  // \end{itemize}
  // The execution of the \doxygen{otb}{FineRegistrationImageFilter} will be triggered by
  // the \code{Update()} call on the writer at the end of the
  // pipeline. Make sure to use a
  // \doxygen{otb}{ImageFileWriter} if you want to benefit
  // from the streaming features.
  //
  // Software Guide : EndLatex

  typedef otb::ImageOfVectorsToMonoChannelExtractROI<DeformationFieldType,
      InputImageType>
  ChannelExtractionFilterType;
  ChannelExtractionFilterType::Pointer channelExtractor =
    ChannelExtractionFilterType::New();

  channelExtractor->SetInput(registrator->GetOutputDeformationField());
  channelExtractor->SetChannel(1);

  typedef itk::RescaleIntensityImageFilter<InputImageType,
      OutputImageType> RescalerType;
  RescalerType::Pointer fieldRescaler = RescalerType::New();

  fieldRescaler->SetInput(channelExtractor->GetOutput());
  fieldRescaler->SetOutputMaximum(255);
  fieldRescaler->SetOutputMinimum(0);

  typedef otb::ImageFileWriter<OutputImageType> DFWriterType;

  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput(fieldRescaler->GetOutput());
  dfWriter->Update();

  channelExtractor->SetChannel(2);
  dfWriter->SetFileName(argv[4]);
  dfWriter->Update();

  typedef otb::WarpImageFilter<InputImageType, InputImageType,
      DeformationFieldType> WarperType;
  WarperType::Pointer warper = WarperType::New();

  InputImageType::PixelType padValue = 4.0;

  warper->SetInput(mReader->GetOutput());
  warper->SetDeformationField(registrator->GetOutputDeformationField());
  warper->SetEdgePaddingValue(padValue);

  typedef itk::RescaleIntensityImageFilter<MetricImageType,
        OutputImageType> MetricRescalerType;

  MetricRescalerType::Pointer metricRescaler = MetricRescalerType::New();
  metricRescaler->SetInput(registrator->GetOutput());
  metricRescaler->SetOutputMinimum(0);
  metricRescaler->SetOutputMaximum(255);

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetInput(metricRescaler->GetOutput());
  writer1->SetFileName(argv[5]);
  writer1->Update();

  typedef itk::CastImageFilter<InputImageType, OutputImageType> CastFilterType;
  CastFilterType::Pointer caster =  CastFilterType::New();
  caster->SetInput(warper->GetOutput());


  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(argv[6]);
  writer2->SetInput(caster->GetOutput());
  writer2->Update();

  // Software Guide : BeginLatex
  //
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
  // \includegraphics[width=0.2\textwidth]{fcDeformationFieldOutput-horizontal.eps}
  // \includegraphics[width=0.2\textwidth]{fcMRSDDeformationFieldOutput-horizontal.eps}
  // \itkcaption[Deformation field and resampling from fine registration]{From left
  // to right and top to bottom: fixed input image, moving image with a low stereo angle,
  // local correlation field, local mean reciprocal square difference field,
  // resampled image based on correlation, resampled image based on mean reciprocal square difference,
  //  estimated epipolar deformation using on correlation,
  // estimated epipolar deformation using mean reciprocal square difference.
  // }
  // \label{fig:FineCorrelationImageFilterOUTPUT}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
