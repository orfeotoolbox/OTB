#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {RP001R.png}
//    OUTPUTS: {RP001RAlign.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{ImageToPathListAlignFilter}.
// This filter allows to extract meaninful alignments. Alignments
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
// facility class \doxygen{DrawPathFilter} which draws a
// \code{itk::PolyLineParametricPath} on top of a given image.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkPolyLineParametricPath.h"
#include "otbDrawPathFilter.h"
// Software Guide : EndCodeSnippet

#include "itkImage.h"
#include "itkImageFileWriter.h"


#include "otbImageFileReader.h"

#include <stdio.h>
#include <iostream>

int main( int argc, char ** argv )
{
  if(argc!=3)
    {

    std::cout << "Usage : " << argv[0] << " inputImage outputImage" << std::endl;
    return EXIT_FAILURE;
    
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
       

  typedef unsigned char                                   InputPixelType;
  typedef unsigned char   	                        OutputPixelType;

  const   unsigned int        	                        Dimension = 2;

  typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

  typedef otb::ImageFileReader< InputImageType  >         ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >         WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename  );

  reader->Update();

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{ImageToPathListAlignFilter} is templated over the
  //  input image type and the output path type, so we start by
  //  defining:
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::PolyLineParametricPath< Dimension >		  PathType;
  typedef otb::ImageToPathListAlignFilter<InputImageType,PathType>
                                                       ListAlignFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Next, we build the pipeline.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  ListAlignFilterType::Pointer alignFilter = ListAlignFilterType::New();

  alignFilter->SetInput( reader->GetOutput() );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We can choose the number of accepted false alarms in the
  // detection with the method \code{SetEps()} for which the parameter
  // is of the form $-log10(\text{max. number of false alarms})$.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  alignFilter->SetEps( 5 );
  // Software Guide : EndCodeSnippet
  
  alignFilter->Update();

  //  Software Guide : BeginLatex
  //
  //  As stated, above, the \doxygen{DrawPathFilter}, is useful for
  //  drawint the detected alignments. This class is templated over
  //  the input image and path types and also on the output image
  //  type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::DrawPathFilter< InputImageType, PathType,
                                   OutputImageType >  DrawPathFilterType;

  DrawPathFilterType::Pointer drawPathFilter = DrawPathFilterType::New();
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  // We will now go through the list of detected paths and feed them
  // to the \doxygen{DrawPathFilter} inside a loop. We will use a list
  // iterator inside a \code{while} statement.

  // Software Guide : BeginCodeSnippet
  typedef ListAlignFilterType::OutputPathListType ListType;

  
  ListType* pathList = alignFilter->GetOutput();

  ListType::iterator listIt = pathList->begin();

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We define a dummy image will be iteratively fed to the
  // \doxygen{DrawPathFilter} after the drawing of each alignment.
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
  while( listIt != pathList->end())
    {

    drawPathFilter->SetImageInput( backgroundImage );
    drawPathFilter->SetPathInput( *(listIt) );

    drawPathFilter->Update();

    backgroundImage = drawPathFilter->GetOutput();
    
    ++listIt;

    }
    
  writer->SetInput( drawPathFilter->GetOutput() );
  // Software Guide : EndCodeSnippet
  
  writer->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:Align} shows the result of applying the alignment
  // detection to a small patch extracted from a Spot 5 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{RP001R.eps}
  // \includegraphics[width=0.25\textwidth]{RP001RAlign.eps}
  // \itkcaption[Lee Filter Application]{Result of applying the
  // \doxygen{ImageToPathListAlignFilter} to a Spot 5 image of a round-about.} 
  // \label{fig:Align}
  // \end{figure}
  //
  //  Software Guide : EndLatex 

  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{FrostImageFilter}
  //  \end{itemize}
  //
  //  Software Guide : EndLatex
  


  return EXIT_SUCCESS;
}


