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
#include "itkMacro.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {suburb2.jpeg}
//    OUTPUTS: {suburb2_synthesis.jpeg}
//    4 2
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{MorphologicalPyramidSynthesisFilter}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMorphologicalPyramidSynthesisFilter.h"
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The mathematical morphology filters to be used have also to be
// included here, as well as the
// \doxygen{otb}{MorphologicalPyramidAnalyseFilter} in order to
// perform the analysis step.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMorphologicalPyramidAnalysisFilter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int main(int argc, char * argv[])
{

  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile iterations decimationRatio" << std::endl;
    return EXIT_FAILURE;
    }

  const char *       inputFilename = argv[1];
  const char *       outputFilename = argv[2];
  const unsigned int numberOfLevels = atoi(argv[3]);
  const float        decimationRatio = atof(argv[4]);

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
  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension>
  StructuringElementType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now define the type of the filter to be used by the
// morphological pyramid. In this case, we choose to use an
// \doxygen{otb}{OpeningClosingMorphologicalFilter} which is just the
// concatenation of an opening and a closing. This filter is theplated
// over the input and output image types and the structurung element
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
// We can now define the type of the morpholoical pyramid
// filter. The filter is templated over the input and output mage
// types and the {\em lowpas} morphological filter to be used.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType,
      OutputImageType,
      OpeningClosingFilterType>
  PyramidAnalysisFilterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can finally define the type of the morpholoical pyramid synthesis
// filter. The filter is templated over the input and output mage
// types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::MorphologicalPyramidSynthesisFilter<InputImageType,
      OutputImageType>
  PyramidSynthesisFilterType;
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
  PyramidAnalysisFilterType::Pointer pyramidAnalysis =
    PyramidAnalysisFilterType::New();
  pyramidAnalysis->SetNumberOfLevels(numberOfLevels);
  pyramidAnalysis->SetDecimationRatio(decimationRatio);
  pyramidAnalysis->SetInput(reader->GetOutput());
  pyramidAnalysis->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Once the analysis step is finished we can proceed to the synthesis
// of the image from its different levels of decomposition.
//  The morphological pyramid has 5
// types of output:
// \begin{itemize}
// \item the Analysisd image at each level of the pyramid through the
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
// This outputs can be used as input of the synthesis filter by using
// the appropriate methods.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  PyramidSynthesisFilterType::Pointer pyramidSynthesis =
    PyramidSynthesisFilterType::New();
  pyramidSynthesis->SetInput(pyramidAnalysis->GetOutput()->Back());
  pyramidSynthesis->SetSupFilter(pyramidAnalysis->GetSupFilter());
  pyramidSynthesis->SetSupDeci(pyramidAnalysis->GetSupDeci());
  pyramidSynthesis->SetInfFilter(pyramidAnalysis->GetInfFilter());
  pyramidSynthesis->SetInfDeci(pyramidAnalysis->GetInfDeci());
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After that, we plug the pipeline and run it by calling the
// \code{Update()} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  pyramidSynthesis->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We finally instatiate a the writer in order to save the result
// image to a file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(pyramidSynthesis->GetOutput()->Back());
  writer->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Since the synthesis operation is applied on the result of the
// analysis, the input and the output images should be identical. This
// is the case as shown in figure \ref{fig:PYR_ANSYN}.
// \begin{figure}
// \center
// \includegraphics[width=0.44\textwidth]{suburb2.eps}
// \includegraphics[width=0.44\textwidth]{suburb2_synthesis.eps}
// \itkcaption[Morphological pyramid analysis and synthesis]{Result of
// the morphological pyramid analysis and synthesis. Left: original
// image. Right: result of applying the analysis and the synthesis steps.}
// \label{fig:PYR_ANSYN}
// \end{figure}
//
// Of course, in a real application, a specific processing will be
// applied after the analysis and before the synthesis to, for
// instance, denoise the image by removing pixels at the finer scales, etc.
//
// Software Guide : EndLatex

  return EXIT_SUCCESS;
}
