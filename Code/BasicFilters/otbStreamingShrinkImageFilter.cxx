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
#include "otbStreamingShrinkImageFilter.h"

//encapsulate  adaptative splitter
#include "otbImageRegionAdaptativeSplitter.h"
namespace otb
{

unsigned int
StreamingShrinkImageRegionSplitter
::GetNumberOfSplits(const RegionType& region, unsigned int requestedNumber)
{
  //New method
  // Set parameters
  this->SetImageRegion(region);
  this->SetRequestedNumberOfSplits(requestedNumber);

  // Check if we need to compute split map again
  m_Lock.Lock();
  if(!m_IsUpToDate)
    {
    // Do so if we need to
    this->EstimateSplitMap(requestedNumber);
    }
  m_Lock.Unlock();
  otbMsgDevMacro(<<"Stream vector size " << m_StreamVector.size() << std::endl);
  // Return the size of the split map
  return m_StreamVector.size();
}


StreamingShrinkImageRegionSplitter::RegionType
StreamingShrinkImageRegionSplitter
::GetSplit(unsigned int i, unsigned int numberOfPieces, const RegionType& region)
{
  // Set parameters
  this->SetImageRegion(region);

  // Check if we need to compute split map agagin
  m_Lock.Lock();
  if(!m_IsUpToDate)
    {
    // Do so if we need to
    this->EstimateSplitMap(numberOfPieces);
    }
  m_Lock.Unlock();

  // Return the requested split
  return m_StreamVector.at(i);
}

void
StreamingShrinkImageRegionSplitter
::EstimateSplitMap(unsigned int numberOfPieces)
{
  // Clear previous split map
  m_StreamVector.clear();

  //Encapsulate adpatative splitter to get a list of streams
  //adapted to the tiling scheme
  typedef ImageRegionAdaptativeSplitter<2> AdaptativeSplitterType;
  AdaptativeSplitterType::Pointer internalSplitter = AdaptativeSplitterType::New();

  internalSplitter->SetTileHint(m_TileHint);
  unsigned int nbSplits = internalSplitter->GetNumberOfSplits(m_ImageRegion,numberOfPieces);

  //Iterate over streams computed by the adaptative splitter
  for (unsigned int i = 0; i < nbSplits; ++i)
    {
    RegionType region;
    IndexType  index;

    region = internalSplitter->GetSplit(i,numberOfPieces,m_ImageRegion);
    index = region.GetIndex();

    const unsigned int sizeY = region.GetSize()[1] - 1;

    for (unsigned int j = 0; j < sizeY; ++j)
      {
      //Add region aligned with the shrink factor
      if (((index[1]+j) % m_TileSizeAlignment) == 0)
        {
        RegionType splitRegion;

        splitRegion.SetIndex(0, index[0]);
        splitRegion.SetIndex(1, index[1]+j);

        splitRegion.SetSize(0, region.GetSize()[0]);
        splitRegion.SetSize(1, 1);

        // Handle the borders
        splitRegion.Crop(region);

        //Add this split to the vector (we keep the order given by
        //the adaptative splitter)
        m_StreamVector.push_back(splitRegion);
        }

      }
    }
  m_IsUpToDate = true;
  return;
}

void
StreamingShrinkImageRegionSplitter
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "SplitsPerDimension : " << m_SplitsPerDimension << std::endl;
  os << indent << "TileDimension      : " << m_TileDimension << std::endl;
  os << indent << "TileSizeAlignment  : " << m_TileSizeAlignment << std::endl;
}

} // End namespace otb
