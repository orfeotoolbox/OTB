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
//  INPUTS: {detected_lines.png}
//  OUTPUTS: {detected_local_hough.png}
//  30 10 1 50
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{ExtractSegmentsImageFilter}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLocalHoughFilter.h"
#include "otbDrawLineSpatialObjectListFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{

  if (argc != 7)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr <<
    " outputImageFile  LocalHoughRadius LocalHoughOverlap LocalHoughNumberOfLines LocalHoughThreshold"
              << std::endl;

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
  typedef otb::LocalHoughFilter<InternalImageType> LocalHoughType;
  typedef otb::DrawLineSpatialObjectListFilter<InternalImageType,
      OutputImageType>
  DrawLineListType;
  // Software Guide : EndCodeSnippet

  typedef itk::RescaleIntensityImageFilter<InternalImageType,
      OutputImageType> RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());

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
  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to SmartPointers.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();

  LocalHoughType::Pointer localHough = LocalHoughType::New();

  DrawLineListType::Pointer drawLineList = DrawLineListType::New();
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

  unsigned int LocalHoughRadiusX((unsigned int) ::atoi(argv[3]));
  unsigned int LocalHoughRadiusY((unsigned int) ::atoi(argv[3]));
  unsigned int LocalHoughOverlapX((unsigned int) ::atoi(argv[4]));
  unsigned int LocalHoughOverlapY((unsigned int) ::atoi(argv[4]));
  unsigned int LocalHoughNumberOfLines((unsigned int) ::atoi(argv[5]));
  float        LocalHoughThreshold((float) ::atoi(argv[6]));

  LocalHoughType::SizeType LocalHoughRadius;
  LocalHoughRadius[0] = LocalHoughRadiusX;
  LocalHoughRadius[1] = LocalHoughRadiusY;

  LocalHoughType::SizeType LocalHoughOverlap;
  LocalHoughOverlap[0] = LocalHoughOverlapX;
  LocalHoughOverlap[1] = LocalHoughOverlapY;

  localHough->SetRadius(LocalHoughRadius);
  localHough->SetOverlap(LocalHoughOverlap);
  localHough->SetNumberOfLines(LocalHoughNumberOfLines);
  localHough->SetThreshold(LocalHoughThreshold);

  //  Software Guide : BeginLatex
  //
  //  The image obtained with the reader is passed as input to the
  //  \doxygen{otb}{ExtractSegmentsImageFilter}. The pipeline is built as follows.
  //
  //  \index{otb::ExtractSegmentsImageFilter!SetInput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  localHough->SetInput(reader->GetOutput());

  drawLineList->SetInput(reader->GetOutput());
  drawLineList->SetInputLineSpatialObjectList(localHough->GetOutput());
  writer->SetFileName(argv[2]);
  writer->SetInput(drawLineList->GetOutput());
  writer->Update();
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:LOCAL_HOUGH}
  // shows the result of applying the \doxygen{otb}{LocalHoughImageFilter}.
  // \begin{figure} \center
  // \includegraphics[width=0.25\textwidth]{detected_lines.eps}
  // \includegraphics[width=0.25\textwidth]{detected_local_hough.eps}
  // \itkcaption[Line Correlation Detector Application]{Result of applying
  // the \doxygen{otb}{LocalHoughImageFilter}. From left to right :
  // original image, extracted segments.}  \label{fig:LOCAL_HOUGH} \end{figure}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
