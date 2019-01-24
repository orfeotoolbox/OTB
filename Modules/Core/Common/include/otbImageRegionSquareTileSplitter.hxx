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

#ifndef otbImageRegionSquareTileSplitter_hxx
#define otbImageRegionSquareTileSplitter_hxx

#include "otbImageRegionSquareTileSplitter.h"
#include "otbMath.h"
#include "otbMacro.h"

namespace otb
{

template <unsigned int VImageDimension>
unsigned int
ImageRegionSquareTileSplitter<VImageDimension>
::GetNumberOfSplits(const RegionType& region, unsigned int requestedNumber)
{
  unsigned int theoricalNbPixelPerTile = region.GetNumberOfPixels() / requestedNumber;
  unsigned int theoricalTileDimension = static_cast<unsigned int> (std::sqrt(static_cast<double>(theoricalNbPixelPerTile)) );

  // Take the next multiple of m_TileSizeAlignment (eventually generate more splits than requested)
  m_TileDimension = (theoricalTileDimension + m_TileSizeAlignment - 1) / m_TileSizeAlignment * m_TileSizeAlignment;

  // Minimal tile size is m_TileSizeAlignment * m_TileSizeAlignment
  if (m_TileDimension < m_TileSizeAlignment)
    {
    otbMsgDevMacro(<< "Warning: clamping tile size to " << m_TileSizeAlignment << " * " << m_TileSizeAlignment);
    m_TileDimension = m_TileSizeAlignment;
    }

  unsigned int numPieces = 1;
  const SizeType&  regionSize = region.GetSize();
  for (unsigned int j = 0; j < VImageDimension; ++j)
    {
    m_SplitsPerDimension[j] = (regionSize[j] + m_TileDimension - 1) / m_TileDimension;
    numPieces *= m_SplitsPerDimension[j];
    }

  otbMsgDevMacro(<< "Tile dimension : " << m_TileDimension)
  otbMsgDevMacro(<< "Number of splits per dimension : " << m_SplitsPerDimension[0] << " " <<  m_SplitsPerDimension[1])

  return numPieces;
}

template <unsigned int VImageDimension>
itk::ImageRegion<VImageDimension>
ImageRegionSquareTileSplitter<VImageDimension>
::GetSplit(unsigned int i, unsigned int itkNotUsed(numberOfPieces), const RegionType& region)
{
  RegionType splitRegion;
  IndexType  splitIndex;

  // Compute the actual number of splits
  unsigned int numPieces = 1;
  for (unsigned int j = 0; j < VImageDimension; ++j)
    {
    numPieces *= m_SplitsPerDimension[j];
    }

  // Sanity check
  if (i >= numPieces)
    {
    itkExceptionMacro("Asked for split number " << i << " but region contains only " << numPieces << " splits");
    }

  // Compute the split index in the streaming grid
  unsigned int remaining = i;
  for (unsigned int j = VImageDimension - 1; j > 0; --j)
    {
    splitIndex[j] = remaining / m_SplitsPerDimension[VImageDimension - 1 - j];
    remaining = remaining % m_SplitsPerDimension[VImageDimension - 1 - j];
    }
  splitIndex[0] = remaining;

  // Transform the split index to the actual coordinates
  for (unsigned int j = 0; j < VImageDimension; ++j)
    {
    splitRegion.SetIndex(j, region.GetIndex(j) + m_TileDimension * splitIndex[j]);
    splitRegion.SetSize(j, m_TileDimension);
    }

  // Handle the borders
  splitRegion.Crop(region);

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
  os << indent << "SplitsPerDimension : " << m_SplitsPerDimension << std::endl;
  os << indent << "TileDimension      : " << m_TileDimension << std::endl;
  os << indent << "TileSizeAlignment  : " << m_TileSizeAlignment << std::endl;

}

} // end namespace itk

#endif
