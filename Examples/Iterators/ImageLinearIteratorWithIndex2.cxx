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


// This example shows how to use the \doxygen{itk}{ImageLinearIteratorWithIndex} for
// computing the mean across time of a 4D image where the first three
// dimensions correspond to spatial coordinates and the fourth dimension
// corresponds to time. The result of the mean across time is to be stored in a
// 3D image.
//
// \index{Iterators!and 4D images}
// \index{ImageLinearIteratorWithIndex!4D images}

#include "otbImage.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  // Verify the number of parameters on the command line.
  if (argc < 3)
  {
    std::cerr << "Missing parameters. " << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " input4DImageFile output3DImageFile" << std::endl;
    return EXIT_FAILURE;
  }

  // First we declare the types of the images

  using PixelType   = unsigned char;
  using Image3DType = otb::Image<PixelType, 3>;
  using Image4DType = otb::Image<PixelType, 4>;

  using Reader4DType = otb::ImageFileReader<Image4DType>;
  using Writer3DType = otb::ImageFileWriter<Image3DType>;

  Reader4DType::Pointer reader4D = Reader4DType::New();
  reader4D->SetFileName(argv[1]);

  try
  {
    reader4D->Update();
  }
  catch (itk::ExceptionObject& excp)
  {
    std::cerr << "Error writing the image" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
  }

  Image4DType::ConstPointer image4D = reader4D->GetOutput();

  Image3DType::Pointer image3D = Image3DType::New();
  using Index3DType            = Image3DType::IndexType;
  using Size3DType             = Image3DType::SizeType;
  using Region3DType           = Image3DType::RegionType;
  using Spacing3DType          = Image3DType::SpacingType;
  using Origin3DType           = Image3DType::PointType;

  using Index4DType   = Image4DType::IndexType;
  using Size4DType    = Image4DType::SizeType;
  using Spacing4DType = Image4DType::SpacingType;
  using Origin4DType  = Image4DType::PointType;

  Index3DType   index3D;
  Size3DType    size3D;
  Spacing3DType spacing3D;
  Origin3DType  origin3D;

  Image4DType::RegionType region4D = image4D->GetBufferedRegion();

  Index4DType   index4D   = region4D.GetIndex();
  Size4DType    size4D    = region4D.GetSize();
  Spacing4DType spacing4D = image4D->GetSignedSpacing();
  Origin4DType  origin4D  = image4D->GetOrigin();

  for (unsigned int i = 0; i < 3; ++i)
  {
    size3D[i]    = size4D[i];
    index3D[i]   = index4D[i];
    spacing3D[i] = spacing4D[i];
    origin3D[i]  = origin4D[i];
  }

  image3D->SetSignedSpacing(spacing3D);
  image3D->SetOrigin(origin3D);

  Region3DType region3D;
  region3D.SetIndex(index3D);
  region3D.SetSize(size3D);

  image3D->SetRegions(region3D);
  image3D->Allocate();

  using SumType  = itk::NumericTraits<PixelType>::AccumulateType;
  using MeanType = itk::NumericTraits<SumType>::RealType;

  const unsigned int timeLength = region4D.GetSize()[3];

  using IteratorType = itk::ImageLinearConstIteratorWithIndex<Image4DType>;

  IteratorType it(image4D, region4D);
  it.SetDirection(3); // Walk along time dimension
  it.GoToBegin();
  while (!it.IsAtEnd())
  {
    SumType sum = itk::NumericTraits<SumType>::Zero;
    it.GoToBeginOfLine();
    index4D = it.GetIndex();
    while (!it.IsAtEndOfLine())
    {
      sum += it.Get();
      ++it;
    }
    MeanType mean = static_cast<MeanType>(sum) / static_cast<MeanType>(timeLength);

    index3D[0] = index4D[0];
    index3D[1] = index4D[1];
    index3D[2] = index4D[2];

    image3D->SetPixel(index3D, static_cast<PixelType>(mean));
    it.NextLine();
  }

  // As you can see, we avoid to use a 3D iterator to walk
  // over the mean image. The reason is that there is no
  // guarantee that the 3D iterator will walk in the same
  // order as the 4D. Iterators just adhere to their contract
  // of visiting all the pixel, but do not enforce any particular
  // order for the visits.  The linear iterator guarantees to
  // visit the pixels along a line of the image in the order
  // in which they are placed in the line, but do not states
  // in what order one line will be visited with respect to
  // other lines.  Here we simply take advantage of knowing
  // the first three components of the 4D iterator index,
  // and use them to place the resulting mean value in the
  // output 3D image.

  Writer3DType::Pointer writer3D = Writer3DType::New();
  writer3D->SetFileName(argv[2]);
  writer3D->SetInput(image3D);

  try
  {
    writer3D->Update();
  }
  catch (itk::ExceptionObject& excp)
  {
    std::cerr << "Error writing the image" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
