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
//    INPUTS: {GomaSmall.png}
//    OUTPUTS: {GomaSmallFrostFiltered.png}
//    5 0.1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{FrostImageFilter}.
// This filter belongs to the family of the edge-preserving smoothing
// filters which are usually used for speckle reduction in radar
// images.
//
// This filter uses a negative exponential convolution kernel.
// The output of the filter for pixel p is:
//      $ \hat I_{s}=\sum_{p\in\eta_{p}} m_{p}I_{p} $
//
// where :   $ m_{p}=\frac{KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})}{\sum_{p\in\eta_{p}} KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})} $
//    and  $ d_{s, p}=\sqrt{(i-i_{p})^2+(j-j_{p})^2} $
//
// \begin{itemize}
// \item $ K $     : the decrease coefficient
// \item $ (i, j)$ : the coordinates of the pixel inside the region
// defined by $ \eta_{s} $
// \item $ (i_{p}, j_{p})$ : the coordinates of the pixels belonging to $ \eta_{p} \subset \eta_{s} $
// \item $ C_{s}$ : the variation coefficient computed over $ \eta_{p}$
// \end{itemize}
//
//
//
// Most of this example is similar to the previous one and only the differences
// will be highlighted.
//
// First, we need to include the header:
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbFrostImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{

  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile radius deramp" << std::endl;
    return EXIT_FAILURE;
    }

  typedef  unsigned char PixelType;

  typedef otb::Image<PixelType,  2> InputImageType;
  typedef otb::Image<PixelType,  2> OutputImageType;

  //  Software Guide : BeginLatex
  //
  //  The filter can be instantiated using the image types defined previously.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::FrostImageFilter<InputImageType, OutputImageType> FilterType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);

  //  Software Guide : BeginLatex
  //
  //  The image obtained with the reader is passed as input to the
  //  \doxygen{otb}{FrostImageFilter}.
  //
  //  \index{otb::FrostImageFilter!SetInput()}
  //  \index{otb::FileImageReader!GetOutput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The method \code{SetRadius()} defines the size of the window to
  //  be used for the computation of the local statistics. The method
  //  \code{SetDeramp()} sets the $K$ coefficient.
  //
  //  \index{otb::FrostImageFilter!SetRadius()}
  //  \index{otb::FrostImageFilter!SetDeramp()}
  //  \index{SetDeramp()!otb::FrostImageFilter}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  FilterType::SizeType Radius;
  Radius[0] = atoi(argv[3]);
  Radius[1] = atoi(argv[3]);

  filter->SetRadius(Radius);
  filter->SetDeramp(atof(argv[4]));
  // Software Guide : EndCodeSnippet

  writer->SetFileName(argv[2]);
  writer->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:FROST_FILTER} shows the result of applying the Frost
  // filter to a SAR image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{GomaSmall.eps}
  // \includegraphics[width=0.44\textwidth]{GomaSmallFrostFiltered.eps}
  // \itkcaption[Frost Filter Application]{Result of applying the
  // \doxygen{otb}{FrostImageFilter} to a SAR image.}
  // \label{fig:FROST_FILTER}
  // \end{figure}
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{otb}{LeeImageFilter}
  //  \end{itemize}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
