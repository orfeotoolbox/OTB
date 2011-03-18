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
typedef otb::ImageRegionSquareTileSplitter<Dimension> SplitterType;

int otbImageRegionSquareTileSplitterNew(int argc, char * argv[])
{
  SplitterType::Pointer splitter = SplitterType::New();

  std::cout << splitter << std::endl;

  return EXIT_SUCCESS;
}

int otbImageRegionSquareTileSplitter(int argc, char * argv[])
{
  typedef SplitterType::IndexType                      IndexType;
  typedef SplitterType::SizeType                       SizeType;
  typedef SplitterType::RegionType                     RegionType;

  typedef unsigned short PixelPrecisionType;
  const unsigned int NbComponents = 8;
  const unsigned int PixelSizeInBytes = NbComponents * sizeof(PixelPrecisionType);

  std::ofstream outfile(argv[1]);
  SplitterType::Pointer filter = FilterType::New();

  RegionType   region, region2;
  unsigned int nb, nbSplitTheoric, nbAsked;
  IndexType    index;
  SizeType     size;

  //Case 1
  index[0] = 45;
  index[1] = 45;
  size[0] = 1000;
  size[1] = 1500;
  nbSplitTheoric = 10;
  nbAsked = 2;

  region.SetSize(size);
  region.SetIndex(index);

  nb = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 1 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  //Case 2
  index[0] = 45;
  index[1] = 45;
  size[0] = 1048576;
  size[1] = 1048576;
  nbSplitTheoric = 16777216;
  nbAsked = 2;

  region.SetSize(size);
  region.SetIndex(index);

  nb = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 2 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  //Case 3
  index[0] = 45;
  index[1] = 45;
  size[0] = 1048576;
  size[1] = 1048576;
  nbSplitTheoric = 23;
  nbAsked = 4;

  region.SetSize(size);
  region.SetIndex(index);

  nb = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 3 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  //Case 4
  index[0] = 45;
  index[1] = 45;
  size[0] = 1048576;
  size[1] = 1024;
  nbSplitTheoric = 16777216;
  nbAsked = 16387;

  region.SetSize(size);
  region.SetIndex(index);

  nb = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 4 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  //Case 5
  index[0] = 0;
  index[1] = 0;
  size[0] = 513;
  size[1] = 5376;
  nbSplitTheoric = 8;
  nbAsked = 9;

  region.SetSize(size);
  region.SetIndex(index);

  nb = filter->GetNumberOfSplits(region, nbSplitTheoric);
  region2 = filter->GetSplit(nbAsked, nb, region);

  outfile << "\nCase 5 \n";
  outfile << "Input region: " << region << std::endl;
  outfile << "Input NumberOfSplits: " << nbSplitTheoric << std::endl;
  outfile << "Output GetNumberOfSplits: " << nb << std::endl;
  outfile << "Output GetSplit(" << nbAsked << "," << nb << ", input region): " << std::endl;
  outfile << "Output region: " << region2 << std::endl;

  //Case 6
  index[0] = 0;
  index[1] = 0;
  size[0] = 3;
  size[1] = 2;
  nbSplitTheoric = 5;
  nbAsked = 0;

  region.SetSize(size);
  region.SetIndex(index);

  nb = filter->GetNumberOfSplits(region, nbSplitTheoric);
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
