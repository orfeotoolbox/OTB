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
//    OUTPUTS: {GomaSmallLeeFiltered.png}
//    3 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{LeeImageFilter}.
// This filter belongs to the family of the edge-preserving smoothing
// filters which are usually used for speckle reduction in radar
// images. The Lee filter \cite{LeeFilter} aplies a linear regression
// which minimizes the mean-square error in the frame of a
// multiplicative speckle model.
//
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLeeImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{

  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile radius NbLooks" << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  Then we must decide what pixel type to use for the image.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  unsigned char PixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The images are defined using the pixel type and the dimension.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType,  2> InputImageType;
  typedef otb::Image<PixelType,  2> OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The filter can be instantiated using the image types defined above.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LeeImageFilter<InputImageType, OutputImageType> FilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
  //  image data from a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType> ReaderType;
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
  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to SmartPointers.
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
  //  \doxygen{otb}{LeeImageFilter}.
  //
  //  \index{otb::LeeImageFilter!SetInput()}
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
  //  \code{SetNbLooks()} sets the number of looks of the input
  //  image.
  //
  //  \index{otb::LeeImageFilter!SetRadius()}
  //  \index{otb::LeeImageFilter!SetNbLooks()}
  //  \index{SetNbLooks()!otb::LeeImageFilter}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  FilterType::SizeType Radius;
  Radius[0] = atoi(argv[3]);
  Radius[1] = atoi(argv[3]);

  filter->SetRadius(Radius);
  filter->SetNbLooks(atoi(argv[4]));
  // Software Guide : EndCodeSnippet

  writer->SetFileName(argv[2]);
  writer->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:LEE_FILTER} shows the result of applying the Lee
  // filter to a SAR image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{GomaSmall.eps}
  // \includegraphics[width=0.44\textwidth]{GomaSmallLeeFiltered.eps}
  // \itkcaption[Lee Filter Application]{Result of applying the
  // \doxygen{otb}{LeeImageFilter} to a SAR image.}
  // \label{fig:LEE_FILTER}
  // \end{figure}
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{otb}{FrostImageFilter}
  //  \end{itemize}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
