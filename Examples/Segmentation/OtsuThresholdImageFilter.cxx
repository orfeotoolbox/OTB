/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {OtsuThresholdImageFilterOutput.png}
//    255 0
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates how to use the \doxygen{itk}{OtsuThresholdImageFilter}.
//
// Software Guide : EndLatex

#include "itkUnaryFunctorImageFilter.h"
// Software Guide : BeginCodeSnippet
#include "itkOtsuThresholdImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{
  if (argc < 5)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputImageFile ";
    std::cerr << " insideValue    outsideValue   "  << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  The next step is to decide which pixel types to use for the input and output
  //  images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  unsigned char InputPixelType;
  typedef  unsigned char OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The input and output image types are now defined using their respective
  //  pixel types and dimensions.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<InputPixelType,  2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The filter type can be instantiated using the input and output image
  //  types defined above.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::OtsuThresholdImageFilter<
      InputImageType, OutputImageType>  FilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
  //  image data from a file. (See Section \ref{sec:IO} on page
  //  \pageref{sec:IO} for more information about reading
  //  and writing data.)
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // An \doxygen{otb}{ImageFileWriter} is instantiated in order to write the output
  // image to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<InputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to \doxygen{itk}{SmartPointer}s.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);

  //  Software Guide : BeginLatex
  //
  //  The image obtained with the reader is passed as input to the
  //  OtsuThresholdImageFilter.
  //
  //  \index{itk::Otsu\-Threshold\-Image\-Filter!SetInput()}
  //  \index{otb::FileImageReader!GetOutput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The method \code{SetOutsideValue()} defines the intensity value to be
  //  assigned to those pixels whose intensities are outside the range defined
  //  by the lower and upper thresholds. The method \code{SetInsideValue()}
  //  defines the intensity value to be assigned to pixels with intensities
  //  falling inside the threshold range.
  //
  //  \index{itk::Otsu\-Threshold\-Image\-Filter!SetOutsideValue()}
  //  \index{itk::Otsu\-Threshold\-Image\-Filter!SetInsideValue()}
  //  \index{SetOutsideValue()!itk::Otsu\-Threshold\-Image\-Filter}
  //  \index{SetInsideValue()!itk::Otsu\-Threshold\-Image\-Filter}
  //
  //  Software Guide : EndLatex

  const OutputPixelType outsideValue = atoi(argv[3]);
  const OutputPixelType insideValue  = atoi(argv[4]);

  // Software Guide : BeginCodeSnippet
  filter->SetOutsideValue(outsideValue);
  filter->SetInsideValue(insideValue);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The method \code{SetNumberOfHistogramBins()} defines the number of bins
  //  to be used for computing the histogram. This histogram will be used
  //  internally in order to compute the Otsu threshold.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetNumberOfHistogramBins(128);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The execution of the filter is triggered by invoking the \code{Update()}
  //  method.   If the filter's output has been passed as input to subsequent
  //  filters, the \code{Update()} call on any posterior filters in the
  //  pipeline will indirectly trigger the update of this filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We print out here the Threshold value that was computed internally by the
  //  filter. For this we invoke the \code{GetThreshold} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  int threshold = filter->GetThreshold();
  std::cout << "Threshold = " << threshold << std::endl;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{OtsuThresholdImageFilterOutput.eps}
  // \itkcaption[OtsuThresholdImageFilter output]{Effect of the OtsuThresholdImageFilter.}
  // \label{fig:OtsuThresholdImageFilterInputOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:OtsuThresholdImageFilterInputOutput} illustrates the
  //  effect of this filter. This
  //  figure shows the limitations of this filter for performing segmentation
  //  by itself. These limitations are particularly noticeable in noisy images
  //  and in images lacking spatial uniformity.
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{itk}{ThresholdImageFilter}
  //  \end{itemize}
  //
  //  Software Guide : EndLatex

  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
