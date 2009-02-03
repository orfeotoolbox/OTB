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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {CloudsOnReunion.tif}
//    OUTPUTS: {CloudDetectionOutput.png}
//    553 467 734 581 0.25 0.8 1.0
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
// The easiest way to use the road extraction filter provided by OTB is to use the composite
// filter. If a modification in the pipeline is required to adapt to a particular situation,
// the step by step example, described in the next section can be adapted.
//
// This example demonstrates the use of the \doxygen{otb}{RoadExtractionFilter}.
// This filter is a composite filter achieving road extraction according to the algorithm
// adapted by E. Christophe and J. Inglada \cite{Christophe2007} from an original method
// proposed in \cite{Lacroix1998}.
//
// The first step toward the use of this filter is the inclusion of the proper header files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

#include "otbCloudDetectionFunctor.h"
#include "otbCloudDetectionFilter.h"

// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkExceptionObject.h"

int main( int argc, char * argv[] )
{

  if (argc != 10)
  {
    std::cerr << "Usage: "<< argv[0];
    std::cerr << "inputFileName outputFileName ";
    std::cerr << "firstPixelComponent secondPixelComponent thirdPixelComponent fourthPixelComponent ";
    std::cerr << "variance ";
    std::cerr << "minThreshold maxThreshold "<<std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int Dimension = 2;
  // Software Guide : BeginLatex
  //
  // Then we must decide what pixel type to use for the image. We choose to do
  // all the computation in floating point precision and rescale the results
  // between 0 and 255 in order to export PNG images.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef double         InputPixelType;
  typedef unsigned short OutputPixelType;

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The images are defined using the pixel type and the dimension. Please note that
  //  the \doxygen{otb}{RoadExtractionFilter} needs an \doxygen{otb}{VectorImage} as input
  //  to handle multispectral images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<InputPixelType,Dimension>                             VectorImageType;
  typedef VectorImageType::PixelType                                             VectorPixelType;
  typedef otb::Image<OutputPixelType,Dimension>                                  OutputImageType;

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We define the type of the polyline that the filter produces. We use the
  // \doxygen{otb}{PolyLineParametricPathWithValue}, which allows the filter to produce
  // a likehood value along with each polyline. The filter is able to produce
  // \doxygen{itk}{PolyLineParametricPath} as well.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet

  typedef otb::Functor::CloudDetectionFunctor<VectorPixelType,InputPixelType >   FunctorType;

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now we can define the \doxygen{otb}{RoadExtractionFilter} that takes a multi-spectral
  // image as input and produces a list of polylines.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

    typedef otb::CloudDetectionFilter<VectorImageType,OutputImageType,FunctorType > CloudDetectionFilterType;

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We also define an \doxygen{otb}{DrawPathListFilter} to draw the output
  // polylines on an image, taking their likehood values into account.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The different filters composing our pipeline are created by invoking their
  // \code{New()} methods, assigning the results to smart pointers.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  ReaderType::Pointer reader = ReaderType::New();
  CloudDetectionFilterType::Pointer cloudDetection = CloudDetectionFilterType::New();
  WriterType::Pointer writer = WriterType::New();

  // Software Guide : EndCodeSnippet

  reader->SetFileName(argv[1]);
  cloudDetection->SetInput(reader->GetOutput());

  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{RoadExtractionFilter} needs to have a reference pixel
  // corresponding to the spectral content likely to represent a road. This is done
  // by passing a pixel to the filter. Here we suppose that the input image
  // has four spectral bands.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  VectorPixelType referencePixel;
  referencePixel.SetSize(4);
  referencePixel.Fill(0.);
  referencePixel[0] = (atof(argv[3]));
  referencePixel[1] = (atof(argv[4]));
  referencePixel[2] = (atof(argv[5]));
  referencePixel[3] = (atof(argv[6]));
  cloudDetection->SetReferencePixel(referencePixel);

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We must also set the  alpha parameter of the filter which allows us to tune the width of the roads
  // we want to extract. Typical value is $1.0$ and should be working in most situations.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  cloudDetection->SetVariance(atof(argv[7]));

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // All other parameter should not influence the results too much in most situation and can
  // be kept at the default value.
  //
  // The amplitude threshold parameter tunes the sensitivity of the vectorization step. A typical
  // value is $5 \cdot 10^{-5}$.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  cloudDetection->SetMinThreshold(atof(argv[8]));
  cloudDetection->SetMaxThreshold(atof(argv[9]));

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The tolerance threshold tunes the sensitivity of the path simplification step.
  // Typical value is $1.0$.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  writer->SetFileName(argv[2]);
  writer->SetInput(cloudDetection->GetOutput());
  writer->Update();

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:ROADEXTRACTION_FILTER} shows the result of applying
  // the road extraction filter to a fusionned Quickbird image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{qb_ExtractRoad_pretty.eps}
  // \includegraphics[width=0.44\textwidth]{ExtractRoadOutput.eps}
  // \itkcaption[Road extraction filter application]{Result of applying
  // the \doxygen{otb}{RoadExtractionFilter} to a fusionned Quickbird
  // image. From left to right : original image, extracted road with their
  // likehood values (color are inverted for display).}
  // \label{fig:ROADEXTRACTION_FILTER}
  // \end{figure}
  //
  // Software Guide : EndLatex


  return EXIT_SUCCESS;
}

