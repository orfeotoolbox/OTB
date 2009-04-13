/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSubsampledImageRegionConstIterator_txx
#define __otbSubsampledImageRegionConstIterator_txx

#include "otbSubsampledImageRegionConstIterator.h"

namespace otb {


template < class TImage >
void
SubsampledImageRegionConstIterator< TImage >
::Increment ()
{
  // Get the index of the last pixel on the span (row)
  IndexType ind = this->m_Image->ComputeIndex( static_cast<typename Superclass::OffsetValueType>(this->m_Offset) );

  const IndexType& startIndex = this->m_Region.GetIndex();
  const SizeType& size = this->m_Region.GetSize();

  // Increment along a row, then wrap at the end of the region row.
  unsigned int dim;

  // Check to see if we are past the last pixel in the region
  // Note that ++ind[0] moves to the next pixel along the row.
  ind[0] += m_SubsampleFactor[0];
  bool done = (ind[0] > m_LastUsableIndex[0] );
  for (unsigned int i=1; done && i < ImageIteratorDimension; i++)
  {
    done = (ind[i] >= m_LastUsableIndex[i]);
  }
  
  // if the iterator is outside the region (but not past region end) then
  // we need to wrap around the region
  dim = 0;
  if (!done)
  {
    while ( ( dim+1 < ImageIteratorDimension )
      && (ind[dim] > m_LastUsableIndex[dim]) )
    {
      ind[dim] = startIndex[dim];
      dim++;
      ind[dim] += m_SubsampleFactor[dim];
    }
  }
  this->m_Offset = this->m_Image->ComputeOffset( ind );
  this->m_SpanEndOffset = this->m_Offset 
                            + static_cast<IndexValueType>( m_SubsampleFactor[0] * ((size[0]-1) / m_SubsampleFactor[0]) )
                            + 1;
  this->m_SpanBeginOffset = this->m_Offset;
}

template < class TImage >
void
SubsampledImageRegionConstIterator< TImage >
::Decrement ()
{
  // Get the index of the first pixel on the span (row)
  IndexType ind = this->m_Image->ComputeIndex( static_cast<IndexValueType>(this->m_Offset) );
  IndexType& startIndex = this->m_Region.GetIndex();

  // Deccrement along a row, then wrap at the beginning of the region row.
  bool done;
  unsigned int dim;

  // Check to see if we are past the first pixel in the region
  // Note that --ind[0] moves to the previous pixel along the row.
  ind[0] -= m_SubsampleFactor[0];
  done = (ind[0] <= startIndex[0] - 1);
  for (unsigned int i=1; done && i < ImageIteratorDimension; i++)
  {
    done = (ind[i] <= startIndex[i]);
  }
  
  // if the iterator is outside the region (but not past region begin) then
  // we need to wrap around the region
  dim = 0;
  if (!done)
  {
    while ( (dim < ImageIteratorDimension - 1)
            && (ind[dim] < startIndex[dim]) )
    {
      ind[dim] = m_LastUsableIndex[dim];
      dim++;
      ind[dim] -= m_SubsampleFactor[dim];
    }
  }
  this->m_Offset = this->m_Image->ComputeOffset( ind );
  this->m_SpanEndOffset = this->m_Offset + 1;
  this->m_SpanBeginOffset = this->m_Offset - m_LastUsableIndex[0];
}

} // end of namespace otb


#endif

