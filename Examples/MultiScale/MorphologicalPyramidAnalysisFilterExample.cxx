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
//    INPUTS: {suburb2.jpeg}
//    OUTPUTS: {suburb2_an_1.jpeg}, {suburb2_an_2.jpeg},  {suburb2_an_3.jpeg}, {suburb2_an_4.jpeg}
//    OUTPUTS: {suburb2_sf_1.jpeg}, {suburb2_sf_2.jpeg}, {suburb2_sf_3.jpeg}, {suburb2_sf_4.jpeg}
//    OUTPUTS: {suburb2_if_1.jpeg}, {suburb2_if_2.jpeg}, {suburb2_if_3.jpeg}, {suburb2_if_4.jpeg}
//    OUTPUTS: {suburb2_id_1.jpeg}, {suburb2_id_2.jpeg}, {suburb2_id_3.jpeg}, {suburb2_id_4.jpeg}
//    OUTPUTS: {suburb2_sd_1.jpeg}, {suburb2_sd_2.jpeg}, {suburb2_sd_3.jpeg}, {suburb2_sd_4.jpeg}
//    4 2
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{MorphologicalPyramidAnalyseFilter}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMorphologicalPyramidAnalysisFilter.h"
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The mathematical morphology filters to be used have also to be
// included here.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkMacro.h"

int main(int argc, char * argv[])
{

  if (argc != 24)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImagePrefix iterations decimationRatio" << std::endl;
    return EXIT_FAILURE;
    }

  const char *       inputFilename = argv[1];
  const unsigned int numberOfLevels = atoi(argv[22]);
  const float        decimationRatio = atof(argv[23]);

// Software Guide : BeginLatex
//
// As usual, we start by defining the types needed for the pixels, the
// images, the image reader and the image writer.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Now, we define the types needed for the morphological filters which
// will be used to build the morphological pyramid. The first thing to
// do is define the structuring element, which in our case, will be a
// \doxygen{itk}{BinaryBallStructuringElement} which is templated over
// the pixel type and the dimension of the image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::BinaryBallStructuringElement<InputPixelType,
      Dimension> StructuringElementType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now define the type of the filter to be used by the
// morphological pyramid. In this case, we choose to use an
// \doxygen{otb}{OpeningClosingMorphologicalFilter} which is just the
// concatenation of an opening and a closing. This filter is templated
// over the input and output image types and the structuring element
// type that we just define above.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType,
      InputImageType,
      StructuringElementType>
  OpeningClosingFilterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can finally define the type of the morpholoical pyramid
// filter. The filter is templated over the input and output image
// types and the {\em lowpas} morphological filter to be used.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType,
      OutputImageType,
      OpeningClosingFilterType>
  PyramidFilterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Since the \doxygen{otb}{MorphologicalPyramidAnalyseFilter}
// generates a list of images as output, it is useful to have an
// iterator to access the images. This is done as follows :
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef PyramidFilterType::OutputImageListType::Iterator
  ImageListIterator;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now instantiate the reader in order to access the input
// image which has to be analysed.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We instantiate the morphological pyramid analysis filter and set
// its parameters which are:
//\begin{itemize}
//    \item the number of iterations or levels of the pyramid;
//    \item the subsample scale or decimation factor between two
// successive pyramid levels.
//\end{itemize}
// After that, we plug the pipeline and run it by calling the
// \code{Update()} method.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  PyramidFilterType::Pointer pyramid = PyramidFilterType::New();
  pyramid->SetNumberOfLevels(numberOfLevels);
  pyramid->SetDecimationRatio(decimationRatio);
  pyramid->SetInput(reader->GetOutput());
  pyramid->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
//  The morphological pyramid has 5
// types of output:
// \begin{itemize}
// \item the analysed image at each level of the pyramid through the
// \code{GetOutput()} method;
// \item the brighter details extracted from the filtering operation  through the
// \code{GetSupFilter()} method;
// \item the darker details extracted from the filtering operation through the
// \code{GetInfFilter()} method;
// \item the brighter details extracted from the resampling operation  through the
// \code{GetSupDeci()} method;
// \item the darker details extracted from the resampling operation  through the
// \code{GetInfDeci()} method;
// to decimation
// \end{itemize}
// Each one of these methods provides a list of images (one for each
// level of analysis), so we can iterate through the image lists by
// using iterators.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageListIterator itAnalyse = pyramid->GetOutput()->Begin();
  ImageListIterator itSupFilter = pyramid->GetSupFilter()->Begin();
  ImageListIterator itInfFilter = pyramid->GetInfFilter()->Begin();
  ImageListIterator itInfDeci = pyramid->GetSupDeci()->Begin();
  ImageListIterator itSupDeci =  pyramid->GetInfDeci()->Begin();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
//  We can now instantiate a writer and use it to write all the images
//  to files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  WriterType::Pointer writer =  WriterType::New();

  int i = 1;

  // Writing the results images
  std::cout << (itAnalyse != (pyramid->GetOutput()->End())) << std::endl;
  while (itAnalyse != pyramid->GetOutput()->End())
    {
    writer->SetInput(itAnalyse.Get());
    writer->SetFileName(argv[0 * 4 + i + 1]);
    writer->Update();

    writer->SetInput(itSupFilter.Get());
    writer->SetFileName(argv[1 * 4 + i + 1]);
    writer->Update();

    writer->SetInput(itInfFilter.Get());
    writer->SetFileName(argv[2 * 4 + i + 1]);
    writer->Update();

    writer->SetInput(itInfDeci.Get());
    writer->SetFileName(argv[3 * 4 + i + 1]);
    writer->Update();

    writer->SetInput(itSupDeci.Get());
    writer->SetFileName(argv[4 * 4 + i + 1]);
    writer->Update();

    ++itAnalyse;
    ++itSupFilter;
    ++itInfFilter;
    ++itInfDeci;
    ++itSupDeci;
    ++i;
    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Figure \ref{fig:PYR_IM} shows the test image to be processed by the
// morphological pyramid.
// \begin{figure}
// \center
// \includegraphics[width=0.5\textwidth]{suburb2.eps}
// \itkcaption[Morphological pyramid analysis]{Test image for the
// morphological pyramid.}
// \label{fig:PYR_IM}
// \end{figure}
//
// Figure \ref{fig:PYR_AN} shows the 4 levels of analysis of the image.
// \begin{figure}
// \center
// \includegraphics[width=0.20\textwidth]{suburb2_an_1.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_an_2.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_an_3.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_an_4.eps}
// \itkcaption[Morphological pyramid analysis]{Result of the
// analysis for 4 levels of the pyramid.}
// \label{fig:PYR_AN}
// \end{figure}
//
// Figure \ref{fig:PYR_SF} shows the 4 levels of bright details.
// \begin{figure}
// \center
// \includegraphics[width=0.20\textwidth]{suburb2_sf_1.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_sf_2.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_sf_3.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_sf_4.eps}
// \itkcaption[Morphological pyramid analysis]{Bright details for 4 levels of the pyramid.}
// \label{fig:PYR_SF}
// \end{figure}
//
// Figure \ref{fig:PYR_IF} shows the 4 levels of dark details.
// \begin{figure}
// \center
// \includegraphics[width=0.20\textwidth]{suburb2_if_1.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_if_2.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_if_3.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_if_4.eps}
// \itkcaption[Morphological pyramid analysis]{Dark details for 4 levels of the pyramid.}
// \label{fig:PYR_IF}
// \end{figure}
//
// Figure \ref{fig:PYR_ID} shows the 4 levels of bright decimation details.
// \begin{figure}
// \center
// \includegraphics[width=0.20\textwidth]{suburb2_id_1.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_id_2.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_id_3.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_id_4.eps}
// \itkcaption[Morphological pyramid analysis]{Bright decimation details for 4 levels of the pyramid.}
// \label{fig:PYR_ID}
// \end{figure}
//
// Figure \ref{fig:PYR_SD} shows the 4 levels of dark decimation details.
// \begin{figure}
// \center
// \includegraphics[width=0.20\textwidth]{suburb2_sd_1.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_sd_2.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_sd_3.eps}
// \includegraphics[width=0.20\textwidth]{suburb2_sd_4.eps}
// \itkcaption[Morphological pyramid analysis]{Dark decimation details for 4 levels of the pyramid.}
// \label{fig:PYR_SD}
// \end{figure}
//
// Software Guide : EndLatex

  return EXIT_SUCCESS;
}
