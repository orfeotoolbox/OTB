/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImageRegionAdaptativeSplitter.h"
#include <fstream>

const int Dimension = 2;
typedef otb::ImageRegionAdaptativeSplitter<Dimension> SplitterType;
typedef SplitterType::RegionType                      RegionType;
typedef RegionType::SizeType                          SizeType;
typedef RegionType::IndexType                         IndexType;

int otbImageRegionAdaptativeSplitterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  SplitterType::Pointer splitter = SplitterType::New();

  std::cout<<splitter<<std::endl;

  return EXIT_SUCCESS;
}

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

  std::ofstream outfile(outfname.c_str());

  region.SetSize(regionSize);
  region.SetIndex(regionIndex);

  SplitterType::Pointer splitter = SplitterType::New();
  splitter->SetTileHint(tileHint);


  unsigned int nbSplits = splitter->GetNumberOfSplits(region, requestedNbSplits);

  outfile<<splitter<<std::endl;
  outfile<<"Split map: "<<std::endl;

  for(unsigned int i = 0; i < nbSplits; ++i)
    {
    outfile<<"Split "<<i<<": "<<splitter->GetSplit(i, requestedNbSplits, region);
    }

  outfile.close();

  return EXIT_SUCCESS;
}
