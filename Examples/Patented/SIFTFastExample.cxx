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

/* Example usage:
./SIFTFastExample Input/ROISpot5.png Output/ROISpot5SIFTFast.png 3
*/


// This example illustrates the use of the \doxygen{otb}{SiftFastImageFilter}.
// The Scale-Invariant Feature Transform (or SIFT) is an algorithm in
// computer vision to detect and describe local features in
// images. The algorithm was published by David Lowe
// \cite{LoweSIFT}. The detection and description of local image
// features can help in object recognition and image registration. The
// SIFT features are local and based on the appearance of the object
// at particular interest points, and are invariant to image scale and
// rotation. They are also robust to changes in illumination, noise,
// occlusion and minor changes in viewpoint.
//
// The first step required to use this filter is to include its header file.

#include "otbSiftFastImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " InputImage OutputImage scales" << std::endl;
    return 1;
  }
  const char* infname             = argv[1];
  const char* outputImageFilename = argv[2];

  const unsigned int scales = atoi(argv[3]);

  const unsigned int Dimension = 2;

  // We will start by defining the required types. We will work with a
  // scalar image of float pixels. We also define the corresponding
  // image reader.

  using RealType   = float;
  using ImageType  = otb::Image<RealType, Dimension>;
  using ReaderType = otb::ImageFileReader<ImageType>;
  // The SIFT descriptors will be stored in a point set containing the
  // vector of features.

  using RealVectorType = itk::VariableLengthVector<RealType>;
  using PointSetType   = itk::PointSet<RealVectorType, Dimension>;
  // The SIFT filter itself is templated over the input image and the
  // generated point set.

  using ImageToFastSIFTKeyPointSetFilterType = otb::SiftFastImageFilter<ImageType, PointSetType>;
  // We instantiate the reader.

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  // We instantiate the filter.

  ImageToFastSIFTKeyPointSetFilterType::Pointer filter = ImageToFastSIFTKeyPointSetFilterType::New();
  // We plug the filter and set the number of scales for the SIFT
  // computation. We can afterwards run the processing with the
  // \code{Update()} method.

  filter->SetInput(reader->GetOutput());
  filter->SetScalesNumber(scales);
  filter->Update();
  // Once the SIFT are computed, we may want to draw them on top of the
  // input image. In order to do this, we will create the following RGB
  // image and the corresponding writer:

  using PixelType       = unsigned char;
  using RGBPixelType    = itk::RGBPixel<PixelType>;
  using OutputImageType = otb::Image<RGBPixelType, 2>;
  using WriterType      = otb::ImageFileWriter<OutputImageType>;

  OutputImageType::Pointer outputImage = OutputImageType::New();
  // We set the regions of the image by copying the information from the
  // input image and we allocate the memory for the output image.

  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
  // We can now proceed to copy the input image into the output one
  // using region iterators. The input image is a grey level one. The
  // output image will be made of color crosses for each SIFT on top of
  // the grey level input image. So we start by copying the grey level
  // values on each of the 3 channels of the color image.

  itk::ImageRegionIterator<OutputImageType> iterOutput(outputImage, outputImage->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType>       iterInput(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  for (iterOutput.GoToBegin(), iterInput.GoToBegin(); !iterOutput.IsAtEnd(); ++iterOutput, ++iterInput)
  {
    OutputImageType::PixelType rgbPixel;
    rgbPixel.SetRed(static_cast<PixelType>(iterInput.Get()));
    rgbPixel.SetGreen(static_cast<PixelType>(iterInput.Get()));
    rgbPixel.SetBlue(static_cast<PixelType>(iterInput.Get()));

    iterOutput.Set(rgbPixel);
  }
  // We are now going to plot color crosses on the output image. We will
  // need to define offsets (top, bottom, left and right) with respect
  // to the SIFT position in order to draw the cross segments.

  ImageType::OffsetType t = {{0, 1}};
  ImageType::OffsetType b = {{0, -1}};
  ImageType::OffsetType l = {{1, 0}};
  ImageType::OffsetType r = {{-1, 0}};
  // Now, we are going to access the point set generated by the SIFT
  // filter. The points are stored into a points container that we are
  // going to walk through using an iterator. These are the types needed
  // for this task:

  using PointsContainerType = PointSetType::PointsContainer;
  using PointsIteratorType  = PointsContainerType::Iterator;
  // We set the iterator to the beginning of the point set.

  PointsIteratorType pIt = filter->GetOutput()->GetPoints()->Begin();
  // We get the information about image size and spacing before drawing
  // the crosses.

  ImageType::SpacingType spacing = reader->GetOutput()->GetSignedSpacing();
  ImageType::PointType   origin  = reader->GetOutput()->GetOrigin();

  // And we iterate through the SIFT set:

  while (pIt != filter->GetOutput()->GetPoints()->End())
  {
    // We get the pixel coordinates for each SIFT by using the
    // \code{Value()} method on the point set iterator. We use the
    // information about size and spacing in order to convert the physical
    // coordinates of the point into pixel coordinates.

    ImageType::IndexType index;

    index[0] = static_cast<unsigned int>(std::floor(static_cast<double>((pIt.Value()[0] - origin[0]) / spacing[0] + 0.5)));

    index[1] = static_cast<unsigned int>(std::floor(static_cast<double>((pIt.Value()[1] - origin[1]) / spacing[1] + 0.5)));
    // We create a green pixel.

    OutputImageType::PixelType keyPixel;
    keyPixel.SetRed(0);
    keyPixel.SetGreen(255);
    keyPixel.SetBlue(0);
    // We draw the crosses using the offsets and checking that we are
    // inside the image, since SIFTs on the image borders would cause an
    // out of bounds pixel access.

    if (outputImage->GetLargestPossibleRegion().IsInside(index))
    {
      outputImage->SetPixel(index, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + t))
        outputImage->SetPixel(index + t, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + b))
        outputImage->SetPixel(index + b, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + l))
        outputImage->SetPixel(index + l, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + r))
        outputImage->SetPixel(index + r, keyPixel);
    }
    ++pIt;
  }

  // Finally, we write the image.

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageFilename);
  writer->SetInput(outputImage);
  writer->Update();

  // Figure~\ref{fig:SIFTFast} shows the result of applying the SIFT
  // point detector to a small patch extracted from a Spot 5 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.40\textwidth]{ROISpot5SIFTFast.eps}
  // \itkcaption[SIFT Application]{Result of applying the
  // \doxygen{otb}{SiftFastImageFilter} to a Spot 5
  // image.}
  // \label{fig:SIFTFast}
  // \end{figure}

  return EXIT_SUCCESS;
}
