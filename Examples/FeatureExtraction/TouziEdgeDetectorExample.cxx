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
./TouziEdgeDetectorExample Input/amst2.png Output/amstTouziEdges.png Output/amstTouziDirections.png 3
*/


// This example illustrates the use of the \doxygen{otb}{TouziEdgeDetectorImageFilter}.
// This filter belongs to the family of the fixed false alarm rate
// edge detectors but it is appropriate for SAR images, where the
// speckle noise is considered as multiplicative. By analogy with the
// classical gradient-based edge detectors which are suited to the
// additive noise case, this filter computes a ratio of local means in
// both sides of the edge \cite{touzi88}. In order to have a
// normalized response, the following computation is performed :
// \begin{equation}
// r = 1 - min\{\frac{\mu_A}{\mu_B},\frac{\mu_B}{\mu_A} \},
// \end{equation}
// where $\mu_A$ and $\mu_B$ are the local means computed at both
// sides of the edge. In order to detect edges with any orientation,
// $r$ is computed for the 4 principal directions and the maximum
// response is kept.
//
// The first step required to use this filter is to include its header file.

#include "otbTouziEdgeDetectorImageFilter.h"

#include "otbImageFileReader.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{

  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputEdgesImageFile outputDirectionsImageFile radius" << std::endl;
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

  using FilterType = otb::TouziEdgeDetectorImageFilter<InternalImageType, InternalImageType>;

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
  //  \doxygen{otb}{TouziEdgeDetectorImageFilter}. The pipeline is built as follows.
  //
  //  \index{otb::TouziEdgeDetectorImageFilter!SetInput()}

  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());

  //  The method \code{SetRadius()} defines the size of the window to
  //  be used for the computation of the local means.
  //
  //  \index{otb::LeeImageFilter!SetRadius()}
  //  \index{otb::LeeImageFilter!NbLooks()}
  //  \index{SetNbLooks()!otb::LeeImageFilter}

  FilterType::SizeType Radius;
  Radius[0] = atoi(argv[4]);
  Radius[1] = atoi(argv[4]);

  filter->SetRadius(Radius);

  //  The filter is executed by invoking the \code{Update()} method. If the
  //  filter is part of a larger image processing pipeline, calling
  //  \code{Update()} on a downstream filter will also trigger update of this
  //  filter.

  filter->Update();

  writer->SetFileName(argv[2]);
  writer->Update();

  // We can also obtain the direction of the edges by invoking the
  // \code{GetOutputDirection()} method.

  writer->SetFileName(argv[3]);
  rescaler->SetInput(filter->GetOutputDirection());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  // Figure~\ref{fig:TOUZI_FILTER} shows the result of applying the Touzi
  // edge detector
  // filter to a SAR image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{amst.eps}
  // \includegraphics[width=0.25\textwidth]{amstTouziEdges.eps}
  // \includegraphics[width=0.25\textwidth]{amstTouziDirections.eps}
  // \itkcaption[Touzi Edge Detector Application]{Result of applying the
  // \doxygen{otb}{TouziEdgeDetectorImageFilter} to a SAR image. From left to right :
  // original image, edge intensity and edge orientation.}
  // \label{fig:TOUZI_FILTER}
  // \end{figure}

  return EXIT_SUCCESS;
}
