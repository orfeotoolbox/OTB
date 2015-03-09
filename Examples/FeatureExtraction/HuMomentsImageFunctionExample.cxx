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
#include "otbImage.h"

#include "otbImageFileReader.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROISpot5.png}
//    2
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{HuMomentsImageFunction}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbHuMomentsImageFunction.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " moment_number" << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const unsigned int radius   = atoi(argv[2]);

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{otb}{HuImageFunction} is templated over the
  //  input image type and the output (real) type value, so we start by
  //  defining:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::HuMomentsImageFunction<InputImageType>  HuType;
  typedef HuType::OutputType                           MomentType;

  HuType::Pointer hmFunction = HuType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // We can choose the region and the pixel of the image which will
  // used as coordinate origin
  // for the moment computation
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  InputImageType::RegionType region;
  InputImageType::SizeType   size;
  InputImageType::IndexType  start;

  start[0] = 0;
  start[1] = 0;
  size[0] = 50;
  size[1] = 50;

  reader->Update();
  InputImageType::Pointer image = reader->GetOutput();

  region.SetIndex(start);
  region.SetSize(size);

  image->SetRegions(region);
  image->Update();

  InputImageType::IndexType center;
  center[0] = start[0] + size[0] / 2;
  center[1] = start[1] + size[1] / 2;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Next, we plug the input image into the complex moment function
  // and we set its parameters.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  hmFunction->SetInputImage(image);
  hmFunction->SetNeighborhoodRadius(radius);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // In order to get the value of the moment, we call the
  // \code{EvaluateAtIndex} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MomentType Result = hmFunction->EvaluateAtIndex(center);

  for (unsigned int j=0; j<7; ++j)
    {
    std::cout << "The moment of order " << j+1 <<
      " is equal to " << Result[j] << std::endl;
    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{otb}{HuPathFunction}
  //  \end{itemize}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
