/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
./ExtractROI Input/IMAGERY_SSECH.tif Output/ROI_IMAGERY_RGB.png Output/ROI_IMAGERY_MIR.png 0 0 100 100
*/


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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"


int main(int argc, char* argv[])
{

  // R\'ecuperation de arguments
  if (argc < 7)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile outputImageFileRGB outputImageFileLastBand startX startY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
  }

  //  The program arguments define the image file names as well as the
  //  rectangular area to be extracted.

  const char* inputFilename     = argv[1];
  const char* outputFilenameRGB = argv[2];
  const char* outputFilenameMIR = argv[3];

  unsigned int startX((unsigned int)::atoi(argv[4]));
  unsigned int startY((unsigned int)::atoi(argv[5]));
  unsigned int sizeX((unsigned int)::atoi(argv[6]));
  unsigned int sizeY((unsigned int)::atoi(argv[7]));

  //  As usual, we define the input and output pixel types.

  using InputPixelType  = unsigned char;
  using OutputPixelType = unsigned char;

  //  First of all, we extract the multiband part by using the
  //  \doxygen{otb}{MultiChannelExtractROI} class, which is templated
  //  over the input and output pixel types. This class in not
  //  templated over the images types in order to force these images
  //  to be of \doxygen{otb}{VectorImage} type.

  using ExtractROIFilterType = otb::MultiChannelExtractROI<InputPixelType, OutputPixelType>;

  //  We create the extractor filter by using the \code{New} method of
  //  the class and we set its parameters.

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetStartX(startX);
  extractROIFilter->SetStartY(startY);
  extractROIFilter->SetSizeX(sizeX);
  extractROIFilter->SetSizeY(sizeY);

  //  We must tell the filter which are the channels to be used. When
  //  selecting contiguous bands, we can use the
  //  \code{SetFirstChannel} and the \code{SetLastChannel}. Otherwise,
  //  we select individual channels by using the \code{SetChannel} method.

  extractROIFilter->SetFirstChannel(1);
  extractROIFilter->SetLastChannel(3);

  //  We will use the OTB readers and writers for file access.

  using ReaderType = otb::ImageFileReader<ExtractROIFilterType::InputImageType>;
  using WriterType = otb::ImageFileWriter<ExtractROIFilterType::InputImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //  Since the number of bands of the input image is dynamically set
  //  at runtime, the \code{UpdateOutputInformation} method of the reader must be
  //  called before using the extractor filter.

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();
  writer->SetFileName(outputFilenameRGB);

  //  We can then build the pipeline as usual.

  extractROIFilter->SetInput(reader->GetOutput());

  writer->SetInput(extractROIFilter->GetOutput());

  //  And execute the pipeline by calling the \code{Update} method of
  //  the writer.

  writer->Update();

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

  using ExtractROIMonoFilterType = otb::MultiToMonoChannelExtractROI<InputPixelType, OutputPixelType>;

  ExtractROIMonoFilterType::Pointer extractROIMonoFilter = ExtractROIMonoFilterType::New();

  extractROIMonoFilter->SetStartX(startX);
  extractROIMonoFilter->SetStartY(startY);
  extractROIMonoFilter->SetSizeX(sizeX);
  extractROIMonoFilter->SetSizeY(sizeY);

  // For this filter, only one output channel has to be selected.

  extractROIMonoFilter->SetChannel(4);

  using monoReaderType               = otb::ImageFileReader<ExtractROIMonoFilterType::InputImageType>;
  using monoWriterType               = otb::ImageFileWriter<ExtractROIMonoFilterType::OutputImageType>;
  monoReaderType::Pointer monoReader = monoReaderType::New();
  monoWriterType::Pointer monoWriter = monoWriterType::New();

  monoReader->SetFileName(inputFilename);
  monoReader->Update(); // Needed to know the number of channels in the image
  monoWriter->SetFileName(outputFilenameMIR);
  extractROIMonoFilter->SetInput(monoReader->GetOutput());

  monoWriter->SetInput(extractROIMonoFilter->GetOutput());
  monoWriter->Update();

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

  return EXIT_SUCCESS;
}
