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
./ICAExample Input/wv2_cannes_8bands.tif \
             Output/FastICAOutput.tif \
             Output/InverseFastICAOutput.tif \
             Output/FastICA-input-pretty.png \
             Output/FastICA-output-pretty.png \
             Output/FastICA-invoutput-pretty.png \
             8 \
             20 \
             1.
*/


// This example illustrates the use of the
// \doxygen{otb}{FastICAImageFilter}.
// This filter computes a Fast Independent Components Analysis transform.
//
// Like Principal Components Analysis, Independent Component Analysis
// \cite{jutten1991blind} computes a set of
// orthogonal linear combinations, but the criterion of Fast ICA is
// different: instead of maximizing variance, it tries to maximize
// statistical independence between components.
//
// In the Fast ICA algorithm \cite{hyvarinen1999fast},
// statistical independence is measured by evaluating non-Gaussianity
// of the components, and the maximization is done in an iterative way.

// The first step required to use this filter is to include its header file.

#include "otbFastICAImageFilter.h"


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
  unsigned int       numIterations = atoi(argv[8]);
  double             mu            = atof(argv[9]);

  // We start by defining the types for the images, the reader, and
  // the writer. We choose to work with a \doxygen{otb}{VectorImage},
  // since we will produce a multi-channel image (the independent
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

  typedef otb::FastICAImageFilter<ImageType, ImageType, otb::Transform::FORWARD> FastICAFilterType;
  FastICAFilterType::Pointer                                                     FastICAfilter = FastICAFilterType::New();

  // We then set the number of independent
  // components required as output. We can choose to get less ICs than
  // the number of input bands.

  FastICAfilter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsRequired);

  // We set the number of iterations of the ICA algorithm.

  FastICAfilter->SetNumberOfIterations(numIterations);

  // We also set the $\mu$ parameter.

  FastICAfilter->SetMu(mu);

  // We now instantiate the writer and set the file name for the
  // output image.

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  // We finally plug the pipeline and trigger the ICA computation with
  // the method \code{Update()} of the writer.

  FastICAfilter->SetInput(reader->GetOutput());
  writer->SetInput(FastICAfilter->GetOutput());

  writer->Update();

  // \doxygen{otb}{FastICAImageFilter} allows also to compute inverse
  // transformation from ICA coefficients. In reverse mode, the
  // covariance matrix or the transformation matrix
  // (which may not be square) has to be given.

  typedef otb::FastICAImageFilter<ImageType, ImageType, otb::Transform::INVERSE> InvFastICAFilterType;
  InvFastICAFilterType::Pointer                                                  invFilter = InvFastICAFilterType::New();

  invFilter->SetMeanValues(FastICAfilter->GetMeanValues());
  invFilter->SetStdDevValues(FastICAfilter->GetStdDevValues());
  invFilter->SetTransformationMatrix(FastICAfilter->GetTransformationMatrix());
  invFilter->SetPCATransformationMatrix(FastICAfilter->GetPCATransformationMatrix());
  invFilter->SetInput(FastICAfilter->GetOutput());

  WriterType::Pointer invWriter = WriterType::New();
  invWriter->SetFileName(outputInverseFilename);
  invWriter->SetInput(invFilter->GetOutput());

  invWriter->Update();

  // Figure~\ref{fig:FastICA_FILTER} shows the result of applying forward
  // and reverse FastICA transformation to a 8 bands Worldview2 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{FastICA-input-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{FastICA-output-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{FastICA-invoutput-pretty.eps}
  // \itkcaption[PCA Filter (forward trasnformation)]{Result of applying the
  // \doxygen{otb}{FastICAImageFilter} to an image. From left
  // to right:
  // original image, color composition with first three independent
  // components and output of the
  // inverse mode (the input RGB image).}
  // \label{fig:FastICA_FILTER}
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
  outputPrintFilter->SetInput(FastICAfilter->GetOutput());
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
