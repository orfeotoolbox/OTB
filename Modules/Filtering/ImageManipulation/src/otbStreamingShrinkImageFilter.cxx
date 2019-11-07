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

#include "otbStreamingShrinkImageFilter.h"

namespace otb
{

unsigned int StreamingShrinkImageRegionSplitter::GetNumberOfSplits(const RegionType& region, unsigned int requestedNumber)
{
  unsigned int theoricalNbPixelPerTile = region.GetNumberOfPixels() / requestedNumber;
  unsigned int theoricalTileDimension  = static_cast<unsigned int>(std::sqrt(static_cast<double>(theoricalNbPixelPerTile)));

  // Take the previous multiple of m_ShrinkFactor (eventually generate more splits than requested)
  m_TileDimension = theoricalTileDimension / m_ShrinkFactor * m_ShrinkFactor;

  // Minimal tile size is m_ShrinkFactor * m_ShrinkFactor
  if (m_TileDimension < m_ShrinkFactor)
  {
    otbMsgDevMacro(<< "Using the minimal tile size : " << m_ShrinkFactor << " * " << m_ShrinkFactor);
    m_TileDimension = m_ShrinkFactor;
  }

  const SizeType& regionSize = region.GetSize();
  // Compute the alignment of the sampling grid
  m_TileSizeAlignment = (m_ShrinkFactor - 1) / 2;
  if (m_ShrinkFactor > regionSize[1])
    m_TileSizeAlignment = (regionSize[1] - 1) / 2;

  // Use the computed tile size, and generate (m_TileDimension * 1) tiles
  m_SplitsPerDimension[0] = (regionSize[0] + m_TileDimension - 1) / m_TileDimension;
  m_SplitsPerDimension[1] = (regionSize[1] - m_TileSizeAlignment - 1) / m_ShrinkFactor + 1;

  unsigned int numPieces = 1;
  for (unsigned int j = 0; j < ImageDimension; ++j)
  {
    numPieces *= m_SplitsPerDimension[j];
  }

  otbMsgDevMacro(<< "Tile dimension : " << m_TileDimension)
      otbMsgDevMacro(<< "Number of splits per dimension : " << m_SplitsPerDimension[0] << " " << m_SplitsPerDimension[1])

          return numPieces;
}

StreamingShrinkImageRegionSplitter::RegionType StreamingShrinkImageRegionSplitter::GetSplit(unsigned int i, unsigned int itkNotUsed(numberOfPieces),
                                                                                            const RegionType& region)
{
  RegionType splitRegion;
  IndexType  splitIndex;

  // Compute the actual number of splits
  unsigned int numPieces = 1;
  for (unsigned int j = 0; j < ImageDimension; ++j)
  {
    numPieces *= m_SplitsPerDimension[j];
  }

  if (i >= numPieces)
  {
    itkExceptionMacro("Requested split number " << i << " but region contains only " << numPieces << " splits");
  }

  // Compute the split index in the streaming grid
  splitIndex[1] = i / m_SplitsPerDimension[0];
  splitIndex[0] = i % m_SplitsPerDimension[0];

  // Transform the split index to the actual coordinates
  splitRegion.SetIndex(0, region.GetIndex(0) + m_TileDimension * splitIndex[0]);
  splitRegion.SetIndex(1, region.GetIndex(1) + m_ShrinkFactor * splitIndex[1] + m_TileSizeAlignment);

  splitRegion.SetSize(0, m_TileDimension);
  splitRegion.SetSize(1, 1);

  // Handle the borders
  splitRegion.Crop(region);

  return splitRegion;
}

void StreamingShrinkImageRegionSplitter::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "ShrinkFactor       : " << m_ShrinkFactor << std::endl;
  os << indent << "SplitsPerDimension : " << m_SplitsPerDimension << std::endl;
  os << indent << "TileDimension      : " << m_TileDimension << std::endl;
  os << indent << "TileSizeAlignment  : " << m_TileSizeAlignment << std::endl;
}

} // End namespace otb
