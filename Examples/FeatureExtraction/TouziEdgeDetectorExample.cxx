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
//    INPUTS: {amst.png}
//    OUTPUTS: {amstTouziEdges.png}, {amstTouziDirections.png}
//    3
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{TouziEdgeDetectorImageFilter}.
// This filter belongs to the family of the fixed false alarm rate
// edge detectors but it is apropriate for SAR images, where the
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
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbTouziEdgeDetectorImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{

  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputEdgesImageFile outputDirectionsImageFile radius" <<
    std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with floating point precision
  //  and rescale the results between 0 and 255 in order to export PNG images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  float         InternalPixelType;
  typedef  unsigned char OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The images are defined using the pixel type and the dimension.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<InternalPixelType,  2> InternalImageType;
  typedef otb::Image<OutputPixelType,  2>   OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The filter can be instantiated using the image types defined above.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::TouziEdgeDetectorImageFilter<InternalImageType,
      InternalImageType> FilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{ImageFileReader} class is also instantiated in order to read
  //  image data from a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InternalImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // An \doxygen{ImageFileWriter} is instantiated in order to write the
  // output image to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The intensity rescaling of the results will be carried out by the
  // \code{itk::RescaleIntensityImageFilter} which is templated by the
  // input and output image types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RescaleIntensityImageFilter<InternalImageType,
      OutputImageType> RescalerType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to SmartPointers.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The same is done for the rescaler and the writer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  RescalerType::Pointer rescaler = RescalerType::New();
  WriterType::Pointer   writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  reader->SetFileName(argv[1]);

  //  Software Guide : BeginLatex
  //
  //  The \code{itk::RescaleIntensityImageFilter} needs to know which
  //  is the minimu and maximum values of the output generated
  //  image. Those can be chosen in a generic way by using the
  //  \code{NumericTraits} functions, since they are templated over
  //  the pixel type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The image obtained with the reader is passed as input to the
  //  \doxygen{otb}{TouziEdgeDetectorImageFilter}. The pipeline is built as follows.
  //
  //  \index{otb::TouziEdgeDetectorImageFilter!SetInput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The method \code{SetRadius()} defines the size of the window to
  //  be used for the computation of the local means.
  //
  //  \index{otb::LeeImageFilter!SetRadius()}
  //  \index{otb::LeeImageFilter!NbLooks()}
  //  \index{SetNbLooks()!otb::LeeImageFilter}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  FilterType::SizeType Radius;
  Radius[0] = atoi(argv[4]);
  Radius[1] = atoi(argv[4]);

  filter->SetRadius(Radius);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The filter is executed by invoking the \code{Update()} method. If the
  //  filter is part of a larger image processing pipeline, calling
  //  \code{Update()} on a downstream filter will also trigger update of this
  //  filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->Update();
  // Software Guide : EndCodeSnippet

  writer->SetFileName(argv[2]);
  writer->Update();

  //  Software Guide : BeginLatex
  // We can also obtain the direction of the edges by invoking the
  // \code{GetOutputDirection()} method.
  //  Software Guide : EndLatex

  writer->SetFileName(argv[3]);
  // Software Guide : BeginCodeSnippet
  rescaler->SetInput(filter->GetOutputDirection());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
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
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
