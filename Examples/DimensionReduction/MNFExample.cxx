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
./MNFExample Input/wv2_cannes_8bands.tif \
             Output/MNFOutput.tif \
             Output/InverseMNFOutput.tif \
             Output/MNF-input-pretty.png \
             Output/MNF-output-pretty.png \
             Output/MNF-invoutput-pretty.png \
             8 \
             1 \
             1
*/


// This example illustrates the use of the
// \doxygen{otb}{MNFImageFilter}.  This filter computes a Maximum
// Noise Fraction transform \cite{green1988transformation} using an
// efficient method based on the inner product in order to compute the
// covariance matrix.
//
// The Maximum Noise Fraction transform is a sequence of two Principal
// Component Analysis transforms. The first transform is based on an
// estimated covariance matrix of the noise, and intends to whiten the
// input image (noise with unit variance and no correlation between
// bands).
//
// The second Principal Component Analysis is then applied to the
// noise-whitened image, giving the Maximum Noise Fraction transform.
//
// In this implementation, noise is estimated from a local window.
//
// The first step required to use this filter is to include its header file.

#include "otbMNFImageFilter.h"

// We also need to include the header of the noise filter.
//
// SoftwareGuide : EndLatex

#include "otbLocalActivityVectorImageFilter.h"


int main(int itkNotUsed(argc), char* argv[])
{
  typedef double     PixelType;
  const unsigned int Dimension             = 2;
  const char*        inputFileName         = argv[1];
  const char*        outputFilename        = argv[2];
  const char*        outputInverseFilename = argv[3];
  const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[7]));
  const char*        inpretty      = argv[4];
  const char*        outpretty     = argv[5];
  const char*        invoutpretty  = argv[6];
  unsigned int       vradius       = atoi(argv[8]);
  bool               normalization = atoi(argv[9]);

  // We start by defining the types for the images, the reader, and
  // the writer. We choose to work with a \doxygen{otb}{VectorImage},
  // since we will produce a multi-channel image (the principal
  // components) from a multi-channel input image.

  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileWriter<ImageType>        WriterType;
  // We instantiate now the image reader and we set the image file name.

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  // In contrast with standard Principal Component Analysis, MNF
  // needs an estimation of the noise correlation matrix
  // in the dataset prior to transformation.
  //
  // A classical approach is to use spatial gradient images
  // and infer the noise correlation matrix from it.
  // The method of noise estimation can be customized
  // by templating the \doxygen{otb}{MNFImageFilter}
  // with the desired noise estimation method.
  //
  // In this implementation, noise is estimated from a local window.
  // We define the type of the noise filter.

  // SoftwareGuide : BeginCodeSnippet
  typedef otb::LocalActivityVectorImageFilter<ImageType, ImageType> NoiseFilterType;
  // SoftwareGuide : EndCodeSnippet


  // We define the type for the filter. It is templated over the input
  // and the output image types and also the transformation direction. The
  // internal structure of this filter is a filter-to-filter like structure.
  // We can now the instantiate the filter.

  typedef otb::MNFImageFilter<ImageType, ImageType, NoiseFilterType, otb::Transform::FORWARD> MNFFilterType;
  MNFFilterType::Pointer                                                                      MNFfilter = MNFFilterType::New();

  // We then set the number of principal
  // components required as output. We can choose to get less PCs than
  // the number of input bands.

  MNFfilter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsRequired);

  // We set the radius of the sliding window for noise estimation.

  NoiseFilterType::RadiusType radius = {{vradius, vradius}};
  MNFfilter->GetNoiseImageFilter()->SetRadius(radius);

  // Last, we can activate normalisation.

  MNFfilter->SetUseNormalization(normalization);

  // We now instantiate the writer and set the file name for the
  // output image.

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  // We finally plug the pipeline and trigger the MNF computation with
  // the method \code{Update()} of the writer.

  MNFfilter->SetInput(reader->GetOutput());
  writer->SetInput(MNFfilter->GetOutput());

  writer->Update();

  // \doxygen{otb}{MNFImageFilter} allows also to compute inverse
  // transformation from MNF coefficients. In reverse mode, the
  // covariance matrix or the transformation matrix
  // (which may not be square) has to be given.

  typedef otb::MNFImageFilter<ImageType, ImageType, NoiseFilterType, otb::Transform::INVERSE> InvMNFFilterType;
  InvMNFFilterType::Pointer                                                                   invFilter = InvMNFFilterType::New();

  invFilter->SetMeanValues(MNFfilter->GetMeanValues());
  if (normalization)
    invFilter->SetStdDevValues(MNFfilter->GetStdDevValues());
  invFilter->SetTransformationMatrix(MNFfilter->GetTransformationMatrix());
  invFilter->SetInput(MNFfilter->GetOutput());

  WriterType::Pointer invWriter = WriterType::New();
  invWriter->SetFileName(outputInverseFilename);
  invWriter->SetInput(invFilter->GetOutput());

  invWriter->Update();

  // Figure~\ref{fig:MNF_FILTER} shows the result of applying forward
  // and reverse MNF transformation to a 8 bands Worldview2 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{MNF-input-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{MNF-output-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{MNF-invoutput-pretty.eps}
  // \itkcaption[PCA Filter (forward trasnformation)]{Result of applying the
  // \doxygen{otb}{MNFImageFilter} to an image. From left
  // to right:
  // original image, color composition with first three principal
  // components and output of the
  // inverse mode (the input RGB image).}
  // \label{fig:MNF_FILTER}
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
  outputPrintFilter->SetInput(MNFfilter->GetOutput());
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
