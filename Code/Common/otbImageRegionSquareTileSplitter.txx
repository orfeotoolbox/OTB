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
#ifndef __otbImageRegionSquareTileSplitter_txx
#define __otbImageRegionSquareTileSplitter_txx

#include "otbImageRegionSquareTileSplitter.h"
#include "otbMath.h"
#include "otbMacro.h"

namespace otb
{

/**
   *
 */
template <unsigned int VImageDimension>
unsigned int
ImageRegionSquareTileSplitter<VImageDimension>
::GetNumberOfSplits(const RegionType& region, unsigned int requestedNumber)
{
  if (m_TileSizeInBytes == 0)
    {
    itkExceptionMacro(<< "SetTileSizeInBytes has not been called");
    }

  if (m_PixelSizeInBytes == 0)
    {
    itkExceptionMacro(<< "SetPixelSizeInBytes has not been called");
    }

  // Compute the theorical square tile dimensions
  const unsigned int nbPixelPerTile = m_TileSizeInBytes / m_PixelSizeInBytes;
  unsigned int nbPixelPerDim  = static_cast<unsigned int>( vcl_sqrt(nbPixelPerTile));

  // Use a reference tile size corresponding to a 256 * 256 region of a 4 bands unsigned short image (=512 k)
  const unsigned int ReferenceTileSizeInBytes = 256 * 256 * 4 * 2;
  unsigned int nbPixelPerReferenceTile = ReferenceTileSizeInBytes / m_PixelSizeInBytes;
  unsigned int referenceNbPixelPerDim = static_cast<unsigned int>( vcl_sqrt(static_cast<float>(nbPixelPerReferenceTile)) );
  // Align the tile dimension to the next multiple of 16 using integer division (TIFF tiles are aligned with 16)
  referenceNbPixelPerDim = ( referenceNbPixelPerDim + 15 ) / 16 * 16;

  // Align nbPixelPerDim with the next multiple of referenceNbPixelPerDim using integer division
  m_AlignStep = (nbPixelPerDim + referenceNbPixelPerDim) / referenceNbPixelPerDim * referenceNbPixelPerDim;

  // Use at least 16*16 tiles
  if (m_AlignStep < 16)
    {
    m_AlignStep = 16;
    }

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
ImageRegionSquareTileSplitter<VImageDimension>
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
ImageRegionSquareTileSplitter<VImageDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "AlignStep        : " << m_AlignStep << std::endl;
  os << indent << "PixelSizeInBytes : " << m_PixelSizeInBytes << std::endl;
  os << indent << "TileSizeInBytes  : " << m_TileSizeInBytes << std::endl;
}

} // end namespace itk

#endif
