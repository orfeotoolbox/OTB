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
./GenerateTrainingImageExample Input/ROI_QB_MUL_4.tif Input/LearningROIs.txt Output/ROI_QB_MUL_4_training.png
*/


// This example illustrates how to generate a training image for an
// image supervised classification.

#include "itkNumericTraits.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "itkImageRegionIterator.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc != 4)
  {

    std::cout << "Usage : " << argv[0] << " inputImage roiFile outputTrainingImage" << std::endl;
    return EXIT_FAILURE;
  }

  const char* imageFilename  = argv[1];
  const char* roiFilename    = argv[2];
  const char* outputFilename = argv[3];

  using InputPixelType  = unsigned char;
  using OutputPixelType = unsigned char;

  const unsigned int Dimension = 2;

  using InputImageType  = otb::Image<InputPixelType, Dimension>;
  using OutputImageType = otb::Image<OutputPixelType, Dimension>;

  using ReaderType = otb::ImageFileReader<InputImageType>;
  using WriterType = otb::ImageFileWriter<OutputImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(imageFilename);
  writer->SetFileName(outputFilename);

  reader->Update();

  // We allocate the output image
  OutputImageType::Pointer trainingImage = OutputImageType::New();

  // Declare the type of the index to access images
  using myIndexType = itk::Index<Dimension>;

  // Declare the type of the size
  using mySizeType = itk::Size<Dimension>;

  // Declare the type of the Region
  using myRegionType = itk::ImageRegion<Dimension>;

  // Define their size, and start index
  mySizeType size;
  size[0] = reader->GetOutput()->GetRequestedRegion().GetSize()[0];
  size[1] = reader->GetOutput()->GetRequestedRegion().GetSize()[1];

  myIndexType start;
  start[0] = 0;
  start[1] = 0;

  myRegionType region;
  region.SetIndex(start);
  region.SetSize(size);

  // Initialize Image B
  trainingImage->SetLargestPossibleRegion(region);
  trainingImage->SetBufferedRegion(region);
  trainingImage->SetRequestedRegion(region);
  trainingImage->Allocate();
  OutputPixelType pix;
  pix = itk::NumericTraits<OutputPixelType>::Zero;
  trainingImage->FillBuffer(pix);

  // For each line of the ROIs file, we create a region iterator and
  // assign the value of the labels to the pixels

  std::ifstream roisFile;
  roisFile.open(roiFilename);

  unsigned int nbRois = 0;

  roisFile >> nbRois;

  std::cout << "Nb of ROIS " << int(nbRois) << std::endl;

  while (!roisFile.fail() && (nbRois > 0))
  {

    --nbRois;
    OutputPixelType label = 0;
    unsigned long   xUL, yUL, xBR, yBR, tmp_label = 0;

    roisFile >> tmp_label;
    roisFile >> xUL;
    roisFile >> yUL;
    roisFile >> xBR;
    roisFile >> yBR;

    label = static_cast<OutputPixelType>(tmp_label);

    std::cout << "Label : " << int(label) << std::endl;
    std::cout << "( " << xUL << " , " << yUL << " )" << std::endl;
    std::cout << "( " << xBR << " , " << yBR << " )" << std::endl;

    // Region creation

    start[0] = xUL;
    start[1] = yUL;

    size[0] = xBR - xUL;
    size[1] = yBR - yUL;

    region.SetSize(size);
    region.SetIndex(start);

    // Iterator creation
    using IteratorType = itk::ImageRegionIterator<OutputImageType>;
    IteratorType it(trainingImage, region);

    it.GoToBegin();

    // Iteration and pixel value assignment
    while (!it.IsAtEnd())
    {

      it.Set(static_cast<OutputPixelType>(label));

      // std::cout << (int)static_cast<OutputPixelType>(label) << " -- ";
      // std::cout << (int)it.Get() << std::endl;
      ++it;
    }
  }

  writer->SetInput(trainingImage);

  writer->Update();

  return EXIT_SUCCESS;
}
