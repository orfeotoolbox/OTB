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

#ifndef otbContingencyTableCalculator_txx
#define otbContingencyTableCalculator_txx

#include "otbContingencyTableCalculator.h"
#include "itkVariableLengthVector.h"
#include "itkListSample.h"

#include <set>

namespace otb
{
template<class TClassLabel>
ContingencyTableCalculator<TClassLabel>::ContingencyTableCalculator()
: m_NumberOfRefClasses(0), m_NumberOfProdClasses(0), m_NumberOfSamples(0)
{}

template<class TClassLabel>
void
ContingencyTableCalculator<TClassLabel>
::Clear()
{
  m_LabelCount.clear();
  m_NumberOfRefClasses = 0;
  m_NumberOfProdClasses = 0;
  m_NumberOfSamples = 0;
}

template<class TClassLabel>
template<class TRefIterator, class TProdIterator>
void
ContingencyTableCalculator<TClassLabel>
::Compute(TRefIterator refBegin, TRefIterator refEnd, TProdIterator prodBegin, TProdIterator prodEnd)
{
    while( refBegin != refEnd && prodBegin != prodEnd )
      {
      ++m_LabelCount[refBegin.GetMeasurementVector()[0]][prodBegin.GetMeasurementVector()[0]];
      ++refBegin;
      ++prodBegin;
      ++m_NumberOfSamples;
      }
}

template<class TClassLabel>
template<class TRefIterator, class TProdIterator>
void
ContingencyTableCalculator<TClassLabel>
::Compute(TRefIterator itRef, TProdIterator itProd)
{
  while( !itRef.IsAtEnd() && !itProd.IsAtEnd() )
    {
    ++m_LabelCount[itRef.Get()][itProd.Get()];
    ++itRef;
    ++itRef;
    ++m_NumberOfSamples;
    }
}


template<class TClassLabel>
typename ContingencyTableCalculator<TClassLabel>::ContingencyTableType
ContingencyTableCalculator<TClassLabel>
::GetContingencyTable()
{
  std::set<TClassLabel> refLabels;
  std::set<TClassLabel> prodLabels;

  // Retrieve all labels needed to iterate over all labelCount
  for(typename MapOfClassesType::const_iterator refIt = m_LabelCount.begin(); refIt != m_LabelCount.end(); ++refIt)
    {
      refLabels.insert(refIt->first);
      CountMapType cmt = refIt->second;
      for(typename CountMapType::const_iterator prodIt = cmt.begin(); prodIt != cmt.end(); ++prodIt)
        {
        prodLabels.insert(prodIt->first);
        }
    }

  m_NumberOfRefClasses = refLabels.size();
  m_NumberOfProdClasses = prodLabels.size();

  unsigned int rows = static_cast<unsigned int>(m_NumberOfRefClasses);
  unsigned int cols = static_cast<unsigned int>(m_NumberOfProdClasses);

  std::vector<TClassLabel> referenceLabels;
  std::vector<TClassLabel> producedLabels;

  std::copy(refLabels.begin(), refLabels.end(), std::back_inserter(referenceLabels));
  std::copy(prodLabels.begin(), prodLabels.end(), std::back_inserter(producedLabels));

  ContingencyTableType contingencyTable(referenceLabels, producedLabels);

  for( unsigned int i = 0; i < rows; ++i )
    for( unsigned int j = 0; j < cols; ++j )
      contingencyTable.matrix(i,j) = m_LabelCount[referenceLabels[i]][producedLabels[j]];


  return contingencyTable;
}

}

#endif
