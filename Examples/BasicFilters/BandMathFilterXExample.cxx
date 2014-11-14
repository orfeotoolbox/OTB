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
//    OUTPUTS: {RoadExtractBandMathX.tif}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This filter is based on the mathematical parser library muParserX.
//  The built in functions and operators list is available at:
//  \url{http://articles.beltoforion.de/article.php?a=muparserx}.
//
//  In order to use this filter, at least one input image is to be
// set. An associated variable name can be specified or not by using
// the corresponding SetNthInput method. For the jth (j=1..T) input image, if
// no associated variable name has been specified, a default variable
// name is given by concatenating the prefix "im" with the
// corresponding input index plus one (for instance, im1 is related to the first input).
// If the jth input image is multidimensional, then the variable imj represents a vector whose components are related to its bands.
// In order to access the kth band, the variable observes the following pattern : imjbk.
//
//  Software Guide : EndLatex

#include "itkMacro.h"
#include <iostream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//  Software Guide : BeginLatex
//
// We start by including the needed header files.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBandMathImageFilterX.h"
// Software Guide : EndCodeSnippet

int main( int argc, char* argv[])
{

  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile ";
    std::cerr << " outputImageFile2";
    std::cerr << " context.txt" << std::endl;
    return EXIT_FAILURE;
    }

//  Software Guide : BeginLatex
//
//  We start by the classical \code{typedef}s needed for reading and
//  writing the images. The \doxygen{otb}{BandMathImageFilterX} class
//  works with \doxygen{otb}{VectorImage}.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef double                                                PixelType;
  typedef otb::VectorImage<PixelType, 2>                         ImageType;
  typedef otb::ImageFileReader<ImageType>                        ReaderType;
  typedef otb::ImageFileWriter<ImageType>                        WriterType;
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  We can now define the type for the filter:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::BandMathImageFilterX<ImageType>                  FilterType;
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We instantiate the filter, the reader, and the writer:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The reader and the writer are parametrized with usual settings:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  The aim of this example is to compute a simple high-pass filter.
//  For that purpose, we are going to perform the difference between the original signal
//  and its averaged version. The definition of the expression that follows is only suitable for a 4-band image.
//  So first, we must check this requirement:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
 reader->UpdateOutputInformation();
 if (reader->GetOutput()->GetNumberOfComponentsPerPixel() !=4)
    itkGenericExceptionMacro(<< "Input image must have 4 bands." << std::endl);
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  Now, we can define the expression. The variable im1 represents a pixel (made of 4 components) of the input image.
//  The variable im1b1N5x5 represents a neigborhood of size 5x5 around this pixel (and so on for each band).
//  The last element we need is the operator mean. By setting its inputs with four neigborhoods, we tell this operator to process the four related bands.
//  As output, it will produce a vector of four components; this is consistent with the fact that we wish to perform a difference with im1.
//  Thus, the final expression is as follows: 
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
 filter->SetExpression("im1-mean(im1b1N5x5,im1b2N5x5,im1b3N5x5,im1b4N5x5)");
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  Note that the importance of the averaging is driven by the names of the neigborhood variables.
//  Last thing we have to do, is to set the pipeline:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
 filter->SetNthInput(0,reader->GetOutput());
 writer->SetInput(filter->GetOutput());
 writer->Update();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// Figure~\ref{fig:BandMathImageFilterX} shows the result of our high-pass filter.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{qb_ExtractRoad_pretty.eps}
// \includegraphics[width=0.45\textwidth]{qb_BandMath-res1.eps}
// \itkcaption[Band Math X]{From left to right:
// Original image, high-pass filter output.}
// \label{fig:BandMathImageFilterX}
// \end{figure}
//
// Software Guide : EndLatex


//  Software Guide : BeginLatex
//
//  Now let's see a little bit more complex example.
//  The aim now is to give the central pixel a higher weight. Moreover :
//  - we wish to use smaller neighborhoods 
//  - we wish to drop the 4th band
//  - we wish to add a given number to each band.
//
//  First, we instantiate new filters to later make a proper pipeline: 
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader2 = ReaderType::New();
  WriterType::Pointer writer2 = WriterType::New();
  FilterType::Pointer filter2 = FilterType::New();

  reader2->SetFileName(argv[1]);
  writer2->SetFileName(argv[3]);
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We define a new kernel (rows are separated by semi-colons, whereas their elements are separated by commas): 
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter2->SetMatrix("kernel","{ 0.1 , 0.1 , 0.1 ; 0.1 , 0.2 , 0.1 ; 0.1 , 0.1 , 0.1 }");
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We then define a new constant: 
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter2->SetConstant("cst",1.0);
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We now set the expression (note the use of conv operator): 
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter2->SetExpression("bands(im1,{1,2,3})-conv(kernel,im1b1N3x3,im1b2N3x3,im1b3N3x3) + {cst,cst,cst}");
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  It is possible to export these definitions to a txt file (they will be reusable later thanks to the method ImportContext): 
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter2->ExportContext(argv[4]);
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  And finally, we set the pipeline: 
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
 filter2->SetNthInput(0,reader2->GetOutput());
 writer2->SetInput(filter2->GetOutput());
 writer2->Update();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// Figure~\ref{fig:BandMathImageFilterX} shows the result of the second filter.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{qb_ExtractRoad_pretty.eps}
// \includegraphics[width=0.45\textwidth]{qb_BandMath-res2.eps}
// \itkcaption[Band Math X]{From left to right:
// Original image, second filter output.}
// \label{fig:BandMathImageFilterX}
// \end{figure}
//
// Software Guide : EndLatex

//  Software Guide : BeginLatex
//
// Finally, it is strongly recommended to take a look at the cookbook, where additional information and examples can be found (http://www.orfeo-toolbox.org/packages/OTBCookBook.pdf).
//
//  Software Guide : EndLatex


  return EXIT_SUCCESS;
}
