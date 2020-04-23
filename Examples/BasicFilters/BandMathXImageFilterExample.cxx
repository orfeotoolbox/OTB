/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/* Example usage:
./BandMathXImageFilterExample Input/qb_RoadExtract.tif Output/qb_BandMath-res1.tif Output/qb_BandMath-res2.tif Output/context.txt
*/


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

#include "itkMacro.h"
#include <iostream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

// We start by including the needed header files.

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBandMathXImageFilter.h"

int main(int argc, char* argv[])
{

  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile ";
    std::cerr << " outputImageFile2";
    std::cerr << " context.txt" << std::endl;
    return EXIT_FAILURE;
  }

  //  Then, we set the classical \code{typedef}s needed for reading and
  //  writing the images. The \doxygen{otb}{BandMathXImageFilter} class
  //  works with \doxygen{otb}{VectorImage}.

  using PixelType  = double;
  using ImageType  = otb::VectorImage<PixelType, 2>;
  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<ImageType>;


  //  We can now define the type for the filter:

  using FilterType = otb::BandMathXImageFilter<ImageType>;

  //  We instantiate the filter, the reader, and the writer:

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  //  The reader and the writer are parametrized with usual settings:

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);


  //  The aim of this example is to compute a simple high-pass filter.
  //  For that purpose, we are going to perform the difference between the original signal
  //  and its averaged version. The definition of the expression that follows is only suitable for a 4-band image.
  //  So first, we must check this requirement:

  reader->UpdateOutputInformation();
  if (reader->GetOutput()->GetNumberOfComponentsPerPixel() != 4)
    itkGenericExceptionMacro(<< "Input image must have 4 bands." << std::endl);

  //  Now, we can define the expression. The variable im1 represents a pixel (made of 4 components) of the input image.
  //  The variable im1b1N5x5 represents a neighborhood of size 5x5 around this pixel (and so on for each band).
  //  The last element we need is the operator 'mean'. By setting its inputs with four neigborhoods, we tell this operator to process the four related bands.
  //  As output, it will produce a vector of four components; this is consistent with the fact that we wish to perform a difference with im1.
  //
  //  Thus, the expression is as follows:


  filter->SetExpression("im1-mean(im1b1N5x5,im1b2N5x5,im1b3N5x5,im1b4N5x5)");

  //  Note that the importance of the averaging is driven by the names of the neighborhood variables.
  //  Last thing we have to do, is to set the pipeline:

  filter->SetNthInput(0, reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  // Figure~\ref{fig:BandMathXImageFilter} shows the result of our high-pass filter.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.45\textwidth]{qb_ExtractRoad_pretty.eps}
  // \includegraphics[width=0.45\textwidth]{qb_BandMath-res1.eps}
  // \itkcaption[Band Math X]{From left to right:
  // Original image, high-pass filter output.}
  // \label{fig:BandMathXImageFilter}
  // \end{figure}


  //  Now let's see a little bit more complex example.
  //  The aim now is to give the central pixel a higher weight. Moreover :
  //  - we wish to use smaller neighborhoods
  //  - we wish to drop the 4th band
  //  - we wish to add a given number to each band.
  //
  //  First, we instantiate new filters to later make a proper pipeline:

  ReaderType::Pointer reader2 = ReaderType::New();
  WriterType::Pointer writer2 = WriterType::New();
  FilterType::Pointer filter2 = FilterType::New();

  reader2->SetFileName(argv[1]);
  writer2->SetFileName(argv[3]);

  //  We define a new kernel (rows are separated by semi-colons, whereas their elements are separated by commas):

  filter2->SetMatrix("kernel", "{ 0.1 , 0.1 , 0.1; 0.1 , 0.2 , 0.1; 0.1 , 0.1 , 0.1 }");

  //  We then define a new constant:

  filter2->SetConstant("cst", 1.0);

  //  We now set the expression (note the use of 'dotpr' operator, as well as the 'bands' operator which is used as a band selector):

  filter2->SetExpression("bands(im1,{1,2,3})-dotpr(kernel,im1b1N3x3,im1b2N3x3,im1b3N3x3) + {cst,cst,cst}");

  //  It is possible to export these definitions to a txt file (they will be reusable later thanks to the method ImportContext):

  filter2->ExportContext(argv[4]);


  //  And finally, we set the pipeline:

  filter2->SetNthInput(0, reader2->GetOutput());
  writer2->SetInput(filter2->GetOutput());
  writer2->Update();

  // Figure~\ref{fig:BandMathXImageFilter2} shows the result of the second filter.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.45\textwidth]{qb_ExtractRoad_pretty.eps}
  // \includegraphics[width=0.45\textwidth]{qb_BandMath-res2.eps}
  // \itkcaption[Band Math X]{From left to right:
  // Original image, second filter output.}
  // \label{fig:BandMathXImageFilter2}
  // \end{figure}

  // Finally, it is strongly recommended to take a look at the cookbook, where additional information and examples can be found
  // (http://www.orfeo-toolbox.org/packages/OTBCookBook.pdf).


  return EXIT_SUCCESS;
}
