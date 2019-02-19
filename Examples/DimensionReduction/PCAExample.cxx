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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"

/* Example usage:
./PCAExample Input/wv2_cannes_8bands.tif \
             Output/PCAOutput.tif \
             Output/InversePCAOutput.tif \
             Output/input-pretty.png \
             Output/output-pretty.png \
             Output/invoutput-pretty.png \
             8
*/


// This example illustrates the use of the
// \doxygen{otb}{PCAImageFilter}.
// This filter computes a Principal Component Analysis using an
// efficient method based on the inner product in order to compute the
// covariance matrix.
//
// The first step required to use this filter is to include its header file.

#include "otbPCAImageFilter.h"

int main(int itkNotUsed(argc), char* argv[])
{
  typedef double     PixelType;
  const unsigned int Dimension             = 2;
  const char*        inputFileName         = argv[1];
  const char*        outputFilename        = argv[2];
  const char*        outputInverseFilename = argv[3];
  const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[7]));
  const char*        inpretty     = argv[4];
  const char*        outpretty    = argv[5];
  const char*        invoutpretty = argv[6];


  // We start by defining the types for the images and the reader and
  // the writer. We choose to work with a \doxygen{otb}{VectorImage},
  // since we will produce a multi-channel image (the principal
  // components) from a multi-channel input image.

  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileWriter<ImageType>        WriterType;
  // We instantiate now the image reader and we set the image file name.

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  // We define the type for the filter. It is templated over the input
  // and the output image types and also the transformation direction. The
  // internal structure of this filter is a filter-to-filter like structure.
  // We can now the instantiate the filter.

  typedef otb::PCAImageFilter<ImageType, ImageType, otb::Transform::FORWARD> PCAFilterType;
  PCAFilterType::Pointer                                                     pcafilter = PCAFilterType::New();
  // The only parameter needed for the PCA is the number of principal
  // components required as output. Principal components are linear combination of input components
  // (here the input image  bands),
  // which are selected using Singular Value Decomposition eigen vectors sorted by eigen value.
  // We can choose to get less Principal Components than
  // the number of input bands.

  pcafilter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsRequired);
  // We now instantiate the writer and set the file name for the
  // output image.

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  // We finally plug the pipeline and trigger the PCA computation with
  // the method \code{Update()} of the writer.

  pcafilter->SetInput(reader->GetOutput());
  writer->SetInput(pcafilter->GetOutput());

  writer->Update();

  // \doxygen{otb}{PCAImageFilter} allows also to compute inverse
  // transformation from PCA coefficients. In reverse mode, the
  // covariance matrix or the transformation matrix
  // (which may not be square) has to be given.

  typedef otb::PCAImageFilter<ImageType, ImageType, otb::Transform::INVERSE> InvPCAFilterType;
  InvPCAFilterType::Pointer                                                  invFilter = InvPCAFilterType::New();

  invFilter->SetInput(pcafilter->GetOutput());
  invFilter->SetTransformationMatrix(pcafilter->GetTransformationMatrix());

  WriterType::Pointer invWriter = WriterType::New();
  invWriter->SetFileName(outputInverseFilename);
  invWriter->SetInput(invFilter->GetOutput());

  invWriter->Update();

  // Figure~\ref{fig:PCA_FILTER} shows the result of applying forward
  // and reverse PCA transformation to a 8 bands Worldview2 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{input-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{output-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{invoutput-pretty.eps}
  // \itkcaption[PCA Filter (forward trasnformation)]{Result of applying the
  // \doxygen{otb}{PCAImageFilter} to an image. From left
  // to right:
  // original image, color composition with first three principal
  // components and output of the
  // inverse mode (the input RGB image).}
  // \label{fig:PCA_FILTER}
  // \end{figure}

  // This is for rendering in software guide
  typedef otb::PrintableImageFilter<ImageType, ImageType> PrintFilterType;
  typedef PrintFilterType::OutputImageType                VisuImageType;
  typedef otb::ImageFileWriter<VisuImageType>             VisuWriterType;

  PrintFilterType::Pointer inputPrintFilter        = PrintFilterType::New();
  PrintFilterType::Pointer outputPrintFilter       = PrintFilterType::New();
  PrintFilterType::Pointer invertOutputPrintFilter = PrintFilterType::New();
  VisuWriterType::Pointer  inputVisuWriter         = VisuWriterType::New();
  VisuWriterType::Pointer  outputVisuWriter        = VisuWriterType::New();
  VisuWriterType::Pointer  invertOutputVisuWriter  = VisuWriterType::New();

  inputPrintFilter->SetInput(reader->GetOutput());
  inputPrintFilter->SetChannel(5);
  inputPrintFilter->SetChannel(3);
  inputPrintFilter->SetChannel(2);
  outputPrintFilter->SetInput(pcafilter->GetOutput());
  outputPrintFilter->SetChannel(1);
  outputPrintFilter->SetChannel(2);
  outputPrintFilter->SetChannel(3);
  invertOutputPrintFilter->SetInput(invFilter->GetOutput());
  invertOutputPrintFilter->SetChannel(5);
  invertOutputPrintFilter->SetChannel(3);
  invertOutputPrintFilter->SetChannel(2);

  inputVisuWriter->SetInput(inputPrintFilter->GetOutput());
  outputVisuWriter->SetInput(outputPrintFilter->GetOutput());
  invertOutputVisuWriter->SetInput(invertOutputPrintFilter->GetOutput());

  inputVisuWriter->SetFileName(inpretty);
  outputVisuWriter->SetFileName(outpretty);
  invertOutputVisuWriter->SetFileName(invoutpretty);

  inputVisuWriter->Update();
  outputVisuWriter->Update();
  invertOutputVisuWriter->Update();

  return EXIT_SUCCESS;
}
