/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "itkMacro.h"
#include "otbImage.h"

#include "otbImageFileReader.h"

/* Example usage:
./ComplexMomentPathExample 1 1
*/


// The complex moments can be computed on images, but sometimes we are
// interested in computing them on shapes extracted from images by
// segmentation algorithms. These shapes can be represented by
// \doxygen{itk}{Path}s. This example illustrates the use of the
// \doxygen{otb}{ComplexMomentPathFunction} for the computation of
// complex geometric moments on ITK paths.
//
// The first step required to use this filter is to include its header file.

#include "otbComplexMomentPathFunction.h"
#include "itkPolyLineParametricPath.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " p q" << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int P((unsigned char)::atoi(argv[1]));
  unsigned int Q((unsigned char)::atoi(argv[2]));

  //  The \doxygen{otb}{ComplexMomentPathFunction} is templated over the
  //  input path type and the output complex type value, so we start by
  //  defining:

  const unsigned int Dimension = 2;

  using PathType = itk::PolyLineParametricPath<Dimension>;

  using ComplexType = std::complex<double>;
  using CMType      = otb::ComplexMomentPathFunction<PathType, ComplexType>;

  CMType::Pointer cmFunction = CMType::New();

  PathType::Pointer path = PathType::New();

  path->Initialize();

  using ContinuousIndexType = PathType::ContinuousIndexType;

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

  // Next, we set the parameters of the plug the input path into the complex moment function
  // and we set its parameters.

  cmFunction->SetInputPath(path);
  cmFunction->SetQ(Q);
  cmFunction->SetP(P);

  // Since the paths are defined in physical coordinates, we do not
  // need to set the center for the moment computation as we did
  // with the \doxygen{otb}{ComplexMomentImageFunction}. The same
  // applies for the size of the neighborhood around the
  // center pixel for the moment computation. The moment computation
  // is triggered by calling the \code{Evaluate} method.

  ComplexType Result = cmFunction->Evaluate();

  std::cout << "The moment of order (" << P << "," << Q << ") is equal to " << Result << std::endl;

  return EXIT_SUCCESS;
}
