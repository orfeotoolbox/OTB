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


#include "otbVectorImage.h"
#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbRAMDrivenStrippedStreamingManager.h"
#include "otbTileDimensionTiledStreamingManager.h"
#include "otbRAMDrivenTiledStreamingManager.h"
#include "otbRAMDrivenAdaptativeStreamingManager.h"

#include <fstream>

const int Dimension = 2;
typedef otb::VectorImage<unsigned short, Dimension> ImageType;
typedef otb::NumberOfLinesStrippedStreamingManager<ImageType> NbLinesStrippedStreamingManagerType;
typedef otb::RAMDrivenStrippedStreamingManager<ImageType>     RAMDrivenStrippedStreamingManagerType;
typedef otb::TileDimensionTiledStreamingManager<ImageType>    TileDimensionTiledStreamingManagerType;
typedef otb::RAMDrivenTiledStreamingManager<ImageType>        RAMDrivenTiledStreamingManagerType;
typedef otb::RAMDrivenAdaptativeStreamingManager<ImageType>   RAMDrivenAdaptativeStreamingManagerType;


ImageType::Pointer makeImage(ImageType::RegionType region)
{
  ImageType::Pointer image = ImageType::New();

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(10);

  itk::MetaDataDictionary& dict = image->GetMetaDataDictionary();

  itk::EncapsulateMetaData<unsigned int>(dict, otb::MetaDataKey::TileHintX, 64);
  itk::EncapsulateMetaData<unsigned int>(dict, otb::MetaDataKey::TileHintY, 64);

  return image;
}


int otbNumberOfLinesStrippedStreamingManager(int itkNotUsed(argc), char* argv[])
{
  std::ofstream outfile(argv[1]);

  NbLinesStrippedStreamingManagerType::Pointer streamingManager = NbLinesStrippedStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetNumberOfLinesPerStrip(10);
  streamingManager->PrepareStreaming(makeImage(region), region);

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}


int otbRAMDrivenStrippedStreamingManager(int itkNotUsed(argc), char* argv[])
{
  std::ofstream outfile(argv[1]);

  RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetAvailableRAMInMB(1);
  streamingManager->PrepareStreaming(makeImage(region), region);

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}

int otbTileDimensionTiledStreamingManager(int itkNotUsed(argc), char* argv[])
{
  std::ofstream outfile(argv[1]);

  TileDimensionTiledStreamingManagerType::Pointer streamingManager = TileDimensionTiledStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetTileDimension(100);
  streamingManager->PrepareStreaming(makeImage(region), region);

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}

int otbRAMDrivenTiledStreamingManager(int itkNotUsed(argc), char* argv[])
{
  std::ofstream outfile(argv[1]);

  RAMDrivenTiledStreamingManagerType::Pointer streamingManager = RAMDrivenTiledStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetAvailableRAMInMB(1);
  streamingManager->PrepareStreaming(makeImage(region), region);

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}


int otbRAMDrivenAdaptativeStreamingManager(int itkNotUsed(argc), char* argv[])
{
  std::ofstream outfile(argv[1]);

  RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetAvailableRAMInMB(1);
  streamingManager->PrepareStreaming(makeImage(region), region);

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}
