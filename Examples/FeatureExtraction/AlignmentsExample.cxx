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
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {QB_SuburbAlign.png}
//    2
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{ImageToPathListAlignFilter}.
// This filter allows extracting meaninful alignments. Alignments
// (that is edges and lines) are detected using the {\em Gestalt}
// approach proposed by Desolneux et al. \cite{desolneux}. In this
// context, an event is
// considered meaningful if the expectation of its occurrence would be
// very small in a random image. One can thus consider that in a
// random image the direction of the gradient of a given point is
// uniformly distributed, and that neighbouring pixels have a very low
// probability of having the same gradient direction. This algorithm
// gives a set of straight line segments defined by the two extremity
// coordinates under the form of a \code{std::list} of
// \code{itk::PolyLineParametricPath}.
//
// The first step required to use this filter is to include its header.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageToPathListAlignFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// In order to visualize the detected alignments, we will use the
// facility class \doxygen{otb}{DrawPathFilter} which draws a
// \code{itk::PolyLineParametricPath} on top of a given image.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkPolyLineParametricPath.h"
#include "otbDrawPathFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileWriter.h"

#include "otbImageFileReader.h"

#include <stdio.h>
#include <iostream>

int main(int argc, char *argv[])
{
  if (argc != 4)
    {

    std::cout << "Usage : " << argv[0] << " inputImage outputImage epsilon" <<
    std::endl;
    return EXIT_FAILURE;

    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned short InputPixelType;
  typedef unsigned short OutputPixelType;

  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  reader->Update();

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{otb}{ImageToPathListAlignFilter} is templated over the
  //  input image type and the output path type, so we start by
  //  defining:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::ImageToPathListAlignFilter<InputImageType, PathType>
  ListAlignFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Next, we build the pipeline.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ListAlignFilterType::Pointer alignFilter = ListAlignFilterType::New();

  alignFilter->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We can choose the number of accepted false alarms in the
  // detection with the method \code{SetEps()} for which the parameter
  // is of the form $-log10(\text{max. number of false alarms})$.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  alignFilter->SetEps(atoi(argv[3]));
  // Software Guide : EndCodeSnippet

  alignFilter->Update();

  //  Software Guide : BeginLatex
  //
  //  As stated, above, the \doxygen{otb}{DrawPathFilter}, is useful for
  //  drawint the detected alignments. This class is templated over
  //  the input image and path types and also on the output image
  //  type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::DrawPathFilter<InputImageType, PathType,
      OutputImageType>  DrawPathFilterType;
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  // We will now go through the list of detected paths and feed them
  // to the \doxygen{otb}{DrawPathFilter} inside a loop. We will use a list
  // iterator inside a \code{while} statement.

  // Software Guide : BeginCodeSnippet
  typedef ListAlignFilterType::OutputPathListType ListType;

  ListType* pathList = alignFilter->GetOutput();

  ListType::Iterator listIt = pathList->Begin();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We define a dummy image will be iteratively fed to the
  // \doxygen{otb}{DrawPathFilter} after the drawing of each alignment.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  InputImageType::Pointer backgroundImage = reader->GetOutput();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We iterate through the list and write the result to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  while (listIt != pathList->End())
    {

    DrawPathFilterType::Pointer drawPathFilter = DrawPathFilterType::New();
    drawPathFilter->SetImageInput(backgroundImage);
    drawPathFilter->SetInputPath(listIt.Get());

    drawPathFilter->SetValue(itk::NumericTraits<OutputPixelType>::max());
    drawPathFilter->Update();

    backgroundImage = drawPathFilter->GetOutput();

    ++listIt;

    }

  writer->SetInput(backgroundImage);
  // Software Guide : EndCodeSnippet

  writer->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:Align} shows the result of applying the alignment
  // detection to a small patch extracted from a VHR image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.35\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.35\textwidth]{QB_SuburbAlign.eps}
  // \itkcaption[Alignment Detection Application]{Result of applying the
  // \doxygen{otb}{ImageToPathListAlignFilter} to a VHR image of a suburb.}
  // \label{fig:Align}
  // \end{figure}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
