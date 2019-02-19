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


#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

/* Example usage:
./SIFTDensityExample Input/suburb2.jpeg Output/SIFTDensityOutput.tif Output/PrettySIFTDensityOutput.png 3 3 7
*/


// This example illustrates the use of the
// \doxygen{otb}{KeyPointDensityImageFilter}.
// This filter computes a local density of keypoints (SIFT or SURF,
// for instance) on an image and can
// be useful to detect man made objects or urban areas, for
// instance. The filter has been implemented in a generic way, so that
// the way the keypoints are detected can be chosen by the user.
//
// The first step required to use this filter is to include its header file.

#include "otbKeyPointDensityImageFilter.h"
#include "otbImageToSIFTKeyPointSetFilter.h"

int main(int itkNotUsed(argc), char* argv[])
{
  const char *       infname = argv[1];
  const char *       outfname = argv[2];
  const char *       prettyfname = argv[3];
  const unsigned int scales = atoi(argv[4]);
  const unsigned int octaves = atoi(argv[5]);
  const unsigned int radius = atoi(argv[6]);

  const unsigned int Dimension = 2;
  typedef float PixelType;

  // As usual, we start by defining the types for the images, the reader
  // and the writer.

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType>  WriterType;
  // We define now the type for the keypoint detection. The keypoints
  // will be stored in vector form (they may contain many descriptors)
  // into a point set. The filter for detecting the SIFT is templated
  // over the input image type and the output pointset type.

  typedef itk::VariableLengthVector<PixelType>     RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension> PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType>
  DetectorType;
  // We can now define the filter which will compute the SIFT
  // density. It will be templated over the input and output image
  // types and the SIFT detector.

  typedef otb::KeyPointDensityImageFilter<ImageType, ImageType, DetectorType>
  FilterType;
  // We can instantiate the reader and the writer as wella s the
  // filter and the detector. The detector needs to be instantiated in
  // order to set its parameters.

  ReaderType::Pointer   reader = ReaderType::New();
  WriterType::Pointer   writer = WriterType::New();
  FilterType::Pointer   filter =     FilterType::New();
  DetectorType::Pointer detector = DetectorType::New();
  // We set the file names for the input and the output images.

  reader->SetFileName(infname);
  writer->SetFileName(outfname);
  // We set the parameters for the SIFT detector (the number of
  // octaves and the number of scales per octave).

  detector->SetOctavesNumber(octaves);
  detector->SetScalesNumber(scales);
  // And we pass the detector to the filter and we set the radius for
  // the density estimation.

  filter->SetDetector(detector);
  filter->SetNeighborhoodRadius(radius);
  // We plug the pipeline.

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  // We trigger the execution by calling th \code{Update()} method on
  // the writer, but before that we run the
  // \code{GenerateOutputInformation()} on the reader so the filter
  // gets the information about the image size (needed for the SIFT
  // computation).

  reader->GenerateOutputInformation();
  writer->Update();

  // Figure~\ref{fig:SIFTDENSITY_FILTER} shows the result of applying
  // the key point density filter to an image using the SIFT
  // detector. The main difference with respect to figure
  // \ref{fig:EDGEDENSITY_FILTER} is that for SIFTS, individual trees
  // contribute to the density.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{suburb2.eps}
  // \includegraphics[width=0.25\textwidth]{PrettySIFTDensityOutput.eps}
  // \itkcaption[SIFT Density Filter]{Result of applying the
  // \doxygen{otb}{KeypointDensityImageFilter} to an image. From left
  // to right :
  // original image, SIF density.}
  // \label{fig:SIFTDENSITY_FILTER}
  // \end{figure}

  /************* Image for printing **************/

  typedef otb::Image<unsigned char, 2> OutputImageType;

  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType>
  RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  rescaler->SetInput(filter->GetOutput());

  typedef otb::ImageFileWriter<OutputImageType> OutputWriterType;
  OutputWriterType::Pointer outwriter = OutputWriterType::New();

  outwriter->SetFileName(prettyfname);
  outwriter->SetInput(rescaler->GetOutput());
  outwriter->Update();

  return EXIT_SUCCESS;
}
