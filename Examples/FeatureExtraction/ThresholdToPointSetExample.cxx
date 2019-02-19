/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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
./ThresholdToPointSetExample Input/ROISpot5.png 250 252
*/


// Sometimes, it may be more valuable not to get an image from the threshold
// step but rather a list of coordinates. This can be done with the
// \doxygen{otb}{ThresholdImageToPointSetFilter}.
//
// The following example illustrates the use of the
// \doxygen{otb}{ThresholdImageToPointSetFilter} which provide a list of points
// within given thresholds. Points set are described in section
// \ref{sec:PointSetSection} on page \pageref{sec:PointSetSection}.

#include "otbImage.h"
#include "otbImageFileReader.h"

// The first step required to use this filter is to include the header

#include "otbThresholdImageToPointSetFilter.h"

int main(int argc, char * argv[])
{

  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " lowerThreshold upperThreshold" << std::endl;
    return EXIT_FAILURE;
    }

  //  The next step is to decide which pixel types to use for the input image
  //  and the Point Set as well as their dimension.

  typedef unsigned char PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension>    ImageType;
  typedef itk::PointSet<PixelType, Dimension> PointSetType;

  // A reader is instantiated to read the input image

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  const char * filenamereader = argv[1];
  reader->SetFileName(filenamereader);

  // We get the parameters from the command line for the threshold filter. The
  // lower and upper thresholds parameters are similar to those of the
  // \doxygen{itk}{BinaryThresholdImageFilter} (see Section
  // \ref{sec:BinaryThresholdingImageFilter} on page
  // \pageref{sec:BinaryThresholdingImageFilter} for more information).

  int lowerThreshold = atoi(argv[2]);
  int upperThreshold = atoi(argv[3]);

  //  Then we create the ThresholdImageToPointSetFilter and we pass the
  // parameters.

  typedef otb::ThresholdImageToPointSetFilter
  <ImageType, PointSetType> FilterThresholdType;
  FilterThresholdType::Pointer filterThreshold = FilterThresholdType::New();
  filterThreshold->SetLowerThreshold(lowerThreshold);
  filterThreshold->SetUpperThreshold(upperThreshold);
  filterThreshold->SetInput(0, reader->GetOutput());

  // To manipulate and display the result of this filter, we manually
  // instantiate a point set and we call the \code{Update()} method on the
  // threshold filter to trigger the pipeline execution.
  //
  // After this step, the \code{pointSet} variable contains the point set.

  PointSetType::Pointer pointSet     = PointSetType::New();
  pointSet = filterThreshold->GetOutput();

  filterThreshold->Update();

  // To display each point, we create an iterator on the list of points,
  // which is accessible through the method \code{GetPoints()} of the PointSet.

  typedef PointSetType::PointsContainer ContainerType;
  ContainerType* pointsContainer = pointSet->GetPoints();
  typedef ContainerType::Iterator IteratorType;
  IteratorType itList = pointsContainer->Begin();

  // A while loop enable us to through the list a display the coordinate of
  // each point.

  while (itList != pointsContainer->End())
    {
    std::cout << itList.Value() << std::endl;
    ++itList;
    }

  return EXIT_SUCCESS;
}
