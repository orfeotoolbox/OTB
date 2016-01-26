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
//  OUTPUTS: {amstSegmentExtraction.png}
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
#include "otbExtractSegmentsImageFilter.h"
#include "otbLineRatioDetectorImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
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
      InternalImageType> DetectorType;
  typedef otb::ExtractSegmentsImageFilter<InternalImageType,
      InternalImageType>   ExtractorType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{ImageFileReader} class is also instantiated in order to read
  //  image data from a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InternalImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // An \doxygen{ImageFileWriter} is instantiated in order to write the
  // output image to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The intensity rescaling of the results will be carried out by the
  // \code{itk::RescaleIntensityImageFilter} which is templated by the
  // input and output image types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RescaleIntensityImageFilter<InternalImageType,
      OutputImageType> RescalerType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to SmartPointers.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer    reader = ReaderType::New();
  DetectorType::Pointer  detector = DetectorType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The same is done for the rescaler and the writer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  RescalerType::Pointer rescaler = RescalerType::New();
  WriterType::Pointer   writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  reader->SetFileName(argv[1]);

  //  Software Guide : BeginLatex
  //
  //  The \code{itk::RescaleIntensityImageFilter} needs to know which
  //  is the minimu and maximum values of the output generated
  //  image. Those can be chosen in a generic way by using the
  //  \code{NumericTraits} functions, since they are templated over
  //  the pixel type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());
  // Software Guide : EndCodeSnippet

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
  extractor->SetInputImage(detector->GetOutput());
  extractor->SetInputImageDirection(detector->GetOutputDirection());
  extractor->SetLineValue(0.);
  rescaler->SetInput(extractor->GetOutput());
  writer->SetInput(rescaler->GetOutput());
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

  ExtractorType::SizeType PixelSuppressionRadius;
  PixelSuppressionRadius[0] = PixelSuppressionRadiusX;
  PixelSuppressionRadius[1] = PixelSuppressionRadiusX;

  extractor->SetPixelSuppressionRadius(PixelSuppressionRadius);
  extractor->SetPixelSuppressionAngularBeam(PixelSuppressionAngularBeam);

  ExtractorType::SizeType LocalHoughRadius;
  LocalHoughRadius[0] = LocalHoughRadiusX;
  LocalHoughRadius[1] = LocalHoughRadiusY;

  extractor->SetLocalHoughRadius(LocalHoughRadius);
  extractor->SetLocalHoughNumberOfLines(LocalHoughNumberOfLines);

  extractor->SetFillGapsRadius(FillGapsRadius);
  extractor->SetFillGapsAngularBeam(FillGapsAngularBeam);
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

  // Software Guide : BeginCodeSnippet
  extractor->Update();
  // Software Guide : EndCodeSnippet

  writer->SetFileName(argv[2]);
  writer->Update();

  //  Software Guide : BeginLatex Figure~\ref{fig:LINECORRELATION_FILTER}
  // shows the result of applying the AssymetricFusionOf edge detector filter
  // to a SAR image.  \begin{figure} \center
  // \includegraphics[width=0.25\textwidth]{amst.eps}
  // \includegraphics[width=0.25\textwidth]{amstSegmentExtraction.eps}
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
