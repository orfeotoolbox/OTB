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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {StereoFixed.png}, {StereoMoving.png}
//    OUTPUTS: {fcDeformationFieldOutput-horizontal.png}, {fcDeformationFieldOutput-vertical.png}, {fcCorrelFieldOutput.png}, {fcDResampledOutput2.png}
//    1.0 5 3 10
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example demonstrates the use of the \doxygen{otb}{FineCorrelationImageFilter}. This filter performs deformation estimation
// using the classical maximum of correlation look-up in a search window. Three modes are available corresponding to different
// tradeoffs between accuracy and execution time :
// \begin{itemize}
// \item the \emph{coarse} mode estimates only pixel-wise (integer) offsets,
// \item the \emph{lsqr} mode uses a quadric regression to interpolate the position of the sub-pixel maximum,
// \item the \emph{subpixel} mode up-samples the neighborhood of the coarse maximum up to a given factor and
// search for subpixel maximum in the up-sampled neighborhood. This is the slowest but most accurate mode.
// \end{itemize}
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex

#include "otbImage.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileReader.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkWarpImageFilter.h"

// Software Guide : BeginCodeSnippet
#include "otbFineCorrelationImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

#include <iostream>

int main(int argc, char** argv)
{

  if (argc != 11)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " fixedFileName movingFileName fieldOutNameHorizontal fieldOutNameVertical fieldCorrelation warped ";
    std::cerr << "smoothingSigma correlationRadius explorationRadius subpixelPrecision";

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
  // Several type of \doxygen{otb}{Image} are required to represent the input image, the correlation field,
  // and the deformation field.
  //
  // Software Guide : EndLatex

  //Allocate Images
  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType, ImageDimension> InputImageType;
  typedef otb::Image<PixelType, ImageDimension> CorrelationImageType;
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
  // To make the correlation estimation more robust, the first
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
  typedef otb::FineCorrelationImageFilter<InputImageType,
      CorrelationImageType,
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
  typedef RegistrationFilterType::RadiusType RadiusType;

  RadiusType searchRadius;

  searchRadius[0] = atoi(argv[8]);
  searchRadius[1] = atoi(argv[8]);

  registrator->SetSearchRadius(searchRadius);
// Software Guide : EndCodeSnippet

  std::cout << "Exploration radius " << registrator->GetSearchRadius() << std::endl;

  // Software Guide : BeginLatex
  //
  // \item The window used to compute the local correlation. This window is also defined by its radius:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  RadiusType correlationRadius;
  correlationRadius[0] = atoi(argv[9]);
  correlationRadius[1] = atoi(argv[9]);

  registrator->SetRadius(correlationRadius);
// Software Guide : EndCodeSnippet

  std::cout << "Correlation radius " << registrator->GetRadius() << std::endl;


  // Software Guide : BeginLatex
  //
  // In this example we will be using the subpixel mode. To do so, we need to set the given
  // mode flag as well as the subpixel precision we want to obtain:
  //
  // Software Guide : EndLatex
  registrator->SetRefinementModeToSubPixel();
  registrator->SetSubPixelPrecision(atoi(argv[10]));

  // Software Guide : BeginLatex
  //
  // \end{itemize}
  // The execution of the \doxygen{otb}{FineCorrelationImageFilter} will be triggered by
  // the \code{Update()} call on the writer at the end of the
  // pipeline. Make sure to use a
  // \doxygen{otb}{StreamingImageFileWriter} if you want to benefit
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

  typedef otb::StreamingImageFileWriter<OutputImageType> DFWriterType;

  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput(fieldRescaler->GetOutput());
  dfWriter->Update();

  channelExtractor->SetChannel(2);
  dfWriter->SetFileName(argv[4]);
  dfWriter->Update();

  typedef itk::WarpImageFilter<InputImageType, InputImageType,
      DeformationFieldType> WarperType;
  WarperType::Pointer warper = WarperType::New();

  InputImageType::PixelType padValue = 4.0;

  warper->SetInput(mReader->GetOutput());
  warper->SetDeformationField(registrator->GetOutputDeformationField());
  warper->SetEdgePaddingValue(padValue);

  typedef itk::RescaleIntensityImageFilter<CorrelationImageType,
        OutputImageType> CorrelationRescalerType;

  CorrelationRescalerType::Pointer correlationRescaler = CorrelationRescalerType::New();
  correlationRescaler->SetInput(registrator->GetOutput());
  correlationRescaler->SetOutputMinimum(0);
  correlationRescaler->SetOutputMaximum(255);

  typedef otb::StreamingImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetInput(correlationRescaler->GetOutput());
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
  // applying the \doxygen{otb}{FineCorrelationImageFilter}.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.30\textwidth]{StereoFixed.eps}
  // \includegraphics[width=0.30\textwidth]{StereoMoving.eps}
  // \includegraphics[width=0.30\textwidth]{fcDResampledOutput2.eps}
  // \includegraphics[width=0.30\textwidth]{fcCorrelFieldOutput.eps}
  // \includegraphics[width=0.30\textwidth]{fcDeformationFieldOutput-horizontal.eps}
  // \includegraphics[width=0.30\textwidth]{fcDeformationFieldOutput-vertical.eps}
  // \itkcaption[Deformation field and resampling from fine correlation registration]{From left
  // to right and top to bottom: fixed input image, moving image with a low stereo angle, resampled image,
  // correlation field, estimated deformation field in the horizontal direction, estimated deformation field in the vertical direction.}
  // \label{fig:FineCorrelationImageFilterOUTPUT}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
