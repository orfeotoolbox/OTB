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


#include "otbImageFileReader.h"
#include "itkImageRegionConstIterator.h"
//  Software Guide : BeginLatex
//
//  Usually, the streaming process is hidden within the pipeline. This
//  allows the user to get rid of the annoying task of splitting the
//  images into tiles, and so on. However, for some kinds of
//  processing, we do not really need a pipeline: no writer is needed,
//  only read access to pixel values is wanted. In these cases, one
//  has to explicitly set up the streaming procedure. Fortunately, OTB
//  offers a high level of abstraction for this task. We will need to
//  include the following header files:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbStreamingTraits.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{

  if (argc != 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile nbLinesForStreaming " << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname = argv[1];
  const unsigned int nbLinesForStreaming = atoi(argv[2]);

  typedef float PixelType;

  typedef otb::VectorImage<PixelType, 2>  ImageType;
  typedef otb::ImageFileReader<ImageType> ImageReaderType;

//  Software Guide : BeginLatex
//
//  The \doxygen{otb}{StreamingTraits} class manages the streaming
//  approaches which are possible with the image type over which it is
//  templated. The class \doxygen{itk}{ImageRegionSplitter} is
//  templated over the number of dimensions of the image and will
//  perform the actual image splitting. More information on splitter can be
//  found in section~\ref{sec:Splitters}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::StreamingTraits<ImageType> StreamingTraitsType;
  typedef itk::ImageRegionSplitter<2>     SplitterType;
// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
//
//  Once a region of the image is available, we will use classical
//  region iterators to get the pixels.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef ImageType::RegionType RegionType;

  typedef itk::ImageRegionConstIterator<ImageType> IteratorType;
// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
//
//  We instantiate the image file reader, but in order to avoid
//  reading the whole image, we call the
//  \code{GenerateOutputInformation()} method instead of the
//  \code{Update()} one. \code{GenerateOutputInformation()} will make
//  available the information about sizes, band, resolutions,
//  etc. After that, we can access the largest possible region of the
//  input image.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageReaderType::Pointer reader = ImageReaderType::New();

  reader->SetFileName(infname);

  reader->GenerateOutputInformation();

  RegionType largestRegion = reader->GetOutput()->GetLargestPossibleRegion();
// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
//
//  We set up now the local streaming capabilities by asking the
//  streaming traits to compute the number of regions to split the
//  image into given the splitter, the user defined number of lines,
//  and the input image information.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  SplitterType::Pointer splitter = SplitterType::New();
  unsigned int          numberOfStreamDivisions =
    StreamingTraitsType::CalculateNumberOfStreamDivisions(
      reader->GetOutput(),
      largestRegion,
      splitter,
      otb::SET_BUFFER_NUMBER_OF_LINES,
      0, 0, nbLinesForStreaming);
// Software Guide : EndCodeSnippet
  std::cout << "The images will be streamed into " <<
  numberOfStreamDivisions << " parts." << std::endl;

//  Software Guide : BeginLatex
//
//  We can now get the split regions and iterate through them.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  unsigned int piece = 0;
  RegionType   streamingRegion;

  for (piece = 0;
       piece < numberOfStreamDivisions;
       piece++)
    {
// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
//
//  We get the region
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    streamingRegion =
      splitter->GetSplit(piece, numberOfStreamDivisions, largestRegion);

    std::cout << "Processing region: " << streamingRegion << std::endl;
// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
//
//  We ask the reader to provide the region.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    reader->GetOutput()->SetRequestedRegion(streamingRegion);
    reader->GetOutput()->PropagateRequestedRegion();
    reader->GetOutput()->UpdateOutputData();
// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
//
//  We declare an iterator and walk through the region.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    IteratorType it(reader->GetOutput(), streamingRegion);
    it.GoToBegin();

    while (!it.IsAtEnd())
      {
      std::cout << it.Get() << std::endl;
      ++it;
      }
// Software Guide : EndCodeSnippet
    }
  return EXIT_SUCCESS;
}
