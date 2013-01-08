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
//    INPUTS: {IMAGERY_SSECH.tif}
//    OUTPUTS: {PrintableExampleOutput1.jpg}
//    1 2 3
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {IMAGERY_SSECH.tif}
//    OUTPUTS: {PrintableExampleOutput2.jpg}
//    1 4 2
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  Most of the time, satellite images have more than three spectral bands. As we
// are only able to see three colors (red, green and blue), we have to find a way to
// represent these images using only three bands. This is called creating a color
// composition.
//
// Of course, any color composition will not be able to render all the information
// available in the original image. As a consequence, sometimes, creating more than
// one color composition will be necessary.
//
// If you want to obtain an image with natural colors, you have to match the wavelength
// captured by the satellite with those captured by your eye: thus matching the red band
// with the red color, etc.
//
// Some satellites (SPOT 5 is an example) do not acquire all the {\em human} spectral bands:
// the blue can be missing and replaced by some other wavelength of interest for a specific application.
// In these situations, another mapping has to be created. That's why, the vegetation often appears in
// red in satellite images (see on left of figure~\ref{fig:PRINTABLE_FILTER}).
//
// The band order in the image products can be also quite tricky. It could be in the wavelength order,
// as it is the case for Quickbird (1: Blue, 2: Green, 3: Red, 4: NIR), in this case, you
// have to be carefull to reverse the order if you want a natural display. It could also be reverse
// to facilitate direct viewing, as for SPOT5 (1: NIR, 2: Red, 3: Green, 4: SWIR) but in this situations
// you have to be careful when you process the image.
//
//  Software Guide : EndLatex

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"

int main(int argc, char * argv[])
{

  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0] << " <inputImageFile> ";
    std::cerr << " <outputImageFile> <RedBand> <GreenBand> <BlueBand>" <<
    std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  int          redChannelNumber = atoi(argv[3]);
  int          greenChannelNumber = atoi(argv[4]);
  int          blueChannelNumber = atoi(argv[5]);

  typedef double InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  //  Software Guide : BeginLatex
  //
  //  To easily convert the image to a {\em printable} format, i.e. 3 bands
  // \code{unsigned char} value, you can use the \doxygen{otb}{PrintableImageFilter}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::PrintableImageFilter<InputImageType> PrintableFilterType;
  PrintableFilterType::Pointer printableImageFilter = PrintableFilterType::New();

  printableImageFilter->SetInput(reader->GetOutput());
  printableImageFilter->SetChannel(redChannelNumber);
  printableImageFilter->SetChannel(greenChannelNumber);
  printableImageFilter->SetChannel(blueChannelNumber);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  When you create the writer to plug at the output of the \code{printableImageFilter}
  // you may want to use the direct type definition as it is a good way to avoid mismatch:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef PrintableFilterType::OutputImageType           OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(printableImageFilter->GetOutput());

  writer->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:PRINTABLE_FILTER} illustrates different color compositions for a SPOT 5 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{PrintableExampleOutput1.eps}
  // \includegraphics[width=0.44\textwidth]{PrintableExampleOutput2.eps}
  // \itkcaption[Scaling images]{On the left, a classic SPOT5
  // combination: XS3 in red, XS2 in green and XS1 in blue. On the
  // right another composition: XS3 in red, XS4 in green and XS2 in blue.}
  // \label{fig:PRINTABLE_FILTER}
  // \end{figure}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
