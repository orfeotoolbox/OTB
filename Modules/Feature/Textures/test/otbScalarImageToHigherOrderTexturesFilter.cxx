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

#include "otbScalarImageToHigherOrderTexturesFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkExtractImageFilter.h"
#include "otbMacro.h"

const unsigned int Dimension = 2;
typedef float                            PixelType;
typedef otb::Image<PixelType, Dimension> ImageType;
typedef otb::ScalarImageToHigherOrderTexturesFilter
      <ImageType, ImageType>             TexturesFilterType;
typedef otb::ImageFileReader<ImageType>  ReaderType;
typedef ImageType::IndexType             IndexType;
typedef ImageType::IndexValueType        IndexValueType;
typedef itk::Statistics::ScalarImageToRunLengthFeaturesFilter<ImageType> RunLengthFeaturesFilterType;
typedef itk::ExtractImageFilter<ImageType,ImageType> ExtractImageFilterType;

typedef RunLengthFeaturesFilterType::OffsetType          OffsetType;
typedef RunLengthFeaturesFilterType::OffsetVector        OffsetVector;
typedef RunLengthFeaturesFilterType::OffsetVectorPointer OffsetVectorPointer;


ImageType::Pointer ReadInputImage(const char *  filename)
{
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->Update();
  ImageType::Pointer image = reader->GetOutput();
  image->DisconnectPipeline();
  return image;
}

std::vector<ImageType::Pointer> Compute( ImageType* inputImage, unsigned int nbBins, unsigned int radius, OffsetVector* offsetVector )
{

  TexturesFilterType::Pointer textureFilter = TexturesFilterType::New();

  TexturesFilterType::SizeType sradius;
  sradius.Fill(radius);


  textureFilter->SetInput(inputImage);
  textureFilter->SetNumberOfBinsPerAxis(nbBins);
  textureFilter->SetRadius(sradius);

  if ( offsetVector )
  {
    textureFilter->SetOffsets(offsetVector);
  }
  textureFilter->SetInputImageMinimum(0);
  textureFilter->SetInputImageMaximum(256);

  textureFilter->Update();

  std::vector<ImageType::Pointer> outputs;
  for (int i = 0; i < 10; ++i)
  {
    ImageType::Pointer output = textureFilter->GetOutput(i);
    output->DisconnectPipeline();
    outputs.push_back(output);
  }
  return outputs;
}

int ValidateAt( ImageType* inputImage, std::vector<ImageType::Pointer> computeResults, unsigned int nbBins, unsigned int radius, OffsetVector* offsetVector, IndexValueType x, IndexValueType y )
{

  ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  ImageType::RegionType region;
  region.SetIndex(0,x - radius);
  region.SetIndex(1,y - radius);
  region.SetSize(0,2 * radius + 1);
  region.SetSize(1,2 * radius + 1);
  region.Crop(inputImage->GetLargestPossibleRegion());
  extract->SetExtractionRegion(region);
  extract->SetInput(inputImage);

  RunLengthFeaturesFilterType::Pointer itkFilter = RunLengthFeaturesFilterType::New();
  itkFilter->SetInput(extract->GetOutput());
  itkFilter->SetNumberOfBinsPerAxis(nbBins);
  if ( offsetVector )
  {
    itkFilter->SetOffsets(offsetVector);
  }
  itkFilter->SetPixelValueMinMax(0, 256);

  ImageType::PointType  topLeftPoint;
  ImageType::PointType  bottomRightPoint;
  ImageType::IndexType  topLeftIndex = region.GetIndex();
  ImageType::IndexType  bottomRightIndex = region.GetIndex();
  bottomRightIndex[0] +=  2 * radius;
  bottomRightIndex[1] +=  2 * radius;
  inputImage->TransformIndexToPhysicalPoint(topLeftIndex, topLeftPoint );
  inputImage->TransformIndexToPhysicalPoint(bottomRightIndex, bottomRightPoint );
  double maxDistance = topLeftPoint.EuclideanDistanceTo(bottomRightPoint);

  itkFilter->SetDistanceValueMinMax(0, maxDistance );
  itkFilter->Update();

  RunLengthFeaturesFilterType::FeatureValueVector&
     featuresMeans = *(itkFilter->GetFeatureMeans().GetPointer());

  for (int i = 0; i < 10; ++i)
  {
    ImageType::IndexType idx;
    idx[0] = x;
    idx[1] = y;

    PixelType output = computeResults[i]->GetPixel(idx);
    PixelType reference = featuresMeans[i];

    otbLogMacro(Debug, << "index = " << idx
      << " ; compute = " << output
      << " ; ref = " << reference);

    if (reference != output)
    {
      std::cerr << "Error at index " << idx << ", feature #" << i << ", got " << output << " and ref is " << reference << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}


int otbScalarImageToHigherOrderTexturesFilter(int argc, char * argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0] << " infname" << std::endl;
    return EXIT_FAILURE;
    }
  const char *       infname      = argv[1];
  const unsigned int nbBins = 8;

  unsigned int radius;
  OffsetVectorPointer offsetVector;

  // radius = 5, offset = [1,1]
  {
    radius = 5;

    OffsetType offset;
    offset[0] = 1;
    offset[1] = 1;

    offsetVector = OffsetVector::New();
    offsetVector->push_back(offset);

    std::cout << "Testing radius = " << radius << " and offset = " << offset << std::endl;

    ImageType::Pointer inputImage = ReadInputImage(infname);
    std::vector<ImageType::Pointer> results = Compute(inputImage, nbBins, radius, offsetVector);

    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 5, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 7, 11) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 10, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
  }

  // radius = 7, offset = [1,2]
  {
    radius = 7;

    OffsetType offset;
    offset[0] = 1;
    offset[1] = 2;

    offsetVector = OffsetVector::New();
    offsetVector->push_back(offset);

    std::cout << "Testing radius = " << radius << " and offset = " << offset << std::endl;

    ImageType::Pointer inputImage = ReadInputImage(infname);
    std::vector<ImageType::Pointer> results = Compute(inputImage, nbBins, radius, offsetVector);

    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 7, 11) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 5, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 7, 11) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 10, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
  }

  // radius = 3, offset = defaults (average of all directions)
  {
    radius = 3;

    std::cout << "Testing radius = " << radius << " and default offsets" << std::endl;

    ImageType::Pointer inputImage = ReadInputImage(infname);
    std::vector<ImageType::Pointer> results = Compute(inputImage, nbBins, radius, nullptr);

    if ( ValidateAt(inputImage, results, nbBins, radius, nullptr, 7, 11) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, nullptr, 5, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, nullptr, 7, 11) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, nullptr, 10, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
  }

  // radius = 7, offset = [0,-1]
  {
    radius = 7;

    OffsetType offset;
    offset[0] = 1;
    offset[1] = -1;

    offsetVector = OffsetVector::New();
    offsetVector->push_back(offset);

    std::cout << "Testing radius = " << radius << " and offset = " << offset << std::endl;

    ImageType::Pointer inputImage = ReadInputImage(infname);
    std::vector<ImageType::Pointer> results = Compute(inputImage, nbBins, radius, offsetVector);

    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 7, 11) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 5, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 7, 11) == EXIT_FAILURE )
      return EXIT_FAILURE;
    if ( ValidateAt(inputImage, results, nbBins, radius, offsetVector, 10, 5) == EXIT_FAILURE )
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
