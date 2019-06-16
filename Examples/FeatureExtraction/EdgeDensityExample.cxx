/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

/* Example usage:
./EdgeDensityExample Input/suburb2.jpeg Output/EdgeDensityOutput.tif Output/PrettyEdgeDensityOutput.png 7 50 10 1.0 0.01
*/


// This example illustrates the use of the
// \doxygen{otb}{EdgeDensityImageFilter}.
// This filter computes a local density of edges on an image and can
// be useful to detect man made objects or urban areas, for
// instance. The filter has been implemented in a generic way, so that
// the way the edges are detected and the way their density is
// computed can be chosen by the user.
//
// The first step required to use this filter is to include its header file.

#include "otbEdgeDensityImageFilter.h"
// We will also include the header files for the edge detector (a
// Canny filter) and the density estimation (a simple count on a
// binary image).
//
// The first step required to use this filter is to include its header file.

#include "itkCannyEdgeDetectionImageFilter.h"
#include "otbBinaryImageDensityFunction.h"

int main(int itkNotUsed(argc), char* argv[])
{

  const char* infname        = argv[1];
  const char* outfname       = argv[2];
  const char* prettyfilename = argv[3];

  const unsigned int radius = atoi(argv[4]);

  /*--*/

  const unsigned int Dimension = 2;
  using PixelType              = float;

  /** Variables for the canny detector*/
  const PixelType upperThreshold = static_cast<PixelType>(atof(argv[5]));
  const PixelType lowerThreshold = static_cast<PixelType>(atof(argv[6]));
  const double    variance       = atof(argv[7]);
  const double    maximumError   = atof(argv[8]);

  // As usual, we start by defining the types for the images, the reader
  // and the writer.

  using ImageType  = otb::Image<PixelType, Dimension>;
  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<ImageType>;
  // We define now the type for the function which will be used by the
  // edge density filter to estimate this density. Here we choose a
  // function which counts the number of non null pixels per area. The
  // function takes as template the type of the image to be processed.

  using CountFunctionType = otb::BinaryImageDensityFunction<ImageType>;
  // These {\em non null pixels} will be the result of an edge
  // detector. We use here the classical Canny edge detector, which is
  // templated over the input and output image types.

  using CannyDetectorType = itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>;
  // Finally, we can define the type for the edge density filter which
  // takes as template the input and output image types, the edge
  // detector type, and the count function type..

  using EdgeDensityFilterType = otb::EdgeDensityImageFilter<ImageType, ImageType, CannyDetectorType, CountFunctionType>;
  // We can now instantiate the different processing objects of the
  // pipeline using the \code{New()} method.

  ReaderType::Pointer            reader      = ReaderType::New();
  EdgeDensityFilterType::Pointer filter      = EdgeDensityFilterType::New();
  CannyDetectorType::Pointer     cannyFilter = CannyDetectorType::New();
  WriterType::Pointer            writer      = WriterType::New();
  // The edge detection filter needs to be instantiated because we
  // need to set its parameters. This is what we do here for the Canny
  // filter.

  cannyFilter->SetUpperThreshold(upperThreshold);
  cannyFilter->SetLowerThreshold(lowerThreshold);
  cannyFilter->SetVariance(variance);
  cannyFilter->SetMaximumError(maximumError);
  // After that, we can pass the edge detector to the filter which
  // will be used it internally.

  filter->SetDetector(cannyFilter);
  filter->SetNeighborhoodRadius(radius);
  // Finally, we set the file names for the input and the output
  // images and we plug the pipeline. The \code{Update()} method of
  // the writer will trigger the processing.

  reader->SetFileName(infname);
  writer->SetFileName(outfname);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  // Figure~\ref{fig:EDGEDENSITY_FILTER} shows the result of applying
  // the edge density filter to an image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{suburb2.eps}
  // \includegraphics[width=0.25\textwidth]{PrettyEdgeDensityOutput.eps}
  // \itkcaption[Edge Density Filter]{Result of applying the
  // \doxygen{otb}{EdgeDensityImageFilter} to an image. From left to right :
  // original image, edge density.}
  // \label{fig:EDGEDENSITY_FILTER}
  // \end{figure}

  /************* Image for printing **************/

  using OutputImageType = otb::Image<unsigned char, 2>;

  using RescalerType = itk::RescaleIntensityImageFilter<ImageType, OutputImageType>;

  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  rescaler->SetInput(filter->GetOutput());

  using OutputWriterType              = otb::ImageFileWriter<OutputImageType>;
  OutputWriterType::Pointer outwriter = OutputWriterType::New();

  outwriter->SetFileName(prettyfilename);
  outwriter->SetInput(rescaler->GetOutput());
  outwriter->Update();

  return EXIT_SUCCESS;
}
