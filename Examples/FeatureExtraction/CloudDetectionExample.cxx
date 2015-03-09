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
//  INPUTS: {CloudsOnReunion.tif}
//  OUTPUTS: {CloudDetectionOutput.tif} , {pretty_CloudsOnReunion.png} , {pretty_CloudDetectionOutput.png}
//  553 467 734 581 0.4 0.6 1.0
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// The cloud detection functor is a processing chain composed by the
// computation of a spectral angle (with SpectralAngleFunctor).  The
// result is multiplied by a gaussian factor (with
// CloudEstimatorFunctor) and finally thresholded to obtain a binary
// image (with CloudDetectionFilter).  However, modifications can be
// added in the pipeline to adapt to a particular situation.
//
// This example demonstrates the use of the
// \doxygen{otb}{CloudDetectionFilter}.  This filter uses the spectral
// angle principle to measure the radiometric gap between a reference
// pixel and the other pixels of the image.
//
// The first step toward the use of this filter is the inclusion of
// the proper header files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbCloudDetectionFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiChannelExtractROI.h"

int main(int argc, char * argv[])
{

  if (argc != 12)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    "inputFileName outputFileName printableInputFileName printableOutputFileName";
    std::cerr <<
    "firstPixelComponent secondPixelComponent thirdPixelComponent fourthPixelComponent ";
    std::cerr << "variance ";
    std::cerr << "minThreshold maxThreshold " << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  // Software Guide : BeginLatex
  //
  // Then we must decide what pixel type to use for the images. We choose to do
  // all the computations in double precision.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double InputPixelType;
  typedef double OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The images are defined using the pixel type and the
  //  dimension. Please note that the
  //  \doxygen{otb}{CloudDetectionFilter} needs an
  //  \doxygen{otb}{VectorImage} as input to handle multispectral
  //  images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<InputPixelType, Dimension> VectorImageType;
  typedef VectorImageType::PixelType                  VectorPixelType;
  typedef otb::Image<OutputPixelType, Dimension>      OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We define the functor type that the filter will use. We use the
  // \doxygen{otb}{CloudDetectionFunctor}.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef otb::Functor::CloudDetectionFunctor<VectorPixelType,
      OutputPixelType>   FunctorType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now we can define the \doxygen{otb}{CloudDetectionFilter} that
  // takes a multi-spectral image as input and produces a binary
  // image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::CloudDetectionFilter<VectorImageType, OutputImageType,
      FunctorType> CloudDetectionFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in
  //  order to read image data from a file. Then, an
  //  \doxygen{otb}{ImageFileWriter} is instantiated in order to write
  //  the output image to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The different filters composing our pipeline are created by invoking their
  // \code{New()} methods, assigning the results to smart pointers.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer               reader = ReaderType::New();
  CloudDetectionFilterType::Pointer cloudDetection =
    CloudDetectionFilterType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  reader->SetFileName(argv[1]);
  cloudDetection->SetInput(reader->GetOutput());

  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{CloudDetectionFilter} needs to have a reference
  // pixel corresponding to the spectral content likely to represent a
  // cloud. This is done by passing a pixel to the filter. Here we
  // suppose that the input image has four spectral bands.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  VectorPixelType referencePixel;
  referencePixel.SetSize(4);
  referencePixel.Fill(0.);
  referencePixel[0] = (atof(argv[5]));
  referencePixel[1] = (atof(argv[6]));
  referencePixel[2] = (atof(argv[7]));
  referencePixel[3] = (atof(argv[8]));
  cloudDetection->SetReferencePixel(referencePixel);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We must also set the variance parameter of the filter and the
  // parameter of the gaussian functor.  The bigger the value, the
  // more tolerant the detector will be.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  cloudDetection->SetVariance(atof(argv[9]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The minimum and maximum thresholds are set to binarise the final result.
  // These values have to be between 0 and 1.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  cloudDetection->SetMinThreshold(atof(argv[10]));
  cloudDetection->SetMaxThreshold(atof(argv[11]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginCodeSnippet
  writer->SetFileName(argv[2]);
  writer->SetInput(cloudDetection->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:CLOUDDETECTION_FILTER} shows the result of applying
  // the cloud detection filter to a cloudy image.
  // \begin{figure} \center
  // \includegraphics[width=0.44\textwidth]{pretty_CloudsOnReunion.eps}
  // \includegraphics[width=0.44\textwidth]{pretty_CloudDetectionOutput.eps}
  // \itkcaption[Cloud Detection Example]{From left to right : original image, cloud mask resulting from processing.}
  // \label{fig:CLOUDDETECTION_FILTER}
  // \end{figure}
  //
  // Software Guide : EndLatex

  // Pretty image creation for printing
  typedef otb::Image<unsigned char,
      Dimension>
  OutputPrettyImageType;
  typedef otb::VectorImage<unsigned char,
      Dimension>
  InputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>
  WriterPrettyOutputType;
  typedef otb::ImageFileWriter<InputPrettyImageType>
  WriterPrettyInputType;
  typedef itk::RescaleIntensityImageFilter<OutputImageType,
      OutputPrettyImageType>
  RescalerOutputType;
  typedef otb::VectorRescaleIntensityImageFilter<VectorImageType,
      InputPrettyImageType>
  RescalerInputType;
  typedef otb::MultiChannelExtractROI<InputPixelType,
      InputPixelType>
  ChannelExtractorType;

  ChannelExtractorType::Pointer  selecter           = ChannelExtractorType::New();
  RescalerInputType::Pointer     inputRescaler     = RescalerInputType::New();
  WriterPrettyInputType::Pointer prettyInputWriter = WriterPrettyInputType::New();
  selecter->SetInput(reader->GetOutput());
  selecter->SetChannel(3);
  selecter->SetChannel(2);
  selecter->SetChannel(1);
  inputRescaler->SetInput(selecter->GetOutput());
  VectorPixelType minimum, maximum;
  minimum.SetSize(3);
  maximum.SetSize(3);
  minimum.Fill(0);
  maximum.Fill(255);
  inputRescaler->SetOutputMinimum(minimum);
  inputRescaler->SetOutputMaximum(maximum);
  prettyInputWriter->SetFileName(argv[3]);
  prettyInputWriter->SetInput(inputRescaler->GetOutput());

  RescalerOutputType::Pointer     outputRescaler     = RescalerOutputType::New();
  WriterPrettyOutputType::Pointer prettyOutputWriter =
    WriterPrettyOutputType::New();
  outputRescaler->SetInput(cloudDetection->GetOutput());
  outputRescaler->SetOutputMinimum(0);
  outputRescaler->SetOutputMaximum(255);
  prettyOutputWriter->SetFileName(argv[4]);
  prettyOutputWriter->SetInput(outputRescaler->GetOutput());

  prettyInputWriter->Update();
  prettyOutputWriter->Update();

  return EXIT_SUCCESS;
}
