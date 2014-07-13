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
//    1 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// The complex moments can be computed on images, but sometimes we are
// interested in computing them on shapes extracted from images by
// segmentation algorithms. These shapes can be represented by
// \doxygen{itk}{Path}s. This example illustrates the use of the
// \doxygen{otb}{ComplexMomentPathFunction} for the computation of
// complex geometric moments on ITK paths.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbComplexMomentPathFunction.h"
// Software Guide : EndCodeSnippet
#include "itkPolyLineParametricPath.h"

int main(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " p q" << std::endl;
    return EXIT_FAILURE;
    }

  unsigned int P((unsigned char) ::atoi(argv[1]));
  unsigned int Q((unsigned char) ::atoi(argv[2]));

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{otb}{ComplexMomentPathFunction} is templated over the
  //  input path type and the output complex type value, so we start by
  //  defining:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;

  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef std::complex<double>                                  ComplexType;
  typedef otb::ComplexMomentPathFunction<PathType, ComplexType> CMType;

  CMType::Pointer cmFunction = CMType::New();
  // Software Guide : EndCodeSnippet

  PathType::Pointer path = PathType::New();

  path->Initialize();

  typedef PathType::ContinuousIndexType ContinuousIndexType;

  ContinuousIndexType cindex;

  // Draw a square:

  path->Initialize();

  cindex[0] = 30;
  cindex[1] = 30;
  path->AddVertex(cindex);
  cindex[0] = 30;
  cindex[1] = 130;
  path->AddVertex(cindex);
  cindex[0] = 130;
  cindex[1] = 130;
  path->AddVertex(cindex);
  cindex[0] = 130;
  cindex[1] = 30;
  path->AddVertex(cindex);

  //  Software Guide : BeginLatex
  //
  // Next, we set the parameters of the plug the input path into the complex moment function
  // and we set its parameters.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  cmFunction->SetInputPath(path);
  cmFunction->SetQ(Q);
  cmFunction->SetP(P);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Since the paths are defined in physical coordinates, we do not
  // need to set the center for the moment computation as we did
  // with the \doxygen{otb}{ComplexMomentImageFunction}. The same
  // applies for the size of the neighborhood around the
  // center pixel for the moment computation. The moment computation
  // is triggered by calling the \code{Evaluate} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ComplexType Result = cmFunction->Evaluate();

  std::cout << "The moment of order (" << P << "," << Q <<
  ") is equal to " << Result << std::endl;
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
