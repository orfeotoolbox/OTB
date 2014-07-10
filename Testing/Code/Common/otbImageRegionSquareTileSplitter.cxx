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
#include "otbImageRegionSquareTileSplitter.h"
#include <fstream>

const int Dimension = 2;
typedef otb::ImageRegionSquareTileSplitter<Dimension> SquareTileSplitterType;
typedef SquareTileSplitterType::IndexType                       IndexType;
typedef SquareTileSplitterType::SizeType                        SizeType;
typedef SquareTileSplitterType::RegionType                      RegionType;


int otbImageRegionSquareTileSplitterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  SquareTileSplitterType::Pointer splitter = SquareTileSplitterType::New();

  std::cout << splitter << std::endl;

  return EXIT_SUCCESS;
}

int TestSplitter(const RegionType& region, unsigned int PixelSize, unsigned int MaxTileSize, std::ostream& os)
{
  os << "----------------------------------" << std::endl;
  os << "Region    : " << region << std::endl;
  os << "PixelSize : " << PixelSize << std::endl;
  os << "MaxTileSize  : " << MaxTileSize << std::endl;

  SquareTileSplitterType::Pointer splitter;
  splitter = SquareTileSplitterType::New();

  unsigned int requestedNbSplits = region.GetNumberOfPixels() * PixelSize / MaxTileSize;
  if (requestedNbSplits == 0)
    requestedNbSplits = 1;
  os << "Requested Number of splits  : " << requestedNbSplits << std::endl;

  const unsigned int nbSplits = splitter->GetNumberOfSplits(region, requestedNbSplits);
  os << "Actual Number of splits  : " << nbSplits << std::endl;


  RegionType split;

  // First split :
  split = splitter->GetSplit(0, nbSplits, region);
  os << "First Split : " << split
     << "(" << split.GetNumberOfPixels() * PixelSize << " bytes)" << std::endl;

  if (nbSplits > 1)
    {
    // Second split :
    split = splitter->GetSplit(1, nbSplits, region);
    os << "Second Split : " << split
       << "(" << split.GetNumberOfPixels() * PixelSize << " bytes)" << std::endl;
    }

  if (nbSplits > 2)
    {
    // Last split :
    split = splitter->GetSplit(nbSplits - 1, nbSplits, region);
    os << "Last Split : " << split
       << "(" << split.GetNumberOfPixels() * PixelSize << " bytes)" << std::endl;
    }

  return EXIT_SUCCESS;
}


int otbImageRegionSquareTileSplitter(int itkNotUsed(argc), char * argv[])
{
  std::ofstream outfile(argv[1]);
  RegionType region;

  // Test with a 0-based indexed region
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 1024);
  region.SetSize(1, 1024);
  TestSplitter(region, 1, 128, outfile);
  TestSplitter(region, 1, 512*512, outfile);
  TestSplitter(region, 2, 512*512, outfile);
  TestSplitter(region, 4, 512*512, outfile);
  TestSplitter(region, 8, 512*512, outfile);

  // Test with a shifted region
  region.SetIndex(0, 42);
  region.SetIndex(1, 42);
  region.SetSize(0, 1000);
  region.SetSize(1, 1000);
  TestSplitter(region, 1, 128, outfile);
  TestSplitter(region, 1, 512*512, outfile);
  TestSplitter(region, 2, 512*512, outfile);
  TestSplitter(region, 4, 512*512, outfile);
  TestSplitter(region, 8, 512*512, outfile);

  // Test with a negative shift
  region.SetIndex(0, -42);
  region.SetIndex(1, -42);
  region.SetSize(0, 1000);
  region.SetSize(1, 1000);
  TestSplitter(region, 1, 128, outfile);
  TestSplitter(region, 1, 512*512, outfile);
  TestSplitter(region, 2, 512*512, outfile);
  TestSplitter(region, 4, 512*512, outfile);
  TestSplitter(region, 8, 512*512, outfile);

  // Test with a reduced size
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 1);
  region.SetSize(1, 1);
  TestSplitter(region, 1, 128, outfile);
  TestSplitter(region, 1, 512*512, outfile);
  TestSplitter(region, 2, 512*512, outfile);
  TestSplitter(region, 4, 512*512, outfile);
  TestSplitter(region, 8, 512*512, outfile);

  // Test with a reduced size, shifted
  region.SetIndex(0, 42);
  region.SetIndex(1, 42);
  region.SetSize(0, 1);
  region.SetSize(1, 1);
  TestSplitter(region, 1, 128, outfile);
  TestSplitter(region, 1, 512*512, outfile);
  TestSplitter(region, 2, 512*512, outfile);
  TestSplitter(region, 4, 512*512, outfile);
  TestSplitter(region, 8, 512*512, outfile);

  outfile.close();

  return EXIT_SUCCESS;
}
