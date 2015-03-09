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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {suburb2.jpeg}
//    OUTPUTS: {EdgeDensityOutput.tif}, {PrettyEdgeDensityOutput.png}
//    7 50 10 1.0 0.01
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{EdgeDensityImageFilter}.
// This filter computes a local density of edges on an image and can
// be useful to detect man made objects or urban areas, for
// instance. The filter has been implemented in a generic way, so that
// the way the edges are detected and the way their density is
// computed can be chosen by the user.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbEdgeDensityImageFilter.h"
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We will also include the header files for the edge detector (a
// Canny filter) and the density estimation (a simple count on a
// binary image).
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkCannyEdgeDetectionImageFilter.h"
#include "otbBinaryImageDensityFunction.h"
// Software Guide : EndCodeSnippet

int main(int itkNotUsed(argc), char* argv[])
{

  const char * infname       = argv[1];
  const char * outfname      = argv[2];
  const char * prettyfilename      = argv[3];

  const unsigned int radius       = atoi(argv[4]);

  /*--*/

  const unsigned int Dimension = 2;
  typedef float PixelType;

  /** Variables for the canny detector*/
  const PixelType upperThreshold   = static_cast<PixelType>(atof(argv[5]));
  const PixelType lowerThreshold   = static_cast<PixelType>(atof(argv[6]));
  const double    variance         = atof(argv[7]);
  const double    maximumError     = atof(argv[8]);

  // Software Guide : BeginLatex
  //
  // As usual, we start by defining the types for the images, the reader
  // and the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType>  WriterType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We define now the type for the function which will be used by the
  // edge density filter to estimate this density. Here we choose a
  // function which counts the number of non null pixels per area. The
  // fucntion takes as template the type of the image to be processed.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::BinaryImageDensityFunction<ImageType> CountFunctionType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // These {\em non null pixels} will be the result of an edge
  // detector. We use here the classical Canny edge detector, which is
  // templated over the input and output image types.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>
  CannyDetectorType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // Finally, we can define the type for the edge density filter which
  // takes as template the input and output image types, the edge
  // detector type, and the count fucntion type..
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::EdgeDensityImageFilter<ImageType, ImageType, CannyDetectorType,
      CountFunctionType> EdgeDensityFilterType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We can now instantiate the different processing objects of the
  // pipeline using the \code{New()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer            reader = ReaderType::New();
  EdgeDensityFilterType::Pointer filter = EdgeDensityFilterType::New();
  CannyDetectorType::Pointer     cannyFilter = CannyDetectorType::New();
  WriterType::Pointer            writer = WriterType::New();
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // The edge detection filter needs to be instantiated because we
  // need to set its parameters. This is what we do here for the Canny
  // filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  cannyFilter->SetUpperThreshold(upperThreshold);
  cannyFilter->SetLowerThreshold(lowerThreshold);
  cannyFilter->SetVariance(variance);
  cannyFilter->SetMaximumError(maximumError);
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // After that, we can pass the edge detector to the filter which
  // will be used it internally.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetDetector(cannyFilter);
  filter->SetNeighborhoodRadius(radius);
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // Finally, we set the file names for the input and the output
  // images and we plug the pipeline. The \code{Update()} method of
  // the writer will trigger the processing.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName(infname);
  writer->SetFileName(outfname);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
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
  //
  //  Software Guide : EndLatex

  /************* Image for printing **************/

  typedef otb::Image<unsigned char, 2> OutputImageType;

  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType>
  RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  rescaler->SetInput(filter->GetOutput());

  typedef otb::ImageFileWriter<OutputImageType> OutputWriterType;
  OutputWriterType::Pointer outwriter = OutputWriterType::New();

  outwriter->SetFileName(prettyfilename);
  outwriter->SetInput(rescaler->GetOutput());
  outwriter->Update();

  return EXIT_SUCCESS;
}
