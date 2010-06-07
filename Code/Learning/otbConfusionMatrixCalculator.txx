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
::SetProducedLabels( const ListLabelType * prodLabs)
{
  this->ProcessObject::SetNthInput(0, const_cast< ListLabelType * >( prodLabs ) );
}


template < class TListLabel >
const TListLabel *
ConfusionMatrixCalculator<TListLabel>
::GetProducedLabels( ) const
  {
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }

  return dynamic_cast<const ListLabelType * >(this->ProcessObject::GetInput(1) );
}


template < class TListLabel > 
void
ConfusionMatrixCalculator<TListLabel>
::SetReferenceLabels( const ListLabelType * refLabs)
{
  this->ProcessObject::SetNthInputSetNthInput(0, const_cast< ListLabelType * >( refLabs ) );
  m_ReferenceLabels =   this->GetReferenceLabels();

  typename ListLabelType::ConstIterator  refIterator = m_ReferenceLabels->Begin();

   while( refIterator != m_ReferenceLabels->End() )
    {

    int currentLabel = refIterator.GetMeasurementVector()[0];
    if(find(m_VecOfClasses.begin(), m_VecOfClasses.end(), currentLabel) == m_VecOfClasses.end())
    m_VecOfClasses.push_back(currentLabel);

    ++refIterator;

    }

  m_NumberOfClasses = m_VecOfClasses.size();


}


template < class TListLabel >
const TListLabel *
ConfusionMatrixCalculator<TListLabel>
::GetReferenceLabels( ) const
  {
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return dynamic_cast<const ListLabelType * >(this->ProcessObject::GetInput(0) );
}


template < class TListLabel > 
void
ConfusionMatrixCalculator<TListLabel>
::GenerateData()
{
  
  typename ListLabelType::ConstPointer refLabels = this->GetReferenceLabels();
  typename ListLabelType::ConstPointer prodLabels = this->GetProducedLabels();
  
  

  
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
