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
#ifndef __otbImageRegionTileMapSplitter_txx
#define __otbImageRegionTileMapSplitter_txx

#include "otbImageRegionTileMapSplitter.h"
#include "otbMath.h"
#include "otbMacro.h"

namespace otb
{

/**
   *
 */
template <unsigned int VImageDimension>
unsigned int
ImageRegionTileMapSplitter<VImageDimension>
::GetNumberOfSplits(const RegionType& region, unsigned int requestedNumber)
{
  const SizeType&  regionSize = region.GetSize();
  const IndexType& regionIndex = region.GetIndex();

  // requested number of splits per dimension
  unsigned int numPieces = 1;

  // determine the actual number of pieces that will be generated
  for (unsigned int j = VImageDimension; j > 0; --j)
    {
//    otbMsgDevMacro(<< "*** Dimension: " << j-1);
    unsigned long int remainingToDo =
      static_cast<unsigned long int>(vcl_ceil(static_cast<double>(requestedNumber) / numPieces));
    unsigned int maxPieces = (regionIndex[j - 1] + regionSize[j - 1] - 1) / m_AlignStep - regionIndex[j - 1]
                             / m_AlignStep + 1;
    unsigned int stepPerPiece = 1;
    if (remainingToDo < maxPieces)
      {
      stepPerPiece = static_cast<unsigned int> (vcl_floor(static_cast<double> (maxPieces) / remainingToDo));
      if ((remainingToDo - 1) * (stepPerPiece + 1) < maxPieces)
        {
        stepPerPiece += 1;
        }
      }
    unsigned int maxPieceUsed = static_cast<unsigned int> (vcl_ceil(static_cast<double> (maxPieces) / stepPerPiece));
    m_SplitsPerDimension[j - 1] = maxPieceUsed;
//    otbMsgDevMacro("*** maxPieces stepPerPiece maxPieceUsed " << maxPieces
//                      << " " << stepPerPiece << " " << maxPieceUsed);
    numPieces *= maxPieceUsed;
    }
//  otbMsgDevMacro("*** numPieces " << numPieces);
  return numPieces;
}

/**
   *
 */
template <unsigned int VImageDimension>
itk::ImageRegion<VImageDimension>
ImageRegionTileMapSplitter<VImageDimension>
::GetSplit(unsigned int i, unsigned int numberOfPieces, const RegionType& region)
{
  RegionType splitRegion;
  IndexType  splitIndex, regionIndex;
  SizeType   splitSize, regionSize;

  // Initialize the splitRegion to the requested region
  splitRegion = region;
  splitIndex = splitRegion.GetIndex();
  splitSize = splitRegion.GetSize();

  regionSize = region.GetSize();
  regionIndex = region.GetIndex();

  unsigned int numPieces = GetNumberOfSplits(region, numberOfPieces);
  if (i > numPieces)
    {
    itkDebugMacro("  Cannot Split");
    return splitRegion;
    }

  unsigned int stackSize = 1;
  for (unsigned int j = 0; j < VImageDimension; ++j)
    {
    unsigned int slicePos = (i % (stackSize * m_SplitsPerDimension[j])) / stackSize;
    stackSize *= m_SplitsPerDimension[j];

    unsigned int generalSplitSize =
      static_cast<unsigned int> (vcl_ceil(static_cast<double> (regionSize[j]) / (m_SplitsPerDimension[j]
                                                                                 *
                                                                                 m_AlignStep))) * m_AlignStep;
    if (slicePos == 0)
      {
      splitIndex[j] = regionIndex[j];
      }
    else
      {
      splitIndex[j] = (regionIndex[j] / generalSplitSize + slicePos) * generalSplitSize;
      }
    if (slicePos == 0)
      {
      splitSize[j] = generalSplitSize - (regionIndex[j] % generalSplitSize);
      }
    else if (slicePos == m_SplitsPerDimension[j] - 1)
      {
      splitSize[j] = regionSize[j] - (generalSplitSize - (regionIndex[j] % generalSplitSize))
                     - (m_SplitsPerDimension[j] - 2) * generalSplitSize;
      }
    else
      {
      splitSize[j] = generalSplitSize;
      }
    }

  // set the split region ivars
  splitRegion.SetIndex(splitIndex);
  splitRegion.SetSize(splitSize);

  return splitRegion;
}

/**
       *
 */
template <unsigned int VImageDimension>
void
ImageRegionTileMapSplitter<VImageDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif
