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
//  INPUTS: {amst.png}
//  OUTPUTS: {amstSegmentExtractionBySteps.png}
//  10 3 10 0.5 10 10 3 10 0.5
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{ExtractSegmentsImageFilter}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLineRatioDetectorImageFilter.h"
#include "otbPixelSuppressionByDirectionImageFilter.h"
#include "otbLocalHoughFilter.h"
#include "otbFillGapsFilter.h"
#include "otbDrawLineSpatialObjectListFilter.h"
#include "itkRescaleIntensityImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{

  if (argc != 12)
    {
    for (int i = 0; i < argc; ++i)
      std::cerr << argv[i] << std::endl;
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputSegmentsImageFile length width ";
    std::cerr << " PixelSuppressionRadiusX PixelSuppressionAngularBeam ";

    std::cerr << " LocalHoughRadiusX LocalHoughRadiusY LocalHoughNumberOfLines";
    std::cerr << " FillGapsRadius FillGapsAngularBeam" << std::endl;

    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with floating point precision
  //  and rescale the results between 0 and 255 in order to export PNG images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  float         InternalPixelType;
  typedef  unsigned char OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The images are defined using the pixel type and the dimension.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<InternalPixelType,  2> InternalImageType;
  typedef otb::Image<OutputPixelType,  2>   OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The filter can be instantiated using the image types defined above.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LineRatioDetectorImageFilter<InternalImageType,
      InternalImageType>  DetectorType;

  typedef otb::PixelSuppressionByDirectionImageFilter<InternalImageType,
      InternalImageType>
  PixelSuppressionType;

  typedef otb::LocalHoughFilter<InternalImageType> LocalHoughType;
  typedef otb::FillGapsFilter                      FillGapsType;

  typedef otb::DrawLineSpatialObjectListFilter<InternalImageType,
      OutputImageType>
  DrawLineListType;

  typedef itk::RescaleIntensityImageFilter<InternalImageType> RescalerType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
  //  image data from a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InternalImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // An \doxygen{otb}{ImageFileWriter} is instantiated in order to write the
  // output image to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //

  //  Software Guide : BeginLatex
  //
  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to SmartPointers.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer   reader = ReaderType::New();
  DetectorType::Pointer detector = DetectorType::New();

  PixelSuppressionType::Pointer pixelSuppression = PixelSuppressionType::New();
  LocalHoughType::Pointer       localHough = LocalHoughType::New();
  FillGapsType::Pointer         fillGaps = FillGapsType::New();
  DrawLineListType::Pointer     drawLineList = DrawLineListType::New();
  RescalerType::Pointer         rescaler = RescalerType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The same is done for the writer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  reader->SetFileName(argv[1]);

  //  Software Guide : BeginLatex
  //
  //  The image obtained with the reader is passed as input to the
  //  \doxygen{otb}{ExtractSegmentsImageFilter}. The pipeline is built as follows.
  //
  //  \index{otb::ExtractSegmentsImageFilter!SetInput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  detector->SetInput(reader->GetOutput());
  pixelSuppression->SetInputImage(detector->GetOutput());
  pixelSuppression->SetInputImageDirection(detector->GetOutputDirection());

  rescaler->SetInput(pixelSuppression->GetOutput());

  localHough->SetInput(rescaler->GetOutput());

  fillGaps->SetInput (localHough->GetOutput());

  drawLineList->SetInput(reader->GetOutput());
  drawLineList->SetInputLineSpatialObjectList(fillGaps->GetOutput());

  writer->SetInput(drawLineList->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The methods \code{SetLengthLine()} and \code{SetWidthLine()}
  //  allow setting the minimum length and the typical witdh of the
  //  lines which are to be detected.
  //
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  detector->SetLengthLine(atoi(argv[3]));
  detector->SetWidthLine(atoi(argv[4]));

  unsigned int PixelSuppressionRadiusX((unsigned int) ::atoi(argv[5]));
  float        PixelSuppressionAngularBeam((float) ::atof(argv[6]));

  unsigned int LocalHoughRadiusX((unsigned int) ::atoi(argv[7]));
  unsigned int LocalHoughRadiusY((unsigned int) ::atoi(argv[8]));
  unsigned int LocalHoughNumberOfLines((unsigned int) ::atoi(argv[9]));

  float FillGapsRadius((float) ::atoi(argv[10]));
  float FillGapsAngularBeam((float) ::atof(argv[11]));

  PixelSuppressionType::SizeType PixelSuppressionRadius;
  PixelSuppressionRadius[0] = PixelSuppressionRadiusX;
  PixelSuppressionRadius[1] = PixelSuppressionRadiusX;

  pixelSuppression->SetRadius(PixelSuppressionRadius);
  pixelSuppression->SetAngularBeam(PixelSuppressionAngularBeam);

  PixelSuppressionType::SizeType LocalHoughRadius;
  LocalHoughRadius[0] = LocalHoughRadiusX;
  LocalHoughRadius[1] = LocalHoughRadiusY;

  localHough->SetRadius(LocalHoughRadius);
  localHough->SetNumberOfLines(LocalHoughNumberOfLines);

  fillGaps->SetRadius(FillGapsRadius);
  fillGaps->SetAngularBeam(FillGapsAngularBeam);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // \textbf{FIXME: set the methods}
  //   \index{otb::AssymetricFusionOfDetector!SetWidthLine()}
  //  \index{otb::AssymetricFusionOfDetector!SetLengthLine()}

  //  The filter is executed by invoking the \code{Update()} method. If the
  //  filter is part of a larger image processing pipeline, calling
  //  \code{Update()} on a downstream filter will also trigger update of this
  //  filter.
  //
  //  Software Guide : EndLatex

  writer->SetFileName(argv[2]);
  writer->Update();

  //  Software Guide : BeginLatex
  // We can also obtain the direction of the lines by invoking the
  // \code{GetOutputDirections()} method. \textbf{FIXME : implement
  // the method!!}
  //  Software Guide : EndLatex

  //  Software Guide : BeginLatex Figure~\ref{fig:LINECORRELATION_FILTER}
  // shows the result of applying the AssymetricFusionOf edge detector filter
  // to a SAR image.  \begin{figure} \center
  // \includegraphics[width=0.25\textwidth]{amst.eps}
  // \includegraphics[width=0.25\textwidth]{amstSegmentExtractionBySteps.eps}
  // \itkcaption[Line Correlation Detector Application]{Result of applying
  // the \doxygen{otb}{AssymetricFusionOfDetectorImageFilter} to a SAR
  // image. From left to right : original image, line intensity and
  // edge orientation.}  \label{fig:LINECORRELATION_FILTER} \end{figure}
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{otb}{AssymetricFusionOfDetectorImageFilter}
  //  \end{itemize}
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
