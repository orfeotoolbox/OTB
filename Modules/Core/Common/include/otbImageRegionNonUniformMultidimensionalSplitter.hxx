/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbImageRegionNonUniformMultidimensionalSplitter_hxx
#define otbImageRegionNonUniformMultidimensionalSplitter_hxx
#include "otbImageRegionNonUniformMultidimensionalSplitter.h"

namespace otb
{

/**
 *
 */
template <unsigned int VImageDimension>
unsigned int
ImageRegionNonUniformMultidimensionalSplitter<VImageDimension>
::GetNumberOfSplits(const RegionType& region, unsigned int requestedNumber)
{
  const SizeType& regionSize = region.GetSize();

  // requested number of splits per dimension
  double splitsPerDimension[VImageDimension];
//     ::ceil( std::pow((double) requestedNumber, 1.0/(double) VImageDimension));

  unsigned int numberOfPiecesLeft = requestedNumber;
  unsigned int j, numPieces;
  numPieces = 1;

  for (j = VImageDimension; j > 0; --j)
    {
    if (regionSize[j - 1] < numberOfPiecesLeft)
      {
      splitsPerDimension[j - 1] = regionSize[j - 1];
      }
    else
      {
      splitsPerDimension[j - 1] = numberOfPiecesLeft;
      }
    numberOfPiecesLeft = (unsigned int)
                         ::ceil(numberOfPiecesLeft / splitsPerDimension[j - 1]);
    numPieces *= (unsigned int) splitsPerDimension[j - 1];
    }

  // if a given dimension has fewer pixels that splitsPerDimension, then
  // only split number of pixels times
//   unsigned int i, numPieces;
//   numPieces = 1;
//   for (i=0; i < VImageDimension; ++i)
//     {
//     if (regionSize[i] < splitsPerDimension)
//       {
//       numPieces *= regionSize[i];
//       }
//     else
//       {
//       numPieces *= (unsigned int) splitsPerDimension;
//       }
//     }

  return numPieces;
}

/**
 *
 */
template <unsigned int VImageDimension>
itk::ImageRegion<VImageDimension>
ImageRegionNonUniformMultidimensionalSplitter<VImageDimension>
::GetSplit(unsigned int i, unsigned int numberOfPieces,
           const RegionType& region)
{
  RegionType splitRegion;
  IndexType  splitIndex;
  SizeType   splitSize, regionSize;

  // Initialize the splitRegion to the requested region
  splitRegion = region;
  splitIndex = splitRegion.GetIndex();
  splitSize = splitRegion.GetSize();

  regionSize = region.GetSize();

  // requested number of splits per dimension
  double splitsPerDimension[VImageDimension];
//     ::ceil( std::pow((double) numberOfPieces, 1.0/(double) VImageDimension));

  unsigned int numberOfPiecesLeft = numberOfPieces;

  unsigned int j;
  for (j = VImageDimension; j > 0; --j)
    {
    if (regionSize[j - 1] < numberOfPiecesLeft)
      {
      splitsPerDimension[j - 1] = regionSize[j - 1];
      }
    else
      {
      splitsPerDimension[j - 1] = numberOfPiecesLeft;
      }
    numberOfPiecesLeft = (unsigned int)
                         ::ceil(numberOfPiecesLeft / splitsPerDimension[j - 1]);
    }

  // if a given dimension has fewer pixels that splitsPerDimension, then
  // only split number of pixels times
  unsigned int splits[VImageDimension], pixelsPerSplit[VImageDimension];
  unsigned int numPieces;
  unsigned int ijk[VImageDimension];
  unsigned int offsetTable[VImageDimension];
  numPieces = 1;
  for (j = 0; j < VImageDimension; ++j)
    {
    offsetTable[j] = numPieces;
    if (regionSize[j] < splitsPerDimension[j])
      {
      splits[j] = regionSize[j];
      pixelsPerSplit[j] = 1;
      numPieces *= regionSize[j];
      }
    else
      {
      splits[j] = (unsigned int) splitsPerDimension[j];
      pixelsPerSplit[j] = (unsigned int) ::ceil(regionSize[j]
                                                / (double) splits[j]);
      numPieces *= (unsigned int) splitsPerDimension[j];
      }
    }

  // determine which split we are in
  unsigned int offset = i;
  for (j = VImageDimension - 1; j > 0; j--)
    {
    ijk[j] = offset / offsetTable[j];
    offset -= (ijk[j] * offsetTable[j]);
    }
  ijk[0] = offset;

  // compute the split
  for (j = 0; j < VImageDimension; ++j)
    {
    splitIndex[j] += ijk[j] * pixelsPerSplit[j];
    if (ijk[j] < splits[j] - 1)
      {
      splitSize[j] = pixelsPerSplit[j];
      }
    else
      {
      // this dimension is falling off the edge of the image
      splitSize[j] = splitSize[j] - ijk[j] * pixelsPerSplit[j];
      }
    }

  // set the split region ivars
  splitRegion.SetIndex(splitIndex);
  splitRegion.SetSize(splitSize);

  itkDebugMacro("  Split Piece: " << std::endl << splitRegion);

  return splitRegion;
}

/**
 *
 */
template <unsigned int VImageDimension>
void
ImageRegionNonUniformMultidimensionalSplitter<VImageDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
