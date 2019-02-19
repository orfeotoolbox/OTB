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
./HuMomentsImageFunctionExample Input/ROISpot5.png 2
*/


// This example illustrates the use of the \doxygen{otb}{HuMomentsImageFunction}.
//
// The first step required to use this filter is to include its header file.

#include "otbHuMomentsImageFunction.h"

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

  //  The \doxygen{otb}{HuImageFunction} is templated over the
  //  input image type and the output (real) type value, so we start by
  //  defining:

  typedef otb::HuMomentsImageFunction<InputImageType>  HuType;
  typedef HuType::OutputType                           MomentType;

  HuType::Pointer hmFunction = HuType::New();

  // We can choose the region and the pixel of the image which will
  // used as coordinate origin
  // for the moment computation

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

  // Next, we plug the input image into the complex moment function
  // and we set its parameters.

  hmFunction->SetInputImage(image);
  hmFunction->SetNeighborhoodRadius(radius);

  // In order to get the value of the moment, we call the
  // \code{EvaluateAtIndex} method.

  MomentType Result = hmFunction->EvaluateAtIndex(center);

  for (unsigned int j=0; j<7; ++j)
    {
    std::cout << "The moment of order " << j+1 <<
      " is equal to " << Result[j] << std::endl;
    }

  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{otb}{HuPathFunction}
  //  \end{itemize}

  return EXIT_SUCCESS;
}
