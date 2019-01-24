/*
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

#ifndef otbContingencyTableCalculator_hxx
#define otbContingencyTableCalculator_hxx

#include "otbContingencyTableCalculator.h"
#include "itkMacro.h"
#include "itkVariableLengthVector.h"
#include "itkListSample.h"

#include <set>
#include <iterator>

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

  if( refBegin != refEnd || prodBegin != prodEnd )
    itkExceptionMacro(<< "The references and produced labels did not end simultaneously.");
}

template<class TClassLabel>
template<class TRefIterator, class TProdIterator>
void
ContingencyTableCalculator<TClassLabel>
::Compute(TRefIterator itRef, TProdIterator itProd, bool refHasNoData, typename TRefIterator::InternalPixelType refNoData,
          bool prodHasNoData, typename TProdIterator::InternalPixelType prodNoData)
{
  while( !itRef.IsAtEnd() && !itProd.IsAtEnd() )
    {
    if((!prodHasNoData || itProd.Get()!=prodNoData)
       &&(!refHasNoData || itRef.Get()!=refNoData))
      {
      ++m_LabelCount[itRef.Get()][itProd.Get()];
      ++m_NumberOfSamples;
      }
    ++itRef;
    ++itProd;
    }

  if( !itRef.IsAtEnd() || !itProd.IsAtEnd() )
    itkExceptionMacro(<< "The references and produced labels did not end simultaneously.");

}


template<class TClassLabel>
typename ContingencyTableCalculator<TClassLabel>::ContingencyTablePointerType
ContingencyTableCalculator<TClassLabel>
::BuildContingencyTable()
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

  std::vector<TClassLabel> referenceLabels(refLabels.begin(), refLabels.end());
  std::vector<TClassLabel> producedLabels(prodLabels.begin(), prodLabels.end());

  ContingencyTablePointerType contingencyTable = ContingencyTableType::New();
  contingencyTable->SetLabels(referenceLabels, producedLabels);

  for( unsigned int i = 0; i < rows; ++i )
    for( unsigned int j = 0; j < cols; ++j )
      contingencyTable->matrix(i,j) = m_LabelCount[referenceLabels[i]][producedLabels[j]];


  return contingencyTable;
}

}

#endif
