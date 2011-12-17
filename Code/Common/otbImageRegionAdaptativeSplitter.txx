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
  if(!m_IsUpToDate)
    {
    // Do so if we need to
    this->EstimateSplitMap();
    }

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
  if(!m_IsUpToDate)
    {
    // Do so if we need to
    this->EstimateSplitMap();
    }

  // Return the requested split
  return m_StreamVector.at(i);
}

template <unsigned int VImageDimension>
void
ImageRegionAdaptativeSplitter<VImageDimension>
::EstimateSplitMap()
{

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
  os<<indent<<"TileHint: "<<m_TileHint<<std::endl;
  os<<indent<<"RequestedNumberOfSpits: "<<m_RequestedNumberOfSplits<<std::endl;
  os<<indent<<"StreamVector size: "<<m_StreamVector.size()<<std::endl;
}

} // end namespace itk

#endif
