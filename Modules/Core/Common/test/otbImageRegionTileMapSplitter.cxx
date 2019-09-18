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

#include "otbImageRegionTileMapSplitter.h"
#include <fstream>

int otbImageRegionTileMapSplitter(int itkNotUsed(argc), char* argv[])
{

  const int                                          Dimension = 2;
  typedef otb::ImageRegionTileMapSplitter<Dimension> FilterType;
  typedef FilterType::IndexType                      IndexType;
  typedef FilterType::SizeType                       SizeType;
  typedef FilterType::RegionType                     RegionType;

  std::ofstream       outfile(argv[1]);
  FilterType::Pointer filter = FilterType::New();

  RegionType   region, region2;
  unsigned int nb, nbSplitTheoric, nbAsked;
  IndexType    index;
  SizeType     size;

  // Case 1
  index[0]       = 45;
  index[1]       = 45;
  size[0]        = 1000;
  size[1]        = 1500;
  nbSplitTheoric = 10;
  nbAsked        = 2;

  region.SetSize(size);
  region.SetIndex(index);

  nb      = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 1 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  // Case 2
  index[0]       = 45;
  index[1]       = 45;
  size[0]        = 1048576;
  size[1]        = 1048576;
  nbSplitTheoric = 16777216;
  nbAsked        = 2;

  region.SetSize(size);
  region.SetIndex(index);

  nb      = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 2 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  // Case 3
  index[0]       = 45;
  index[1]       = 45;
  size[0]        = 1048576;
  size[1]        = 1048576;
  nbSplitTheoric = 23;
  nbAsked        = 4;

  region.SetSize(size);
  region.SetIndex(index);

  nb      = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 3 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  // Case 4
  index[0]       = 45;
  index[1]       = 45;
  size[0]        = 1048576;
  size[1]        = 1024;
  nbSplitTheoric = 16777216;
  nbAsked        = 16387;

  region.SetSize(size);
  region.SetIndex(index);

  nb      = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 4 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  // Case 5
  index[0]       = 0;
  index[1]       = 0;
  size[0]        = 513;
  size[1]        = 5376;
  nbSplitTheoric = 8;
  nbAsked        = 9;

  region.SetSize(size);
  region.SetIndex(index);

  nb      = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 5 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
