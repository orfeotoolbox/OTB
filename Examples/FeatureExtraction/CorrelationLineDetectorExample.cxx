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
//    OUTPUTS: {amstLineCorrelations.png}, {amstLineCorrelationDirections.png}
//    5 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{CorrelationLineDetectorImageFilter}.
// This filter is used for line detection in SAR images. Its principle
// is described in \cite{tup-98}: a line is detected if two parallel
// edges are present in the images. These edges are detected with the
// correlation of means detector.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLineCorrelationDetectorImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{

  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr <<
    " outputEdgesImageFile outputDirectionsImageFile length width" << std::endl;
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
  typedef otb::LineCorrelationDetectorImageFilter<InternalImageType,
      InternalImageType>
  FilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
  //  image data from a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InternalImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // An \doxygen{otb}{ImageFileWriter} is instantiated in order to write the
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
  //  \doxygen{otb}{LineCorrelationDetectorImageFilter}. The pipeline is built as follows.
  //
  //  \index{otb::LineCorrelationDetectorImageFilter!SetInput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The methods \code{SetLengthLine()} and \code{SetWidthLine()}
  //  allow setting the minimum length and the typical witdh of the
  //  lines which are to be detected.
  //
  //  \index{otb::LineCorrelationDetector!SetWidthLine()}
  //  \index{otb::LineCorrelationDetector!SetLengthLine()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetLengthLine(atoi(argv[4]));
  filter->SetWidthLine(atoi(argv[5]));
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
  // We can also obtain the direction of the lines by invoking the
  // \code{GetOutputDirections()} method.
  //  Software Guide : EndLatex

  writer->SetFileName(argv[3]);
  // Software Guide : BeginCodeSnippet
  rescaler->SetInput(filter->GetOutputDirection());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex Figure~\ref{fig:LINECORRELATION_FILTER}
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
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
