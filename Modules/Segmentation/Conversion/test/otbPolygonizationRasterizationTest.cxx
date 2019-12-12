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

#include "otbImage.h"
#include "otbImageFileReader.h"

#include "otbVectorDataFileReader.h"

#include "otbLabelImageToVectorDataFilter.h"
#include "otbVectorDataToLabelImageFilter.h"
#include "otbStandardOneLineFilterWatcher.h"

#include "itkNumericTraits.h"
#include "itkImageRegionConstIteratorWithIndex.h"

int otbPolygonizationRasterizationTest(int itkNotUsed(argc), char* argv[])
{
  typedef unsigned int PixelType;
  typedef otb::Image<PixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::VectorData<> VectorDataType;

  typedef otb::LabelImageToVectorDataFilter<ImageType> LabelImageToVDFilterType;
  typedef otb::VectorDataToLabelImageFilter<VectorDataType, ImageType> RasterizationFilterType;

  // Read the label image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  // Instantiate  a LabelImageToVectorData filter
  LabelImageToVDFilterType::Pointer polygonize = LabelImageToVDFilterType::New();
  polygonize->SetInput(reader->GetOutput());
  polygonize->Update();

  // rasterize
  RasterizationFilterType::Pointer rasterization = RasterizationFilterType::New();
  rasterization->AddVectorData(polygonize->GetOutput());
  rasterization->SetOutputParametersFromImage(reader->GetOutput());
  rasterization->SetBurnAttribute("DN");
  rasterization->Update();

  // Compare the input label image and the output of the rasterization
  // filter, they must be exactly similar
  itk::ImageRegionConstIteratorWithIndex<ImageType> itRef(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIteratorWithIndex<ImageType> itTest(rasterization->GetOutput(), rasterization->GetOutput()->GetLargestPossibleRegion());

  for (itRef.GoToBegin(), itTest.GoToBegin(); !itRef.IsAtEnd() && !itTest.IsAtEnd(); ++itRef, ++itTest)
  {
    if (itRef.Get() != itTest.Get())
    {
      std::cerr << "Pixel at position " << itRef.GetIndex() << " differs : in=" << itRef.Get() << " while out=" << itTest.Get() << std::endl;
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
