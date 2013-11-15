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
#include "otbVectorData.h"

#include "otbImageFileReader.h"
#include "otbLineSegmentDetector.h"
#include "otbVectorDataToMapFilter.h"
#include "otbAlphaBlendingFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "otbImageFileWriter.h"

#include "itkRescaleIntensityImageFilter.h"

#include "otbVectorDataFileWriter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {Scene.png}
//    OUTPUTS: {RighAngleOutput.tif}, {PrettyRighAngleInput.png},  {PrettyRighAngleOutput.png}
//  0.1 20
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{VectorDataToRightAngleVectorDataFilter}.
// This filter detects the right angles in an image by exploiting the
// output of a line detection algorithm. Typically the
// \doxygen{otb}{LineSegmentDetector} class will be used. The right
// angle detection algorithm is described in detail in
// \cite{RightAngleDetection}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbVectorDataToRightAngleVectorDataFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  const char * infname            = argv[1];
  const char * outfname           = argv[2];
  const char * inprettyfname      = argv[3];
  const char * outprettyfname     = argv[4];
  double       angleThreshold     = atof(argv[5]);
  double       distanceThreshold  = atof(argv[6]);

  const unsigned int    Dimension = 2;
  typedef unsigned char PixelType;
  typedef double        PrecisionType;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType>  WriterType;

  ReaderType::Pointer reader            = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();
  WriterType::Pointer writer = WriterType::New();

  // Software Guide : BeginLatex
  //
  // After defining, as usual, the types for the input image and the
  // image reader, we define the specific types needed for this
  // example. First of all, we will use a vector data
  // to store the detected lines which will be provided by the line
  // segment detector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // The right angle detector's output is a vector data where each point
  // gives the coordinate of the detected angle.
  //
  // Next, We define the type for the line segment detector. A detailed
  // example for this detector can be found in section \ref{sec:LSD}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LineSegmentDetector<ImageType, PrecisionType> LsdFilterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can finally define the type for the right angle detection
  // filter. This filter is templated over the input vector data type
  // provided by the line segment detector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorDataToRightAngleVectorDataFilter<VectorDataType>
    RightAngleFilterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We instantiate the line segment detector and the right angle detector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  LsdFilterType::Pointer        lsdFilter         = LsdFilterType::New();
  RightAngleFilterType::Pointer rightAngleFilter  = RightAngleFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We plug the pipeline.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  lsdFilter->SetInput(reader->GetOutput());
  rightAngleFilter->SetInput(lsdFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // You can choose how far the right angle segments can be, and the tolerance
  // to consider an angle between two segments as an right one.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  rightAngleFilter->SetAngleThreshold(angleThreshold);
  rightAngleFilter->SetDistanceThreshold(distanceThreshold);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We will now draw the right angles on top of the input image. For
  // this, we will draw the detected points on top of the input
  // image. For this matter, we will use a
  // \doxygen{otb}{VectorDataToMapFilter}  which is templated over
  // the  input vector data type and the output image type, and a
  // conbination of a \doxygen{itk}{binaryFunctorImageFilter}
  // and the \doxygen{otb}{Functor}{UnaryFunctorImageFilter}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorDataToMapFilter<VectorDataType,
      ImageType> VectorDataRendererType;
  VectorDataRendererType::Pointer vectorDataRenderer = VectorDataRendererType::New();

  typedef otb::Functor::AlphaBlendingFunctor<PixelType,
    PixelType, PixelType> FunctorType;
  typedef itk::BinaryFunctorImageFilter<ImageType, ImageType,
    ImageType, FunctorType> BlendingFilterType;
  BlendingFilterType::Pointer blendingFilter = BlendingFilterType::New();

  vectorDataRenderer->SetInput(1, lsdFilter->GetOutput());
  vectorDataRenderer->SetInput(rightAngleFilter->GetOutput());

  vectorDataRenderer->SetSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());
  vectorDataRenderer->SetOrigin(reader->GetOutput()->GetOrigin());
  vectorDataRenderer->SetSpacing(reader->GetOutput()->GetSpacing());
  vectorDataRenderer->SetRenderingStyleType(VectorDataRendererType::Binary);

  blendingFilter->SetInput1(reader->GetOutput());
  blendingFilter->SetInput2(vectorDataRenderer->GetOutput());
  blendingFilter->GetFunctor().SetAlpha(0.25);

  writer->SetInput(blendingFilter->GetOutput());
  writer->SetFileName(outfname);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Before calling the \code{Update()} method of the writer in order to
  // trigger the pipeline execution, we call the
  // \doxygen{GenerateOutputInformation()} of the reader, so the
  // filter gets the information about image size and spacing.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->GenerateOutputInformation();
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:RIGHTANGLE_FILTER} shows the result of applying
  // the right angle detection filter to an image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{PrettyRighAngleInput.eps}
  // \includegraphics[width=0.25\textwidth]{PrettyRighAngleOutput.eps}
  // \itkcaption[Right Angle Detection Filter]{Result of applying the
  // \doxygen{otb}{LineSegmentDetector} and the
  // \doxygen{otb}{VectorDataToRightAngleVectorDataFilter} to an image. From left to right :
  // original image, detected right angles.}
  // \label{fig:RIGHTANGLE_FILTER}
  // \end{figure}
  //
  //  Software Guide : EndLatex

  /************** images for printing *********/
  typedef unsigned char OutputPixelType;

  typedef otb::Image<OutputPixelType,
      Dimension>                             OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>
  OutputWriterType;
  typedef itk::RescaleIntensityImageFilter<ImageType,
      OutputImageType> RescalerType;

  RescalerType::Pointer     rescaler = RescalerType::New();
  OutputWriterType::Pointer outwriter = OutputWriterType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  rescaler->SetInput(reader->GetOutput());
  outwriter->SetInput(rescaler->GetOutput());
  outwriter->SetFileName(inprettyfname);
  outwriter->Update();

  rescaler->SetInput(blendingFilter->GetOutput());
  outwriter->SetInput(rescaler->GetOutput());
  outwriter->SetFileName(outprettyfname);
  outwriter->Update();

  return EXIT_SUCCESS;
}
