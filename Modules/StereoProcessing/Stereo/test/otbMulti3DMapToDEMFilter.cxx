/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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


#include "otbMulti3DMapToDEMFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"
#include <string>
#include "otbSpatialReference.h"

typedef otb::Image<double, 2> ImageType;

typedef otb::VectorImage<double, 2> VectorImageType;

typedef otb::Multi3DMapToDEMFilter<VectorImageType, ImageType, ImageType> Multi3DFilterType;


int otbMulti3DMapToDEMFilterEPSG(int argc, char* argv[])
{


  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::ImageFileReader<VectorImageType> ReaderVectorType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  typedef otb::ObjectList<ReaderType>           MaskReaderListType;
  typedef otb::ObjectList<ReaderVectorType>     MapReaderListType;


  if ((argc - 15) % 2 != 0)
  {
    std::cout << "Usage: " << argv[0] << " 3DMapImage1 .mask1... 3DMapImageN maskN  DEMoutput DEMGridStep FusionMode ThreadNb StreamNb  <x start index> <y "
                                         "start index> <x origin> <y origin> <x size> <y size> <x spacing> <y spacing> <epsg code>"
              << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int mapSize = (argc - 15) / 2;


  MapReaderListType::Pointer  mapReaderList  = MapReaderListType::New();
  MaskReaderListType::Pointer maskReaderList = MaskReaderListType::New();
  for (unsigned int i = 0; i < (mapSize); i++)
  {
    mapReaderList->PushBack(ReaderVectorType::New());
    maskReaderList->PushBack(ReaderType::New());
  }


  // unsigned int i=1;
  unsigned int index = 0;
  for (unsigned int i = 1; i < (2 * mapSize + 1); i = i + 2)
  {
    mapReaderList->GetNthElement(index)->SetFileName(argv[i]);
    mapReaderList->GetNthElement(index)->UpdateOutputInformation();

    maskReaderList->GetNthElement(index)->SetFileName(argv[i + 1]);
    maskReaderList->GetNthElement(index)->UpdateOutputInformation();
    index++;
  }

  float gridStep = atof(argv[argc - 13]);

  std::cout << "gridStep " << gridStep << std::endl;

  int fusionMode = atof(argv[argc - 12]);

  std::cout << "fusionMode " << fusionMode << std::endl;


  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
  multiFilter->SetNumberOf3DMaps(mapSize);
  multiFilter->SetDEMGridStep(gridStep);
  multiFilter->SetCellFusionMode(fusionMode);

  for (unsigned int i = 0; i < mapSize; i++)
  {
    multiFilter->Set3DMapInput(i, mapReaderList->GetNthElement(i)->GetOutput());
    multiFilter->SetMaskInput(i, maskReaderList->GetNthElement(i)->GetOutput());
  }

  // set output parameters
  VectorImageType::IndexType start;
  start[0] = atoi(argv[argc - 9]);
  start[1] = atoi(argv[argc - 8]);
  multiFilter->SetOutputStartIndex(start);

  VectorImageType::SizeType size;
  size[0] = atoi(argv[argc - 5]); // X size
  size[1] = atoi(argv[argc - 4]); // Y size
  multiFilter->SetOutputSize(size);

  VectorImageType::SpacingType spacing;
  spacing[0] = atof(argv[argc - 3]);
  spacing[1] = atof(argv[argc - 2]);
  multiFilter->SetOutputSpacing(spacing);

  VectorImageType::PointType origin;
  origin[0] = strtod(argv[argc - 7], nullptr);
  origin[1] = strtod(argv[argc - 6], nullptr);
  multiFilter->SetOutputOrigin(origin);

  std::string projectionRef = otb::SpatialReference::FromEPSG(atoi(argv[argc - 1])).ToWkt();

  std::cout << "projection Reference :" << std::endl << projectionRef << std::endl;

  multiFilter->SetProjectionRef(projectionRef);
  WriterType::Pointer writer = WriterType::New();

  multiFilter->SetNumberOfThreads(atoi(argv[argc - 11]));

  writer->SetInput(multiFilter->GetOutput());

  writer->SetFileName(argv[argc - 14]);
  writer->SetNumberOfDivisionsStrippedStreaming(atoi(argv[argc - 10]));
  writer->Update();

  return EXIT_SUCCESS;
}

int otbMulti3DMapToDEMFilterManual(int argc, char* argv[])
{


  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::ImageFileReader<VectorImageType> ReaderVectorType;


  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::ObjectList<ReaderType>       MaskReaderListType;
  typedef otb::ObjectList<ReaderVectorType> MapReaderListType;


  if ((argc - 14) % 2 != 0)
  {
    std::cout << "Usage: " << argv[0] << " 3DMapImage1 .mask1... 3DMapImageN maskN  DEMoutput DEMGridStep FusionMode ThreadNb StreamNb  <x start index> <y "
                                         "start index> <x origin> <y origin> <x size> <y size> <x spacing> <y spacing>"
              << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int mapSize = (argc - 14) / 2;


  MapReaderListType::Pointer  mapReaderList  = MapReaderListType::New();
  MaskReaderListType::Pointer maskReaderList = MaskReaderListType::New();
  for (unsigned int i = 0; i < (mapSize); i++)
  {
    mapReaderList->PushBack(ReaderVectorType::New());
    maskReaderList->PushBack(ReaderType::New());
  }


  // unsigned int i=1;
  unsigned int index = 0;
  for (unsigned int i = 1; i < (2 * mapSize + 1); i = i + 2)
  {
    mapReaderList->GetNthElement(index)->SetFileName(argv[i]);
    mapReaderList->GetNthElement(index)->UpdateOutputInformation();

    maskReaderList->GetNthElement(index)->SetFileName(argv[i + 1]);
    maskReaderList->GetNthElement(index)->UpdateOutputInformation();
    index++;
  }

  float gridStep = atof(argv[argc - 12]);

  std::cout << "gridStep " << gridStep << std::endl;

  int fusionMode = atof(argv[argc - 11]);

  std::cout << "fusionMode " << fusionMode << std::endl;


  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
  multiFilter->SetNumberOf3DMaps(mapSize);
  multiFilter->SetDEMGridStep(gridStep);
  multiFilter->SetCellFusionMode(fusionMode);

  for (unsigned int i = 0; i < mapSize; i++)
  {
    multiFilter->Set3DMapInput(i, mapReaderList->GetNthElement(i)->GetOutput());
    multiFilter->SetMaskInput(i, maskReaderList->GetNthElement(i)->GetOutput());
  }

  // set output parameters
  VectorImageType::IndexType start;
  start[0] = atoi(argv[argc - 7]);
  start[1] = atoi(argv[argc - 8]);
  multiFilter->SetOutputStartIndex(start);

  VectorImageType::SizeType size;
  size[0] = atoi(argv[argc - 4]); // X size
  size[1] = atoi(argv[argc - 3]); // Y size
  multiFilter->SetOutputSize(size);

  VectorImageType::SpacingType spacing;
  spacing[0] = atof(argv[argc - 2]);
  spacing[1] = atof(argv[argc - 1]);
  multiFilter->SetOutputSpacing(spacing);

  VectorImageType::PointType origin;
  origin[0] = strtod(argv[argc - 6], nullptr);
  origin[1] = strtod(argv[argc - 5], nullptr);
  multiFilter->SetOutputOrigin(origin);


  WriterType::Pointer writer = WriterType::New();

  multiFilter->SetNumberOfThreads(atoi(argv[argc - 10]));

  writer->SetInput(multiFilter->GetOutput());

  writer->SetFileName(argv[argc - 13]);
  writer->SetNumberOfDivisionsStrippedStreaming(atoi(argv[argc - 15]));
  writer->Update();


  return EXIT_SUCCESS;
}


int otbMulti3DMapToDEMFilter(int argc, char* argv[])
{
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::ImageFileReader<VectorImageType> ReaderVectorType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  typedef otb::ObjectList<ReaderType>           MaskReaderListType;
  typedef otb::ObjectList<ReaderVectorType>     MapReaderListType;

  if ((argc - 6) % 2 != 0)
  {
    std::cout << "Usage: " << argv[0] << " 3DMapImage1 .mask1... 3DMapImageN maskN  DEMoutput DEMGridStep FusionMode ThreadNb StreamNb" << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int mapSize = (argc - 6) / 2;


  MapReaderListType::Pointer  mapReaderList  = MapReaderListType::New();
  MaskReaderListType::Pointer maskReaderList = MaskReaderListType::New();
  for (unsigned int i = 0; i < (mapSize); i++)
  {
    mapReaderList->PushBack(ReaderVectorType::New());
    maskReaderList->PushBack(ReaderType::New());
  }


  // unsigned int i=1;
  unsigned int index = 0;
  for (unsigned int i = 1; i < (2 * mapSize + 1); i = i + 2)
  {
    mapReaderList->GetNthElement(index)->SetFileName(argv[i]);
    mapReaderList->GetNthElement(index)->UpdateOutputInformation();

    maskReaderList->GetNthElement(index)->SetFileName(argv[i + 1]);
    maskReaderList->GetNthElement(index)->UpdateOutputInformation();
    index++;
  }

  float gridStep = atof(argv[argc - 4]);

  std::cout << "gridStep " << gridStep << std::endl;

  int fusionMode = atof(argv[argc - 3]);

  std::cout << "fusionMode " << fusionMode << std::endl;


  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
  multiFilter->SetNumberOf3DMaps(mapSize);
  multiFilter->SetDEMGridStep(gridStep);
  multiFilter->SetCellFusionMode(fusionMode);

  for (unsigned int i = 0; i < mapSize; i++)
  {
    multiFilter->Set3DMapInput(i, mapReaderList->GetNthElement(i)->GetOutput());

    multiFilter->SetMaskInput(i, maskReaderList->GetNthElement(i)->GetOutput());
  }
  multiFilter->SetOutputParametersFrom3DMap();

  WriterType::Pointer writer = WriterType::New();

  multiFilter->SetNumberOfThreads(atoi(argv[argc - 2]));

  writer->SetInput(multiFilter->GetOutput());

  writer->SetFileName(argv[argc - 5]);
  writer->SetNumberOfDivisionsStrippedStreaming(atoi(argv[argc - 1]));
  writer->Update();


  return EXIT_SUCCESS;
}
