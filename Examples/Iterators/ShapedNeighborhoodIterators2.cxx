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
./ShapedNeighborhoodIterators2 Input/BinaryImage.png \
                               Output/ShapedNeighborhoodIterators1b.png \
                               4
*/

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include <math.h>

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Missing parameters. " << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile element_radius" << std::endl;
    return -1;
  }

  using PixelType  = unsigned char;
  using ImageType  = otb::Image<PixelType, 2>;
  using ReaderType = otb::ImageFileReader<ImageType>;

  using ShapedNeighborhoodIteratorType = itk::ConstShapedNeighborhoodIterator<ImageType>;
  using IteratorType                   = itk::ImageRegionIterator<ImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  unsigned int element_radius = ::atoi(argv[3]);

  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  ImageType::Pointer output = ImageType::New();
  output->SetRegions(reader->GetOutput()->GetRequestedRegion());
  output->Allocate();

  using FaceCalculatorType = itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<ImageType>;

  FaceCalculatorType                         faceCalculator;
  FaceCalculatorType::FaceListType           faceList;
  FaceCalculatorType::FaceListType::iterator fit;

  ShapedNeighborhoodIteratorType::RadiusType radius;
  radius.Fill(element_radius);

  faceList = faceCalculator(reader->GetOutput(), output->GetRequestedRegion(), radius);

  IteratorType out;
  const float  rad = static_cast<float>(element_radius);

  const PixelType background_value = 0;
  const PixelType foreground_value = 255;

  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
  {
    ShapedNeighborhoodIteratorType it(radius, reader->GetOutput(), *fit);
    out = IteratorType(output, *fit);

    // Creates a circular structuring element by activating all the pixels less
    // than radius distance from the center of the neighborhood.
    for (float y = -rad; y <= rad; y++)
    {
      for (float x = -rad; x <= rad; x++)
      {
        ShapedNeighborhoodIteratorType::OffsetType off;

        float dis = ::sqrt(x * x + y * y);
        if (dis <= rad)
        {
          off[0] = static_cast<int>(x);
          off[1] = static_cast<int>(y);
          it.ActivateOffset(off);
        }
      }
    }

    // The logic of the inner loop can be rewritten to perform
    // dilation.  Dilation of the set $I$ by $E$ is the set of all $x$ such that
    // $E$ positioned at $x$ contains at least one element in $I$.

    // Implements dilation
    for (it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out)
    {
      ShapedNeighborhoodIteratorType::ConstIterator ci;

      bool flag = false;
      for (ci = it.Begin(); ci != it.End(); ci++)
      {
        if (ci.Get() != background_value)
        {
          flag = true;
          break;
        }
      }
      if (flag == true)
      {
        out.Set(foreground_value);
      }
      else
      {
        out.Set(background_value);
      }
    }
  }

  // The output image is written and visualized directly as a binary image of
  // \code{unsigned chars}.  Figure~\ref{fig:ShapedNeighborhoodExample2}
  // illustrates the results of  dilation on the image
  // \code{Examples/Data/BinaryImage.png}.  Applying erosion and dilation
  // in sequence effects the morphological operations of opening and closing.
  //
  // \begin{figure} \centering
  // \includegraphics[width=0.18\textwidth]{BinaryImage.eps}
  // %\includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1a.eps}
  // \includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1b.eps}
  // %\includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1c.eps}
  // %\includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1d.eps}
  // \itkcaption[Binary image morphology]{The effects of morphological operations
  // on a binary image using a circular structuring element of size 4.
  // Left: original image. Right: dilation.}
  // \protect\label{fig:ShapedNeighborhoodExample2}
  // \end{figure}

  using WriterType = otb::ImageFileWriter<ImageType>;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(output);
  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  return EXIT_SUCCESS;
}
