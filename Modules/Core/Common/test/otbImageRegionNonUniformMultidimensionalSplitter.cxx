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
#include "otbImageRegionNonUniformMultidimensionalSplitter.h"
#include <fstream>

int otbImageRegionNonUniformMultidimensionalSplitter(int itkNotUsed(argc), char * argv[])
{
  const int Dimension = 2;
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
  const char * outfname(argv[7]);

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
