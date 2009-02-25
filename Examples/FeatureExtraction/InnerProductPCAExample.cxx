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
#include "otbMultiToMonoChannelExtractROI.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png}
//    OUTPUTS: {InnerProductPCAOutput.tif}, {PrettyInnerProductPCAOutput1.png}, {PrettyInnerProductPCAOutput2.png}, {PrettyInnerProductPCAOutput3.png}
//    3
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{InnerProductPCAImageFilter}. 
// This filter computes a Principal Component Analysis using an
// efficient method based on the inner product in order to compute the
// covariance matrix.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbInnerProductPCAImageFilter.h"
// Software Guide : EndCodeSnippet

int main( int argc, char* argv[] )
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  const char * inputFileName = argv[1];
  const char * outputFilename = argv[2];
  const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[6]));


  // Software Guide : BeginLatex
  //
  // We start by defining the types for the images and the reader and
  // the writer. We choose to work with a \doxygen{otb}{VectorImage},
  // since we will produce a multi-channel image (the principal
  // components) from a multi-channel input image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader< ImageType >     ReaderType;
  typedef otb::ImageFileWriter< ImageType >     WriterType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We instantiate now the image reader and we set the image file name.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer     reader     = ReaderType::New();
  reader->SetFileName(inputFileName);
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We define the type for the filter. It is templated over the input
  // and the output image types. We the instantiate the filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::InnerProductPCAImageFilter<ImageType,ImageType> PCAFilterType;
  PCAFilterType::Pointer     pcafilter     = PCAFilterType::New();

  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // The only parameter needed for the PCA is the number of principal
  // components required as output. We can choose to get less PCs than
  // the number of input bands.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  pcafilter->SetNumberOfPrincipalComponentsRequired(
                                    numberOfPrincipalComponentsRequired);

  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We now instantiate the writer and set the file name for the
  // output image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  WriterType::Pointer     writer     = WriterType::New();
  writer->SetFileName(outputFilename);

  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We finally plug the pipeline and trigger the PCA computation with
  // the method \code{Update()} of the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  pcafilter->SetInput(reader->GetOutput());
  writer->SetInput(pcafilter->GetOutput());
  
  writer->Update();

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:INNERPRODUCTPCA_FILTER} shows the result of applying
  // the PCA to a 3 band RGB image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{ROI_QB_MUL_1.eps}
  // \includegraphics[width=0.25\textwidth]{PrettyInnerProductPCAOutput1.eps}
  // \includegraphics[width=0.25\textwidth]{PrettyInnerProductPCAOutput2.eps}
  // \includegraphics[width=0.25\textwidth]{PrettyInnerProductPCAOutput3.eps}
  // \itkcaption[Inner Product PCA Filter]{Result of applying the
  // \doxygen{otb}{InnerProductPCAImageFilter} to an image. From left
  // to right and top to bottom: 
  // original image, first PC, second PC, third PC.}
  // \label{fig:INNERPRODUCTPCA_FILTER}
  // \end{figure}
  //
  //  Software Guide : EndLatex



  typedef otb::Image<PixelType,Dimension> MonoImageType;
  
  typedef otb::MultiToMonoChannelExtractROI< PixelType, PixelType >
                                                    ExtractROIFilterType;

  typedef otb::Image<unsigned char, 2> OutputImageType;
  typedef otb::ImageFileWriter< OutputImageType >  WriterType2;
  typedef itk::RescaleIntensityImageFilter< MonoImageType, OutputImageType> RescalerType;
  
  for(unsigned int cpt=0 ; cpt < numberOfPrincipalComponentsRequired ; cpt++)
  {
    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    RescalerType::Pointer rescaler = RescalerType::New();
    
    WriterType2::Pointer     writer2     = WriterType2::New();
    extractROIFilter->SetInput(pcafilter->GetOutput());
    extractROIFilter->SetChannel(cpt+1);

    rescaler->SetInput( extractROIFilter->GetOutput());
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);
    
    writer2->SetInput(rescaler->GetOutput());
    writer2->SetFileName(argv[cpt+3]);
    writer2->Update();
  }


  return EXIT_SUCCESS;
}
