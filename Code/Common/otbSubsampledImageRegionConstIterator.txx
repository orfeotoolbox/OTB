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

template <class TImage>
SubsampledImageRegionConstIterator<TImage>
::SubsampledImageRegionConstIterator()
  : itk::ImageRegionConstIterator<TImage> ()
{
  m_SubsampleFactor.Fill(1);
  m_SubSampledEndOffset = this->m_EndOffset;

  const IndexType& startIndex = this->m_Region.GetIndex();
  const SizeType&  size = this->m_Region.GetSize();

  for (unsigned int i = 0; i < ImageIteratorDimension; ++i)
    {
    m_FirstUsableIndex[i] = startIndex[i];
    m_LastUsableIndex[i] = startIndex[i] + static_cast<IndexValueType>(size[i] - 1);
    }
}

template <class TImage>
SubsampledImageRegionConstIterator<TImage>
::SubsampledImageRegionConstIterator (const ImageType *ptr, const RegionType& region)
  : itk::ImageRegionConstIterator<TImage> (ptr, region)
{
  m_SubsampleFactor.Fill(1);
  m_SubSampledEndOffset = this->m_EndOffset;

  const IndexType& startIndex = this->m_Region.GetIndex();
  const SizeType&  size = this->m_Region.GetSize();

  for (unsigned int i = 0; i < ImageIteratorDimension; ++i)
    {
    m_FirstUsableIndex[i] = startIndex[i];
    m_LastUsableIndex[i] = startIndex[i] + static_cast<IndexValueType>(size[i] - 1);
    }
}

template <class TImage>
SubsampledImageRegionConstIterator<TImage>
::SubsampledImageRegionConstIterator(const itk::ImageIterator<TImage>& it)
  : itk::ImageRegionConstIterator<TImage>(it)
{
  m_SubsampleFactor.Fill(1);
  m_SubSampledEndOffset = this->m_EndOffset;

  const IndexType& startIndex = this->m_Region.GetIndex();
  const SizeType&  size = this->m_Region.GetSize();

  for (unsigned int i = 0; i < ImageIteratorDimension; ++i)
    {
    m_FirstUsableIndex[i] = startIndex[i];
    m_LastUsableIndex[i] = startIndex[i] + static_cast<IndexValueType>(size[i] - 1);
    }
}

template <class TImage>
SubsampledImageRegionConstIterator<TImage>
::SubsampledImageRegionConstIterator(const itk::ImageConstIterator<TImage>& it)
  : itk::ImageRegionConstIterator<TImage>(it)
{
  m_SubsampleFactor.Fill(1);

  const IndexType& startIndex = this->m_Region.GetIndex();
  const SizeType&  size = this->m_Region.GetSize();

  for (unsigned int i = 0; i < ImageIteratorDimension; ++i)
    {
    m_FirstUsableIndex[i] = startIndex[i];
    m_LastUsableIndex[i] = startIndex[i] + static_cast<IndexValueType>(size[i] - 1);
    }

  m_SubSampledEndOffset = this->m_Image->ComputeOffset(m_LastUsableIndex) + 1;
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::SetSubsampleFactor(typename IndexType::IndexValueType factor)
{
  IndexType index;
  index.Fill(factor);
  SetSubsampleFactor(index);
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::SetSubsampleFactor(const IndexType& factor)
{
  this->m_SubsampleFactor = factor;

  // Evaluate the last possible pixel.
  const IndexType& startIndex = this->m_Region.GetIndex();
  const SizeType&  size = this->m_Region.GetSize();

  for (unsigned int i = 0; i < ImageIteratorDimension; ++i)
    {
    m_FirstUsableIndex[i] = startIndex[i];
    while (m_FirstUsableIndex[i]
           != (m_SubsampleFactor[i] * (m_FirstUsableIndex[i] / m_SubsampleFactor[i])))
      {
      ++m_FirstUsableIndex[i];
      }
    m_LastUsableIndex[i] = startIndex[i]
                           + static_cast<IndexValueType>(m_SubsampleFactor[i] * ((size[i] - 1) / m_SubsampleFactor[i]));
    }

  m_SubSampledBeginOffset = this->m_Image->ComputeOffset(m_FirstUsableIndex);

  //m_SubSampledReverseEndOffset = m_SubSampledBeginOffset - 1;
  m_SubSampledEndOffset = this->m_Image->ComputeOffset(m_LastUsableIndex) + 1;
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::GoToBegin()
{
  this->m_Offset = m_SubSampledBeginOffset;

  const SizeType& size = this->m_Region.GetSize();

  this->m_SpanBeginOffset = this->m_Offset;
  this->m_SpanEndOffset = this->m_Offset
                          + static_cast<IndexValueType>(m_SubsampleFactor[0] * ((size[0] - 1) / m_SubsampleFactor[0]))
                          + 1;
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::GoToEnd()
{
  this->m_Offset = m_SubSampledEndOffset - 1;
  this->m_SpanEndOffset = this->m_Offset + 1;
  this->m_SpanBeginOffset = this->m_Offset - m_LastUsableIndex[0];
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::SetIndex(const IndexType& ind)
{
  IndexType theIndex = ind;

  for (unsigned int i = 0; i < ImageIteratorDimension; ++i)
    {
    while (theIndex[i]
           != (m_SubsampleFactor[i] * (theIndex[i] / m_SubsampleFactor[i])))
      {
      ++theIndex[i];
      }

    if (theIndex[i] >  static_cast<IndexValueType>(this->m_Region.GetIndex()[i] + this->m_Region.GetSize()[i]))
      {
      theIndex[i] = ind[i];
      while (theIndex[i]
             != (m_SubsampleFactor[i] * (theIndex[i] / m_SubsampleFactor[i])))
        {
        --theIndex[i];
        }

      if (theIndex[i] < this->m_Region.GetIndex()[i]) theIndex[i] = ind[i];
      }
    }
  //Superclass::SetIndex( theIndex );

  OffsetType theOffset = this->m_Image->ComputeOffset(theIndex);
  SetOffset(theOffset);
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::SetOffset(const OffsetType& offset)
{
  this->m_Offset = offset;

  const SizeType& size = this->m_Region.GetSize();

  this->m_SpanBeginOffset = this->m_Offset;
  this->m_SpanEndOffset = this->m_Offset
                          + static_cast<IndexValueType>(m_SubsampleFactor[0] * ((size[0] - 1) / m_SubsampleFactor[0]))
                          + 1;

}

template <class TImage>
typename SubsampledImageRegionConstIterator<TImage>::RegionType
SubsampledImageRegionConstIterator<TImage>
::GenerateOutputInformation() const
{
  IndexType startIndex = this->m_Region.GetIndex();
  SizeType  size = this->m_Region.GetSize();

  for (unsigned int i = 0; i < ImageIteratorDimension; ++i)
    {
    startIndex[i] /= m_SubsampleFactor[i];
    --size[i];
    size[i] /= m_SubsampleFactor[i];
    ++size[i];
    }

  RegionType newRegion;
  newRegion.SetIndex(startIndex);
  newRegion.SetSize(size);

#if 0
// #ifndef NDEBUG
  std::cerr << "InitialImageSize (";
  for (unsigned int i = 0; i < ImageIteratorDimension - 1; ++i)
    std::cerr << this->m_Region.GetSize()[i] << ", ";
  std::cerr << this->m_Region.GetSize()[ImageIteratorDimension - 1] << ") with index (";
  for (unsigned int i = 0; i < ImageIteratorDimension - 1; ++i)
    std::cerr << this->m_Region.GetIndex()[i] << ", ";
  std::cerr << this->m_Region.GetIndex()[ImageIteratorDimension - 1] << ")\n";

  std::cerr << "NewRegionSize (";
  for (unsigned int i = 0; i < ImageIteratorDimension - 1; ++i)
    std::cerr << size[i] << ", ";
  std::cerr << size[ImageIteratorDimension - 1] << ") with index (";
  for (unsigned int i = 0; i < ImageIteratorDimension - 1; ++i)
    std::cerr << startIndex[i] <<  ", ";
  std::cerr << startIndex[ImageIteratorDimension - 1] << ")\n";

  std::cerr << "FirstIndex (";
  for (unsigned int i = 0; i < ImageIteratorDimension - 1; ++i)
    std::cerr << m_FirstUsableIndex[i] << ", ";
  std::cerr << m_FirstUsableIndex[ImageIteratorDimension - 1] << ") offset=";
  std::cerr << m_SubSampledBeginOffset << "\n";

  std::cerr << "LastIndex (";
  for (unsigned int i = 0; i < ImageIteratorDimension - 1; ++i)
    std::cerr << m_LastUsableIndex[i] << ", ";
  std::cerr << m_LastUsableIndex[ImageIteratorDimension - 1] << ") offset=";
  std::cerr << m_SubSampledEndOffset << "\n";
#endif

  return newRegion;
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::Increment()
{
  // Get the index of the last pixel on the span (row)
  IndexType ind = this->m_Image->ComputeIndex(
    static_cast<typename TImage::OffsetValueType>(this->m_Offset));

  const IndexType& startIndex = this->m_Region.GetIndex();
  const SizeType&  size = this->m_Region.GetSize();

  // Increment along a row, then wrap at the end of the region row.
  unsigned int dim;

  // Check to see if we are past the last pixel in the region
  // Note that ++ind[0] moves to the next pixel along the row.
  ind[0] += m_SubsampleFactor[0];
  bool done = (ind[0] > m_LastUsableIndex[0]);
  for (unsigned int i = 1; done && i < ImageIteratorDimension; ++i)
    {
    done = (ind[i] >= m_LastUsableIndex[i]);
    }

  // if the iterator is outside the region (but not past region end) then
  // we need to wrap around the region
  dim = 0;
  if (!done)
    {
    while ((dim + 1 < ImageIteratorDimension)
           && (ind[dim] > m_LastUsableIndex[dim]))
      {
      ind[dim] = startIndex[dim];
      ++dim;
      ind[dim] += m_SubsampleFactor[dim];
      }
    }
  this->m_Offset = this->m_Image->ComputeOffset(ind);
  this->m_SpanEndOffset = this->m_Offset
                          + static_cast<IndexValueType>(m_SubsampleFactor[0] * ((size[0] - 1) / m_SubsampleFactor[0]))
                          + 1;
  this->m_SpanBeginOffset = this->m_Offset;
}

template <class TImage>
void
SubsampledImageRegionConstIterator<TImage>
::Decrement()
{
  // Get the index of the first pixel on the span (row)
  IndexType  ind = this->m_Image->ComputeIndex(static_cast<IndexValueType>(this->m_Offset));
  IndexType& startIndex = this->m_Region.GetIndex();

  // Deccrement along a row, then wrap at the beginning of the region row.
  bool         done;
  unsigned int dim;

  // Check to see if we are past the first pixel in the region
  // Note that --ind[0] moves to the previous pixel along the row.
  ind[0] -= m_SubsampleFactor[0];
  done = (ind[0] <= startIndex[0] - 1);
  for (unsigned int i = 1; done && i < ImageIteratorDimension; ++i)
    {
    done = (ind[i] <= startIndex[i]);
    }

  // if the iterator is outside the region (but not past region begin) then
  // we need to wrap around the region
  dim = 0;
  if (!done)
    {
    while ((dim < ImageIteratorDimension - 1)
           && (ind[dim] < startIndex[dim]))
      {
      ind[dim] = m_LastUsableIndex[dim];
      ++dim;
      ind[dim] -= m_SubsampleFactor[dim];
      }
    }
  this->m_Offset = this->m_Image->ComputeOffset(ind);
  this->m_SpanEndOffset = this->m_Offset + 1;
  this->m_SpanBeginOffset = this->m_Offset - m_LastUsableIndex[0];
}

} // end of namespace otb

#endif
