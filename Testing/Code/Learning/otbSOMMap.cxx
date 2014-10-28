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
#include "itkMacro.h"
#include "otbSOMMap.h"
#include "itkRGBPixel.h"
#include "itkVariableLengthVector.h"

int otbSOMMap(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef float                                           InternalPixelType;
  typedef itk::VariableLengthVector<InternalPixelType>    PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType>   DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension> SOMMapType;

  // Instantiation
  SOMMapType::Pointer somMap = SOMMapType::New();

  // Allocation of the som map
  SOMMapType::RegionType region;
  SOMMapType::IndexType  index;
  SOMMapType::SizeType   size;
  index.Fill(0);
  size.Fill(64);
  region.SetIndex(index);
  region.SetSize(size);
  somMap->SetRegions(region);
  somMap->SetNumberOfComponentsPerPixel(3);
  somMap->Allocate();

  // Filling with null pixels
  PixelType nullPixel;
  nullPixel.SetSize(3);
  nullPixel.Fill(0);
  somMap->FillBuffer(nullPixel);

  // Definition of a non-null pixel
  PixelType winner;
  winner.SetSize(3);
  winner.Fill(1);
  index.Fill(32);
  somMap->SetPixel(index, winner);

  // Test of the GetWinner method
  SOMMapType::IndexType winnerIndex = somMap->GetWinner(winner);
  if (winnerIndex != index)
    {
    std::cout << "Bad GetWinner function return." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
