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
./SIFTExample Input/ROISpot5.png Output/ROISpot5SIFT0.png Output/SIFT0.txt 2 3 0 0
*/

/* Example usage:
./SIFTExample Input/ROISpot5.png Output/ROISpot5SIFT1.png Output/SIFT1.txt 2 3 1 0
*/

/* Example usage:
./SIFTExample Input/ROISpot5.png Output/ROISpot5SIFT2.png Output/SIFT2.txt 2 3 2 0
*/

/* Example usage:
./SIFTExample Input/QB_Suburb.png Output/QB_SuburbSIFT5.png Output/SIFT2.txt 2 3 5 0
*/

/* Example usage:
./SIFTExample Input/QB_SuburbRotated10.png Output/QB_SuburbSIFT5Rotated10.png Output/SIFT2.txt 2 3 5 0
*/


// This example illustrates the use of the \doxygen{otb}{ImageToSIFTKeyPointSetFilter}.
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

#include "otbImageToSIFTKeyPointSetFilter.h"
#include "otbImage.h"
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
  if (argc != 8)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " InputImage OutputImage OutputSIFTFile octaves scales threshold ratio" << std::endl;
    return 1;
  }
  const char* infname             = argv[1];
  const char* outfname            = argv[3];
  const char* outputImageFilename = argv[2];

  const unsigned int octaves   = atoi(argv[4]);
  const unsigned int scales    = atoi(argv[5]);
  float              threshold = atof(argv[6]);
  float              ratio     = atof(argv[7]);

  using RealType               = float;
  const unsigned int Dimension = 2;

  // The \doxygen{otb}{ImageToSIFTKeyPointSetFilter} is templated over
  // its input image type and the output point set type. Therefore, we
  // start by defining the needed types.
  using ImageType      = otb::Image<RealType, Dimension>;
  using RealVectorType = itk::VariableLengthVector<RealType>;
  using ReaderType     = otb::ImageFileReader<ImageType>;
  using PointSetType   = itk::PointSet<RealVectorType, Dimension>;

  using ImageToSIFTKeyPointSetFilterType = otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType>;

  // Since the SIFT detector produces a point set, we will need
  // iterators for the coordinates of the points and the data associated
  // with them.
  using PointsContainerType = PointSetType::PointsContainer;
  using PointsIteratorType  = PointsContainerType::Iterator;

  // We can now instantiate the reader and the SIFT filter and plug the pipeline.
  ReaderType::Pointer                       reader = ReaderType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer filter = ImageToSIFTKeyPointSetFilterType::New();

  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());

  // The SIFT filter needs the following parameters:
  // \begin{itemize}
  // \item the number of octaves, that is, the number of levels of undersampling,
  // \item the number of scales (blurring) per octave,
  // \item the low contrast threshold to be applied to each point for the detection
  // on the difference of Gaussians image,
  // \item the threshold on the responses to consider a point as an edge.
  // \end{itemize}
  filter->SetOctavesNumber(octaves);
  filter->SetScalesNumber(scales);

  filter->SetDoGThreshold(threshold);
  filter->SetEdgeThreshold(ratio);

  filter->Update();

  // Figure~\ref{fig:SIFT} shows the result of applying the SIFT
  // point detector to a small patch extracted from a Spot 5 image
  // using different threshold values.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.22\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.22\textwidth]{ROISpot5SIFT0.eps}
  // \includegraphics[width=0.22\textwidth]{ROISpot5SIFT1.eps}
  // \includegraphics[width=0.22\textwidth]{ROISpot5SIFT2.eps}
  // \itkcaption[SIFT Application]{Result of applying the
  // \doxygen{otb}{ImageToSIFTKeyPointSetFilter} to a Spot 5
  // image. Left to right: original image and SIFT with thresholds 0,
  // 1 and 2 respectively.}
  // \label{fig:SIFT}
  // \end{figure}
  // Figure~\ref{fig:SIFT2} shows the result of applying the SIFT
  // point detector to a small patch extracted from a Spot 5 image
  // using different threshold values.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.30\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.30\textwidth]{QB_SuburbSIFT5.eps}
  // \includegraphics[width=0.30\textwidth]{QB_SuburbSIFT5Rotated10.eps}
  // \itkcaption[SIFT Application]{Result of applying the
  // \doxygen{otb}{ImageToSIFTKeyPointSetFilter} to a high resolution image
  // image. Left to right: original image and SIFT on the original
  // and a rotated image respectively.}
  // \label{fig:SIFT2}
  // \end{figure}

  //
  // Building the output image for visualization
  ImageType::OffsetType t = {{0, 1}};
  ImageType::OffsetType b = {{0, -1}};
  ImageType::OffsetType r = {{1, 0}};
  ImageType::OffsetType l = {{-1, 0}};

  using RGBPixelType    = itk::RGBPixel<unsigned char>;
  using OutputImageType = otb::Image<RGBPixelType, 2>;

  using WriterType = otb::ImageFileWriter<OutputImageType>;

  OutputImageType::Pointer outputImage = OutputImageType::New();

  OutputImageType::RegionType region;
  OutputImageType::SizeType   outputSize;
  outputSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  region.SetSize(outputSize);

  OutputImageType::IndexType indexStart;
  indexStart[0] = 0;
  indexStart[1] = 0;
  region.SetIndex(indexStart);

  outputImage->SetRegions(region);
  outputImage->Allocate();

  itk::ImageRegionIterator<OutputImageType> iterOutput(outputImage, reader->GetOutput()->GetLargestPossibleRegion());

  for (iterOutput.GoToBegin(); !iterOutput.IsAtEnd(); ++iterOutput)
  {
    ImageType::IndexType       index   = iterOutput.GetIndex();
    ImageType::PixelType       grayPix = reader->GetOutput()->GetPixel(index);
    OutputImageType::PixelType rgbPixel;
    rgbPixel.SetRed(static_cast<unsigned char>(grayPix));
    rgbPixel.SetGreen(static_cast<unsigned char>(grayPix));
    rgbPixel.SetBlue(static_cast<unsigned char>(grayPix));

    iterOutput.Set(rgbPixel);
  }

  PointsIteratorType        pIt     = filter->GetOutput()->GetPoints()->Begin();
  ImageType::SpacingType    spacing = reader->GetOutput()->GetSignedSpacing();
  ImageType::PointType      origin  = reader->GetOutput()->GetOrigin();
  OutputImageType::SizeType size    = outputImage->GetLargestPossibleRegion().GetSize();

  while (pIt != filter->GetOutput()->GetPoints()->End())
  {
    ImageType::IndexType index;

    index[0] = (unsigned int)(std::floor((double)((pIt.Value()[0] - origin[0]) / spacing[0] + 0.5)));

    index[1] = (unsigned int)(std::floor((double)((pIt.Value()[1] - origin[1]) / spacing[1] + 0.5)));

    OutputImageType::PixelType keyPixel;
    keyPixel.SetRed(0);
    keyPixel.SetGreen(255);
    keyPixel.SetBlue(0);

    if (static_cast<unsigned int>(index[1]) < static_cast<unsigned int>(size[1]) && static_cast<unsigned int>(index[0]) < static_cast<unsigned int>(size[0]))
    {
      outputImage->SetPixel(index, keyPixel);

      if (static_cast<unsigned int>(index[1]) < static_cast<unsigned int>(size[1] - 1))
        outputImage->SetPixel(index + t, keyPixel);

      if (index[1] > 0)
        outputImage->SetPixel(index + b, keyPixel);

      if (static_cast<unsigned int>(index[0]) < static_cast<unsigned int>(size[0] - 1))
        outputImage->SetPixel(index + r, keyPixel);

      if (index[0] > 0)
        outputImage->SetPixel(index + l, keyPixel);
    }
    ++pIt;
  }

  std::ofstream outfile(outfname);
  outfile << filter;
  outfile.close();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outputImage);
  writer->SetFileName(outputImageFilename);
  writer->Update();

  return EXIT_SUCCESS;
}
