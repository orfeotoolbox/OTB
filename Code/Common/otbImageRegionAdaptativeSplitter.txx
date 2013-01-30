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
#ifndef __otbImageRegionAdaptativeSplitter_txx
#define __otbImageRegionAdaptativeSplitter_txx

#include "otbImageRegionAdaptativeSplitter.h"
#include "otbMath.h"
#include "otbMacro.h"

// Defaut when no tile hint available
#include "otbImageRegionSquareTileSplitter.h"

namespace otb
{

template <unsigned int VImageDimension>
unsigned int
ImageRegionAdaptativeSplitter<VImageDimension>
::GetNumberOfSplits(const RegionType& region, unsigned int requestedNumber)
{
  // Set parameters
  this->SetImageRegion(region);
  this->SetRequestedNumberOfSplits(requestedNumber);

  // Check if we need to compute split map agagin
  m_Lock.Lock();
  if(!m_IsUpToDate)
    {
    // Do so if we need to
    this->EstimateSplitMap();
    }
  m_Lock.Unlock();

  // Return the size of the split map
  return m_StreamVector.size();
}

template <unsigned int VImageDimension>
itk::ImageRegion<VImageDimension>
ImageRegionAdaptativeSplitter<VImageDimension>
::GetSplit(unsigned int i, unsigned int itkNotUsed(numberOfPieces), const RegionType& region)
{
  // Set parameters
  this->SetImageRegion(region);

  // Check if we need to compute split map agagin
  m_Lock.Lock();
  if(!m_IsUpToDate)
    {
    // Do so if we need to
    this->EstimateSplitMap();
    }
  m_Lock.Unlock();

  // Return the requested split
  return m_StreamVector.at(i);
}

template <unsigned int VImageDimension>
void
ImageRegionAdaptativeSplitter<VImageDimension>
::EstimateSplitMap()
{
  // Clear previous split map
  m_StreamVector.clear();

  // Handle trivial case
  if(m_RequestedNumberOfSplits == 1 || m_RequestedNumberOfSplits == 0)
    {
    m_StreamVector.push_back(m_ImageRegion);
    m_IsUpToDate = true;
    return;
    }
  // Handle the empty hint case and the case where VImageDimension != 2
  if(m_TileHint[0] == 0 || m_TileHint[1] == 0 || VImageDimension != 2)
    {
    // In this case we fallback to the classical tile splitter
    typename otb::ImageRegionSquareTileSplitter<VImageDimension>::Pointer
      splitter = otb::ImageRegionSquareTileSplitter<VImageDimension>::New();

    // Retrieve nb splits
    unsigned int nbSplits = splitter->GetNumberOfSplits(m_ImageRegion, m_RequestedNumberOfSplits);

    for(unsigned int i = 0; i<nbSplits; ++i)
      {
      m_StreamVector.push_back(splitter->GetSplit(i, m_RequestedNumberOfSplits, m_ImageRegion));
      }
    m_IsUpToDate = true;
    return;
    }

  // Now we can handle the case where we have a tile hint and a
  // non-trivial requested number of splits
  SizeType tilesPerDim, splitsPerDim;

  tilesPerDim[0] = (m_ImageRegion.GetSize()[0] + m_TileHint[0] -1) / m_TileHint[0];
  tilesPerDim[1] = (m_ImageRegion.GetSize()[1] + m_TileHint[1] -1) / m_TileHint[1];

  unsigned int totalTiles = tilesPerDim[0] * tilesPerDim[1];

  // In this case, we have to group input tiles
  if(totalTiles >= m_RequestedNumberOfSplits)
    {
    // Try to group splits
    SizeType groupTiles;
    groupTiles.Fill(1);

    unsigned int i=0;

    // TODO: this should not fall in infinite loop, but add more
    // security just in case.
    while(totalTiles / (groupTiles[0] * groupTiles[1]) > m_RequestedNumberOfSplits)
      {
      if(groupTiles[i] < tilesPerDim[i])
        {
        groupTiles[i]++;
        }
      // TODO: We can be more generic here
      i = (i+1)%2;
      }

    splitsPerDim[0] = tilesPerDim[0] / groupTiles[0];

    // Handle the last small tile if any
    if(tilesPerDim[0] % groupTiles[0] > 0)
      splitsPerDim[0]++;

    splitsPerDim[1] = tilesPerDim[1] / groupTiles[1];
    if(tilesPerDim[1] % groupTiles[1] > 0)
      splitsPerDim[1]++;

    // Fill the tiling scheme
    for(unsigned int splity = 0; splity < splitsPerDim[1]; ++splity)
      {
      for(unsigned int splitx = 0; splitx < splitsPerDim[0]; ++splitx)
        {
        // Build the split
        RegionType newSplit;
        SizeType newSplitSize;
        IndexType newSplitIndex;

        newSplitSize[0] = groupTiles[0] * m_TileHint[0];
        newSplitSize[1] = groupTiles[1] * m_TileHint[1];

        newSplitIndex[0] = splitx * newSplitSize[0];
        newSplitIndex[1] = splity * newSplitSize[1];

        newSplit.SetIndex(newSplitIndex);
        newSplit.SetSize(newSplitSize);

        bool cropped = newSplit.Crop(m_ImageRegion);
        // If newSplit could not be cropped, it means that it is
        // outside m_ImageRegion. In this case we ignore it.
        if(cropped)
          {
          m_StreamVector.push_back(newSplit);
          }
        }
      }
    }
  // In this case, we must divide each tile
  else
    {
    SizeType divideTiles;
    divideTiles.Fill(1);

    unsigned int i = 1;

    while(totalTiles * (divideTiles[0] * divideTiles[1]) < m_RequestedNumberOfSplits)
      {
      if(divideTiles[i] < m_TileHint[i])
        {
        divideTiles[i]++;
        }
      // TODO: We can be more generic here
      i = (i+1)%2;
      }

    SizeType splitSize;
    splitSize[0] = (m_TileHint[0] + divideTiles[0] - 1)/ divideTiles[0];
    splitSize[1] = (m_TileHint[1] + divideTiles[1] - 1)/ divideTiles[1];

    // Fill the tiling scheme
    for(unsigned int tiley = 0; tiley < tilesPerDim[1]; ++tiley)
      {
      for(unsigned int tilex = 0; tilex < tilesPerDim[0]; ++tilex)
        {
        for(unsigned int divy = 0; divy < divideTiles[1]; ++divy)
          {
          for(unsigned int divx = 0; divx < divideTiles[0]; ++divx)
            {
            // Build the split
            RegionType newSplit;
            IndexType newSplitIndex;

            newSplitIndex[0] = tilex * m_TileHint[0] + divx * splitSize[0];
            newSplitIndex[1] = tiley * m_TileHint[1] + divy * splitSize[1];

            newSplit.SetIndex(newSplitIndex);
            newSplit.SetSize(splitSize);

            bool cropped = newSplit.Crop(m_ImageRegion);

            // If newSplit could not be cropped, it means that it is
            // outside m_ImageRegion. In this case we ignore it.
            if(cropped)
              {
              m_StreamVector.push_back(newSplit);
              }
            }
          }
        }
      }
    }
  // Finally toggle the up-to-date flag
  m_IsUpToDate = true;
  return;
}

/**
 *
 */
template <unsigned int VImageDimension>
void
ImageRegionAdaptativeSplitter<VImageDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os<<indent<<"IsUpToDate: "<<(m_IsUpToDate ? "true" : "false")<<std::endl;
  os<<indent<<"ImageRegion: "<<m_ImageRegion<<std::endl;
  os<<indent<<"Tile hint: "<<m_TileHint<<std::endl;
  os<<indent<<"Requested number of splits: "<<m_RequestedNumberOfSplits<<std::endl;
  os<<indent<<"Actual number of splits: "<<m_StreamVector.size()<<std::endl;
}

} // end namespace itk

#endif
