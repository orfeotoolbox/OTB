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

#ifndef __otbGreyLevelCooccurrenceIndexedList_txx
#define __otbGreyLevelCooccurrenceIndexedList_txx

#include "otbGreyLevelCooccurrenceIndexedList.h"

namespace otb
{
template <class THistogram >
GreyLevelCooccurrenceIndexedList<THistogram>
::GreyLevelCooccurrenceIndexedList()
: m_Size(2)
, m_Symmetry(true)
{
}

template <class THistogram >
void
GreyLevelCooccurrenceIndexedList<THistogram>
::Initialize(SizeType size, bool symmetry)
{
  m_Size = size;
  m_Symmetry = symmetry;
  m_LookupArray = LookupArrayType(size[0] * size[1]);
  m_LookupArray.Fill(-1);
  m_TotalFrequency = 0;
}

template <class THistogram >
void
GreyLevelCooccurrenceIndexedList<THistogram>
::Normalize()
{
  //Normalize the co-occurrence indexed list
  typename VectorType::iterator it = m_Vector.begin();
  while( it != m_Vector.end())
    {
    (*it).frequency = (*it).frequency / m_TotalFrequency;
    ++it;
    }
}

template <class THistogram >
typename GreyLevelCooccurrenceIndexedList<THistogram>::RelativeFrequencyType
GreyLevelCooccurrenceIndexedList<THistogram>::
GetFrequency(IndexValueType i, IndexValueType j)
{
  double frequency = 0;
  InstanceIdentifier instanceId = i * m_Size[0] + j;
  if (instanceId < m_LookupArray.size())
    {
    int findex = m_LookupArray[instanceId];
    if(findex > -1 )
      frequency = m_Vector[findex].frequency / m_TotalFrequency;
    }
  return frequency;
}

template <class THistogram >
typename GreyLevelCooccurrenceIndexedList<THistogram>::RelativeFrequencyType
GreyLevelCooccurrenceIndexedList<THistogram>::
GetFrequency(IndexValueType i, IndexValueType j, const VectorType& vect) const
{
  double frequency = 0;
  InstanceIdentifier instanceId = i * m_Size[0] + j;
  if (instanceId < m_LookupArray.size())
    {
    int findex = m_LookupArray[instanceId];
    if(findex > -1 )
      frequency = vect[findex].frequency;
    }
  return frequency;
}

template <class THistogram >
typename GreyLevelCooccurrenceIndexedList<THistogram>::VectorType
GreyLevelCooccurrenceIndexedList<THistogram>
::GetVector()
{
  return m_Vector;
}

template <class THistogram >
void
GreyLevelCooccurrenceIndexedList<THistogram>
::AddPairToList(IndexType index)
{
  AddIndex(index);
  if( m_Symmetry )
    {
    typename IndexType::ValueType temp;
    temp = index[0];
    index[0] = index[1];
    index[1] = temp;
    AddIndex(index);
    }
}

template <class THistogram >
void
GreyLevelCooccurrenceIndexedList<THistogram>
::AddIndex(IndexType index)
{
  InstanceIdentifier instanceId = 0;
  instanceId = index[1] * m_Size[0] + index[0];
  int vindex = m_LookupArray[instanceId];
  if( vindex < 0)
    {
    m_LookupArray[instanceId] = m_Vector.size();
    CooccurrencePairType cooccur;
    cooccur.index = index;
    cooccur.frequency = 1;
    m_Vector.push_back(cooccur);
    }
  else
    {
    m_Vector[vindex].frequency++;
    }
  m_TotalFrequency = m_TotalFrequency + 1;
}

} //end namespace otb

#endif
