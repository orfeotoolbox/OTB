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


// After having generated a classification map, it is possible to
// regularize such a labeled image in order to obtain more homogeneous
// areas, which facilitates its interpretation. For this
// purpose, the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter} was
// implemented. Like a morphological filter, this filter uses majority
// voting in a ball shaped neighborhood in order to set each pixel of the
// classification map to the most representative label value in its
// neighborhood.
//
// In this example we will illustrate its use. We start by including the
// appropriate header file.

#include "otbNeighborhoodMajorityVotingImageFilter.h"

#include "itkMacro.h"
#include "otbImage.h"
#include <iostream>

#include <otbImageFileReader.h>
#include "otbImageFileWriter.h"


int main(int itkNotUsed(argc), char* argv[])
{
  // Since the input image is a classification map, we will assume a
  // single band input image for which each pixel value is a label coded
  // on 8 bits as an integer between 0 and 255.

  using IOLabelPixelType       = unsigned char; // 8 bits
  const unsigned int Dimension = 2;

  // Thus, both input and output images are single band labeled images,
  // which are composed of the same type of pixels in this example
  // (unsigned char).

  using IOLabelImageType = otb::Image<IOLabelPixelType, Dimension>;


  // We can now define the type for the neighborhood majority voting filter,
  // which is templated over its input and output images types as well as its
  // structuring element type. Choosing only the input image type in the template
  // of this filter induces that, both input and output images types are the same
  // and that the structuring element is a ball
  // (\doxygen{itk}{BinaryBallStructuringElement}).

  // Neighborhood majority voting filter type
  using NeighborhoodMajorityVotingFilterType = otb::NeighborhoodMajorityVotingImageFilter<IOLabelImageType>;


  // Since the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter} is a
  // neighborhood based image filter, it is necessary to set the structuring
  // element which will be used for the majority voting process. By default, the
  // structuring element is a ball
  // (\doxygen{itk}{BinaryBallStructuringElement}) with a radius defined by two sizes
  // (respectively along X and Y). Thus, it is possible to handle anisotropic
  // structuring elements such as ovals.

  // Binary ball Structuring Element type
  using StructuringType = NeighborhoodMajorityVotingFilterType::KernelType;
  using RadiusType      = StructuringType::RadiusType;


  // Finally, we define the reader and the writer.

  using ReaderType = otb::ImageFileReader<IOLabelImageType>;
  using WriterType = otb::ImageFileWriter<IOLabelImageType>;


  const char* inputFileName  = argv[1];
  const char* outputFileName = argv[2];


  // We instantiate the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter} and the
  // reader objects.

  // Neighborhood majority voting filter
  NeighborhoodMajorityVotingFilterType::Pointer NeighMajVotingFilter;
  NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);


  std::string      KeepOriginalLabelBoolStr = argv[3];
  unsigned int     radiusX                  = atoi(argv[4]);
  unsigned int     radiusY                  = atoi(argv[5]);
  IOLabelPixelType noDataValue              = atoi(argv[6]);
  IOLabelPixelType undecidedValue           = atoi(argv[7]);


  // The ball shaped structuring element seBall is instantiated and its
  // two radii along X and Y are initialized.

  StructuringType seBall;
  RadiusType      rad;

  rad[0] = radiusX;
  rad[1] = radiusY;

  seBall.SetRadius(rad);
  seBall.CreateStructuringElement();


  // Then, this ball shaped neighborhood is used as the kernel structuring element
  // for the \doxygen{otb}{NeighborhoodMajorityVotingImageFilter}.

  NeighMajVotingFilter->SetKernel(seBall);

  // Not classified input pixels are assumed to have the noDataValue label
  // and will keep this label in the output image.

  NeighMajVotingFilter->SetLabelForNoDataPixels(noDataValue);


  // Furthermore, since the majority voting regularization may lead to different
  // majority labels in the neighborhood, in this case, it would be important to define
  // the filter's behaviour. For this purpose, a Boolean parameter is used
  // in the filter to choose whether pixels with more than one majority class are set
  // to undecidedValue (true), or to their Original labels (false = default value)
  // in the output image.


  NeighMajVotingFilter->SetLabelForUndecidedPixels(undecidedValue);

  if (KeepOriginalLabelBoolStr.compare("true") == 0)
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(true);
  }
  else
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(false);
  }


  // We plug the pipeline and
  // trigger its execution by updating the output of the writer.


  NeighMajVotingFilter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(NeighMajVotingFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
