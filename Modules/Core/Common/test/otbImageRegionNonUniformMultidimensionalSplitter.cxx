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

#include "otbImageRegionNonUniformMultidimensionalSplitter.h"
#include <fstream>

int otbImageRegionNonUniformMultidimensionalSplitter(int itkNotUsed(argc), char* argv[])
{
  const int                                                             Dimension = 2;
  typedef otb::ImageRegionNonUniformMultidimensionalSplitter<Dimension> FilterType;
  typedef FilterType::IndexType                                         IndexType;
  typedef FilterType::SizeType                                          SizeType;
  typedef FilterType::RegionType                                        RegionType;

  IndexType index;
  index[0] = atoi(argv[1]);
  index[1] = atoi(argv[2]);
  SizeType size;
  size[0] = atoi(argv[3]);
  size[1] = atoi(argv[4]);
  unsigned int nbSplitTheoric(atoi(argv[5]));
  unsigned int nbAsked(atoi(argv[6]));
  const char*  outfname(argv[7]);

  RegionType region;

  region.SetSize(size);
  region.SetIndex(index);

  FilterType::Pointer filter = FilterType::New();

  unsigned int nb = filter->GetNumberOfSplits(region, nbSplitTheoric);

  RegionType region2 = filter->GetSplit(nbAsked, nb, region);

  std::ofstream outfile(outfname);

  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "    " << region2 << std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
