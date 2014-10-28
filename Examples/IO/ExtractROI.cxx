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
//    INPUTS: {IMAGERY_SSECH.tif}
//    OUTPUTS: {ROI_IMAGERY_RGB.png}, {ROI_IMAGERY_MIR.png}
//    0 0 100 100
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example shows the use of the
//  \doxygen{otb}{MultiChannelExtractROI} and
//  \doxygen{otb}{MultiToMonoChannelExtractROI} which allow the
//  extraction of ROIs from multiband images stored into
//  \doxygen{otb}{VectorImage}s. The first one povides a Vector Image
//  as output, while the second one provides a classical
//  \doxygen{otb}{Image} with a scalar pixel type. The present example
//  shows how to extract a ROI from a 4-band SPOT 5 image and to
//  produce a first multi-band 3-channel image and a second
//  mono-channel one for the SWIR band.

//  We start by including the needed header files.
//  \index{otb::ExtractROI!header}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"
// Software Guide : EndCodeSnippet


int main(int argc, char * argv[])
{

  // R\'ecuperation de arguments
  if (argc < 7)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] <<
    "  inputImageFile outputImageFileRGB outputImageFileLastBand startX startY sizeX sizeY"
              << std::endl;
    return EXIT_FAILURE;
    }

//  Software Guide : BeginLatex
//
//  The program arguments define the image file names as well as the
//  rectangular area to be extracted.
//
//  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const char * inputFilename  = argv[1];
  const char * outputFilenameRGB = argv[2];
  const char * outputFilenameMIR = argv[3];

  unsigned int startX((unsigned int) ::atoi(argv[4]));
  unsigned int startY((unsigned int) ::atoi(argv[5]));
  unsigned int sizeX((unsigned int) ::atoi(argv[6]));
  unsigned int sizeY((unsigned int) ::atoi(argv[7]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  As usual, we define the input and output pixel types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef   unsigned char InputPixelType;
  typedef   unsigned char OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  First of all, we extract the multiband part by using the
  //  \doxygen{otb}{MultiChannelExtractROI} class, which is templated
  //  over the input and output pixel types. This class in not
  //  templated over the images types in order to force these images
  //  to be of \doxygen{otb}{VectorImage} type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MultiChannelExtractROI<InputPixelType,
      OutputPixelType>  ExtractROIFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We create the extractor filter by using the \code{New} method of
  //  the class and we set its parameters.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetStartX(startX);
  extractROIFilter->SetStartY(startY);
  extractROIFilter->SetSizeX(sizeX);
  extractROIFilter->SetSizeY(sizeY);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We must tell the filter which are the channels to be used. When
  //  selecting contiguous bands, we can use the
  //  \code{SetFirstChannel} and the \code{SetLastChannel}. Otherwise,
  //  we select individual channels by using the \code{SetChannel} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  extractROIFilter->SetFirstChannel(1);
  extractROIFilter->SetLastChannel(3);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We will use the OTB readers and writers for file access.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ExtractROIFilterType::InputImageType> ReaderType;
  typedef otb::ImageFileWriter<ExtractROIFilterType::InputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Since the number of bands of the input image is dynamically set
  //  at runtime, the \code{UpdateOutputInformation} method of the reader must be
  //  called before using the extractor filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();
  writer->SetFileName(outputFilenameRGB);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We can then build the pipeline as usual.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  extractROIFilter->SetInput(reader->GetOutput());

  writer->SetInput(extractROIFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  And execute the pipeline by calling the \code{Update} method of
  //  the writer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The usage of the \doxygen{otb}{MultiToMonoChannelExtractROI} is
  //  similar to the one of the \doxygen{otb}{MultiChannelExtractROI}
  //  described above.
  //
  //  The goal now is to extract an ROI from a multi-band image and
  //  generate a mono-channel image as output.
  //
  //  We could use the \doxygen{otb}{MultiChannelExtractROI} and
  //  select a single channel, but using the
  //  \doxygen{otb}{MultiToMonoChannelExtractROI} we generate a
  //  \doxygen{otb}{Image} instead of an
  //  \doxygen{otb}{VectorImage}. This is useful from a computing and
  //  memory usage point of view.
  //  This class is also templated over the pixel types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MultiToMonoChannelExtractROI<InputPixelType,
      OutputPixelType>
  ExtractROIMonoFilterType;
  // Software Guide : EndCodeSnippet

  ExtractROIMonoFilterType::Pointer extractROIMonoFilter =
    ExtractROIMonoFilterType::New();

  extractROIMonoFilter->SetStartX(startX);
  extractROIMonoFilter->SetStartY(startY);
  extractROIMonoFilter->SetSizeX(sizeX);
  extractROIMonoFilter->SetSizeY(sizeY);

  //  Software Guide : BeginLatex
  //
  // For this filter, only one output channel has to be selected.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  extractROIMonoFilter->SetChannel(4);
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<ExtractROIMonoFilterType::InputImageType>
  monoReaderType;
  typedef otb::ImageFileWriter<ExtractROIMonoFilterType::OutputImageType>
  monoWriterType;
  monoReaderType::Pointer monoReader = monoReaderType::New();
  monoWriterType::Pointer monoWriter = monoWriterType::New();

  monoReader->SetFileName(inputFilename);
  monoReader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
  monoWriter->SetFileName(outputFilenameMIR);
  extractROIMonoFilter->SetInput(monoReader->GetOutput());

  monoWriter->SetInput(extractROIMonoFilter->GetOutput());
  monoWriter->Update();

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{IMAGERY_SSECH.eps}
  // \itkcaption[Initial SPOT 5 image]{Quicklook of the original SPOT
  // 5 image.}
  // \label{fig:IMAGERY_SSECH}
  // \end{figure}
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{ROI_IMAGERY_RGB.eps}
  // \includegraphics[width=0.44\textwidth]{ROI_IMAGERY_MIR.eps}
  // \itkcaption[ROI of a SPOT5 image]{Result of the
  // extraction. Left: 3-channel image. Right: mono-band image.}
  // \label{fig:ROI_IMAGERY}
  // \end{figure}
  //
  //  Figure \ref{fig:ROI_IMAGERY} illustrates the result of the
  //  application of both extraction filters on the image presented in
  //  figure \ref{fig:IMAGERY_SSECH}.
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;

}
