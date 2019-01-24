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

#include "otbImageRegionAdaptativeSplitter.h"
#include <fstream>

const int Dimension = 2;
typedef otb::ImageRegionAdaptativeSplitter<Dimension> SplitterType;
typedef SplitterType::RegionType                      RegionType;
typedef RegionType::SizeType                          SizeType;
typedef RegionType::IndexType                         IndexType;


int otbImageRegionAdaptativeSplitter(int itkNotUsed(argc), char * argv[])
{
  SizeType regionSize, tileHint;
  IndexType regionIndex;
  RegionType region;
  unsigned int requestedNbSplits;

  regionIndex[0] = atoi(argv[1]);
  regionIndex[1] = atoi(argv[2]);
  regionSize[0]  = atoi(argv[3]);
  regionSize[1]  = atoi(argv[4]);
  tileHint[0]    = atoi(argv[5]);
  tileHint[1]    = atoi(argv[6]);
  requestedNbSplits = atoi(argv[7]);
  std::string outfname = argv[8];

  std::ofstream outfile(outfname);

  region.SetSize(regionSize);
  region.SetIndex(regionIndex);

  SplitterType::Pointer splitter = SplitterType::New();
  splitter->SetTileHint(tileHint);


  unsigned int nbSplits = splitter->GetNumberOfSplits(region, requestedNbSplits);
  std::vector<RegionType> splits;

  outfile<<splitter<<std::endl;
  outfile<<"Split map: "<<std::endl;

  for(unsigned int i = 0; i < nbSplits; ++i)
    {
    RegionType tmpRegion = splitter->GetSplit(i, requestedNbSplits, region);
    splits.push_back(tmpRegion);
    outfile<<"Split "<<i<<": "<<tmpRegion;
    }

  outfile.close();

  // Basic consistency check on split map
  if (requestedNbSplits != nbSplits)
    {
    std::cout << "Wrong number of splits : got "<<nbSplits<<" , expected "<<requestedNbSplits<< std::endl;
    }
  IndexType tmpIndex;
  for (unsigned int i=regionIndex[0] ; i<(regionIndex[0]+regionSize[0]) ; ++i)
    {
    for (unsigned int j=regionIndex[1] ; j<(regionIndex[1]+regionSize[1]) ; ++j)
      {
      tmpIndex[0] = i;
      tmpIndex[1] = j;
      unsigned int count = 0;
      for (unsigned int k=0 ; k<nbSplits ; ++k )
        {
        if (splits[k].IsInside(tmpIndex))
          {
          count++;
          }
        }
      if (count == 0)
        {
        std::cout << "Index ["<<i<<","<<j<<"] is missing in split map" << std::endl;
        return EXIT_FAILURE;
        }
      if (count > 1)
        {
        std::cout << "Index ["<<i<<","<<j<<"] occurs more than once in the split map" << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  unsigned int pixelInSplit = 0;
  for (unsigned int k=0 ; k<nbSplits ; ++k )
    {
    pixelInSplit += splits[k].GetSize(0) * splits[k].GetSize(1);
    }
  if (pixelInSplit != regionSize[0]*regionSize[1])
    {
    std::cout << "Wrong number of pixels in split : got "<<pixelInSplit << " , expected "<< regionSize[0]*regionSize[1] << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
