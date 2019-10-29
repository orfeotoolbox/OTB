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
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkListSample.h"

#include "otbSparseWvltToAngleMapperListFilter.h"

int otbSparseWvltToAngleMapperListFilterTest(int, char* [])
{
  // number of images to consider
  const unsigned int Dimension     = 2;
  const unsigned int nbInputImages = 2;

  std::string inputImageName[nbInputImages];
  for (unsigned int i = 0; i < nbInputImages; i++)
    inputImageName[i] = "";
  // Parameter (to be changed if necessary)
  const double threshold = 10.;

  // Main type definition
  typedef float  PixelType;
  typedef double PrecisionType;

  typedef itk::FixedArray<PrecisionType, nbInputImages - 1> AngleType;
  typedef itk::Statistics::ListSample<AngleType> AngleListType;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageList<ImageType> ImageListType;

  // Reading input images
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ObjectList<ReaderType>     ReaderListType;
  ReaderListType::Pointer                 reader = ReaderListType::New();
  reader->Resize(nbInputImages);

  typedef otb::ObjectList<ImageListType> ListOfImageListType;
  ListOfImageListType::Pointer           listOfInputImages = ListOfImageListType::New();
  listOfInputImages->Resize(nbInputImages);

  for (unsigned int i = 0; i < nbInputImages; i++)
  {
    reader->SetNthElement(i, ReaderType::New());
    reader->GetNthElement(i)->SetFileName(inputImageName[i]);
    reader->GetNthElement(i)->Update();

    // Here, we will have 'nbInputImages' image lists of one image each only

    listOfInputImages->SetNthElement(i, ImageListType::New());
    listOfInputImages->GetNthElement(i)->PushBack(reader->GetNthElement(i)->GetOutput());
  }

  // Filter
  typedef otb::SparseWvltToAngleMapperListFilter<ImageListType, AngleListType, nbInputImages> AngleListFilterType;

  AngleListFilterType::Pointer filter = AngleListFilterType::New();
  for (unsigned int i = 0; i < nbInputImages; i++)
  {
    filter->SetInput(i, listOfInputImages->GetNthElement(i));
  }
  filter->SetThresholdValue(threshold);

  filter->Update();

  std::cerr << "Number of sample found over " << threshold << " : " << filter->GetOutputSampleList()->Size() << "\n";

  return EXIT_SUCCESS;
}
