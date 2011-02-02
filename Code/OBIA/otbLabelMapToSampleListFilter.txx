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
#ifndef __otbLabelMapToSampleListFilter_txx
#define __otbLabelMapToSampleListFilter_txx

#include "otbLabelMapToSampleListFilter.h"

namespace otb
{

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::LabelMapToSampleListFilter()
{
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::~LabelMapToSampleListFilter()
{
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
void
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::Compute()
{
  m_OutputSampleList = OutputSampleListType::New();
  
  typename InputLabelMapType::LabelObjectContainerType::const_iterator it
    = m_InputLabelMap->GetLabelObjectContainer().begin();

  // iterate on label objects
  while(it != m_InputLabelMap->GetLabelObjectContainer().end())
    {
    m_OutputSampleList->PushBack(m_MeasurementFunctor(it->second));
    ++it;
    }
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
void
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
