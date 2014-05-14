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
template <class TPixel>
GreyLevelCooccurrenceIndexedList<TPixel>::
GreyLevelCooccurrenceIndexedList():
m_Size(),
m_Symmetry(true),
m_TotalFrequency(0),
m_InputImageMinimum(0),
m_InputImageMaximum(255),
m_ClipBinsAtEnds(true)
{

}

template <class TPixel >
void
GreyLevelCooccurrenceIndexedList<TPixel>::
Initialize(const unsigned int nbins, const PixelValueType min,
                  const PixelValueType max, const bool symmetry)
{
  PixelPairType lowerBound;
  PixelPairType upperBound;

  m_Size.Fill(nbins);
  m_InputImageMinimum = min;
  m_InputImageMaximum = max;
  lowerBound.Fill(min);
  upperBound.Fill(max+1);

  m_Symmetry = symmetry;
  m_LookupArray = LookupArrayType(m_Size[0] * m_Size[1]);
  m_LookupArray.Fill(-1);
  m_TotalFrequency = 0;

  // adjust the sizes of min max value containers
  unsigned int dim;
  m_Min.resize( PixelPairSize );
  for ( dim = 0; dim < PixelPairSize; dim++ )
    {
    m_Min[dim].resize(m_Size[dim]);
    }

  m_Max.resize( PixelPairSize );
  for ( dim = 0; dim < PixelPairSize; dim++ )
    {
    m_Max[dim].resize(m_Size[dim]);
    }

  float interval;
  for ( unsigned int i = 0; i < PixelPairSize; i++ )
    {
    if ( m_Size[i] > 0 )
      {
      interval = static_cast<float>( upperBound[i] - lowerBound[i] )
        / static_cast< PixelValueType >( m_Size[i] );

      // Set the min vector and max vector
      for ( unsigned int j = 0; j < static_cast< unsigned int >( m_Size[i] - 1 ); j++ )
        {
        this->SetBinMin( i, j, (PixelValueType)( lowerBound[i]
                                                 + ( (float)j * interval ) ) );
        this->SetBinMax( i, j, (PixelValueType)( lowerBound[i]
                                                 + ( ( (float)j + 1 ) * interval ) ) );
        }
      this->SetBinMin( i, m_Size[i] - 1,
                       (PixelValueType)( lowerBound[i]
                                         + ( ( (float)m_Size[i] - 1 ) * interval ) ) );
      this->SetBinMax( i, m_Size[i] - 1,
                       (PixelValueType)( upperBound[i] ) );
      }
    }
}

template <class TPixel >
void
GreyLevelCooccurrenceIndexedList<TPixel>::
SetBinMin(const unsigned int dimension, const InstanceIdentifier nbin,
            PixelValueType min)
{
  m_Min[dimension][nbin] = min;
}

template <class TPixel >
void
GreyLevelCooccurrenceIndexedList<TPixel>::
SetBinMax(const unsigned int dimension, const InstanceIdentifier nbin,
            PixelValueType max)
{
  m_Max[dimension][nbin] = max;
}

template <class TPixel >
bool
GreyLevelCooccurrenceIndexedList<TPixel>::
GetIndex(const PixelPairType & pixelPair, IndexType & index) const
{
  // now using something similar to binary search to find
  // index.
  unsigned int dim;

  int begin;
  int mid;
  int end;

  PixelValueType median;
  PixelValueType tempPixelValue;

  for ( dim = 0; dim < PixelPairSize; dim++ )
    {
    tempPixelValue = pixelPair[dim];
    begin = 0;

    if ( tempPixelValue < m_Min[dim][begin] )
      {
      // one of measurement is below the minimum
      // its ok if we extend the bins to infinity.. not ok if we don't
      if ( !m_ClipBinsAtEnds )
        {
        index[dim] = (IndexValueType)0;
        continue;
        }
      else
        {   // set an illegal value and return 0
        index[dim] = (IndexValueType)m_Size[dim];
        return false;
        }
      }

    end = m_Min[dim].size() - 1;
    if ( tempPixelValue >= m_Max[dim][end] )
      {
      // one of measurement is above the maximum
      // its ok if we extend the bins to infinity.. not ok if we don't
      // Need to include the last endpoint in the last bin.
      if ( !m_ClipBinsAtEnds || tempPixelValue ==  m_Max[dim][end])
        {
        index[dim] = (IndexValueType)m_Size[dim] - 1;
        continue;
        }
      else
        {   // set an illegal value and return 0
        index[dim] = (IndexValueType)m_Size[dim];
        return false;
        }
      }

    // Binary search for the bin where this measurement could be
    mid = ( end + 1 ) / 2;
    median = m_Min[dim][mid];

    while ( true )
      {
      if ( tempPixelValue < median )
        {
        end = mid - 1;
        }
      else if ( tempPixelValue > median )
        {
        // test whether it is inside the current bin by comparing to the max of
        // this bin.
        if ( tempPixelValue <  m_Max[dim][mid]
             && tempPixelValue >= m_Min[dim][mid] )
          {
          index[dim] = mid;
          break;
          }
        // otherwise, continue binary search
        begin = mid + 1;
        }
      else
        {
        index[dim] = mid;
        break;
        }
      mid = begin + ( end - begin ) / 2;
      median = m_Min[dim][mid];
      } // end of while
    }   // end of for()
  return true;
}

template <class TPixel >
void
GreyLevelCooccurrenceIndexedList<TPixel>::
AddPixelPair(const PixelValueType& pixelvalue1, const PixelValueType& pixelvalue2)
{

  if ( pixelvalue1 < m_InputImageMinimum
       || pixelvalue1 > m_InputImageMaximum )
    {
    return; // don't put a pixel in the histogram if the value
            // is out-of-bounds.
    }

  if ( pixelvalue2 < m_InputImageMinimum
       || pixelvalue2 > m_InputImageMaximum )
    {
    return; // don't put a pixel in the histogram if the value
           // is out-of-bounds.
    }

  IndexType index;
  PixelPairType ppair( PixelPairSize);
  ppair[0] = pixelvalue1;
  ppair[1] = pixelvalue2;

  //Get Index of the histogram for the given pixel pair;
  this->GetIndex(ppair, index);
  //Add the index and set/update the frequency of the pixel pair. if m_Symmetry
  //is true the index is swapped and added to vector again.
  this->AddPairToVector(index);
  if(m_Symmetry)
    {
    IndexValueType temp;
    temp = index[0];
    index[0] = index[1];
    index[1] = temp;
    this->AddPairToVector(index);
    }
}

template <class TPixel>
typename GreyLevelCooccurrenceIndexedList<TPixel>::RelativeFrequencyType
GreyLevelCooccurrenceIndexedList<TPixel>::
GetFrequency(IndexValueType i, IndexValueType j)
{
  double frequency = 0;
  InstanceIdentifier instanceId = i * m_Size[0] + j;
  if (instanceId < m_LookupArray.size())
    {
    int findex = m_LookupArray[instanceId];
    if(findex > -1 )
      frequency = m_Vector[findex].second / m_TotalFrequency;
    }
  return frequency;
}

template <class TPixel>
typename GreyLevelCooccurrenceIndexedList<TPixel>::RelativeFrequencyType
GreyLevelCooccurrenceIndexedList<TPixel>::
GetFrequency(IndexValueType i, IndexValueType j, const VectorType& vect) const
{
  double frequency = 0;
  InstanceIdentifier instanceId = i * m_Size[0] + j;
  if (instanceId < m_LookupArray.size())
    {
    int findex = m_LookupArray[instanceId];
    if(findex > -1 )
      frequency = vect[findex].second;
    }
  return frequency;
}

template <class TPixel>
typename GreyLevelCooccurrenceIndexedList<TPixel>::VectorType
GreyLevelCooccurrenceIndexedList<TPixel>
::GetVector()
{
  return m_Vector;
}

template <class TPixel>
void
GreyLevelCooccurrenceIndexedList<TPixel>
::AddPairToVector(IndexType index)
{
  InstanceIdentifier instanceId = 0;
  instanceId = index[1] * m_Size[0] + index[0];
  int vindex = m_LookupArray[instanceId];
  if( vindex < 0)
    {
    m_LookupArray[instanceId] = m_Vector.size();

    CooccurrencePairType cooccur;
    cooccur = std::make_pair(index,1);
    m_Vector.push_back(cooccur);
    }
  else
    {
    m_Vector[vindex].second++;
    }
  m_TotalFrequency = m_TotalFrequency + 1;
}

template <class TPixel>
void
GreyLevelCooccurrenceIndexedList<TPixel>
::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Symmetry: " << this->m_Symmetry << std::endl;
  os << indent << "TotalFrequency: " << this->m_TotalFrequency << std::endl;
  os << indent << "Size: " << m_Size;
  os << indent << "CooccurrenceIndexedList: " << std::endl;
  typename VectorType::const_iterator it;
  for (it = m_Vector.begin(); it != m_Vector.end(); ++it)
    {
    std::cerr << "index=" << (*it).first << ", frequency=" << (*it).second << std::endl;
    }
  std::cerr << std::endl;
}

} //end namespace otb

#endif
