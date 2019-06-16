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


/* Example usage:
./CorrelationLineDetectorExample Input/amst2.png Output/amstLineCorrelations.png Output/amstLineCorrelationDirections.png 5 1
*/


// This example illustrates the use of the \doxygen{otb}{CorrelationLineDetectorImageFilter}.
// This filter is used for line detection in SAR images. Its principle
// is described in \cite{tup-98}: a line is detected if two parallel
// edges are present in the images. These edges are detected with the
// correlation of means detector.
//
// The first step required to use this filter is to include its header file.

#include "otbLineCorrelationDetectorImageFilter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{

  if (argc != 6)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputEdgesImageFile outputDirectionsImageFile length width" << std::endl;
    return EXIT_FAILURE;
  }

  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with floating point precision
  //  and rescale the results between 0 and 255 in order to export PNG images.

  using InternalPixelType = float;
  using OutputPixelType   = unsigned char;

  //  The images are defined using the pixel type and the dimension.

  using InternalImageType = otb::Image<InternalPixelType, 2>;
  using OutputImageType   = otb::Image<OutputPixelType, 2>;

  //  The filter can be instantiated using the image types defined above.

  using FilterType = otb::LineCorrelationDetectorImageFilter<InternalImageType, InternalImageType>;

  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
  //  image data from a file.

  using ReaderType = otb::ImageFileReader<InternalImageType>;

  // An \doxygen{otb}{ImageFileWriter} is instantiated in order to write the
  // output image to a file.

  using WriterType = otb::ImageFileWriter<OutputImageType>;

  // The intensity rescaling of the results will be carried out by the
  // \code{itk::RescaleIntensityImageFilter} which is templated by the
  // input and output image types.

  using RescalerType = itk::RescaleIntensityImageFilter<InternalImageType, OutputImageType>;

  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to SmartPointers.

  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();

  //  The same is done for the rescaler and the writer.

  RescalerType::Pointer rescaler = RescalerType::New();
  WriterType::Pointer   writer   = WriterType::New();

  reader->SetFileName(argv[1]);

  //  The \code{itk::RescaleIntensityImageFilter} needs to know which
  //  is the minimu and maximum values of the output generated
  //  image. Those can be chosen in a generic way by using the
  //  \code{NumericTraits} functions, since they are templated over
  //  the pixel type.

  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());

  //  The image obtained with the reader is passed as input to the
  //  \doxygen{otb}{LineCorrelationDetectorImageFilter}. The pipeline is built as follows.
  //
  //  \index{otb::LineCorrelationDetectorImageFilter!SetInput()}

  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());

  //  The methods \code{SetLengthLine()} and \code{SetWidthLine()}
  //  allow setting the minimum length and the typical witdh of the
  //  lines which are to be detected.
  //
  //  \index{otb::LineCorrelationDetector!SetWidthLine()}
  //  \index{otb::LineCorrelationDetector!SetLengthLine()}

  filter->SetLengthLine(atoi(argv[4]));
  filter->SetWidthLine(atoi(argv[5]));

  //  The filter is executed by invoking the \code{Update()} method. If the
  //  filter is part of a larger image processing pipeline, calling
  //  \code{Update()} on a downstream filter will also trigger update of this
  //  filter.

  filter->Update();

  writer->SetFileName(argv[2]);
  writer->Update();

  // We can also obtain the direction of the lines by invoking the
  // \code{GetOutputDirections()} method.

  writer->SetFileName(argv[3]);
  rescaler->SetInput(filter->GetOutputDirection());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  //  Figure~\ref{fig:LINECORRELATION_FILTER}
  // shows the result of applying the LineCorrelation edge detector filter
  // to a SAR image.  \begin{figure} \center
  // \includegraphics[width=0.25\textwidth]{amst.eps}
  // \includegraphics[width=0.25\textwidth]{amstLineCorrelations.eps}
  // \includegraphics[width=0.25\textwidth]{amstLineCorrelationDirections.eps}
  // \itkcaption[Line Correlation Detector Application]{Result of applying
  // the \doxygen{otb}{LineCorrelationDetectorImageFilter} to a SAR
  // image. From left to right : original image, line intensity and
  // edge orientation.}  \label{fig:LINECORRELATION_FILTER} \end{figure}
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{otb}{LineCorrelationDetectorImageFilter}
  //  \end{itemize}

  return EXIT_SUCCESS;
}
