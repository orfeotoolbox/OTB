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

#ifndef _otbHistogramStatisticFunction_txx
#define _otbHistogramStatisticFunction_txx

#include "otbHistogramStatisticFunction.h"

namespace otb
{

template< class TInputHistogram, class TOutput >
HistogramStatisticFunction< TInputHistogram, TOutput>
::HistogramStatisticFunction()
{
}
                                                    
template< class TInputHistogram, class TOutput >
typename HistogramStatisticFunction< TInputHistogram, TOutput>::OutputType
HistogramStatisticFunction< TInputHistogram, TOutput>
::GetEntropy()
{
  typename TInputHistogram::ConstPointer histogram = this->GetInputHistogram();

  // TODO: as an improvement, the class could accept multi-dimensional histograms
  // and the user could specify the dimension to apply the algorithm to.
  if (histogram->GetSize().GetSizeDimension() != 1)
    {
    itkExceptionMacro(<<"Histogram must be 1-dimensional.");
    }

  // compute global mean
  typename TInputHistogram::ConstIterator iter = histogram->Begin() ;
  typename TInputHistogram::ConstIterator end = histogram->End() ;

  EntropyType entropy = itk::NumericTraits<EntropyType>::Zero;
  FrequencyType globalFrequency = histogram->GetTotalFrequency();
  
  while (iter != end)
    {
    EntropyType Proba = static_cast<EntropyType>(iter.GetMeasurementVector()[0]);
    Proba /= static_cast<EntropyType>(globalFrequency);
    
    entropy +=  Proba * log (Proba); 
    ++iter ;
    }
  m_entropy = static_cast<OutputType>(entropy); 
  return m_entropy;
}



template< class TInputHistogram, class TOutput >
void
HistogramStatisticFunction< TInputHistogram, TOutput>
::GenerateData()
{
  m_entropy=GetEntropy();
}

template< class TInputHistogram, class TOutput >
void
HistogramStatisticFunction< TInputHistogram, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
