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

namespace otb
{
template<class TListLabel>
ConfusionMatrixCalculator<TListLabel>
::ConfusionMatrixCalculator() :
  m_KappaIndex(0.0), m_OverallAccuracy(0.0), m_NumberOfClasses(0)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses,m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);
  m_ReferenceLabels = ListLabelType::New();
  m_ProducedLabels = ListLabelType::New();
}


template < class TListLabel > 
void 
ConfusionMatrixCalculator<TListLabel>
::Update()
{
  this->GenerateData();
}


template < class TListLabel > 
void
ConfusionMatrixCalculator<TListLabel>
::GenerateData()
{
  
  typename ListLabelType::ConstPointer refLabels = this->GetReferenceLabels();
  typename ListLabelType::ConstPointer prodLabels = this->GetProducedLabels();
  
  typename ListLabelType::ConstIterator  refIterator = m_ReferenceLabels->Begin();

   while( refIterator != m_ReferenceLabels->End() )
    {

    int currentLabel = refIterator.GetMeasurementVector()[0];
    if(find(m_VecOfClasses.begin(), m_VecOfClasses.end(), currentLabel) == m_VecOfClasses.end())
    m_VecOfClasses.push_back(currentLabel);

    ++refIterator;

    }

  m_NumberOfClasses = m_VecOfClasses.size();
  

  
  typename ListLabelType::ConstIterator  prodIterator = prodLabels->Begin();



}


template < class TListLabel > 
void
ConfusionMatrixCalculator<TListLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "TODO";
}

}

#endif
