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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {wv2_cannes_8bands.tif}
//    OUTPUTS: {NAPCAOutput.tif}, {InverseNAPCAOutput.tif}, {napca-input-pretty.png}, {napca-output-pretty.png}, {napca-invoutput-pretty.png}
//    8 1 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{NAPCAImageFilter}.  This filter computes a Noise-Adjusted
// Principal Component Analysis transform \cite{lee1990enhancement} using an
// efficient method based on the inner product in order to compute the
// covariance matrix.
//
// The Noise-Adjusted Principal Component Analysis transform is a sequence
// of two Principal Component Analysis transforms. The first transform is based
// on an estimated covariance matrix of the noise, and intends to whiten the
// input image (noise with unit variance and no correlation between
// bands).
//
// The second Principal Component Analysis is then applied to the
// noise-whitened image, giving the Maximum Noise Fraction transform.
// Applying PCA on noise-whitened image consists in ranking Principal Components according to signal to noise ratio.
//
// It is basically a reformulation of the Maximum Noise Fraction algorithm.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbNAPCAImageFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We also need to include the header of the noise filter.
//
// SoftwareGuide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLocalActivityVectorImageFilter.h"
// Software Guide : EndCodeSnippet


int main(int itkNotUsed(argc), char* argv[])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  const char *       inputFileName = argv[1];
  const char *       outputFilename = argv[2];
  const char *       outputInverseFilename = argv[3];
  const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[7]));
  const char *       inpretty = argv[4];
  const char *       outpretty = argv[5];
  const char *       invoutpretty = argv[6];
  unsigned int vradius = atoi(argv[8]);
  bool normalization = atoi(argv[9]);

  // Software Guide : BeginLatex
  //
  // We start by defining the types for the images, the reader and
  // the writer. We choose to work with a \doxygen{otb}{VectorImage},
  // since we will produce a multi-channel image (the principal
  // components) from a multi-channel input image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileWriter<ImageType>        WriterType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We instantiate now the image reader and we set the image file name.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader     = ReaderType::New();
  reader->SetFileName(inputFileName);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // In contrast with standard Principal Component Analysis, NA-PCA
  // needs an estimation of the noise correlation matrix
  // in the dataset prior to transformation.
  //
  // A classical approach is to use spatial gradient images
  // and infer the noise correlation matrix from it.
  // The method of noise estimation can be customized
  // by templating the \doxygen{otb}{NAPCAImageFilter}
  // with the desired noise estimation method.
  //
  // In this implementation, noise is estimated from a local window.
  // We define the type of the noise filter.
  //
  // Software Guide : EndLatex

  // SoftwareGuide : BeginCodeSnippet
  typedef otb::LocalActivityVectorImageFilter<ImageType,ImageType> NoiseFilterType;
  // SoftwareGuide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // We define the type for the filter. It is templated over the input
  // and the output image types, the noise estimation filter type,
  // and also the transformation direction. The
  // internal structure of this filter is a filter-to-filter like structure.
  // We can now the instantiate the filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::NAPCAImageFilter<ImageType, ImageType,
                                NoiseFilterType,
                                otb::Transform::FORWARD> NAPCAFilterType;
  NAPCAFilterType::Pointer napcafilter     = NAPCAFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We then set the number of principal
  // components required as output. We can choose to get less PCs than
  // the number of input bands.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  napcafilter->SetNumberOfPrincipalComponentsRequired(
    numberOfPrincipalComponentsRequired);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We set the radius of the sliding window for noise estimation.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  NoiseFilterType::RadiusType radius = {{ vradius, vradius }};
  napcafilter->GetNoiseImageFilter()->SetRadius(radius);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Last, we can activate normalisation.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  napcafilter->SetUseNormalization( normalization );
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We now instantiate the writer and set the file name for the
  // output image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  WriterType::Pointer writer     = WriterType::New();
  writer->SetFileName(outputFilename);
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We finally plug the pipeline and trigger the NA-PCA computation with
  // the method \code{Update()} of the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  napcafilter->SetInput(reader->GetOutput());
  writer->SetInput(napcafilter->GetOutput());

  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // \doxygen{otb}{NAPCAImageFilter} allows also to compute inverse
  // transformation from NA-PCA coefficients. In reverse mode, the
  // covariance matrix or the transformation matrix
  // (which may not be square) has to be given.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::NAPCAImageFilter< ImageType, ImageType,
                                 NoiseFilterType,
                                 otb::Transform::INVERSE > InvNAPCAFilterType;
  InvNAPCAFilterType::Pointer invFilter = InvNAPCAFilterType::New();

  invFilter->SetMeanValues( napcafilter->GetMeanValues() );
  if ( normalization )
    invFilter->SetStdDevValues( napcafilter->GetStdDevValues() );
  invFilter->SetTransformationMatrix( napcafilter->GetTransformationMatrix() );
  invFilter->SetInput(napcafilter->GetOutput());

  WriterType::Pointer invWriter = WriterType::New();
  invWriter->SetFileName(outputInverseFilename );
  invWriter->SetInput(invFilter->GetOutput() );

  invWriter->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:NAPCA_FILTER} shows the result of applying forward
  // and reverse NA-PCA transformation to a 8 bands Worldview2 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{napca-input-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{napca-output-pretty.eps}
  // \includegraphics[width=0.32\textwidth]{napca-invoutput-pretty.eps}
  // \itkcaption[PCA Filter (forward trasnformation)]{Result of applying the
  // \doxygen{otb}{NAPCAImageFilter} to an image. From left
  // to right:
  // original image, color composition with first three principal
  // components and output of the
  // inverse mode (the input RGB image).}
  // \label{fig:NAPCA_FILTER}
  // \end{figure}
  //
  //  Software Guide : EndLatex

  // This is for rendering in software guide
  typedef otb::PrintableImageFilter<ImageType,ImageType> PrintFilterType;
  typedef PrintFilterType::OutputImageType               VisuImageType;
  typedef otb::ImageFileWriter<VisuImageType>            VisuWriterType;

  PrintFilterType::Pointer inputPrintFilter = PrintFilterType::New();
  PrintFilterType::Pointer outputPrintFilter = PrintFilterType::New();
  PrintFilterType::Pointer invertOutputPrintFilter = PrintFilterType::New();
  VisuWriterType::Pointer inputVisuWriter = VisuWriterType::New();
  VisuWriterType::Pointer outputVisuWriter = VisuWriterType::New();
  VisuWriterType::Pointer invertOutputVisuWriter = VisuWriterType::New();

  inputPrintFilter->SetInput(reader->GetOutput());
  inputPrintFilter->SetChannel(5);
  inputPrintFilter->SetChannel(3);
  inputPrintFilter->SetChannel(2);
  outputPrintFilter->SetInput(napcafilter->GetOutput());
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
