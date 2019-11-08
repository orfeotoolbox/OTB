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

#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToSingleImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBandMathImageFilter.h"


int otbImageListToSingleImageFilter(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;
  char*              infname   = argv[1];
  char*              outfname1 = argv[2];

  typedef float PixelType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::ImageList<ImageType> ImageListType;

  // IO Typedef
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  // Filter Typedef
  typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType> VectorImageToImageListFilterType;
  typedef otb::ImageListToSingleImageFilter<ImageListType::ImageType> ImageListToSingleImageFilter;
  typedef otb::BandMathImageFilter<ImageType>                         BandMathImageFilterType;

  // Instantiating objects
  VectorImageToImageListFilterType::Pointer filter1   = VectorImageToImageListFilterType::New();
  ImageListToSingleImageFilter::Pointer     filter2_1 = ImageListToSingleImageFilter::New();
  ImageListToSingleImageFilter::Pointer     filter2_2 = ImageListToSingleImageFilter::New();
  BandMathImageFilterType::Pointer          filter3   = BandMathImageFilterType::New();
  ReaderType::Pointer                       reader    = ReaderType::New();
  WriterType::Pointer                       writer    = WriterType::New();

  // Reader
  reader->SetFileName(infname);

  // VectorImage to Image List
  filter1->SetInput(reader->GetOutput());
  filter1->UpdateOutputInformation();

  // Retrieve one image from the image list
  filter2_1->SetExtractedImagePosition(1);
  filter2_1->SetInput(filter1->GetOutput());

  // Retrieve one image from the image list
  filter2_2->SetExtractedImagePosition(2);
  filter2_2->SetInput(filter1->GetOutput());

  // Use an BandMath with the selected image
  filter3->SetNthInput(0, filter2_1->GetOutput());
  filter3->SetNthInput(1, filter2_2->GetOutput());

#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
  filter3->SetExpression(" ( b2 > 1.0 ) ? b1/b2 : 0");
#else
  filter3->SetExpression(" if( b2 > 1.0, b1/b2, 0)");
#endif

  // Write the result
  writer->SetFileName(outfname1);
  writer->SetNumberOfDivisionsTiledStreaming(8);
  writer->SetInput(filter3->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
