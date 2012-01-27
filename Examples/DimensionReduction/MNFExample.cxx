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
//    OUTPUTS: {MNFOutput.tif}, {InverseMNFOutput.tif}, {MNF-input-pretty.png}, {MNF-output-pretty.png}, {MNF-invoutput-pretty.png}
//    8 1 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{MNFImageFilter}.  This filter computes a Minimum
// Noise Fraction transform \cite{nielsen2011kernel} using an
// efficient method based on the inner product in order to compute the
// covariance matrix.
//
// The Minimum Noise Fraction transform is a sequence of two Principal
// Components Analysis transform. The first transform is based on an
// estimated covariance matrix of the noise, and intends to whiten the
// input image (noise with unit variance and no correlation between
// bands). 
//
// The second Principal Components Analysis is then applied to the
// noise-whitened image, giving the Minimum Noise Fraction transform.
//
// In this implementation, noise is estimated from a local window.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMNFImageFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We also need to include the header of the noise filter.
//
// SoftwareGuide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLocalActivityVectorImageFilter.h"
// Software Guide : EndCodeSnippet


int main(int argc, char* argv[])
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
  unsigned int vradius = atoi(argv[7]);
  bool normalization = atoi(argv[8]);

  // Software Guide : BeginLatex
  //
  // We start by defining the types for the images and the reader and
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
  // We define the type of the noise filter.
  //
  // Software Guide : EndLatex

  // SoftwareGuide : BeginCodeSnippet  
  typedef otb::LocalActivityVectorImageFilter<ImageType,ImageType> NoiseFilterType;
  // SoftwareGuide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // We define the type for the filter. It is templated over the input
  // and the output image types and also the transformation direction. The
  // internal structure of this filter is a filter-to-filter like structure.
  // We can now the instantiate the filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MNFImageFilter<ImageType, ImageType,
                                NoiseFilterType,
                                otb::Transform::FORWARD> MNFFilterType;
  MNFFilterType::Pointer MNFfilter     = MNFFilterType::New();
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // We then set the number of principal
  // components required as output. We can choose to get less PCs than
  // the number of input bands.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MNFfilter->SetNumberOfPrincipalComponentsRequired(
    numberOfPrincipalComponentsRequired);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We set the radius of the sliding window for noise estimation.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  NoiseFilterType::RadiusType radius = {{ vradius, vradius }};
  MNFfilter->GetNoiseImageFilter()->SetRadius(radius);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Last, we can activate normalisation.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MNFfilter->SetUseNormalization( normalization );
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
  // We finally plug the pipeline and trigger the MNF computation with
  // the method \code{Update()} of the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MNFfilter->SetInput(reader->GetOutput());
  writer->SetInput(MNFfilter->GetOutput());

  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // \doxygen{otb}{MNFImageFilter} allows also to compute inverse
  // transformation from MNF coefficients. In reverse mode, the
  // covariance matrix or the transformation matrix
  // (which may not be square) has to be given.
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::MNFImageFilter< ImageType, ImageType,
                                 NoiseFilterType,
                                 otb::Transform::INVERSE > InvMNFFilterType;
  InvMNFFilterType::Pointer invFilter = InvMNFFilterType::New();
  
  invFilter->SetMeanValues( MNFfilter->GetMeanValues() );
  if ( normalization )
    invFilter->SetStdDevValues( MNFfilter->GetStdDevValues() );
  invFilter->SetTransformationMatrix( MNFfilter->GetTransformationMatrix() );
  invFilter->SetInput(MNFfilter->GetOutput());
    
  WriterType::Pointer invWriter = WriterType::New();
  invWriter->SetFileName(outputInverseFilename );
  invWriter->SetInput(invFilter->GetOutput() );

  invWriter->Update();
  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  // Figure~\ref{fig:MNF_FILTER} shows the result of applying forward
  // and reverse MNF transformation to a 8 bands Wordlview2 image.
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
