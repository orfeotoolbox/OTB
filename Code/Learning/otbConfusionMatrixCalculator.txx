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
#ifndef __otbConfusionMatrixCalculator_txx
#define __otbConfusionMatrixCalculator_txx

#include <algorithm>
#include "otbMacro.h"

namespace otb
{
template<class TRefListLabel, class TProdListLabel>
ConfusionMatrixCalculator<TRefListLabel,TProdListLabel>
::ConfusionMatrixCalculator() :
  m_KappaIndex(0.0), m_OverallAccuracy(0.0), m_NumberOfClasses(0)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses,m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);
  m_ReferenceLabels = RefListLabelType::New();
  m_ProducedLabels = ProdListLabelType::New();
}


template < class TRefListLabel, class TProdListLabel > 
void 
ConfusionMatrixCalculator<TRefListLabel,TProdListLabel>
::Update()
{
  this->GenerateData();
}


template < class TRefListLabel, class TProdListLabel > 
void
ConfusionMatrixCalculator<TRefListLabel,TProdListLabel>
::GenerateData()
{
  
  typename RefListLabelType::ConstIterator  refIterator = m_ReferenceLabels->Begin();
  typename ProdListLabelType::ConstIterator  prodIterator = m_ProducedLabels->Begin();

  //check that both lists have the same number of samples

  if( m_ReferenceLabels->Size() != m_ProducedLabels->Size() )
    {
    otbMsgDebugMacro(<< "refLabels size = " << m_ReferenceLabels->Size() <<
                        " / proLabels size = " << m_ProducedLabels->Size());
        throw itk::ExceptionObject(__FILE__, __LINE__, "ListSample size missmatch", ITK_LOCATION);
    }

  // count de number of classes

  int countClasses = 0;
   while( refIterator != m_ReferenceLabels->End() )
    {

    int currentLabel = refIterator.GetMeasurementVector()[0];
    if(m_MapOfClasses.find(currentLabel) == m_MapOfClasses.end())
      {
      m_MapOfClasses[currentLabel] = countClasses;
      ++countClasses;
      }
    ++refIterator;

    }

  m_NumberOfClasses = countClasses;

  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses, m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);

  refIterator = m_ReferenceLabels->Begin();
  prodIterator = m_ProducedLabels->Begin();

  while( refIterator != m_ReferenceLabels->End() )
    {
    int refLabel = refIterator.GetMeasurementVector()[0];
    int prodLabel = prodIterator.GetMeasurementVector()[0];

    int refPos = m_MapOfClasses[refLabel];
    int prodPos = m_MapOfClasses[prodLabel];

    m_ConfusionMatrix( refPos,prodPos )+=1;

    ++refIterator;
    ++prodIterator;

    }
  
}


template < class TRefListLabel, class TProdListLabel > 
void
ConfusionMatrixCalculator<TRefListLabel,TProdListLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "TODO";
}

}

#endif
