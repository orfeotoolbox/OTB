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
//    INPUTS: {qb_RoadExtract.tif}
//    OUTPUTS: {ExtractRoadOutput.png}
//    337 557 432 859 1.0 0.00005 1.0 0.39269 1.0 10.0 25.
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
#include "otbPolyLineParametricPathWithValue.h"
#include "otbRoadExtractionFilter.h"
#include "otbDrawPathListFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbMath.h"

#include "itkInvertIntensityImageFilter.h"
#include "itkGrayscaleDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

int main(int argc, char * argv[])
{

  if (argc != 14)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " inputFileName outputFileName firstPixelComponent secondPixelComponent ";
    std::cerr <<
    "thirdPixelComponent fourthPixelComponent alpha amplitudeThrehsold tolerance ";
    std::cerr <<
    "angularThreshold-maxAngle firstMeanDistanceThreshold secondMeanDistanceThreshold ";
    std::cerr << "distanceThreshold" << std::endl;
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
  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The images are defined using the pixel type and the dimension. Please note that
  //  the \doxygen{otb}{RoadExtractionFilter} needs an \doxygen{otb}{VectorImage} as input
  //  to handle multispectral images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
  typedef otb::Image<InputPixelType, Dimension>       InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>      OutputImageType;
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
  typedef otb::PolyLineParametricPathWithValue<InputPixelType,
      Dimension> PathType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now we can define the \doxygen{otb}{RoadExtractionFilter} that takes a multi-spectral
  // image as input and produces a list of polylines.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::RoadExtractionFilter<InputVectorImageType,
      PathType> RoadExtractionFilterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We also define an \doxygen{otb}{DrawPathListFilter} to draw the output
  // polylines on an image, taking their likehood values into account.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::DrawPathListFilter<InputImageType, PathType,
      InputImageType> DrawPathFilterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The intensity rescaling of the results will be carried out by the
  // \doxygen{itk}{RescaleIntensityImageFilter} which is templated by the
  // input and output image types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RescaleIntensityImageFilter<InputImageType,
      OutputImageType> RescalerType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
  //  image data from a file. Then, an \doxygen{otb}{ImageFileWriter}
  //  is instantiated in order to write the output image to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputVectorImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>      WriterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The different filters composing our pipeline are created by invoking their
  // \code{New()} methods, assigning the results to smart pointers.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer               reader = ReaderType::New();
  RoadExtractionFilterType::Pointer roadExtractionFilter
    = RoadExtractionFilterType::New();
  DrawPathFilterType::Pointer drawingFilter = DrawPathFilterType::New();
  RescalerType::Pointer       rescaleFilter = RescalerType::New();
  WriterType::Pointer         writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  reader->SetFileName(argv[1]);

  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{RoadExtractionFilter} needs to have a reference pixel
  // corresponding to the spectral content likely to represent a road. This is done
  // by passing a pixel to the filter. Here we suppose that the input image
  // has four spectral bands.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  InputVectorImageType::PixelType ReferencePixel;
  ReferencePixel.SetSize(4);
  ReferencePixel.SetElement(0, ::atof(argv[3]));
  ReferencePixel.SetElement(1, ::atof(argv[4]));
  ReferencePixel.SetElement(2, ::atof(argv[5]));
  ReferencePixel.SetElement(3, ::atof(argv[6]));
  roadExtractionFilter->SetReferencePixel(ReferencePixel);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We must also set the  alpha parameter of the filter which allows us to tune the width of the roads
  // we want to extract. Typical value is $1.0$ and should be working in most situations.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  roadExtractionFilter->SetAlpha(atof(argv[7]));
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
  roadExtractionFilter->SetAmplitudeThreshold(atof(argv[8]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The tolerance threshold tunes the sensitivity of the path simplification step.
  // Typical value is $1.0$.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  roadExtractionFilter->SetTolerance(atof(argv[9]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Roads are not likely to have sharp turns. Therefore we set the max angle parameter,
  // as well as the link angular threshold. The value is typicaly $\frac{\pi}{8}$.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  roadExtractionFilter->SetMaxAngle(atof(argv[10]));
  roadExtractionFilter->SetAngularThreshold(atof(argv[10]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{RoadExtractionFilter} performs two odd path removing operations at different stage of
  // its execution. The first mean distance threshold and the second mean distance threshold set their criterion
  // for removal. Path are removed if their mean distance between nodes is to small, since such path coming
  // from previous filters are likely to be tortuous. The first removal operation as a typical mean distance
  // threshold parameter of $1.0$, and the second of $10.0$.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  roadExtractionFilter->SetFirstMeanDistanceThreshold(atof(argv[11]));
  roadExtractionFilter->SetSecondMeanDistanceThreshold(atof(argv[12]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{RoadExtractionFilter} is able to link path whose ends are near
  // according to an euclidean distance criterion. The threshold for this distance
  // to link a path is the distance threshold parameter. A typical value is $25$.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  roadExtractionFilter->SetDistanceThreshold(atof(argv[13]));
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We will now create a black background image to draw the resulting polyline on.
  // To achieve this we need to know the size of our input image. Therefore we trigger the
  // \code{GenerateOutputInformation()} of the reader.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->GenerateOutputInformation();
  InputImageType::Pointer blackBackground = InputImageType::New();
  blackBackground->CopyInformation(reader->GetOutput());
  blackBackground->SetRegions(blackBackground->GetLargestPossibleRegion());
  blackBackground->Allocate();
  blackBackground->FillBuffer(0);
  // Software Guide : EndCodeSnippet

  // Software Guide  : BeginLatex
  //
  // We tell the \doxygen{otb}{DrawPathListFilter} to try to use the likehood value
  // embedded within the polyline as a value for drawing this polyline if possible.
  //
  // Software Guide : EndLatex

  // Software Guide  : BeginCodeSnippet

  drawingFilter->UseInternalPathValueOn();

  // Software Guide  : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The \code{itk::RescaleIntensityImageFilter} needs to know which
  //  is the minimum and maximum values of the output generated
  //  image. Those can be chosen in a generic way by using the
  //  \code{NumericTraits} functions, since they are templated over
  //  the pixel type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  rescaleFilter->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaleFilter->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now it is time for some pipeline wiring.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  roadExtractionFilter->SetInput(reader->GetOutput());
  drawingFilter->SetInput(blackBackground);
  drawingFilter->SetInputPath(roadExtractionFilter->GetOutput());
  rescaleFilter->SetInput(drawingFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The update of the pipeline is triggered by the \code{Update()} method
  // of the rescale intensity filter.
  //
  // Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  rescaleFilter->Update();
  // Software Guide : EndCodeSnippet

  // output image enhancement
  typedef itk::BinaryBallStructuringElement<OutputPixelType,
      Dimension>
  StructuringElementType;
  typedef itk::GrayscaleDilateImageFilter<OutputImageType, OutputImageType,
      StructuringElementType>
  DilateFilterType;
  typedef itk::InvertIntensityImageFilter<OutputImageType,
      OutputImageType>
  InvertFilterType;

  StructuringElementType se;
  se.SetRadius(1);
  se.CreateStructuringElement();

  DilateFilterType::Pointer dilater = DilateFilterType::New();

  dilater->SetInput(rescaleFilter->GetOutput());
  dilater->SetKernel(se);

  InvertFilterType::Pointer invertFilter = InvertFilterType::New();
  invertFilter->SetInput(dilater->GetOutput());

  writer->SetFileName(argv[2]);
  writer->SetInput(invertFilter->GetOutput());
  writer->Update();

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
