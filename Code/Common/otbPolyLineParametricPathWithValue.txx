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
#ifndef __otbPolyLineParametricPathWithValue_txx
#define __otbPolyLineParametricPathWithValue_txx

#include "otbPolyLineParametricPathWithValue.h"

#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TValue, unsigned int VDimension>
PolyLineParametricPathWithValue<TValue, VDimension>
::PolyLineParametricPathWithValue() :
  m_Key("Value"), m_Length(-1.0), m_LengthIsValid(false),
  m_BoundingRegion(), m_BoundingRegionIsValid(false)
{
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
  ValueType v;
  itk::EncapsulateMetaData<ValueType>(dict, m_Key, itk::NumericTraits<ValueType>::ZeroValue(v));
}

template <class TValue, unsigned int VDimension>
void PolyLineParametricPathWithValue<TValue, VDimension>
::AddVertex(const ContinuousIndexType& vertex)
{
  Superclass::AddVertex(vertex);
  this->Modified();
}

template <class TValue, unsigned int VDimension>
double PolyLineParametricPathWithValue<TValue, VDimension>
::GetLength() const
{
  if (!m_LengthIsValid)
    {
    ComputeLength();
    }
  return m_Length;
}

template <class TValue, unsigned int VDimension>
void
PolyLineParametricPathWithValue<TValue, VDimension>
::ComputeLength() const
{
  double                      length = 0.0;
  VertexListConstIteratorType it =  this->GetVertexList()->Begin();

  if (this->GetVertexList()->Size() > 1)
    {

    VertexType pt1 = it.Value(); //just init, won't be used like that
    VertexType pt2 = it.Value();

    ++it;
    while (it != this->GetVertexList()->End())
      {
      pt1 = pt2;
      pt2 = it.Value();
      double accum = 0.0;
      for (unsigned int i = 0; i < VDimension; ++i)
        {
        accum += (pt1[i] - pt2[i]) * (pt1[i] - pt2[i]);
        }
      length += vcl_sqrt(accum);
      ++it;
      }

    }
  else //if there is strictly less than 2 points, length is 0
    {
    length = 0.0;
    }

  m_Length = length;
  m_LengthIsValid = true;
}

/**
 * PrintSelf Method
 */
template <class TValue, unsigned int VDimension>
void
PolyLineParametricPathWithValue<TValue, VDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  VertexListConstIteratorType it =  this->GetVertexList()->Begin();
  while (it != this->GetVertexList()->End())
    {
    os << it.Value() << " - ";
    ++it;
    }
  os << std::endl;
}

template <class TValue, unsigned int VDimension>
typename PolyLineParametricPathWithValue<TValue, VDimension>
::RegionType
PolyLineParametricPathWithValue<TValue, VDimension>
::GetBoundingRegion() const
{
  if (!m_BoundingRegionIsValid)
    {
    ComputeBoundingRegion();
    }
  return m_BoundingRegion;
}

/**
 * Bounding Box computation
 */
template <class TValue, unsigned int VDimension>
void
PolyLineParametricPathWithValue<TValue, VDimension>
::ComputeBoundingRegion() const
{
  SizeType  size;
  IndexType index;

  size.Fill(0);
  index.Fill(0);

  IndexType maxId;
  maxId.Fill(0);

  VertexListConstIteratorType it =  this->GetVertexList()->Begin();

  double x = 0.0;
  double y = 0.0;

  if (this->GetVertexList()->Size() > 0)
    {
    x = static_cast<double>(it.Value()[0]);
    y = static_cast<double>(it.Value()[1]);
    index[0] = x;
    index[1] = y;
    maxId[0] = x;
    maxId[1] = y;

    ++it;
    while (it != this->GetVertexList()->End())
      {
      x = static_cast<double>(it.Value()[0]);
      y = static_cast<double>(it.Value()[1]);

      // Index search
      if (x < index[0])
        {
        index[0] = x;
        }
      if (y < index[1])
        {
        index[1] = y;
        }
      // Max Id search for size computation
      if (x > maxId[0])
        {
        maxId[0] = x;
        }
      if (y > maxId[1])
        {
        maxId[1] = y;
        }

      ++it;
      }

    size[0] = maxId[0] - index[0];
    size[1] = maxId[1] - index[1];
    }
  m_BoundingRegion.SetSize(size);
  m_BoundingRegion.SetOrigin(index);
  m_BoundingRegionIsValid = true;
}

template <class TValue, unsigned int VDimension>
void
PolyLineParametricPathWithValue<TValue, VDimension>
::Modified() const
{
  m_LengthIsValid = false;
  m_BoundingRegionIsValid = false;
}

} // end namespace otb
#endif
