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
./IndexedToRGBExample Input/buildingExtractionIndexed.tif Output/buildingExtractionRGB.png Output/buildingExtractionIndexed_scaled.png
*/


//  Some algorithms produce an indexed image as output. In such images,
// each pixel is given a value according to the region number it belongs to.
// This value starting at 0 or 1 is usually an integer value.
// Often, such images are produced by segmentation or classification algorithms.
//
// If such regions are easy to manipulate -- it is easier and faster to compare two integers
// than a RGB value -- it is different when it comes to displaying the results.
//
// Here we present a convient way to convert such indexed image to a color image. In
// such conversion, it is important to ensure that neighborhood region, which are
// likely to have consecutive number have easily dicernable colors. This is done
// randomly using a hash function by the \doxygen{itk}{ScalarToRGBPixelFunctor}.

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"

#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <inputImageFile> ";
    std::cerr << " <outputRGBImageFile> <outputScaledImageFile>" << std::endl;
    return EXIT_FAILURE;
  }
  const char* inputFilename        = argv[1];
  const char* outputRGBFilename    = argv[2];
  const char* outputScaledFilename = argv[3];

  typedef otb::Image<unsigned long, 2>                ImageType;
  typedef otb::Image<itk::RGBPixel<unsigned char>, 2> RGBImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer                     reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  //  The \doxygen{itk}{UnaryFunctorImageFilter} is the filter in charge of
  // calling the functor we specify to do the work for each pixel. Here it is the
  // \doxygen{itk}{ScalarToRGBPixelFunctor}.

  typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>                       ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<ImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer                                                        colormapper = ColorMapFilterType::New();

  colormapper->SetInput(reader->GetOutput());

  typedef otb::ImageFileWriter<RGBImageType> WriterType;
  WriterType::Pointer                        writer = WriterType::New();
  writer->SetFileName(outputRGBFilename);
  writer->SetInput(colormapper->GetOutput());

  writer->Update();

  // The following is just to produce the input image for the software guide
  typedef otb::Image<unsigned char, 2>                                 OutputImageType;
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> RescalerType;
  RescalerType::Pointer                                                rescaler = RescalerType::New();
  rescaler->SetInput(reader->GetOutput());

  typedef otb::ImageFileWriter<OutputImageType> UCharWriterType;
  UCharWriterType::Pointer                      writer2 = UCharWriterType::New();
  writer2->SetFileName(outputScaledFilename);
  writer2->SetInput(rescaler->GetOutput());
  writer2->Update();

  // Figure~\ref{fig:INDEXTORGB_FILTER} shows the result of the conversion
  // from an indexed image to a color image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{buildingExtractionIndexed_scaled.eps}
  // \includegraphics[width=0.44\textwidth]{buildingExtractionRGB.eps}
  // \itkcaption[Scaling images]{The original indexed image (left) and the
  // conversion to color image.}
  // \label{fig:INDEXTORGB_FILTER}
  // \end{figure}

  return EXIT_SUCCESS;
}
