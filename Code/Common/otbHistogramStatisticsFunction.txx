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

#ifndef _otbHistogramStatisticsFunction_txx
#define _otbHistogramStatisticsFunction_txx

#include "otbHistogramStatisticsFunction.h"

namespace otb
{

template< class TInputHistogram, class TOutput >
HistogramStatisticsFunction< TInputHistogram, TOutput>
::HistogramStatisticsFunction()
{
  m_IsModified = true;  
}

template< class TInputHistogram, class TOutput >
typename HistogramStatisticsFunction< TInputHistogram, TOutput>::OutputType
HistogramStatisticsFunction< TInputHistogram, TOutput>
::GetEntropy()
{
  if(m_IsModified == true)
  {
  	this->Update();
  }
  return m_entropy;
}                                                    

template< class TInputHistogram, class TOutput >
typename HistogramStatisticsFunction< TInputHistogram, TOutput>::OutputType
HistogramStatisticsFunction< TInputHistogram, TOutput>
::GetMean()
{
  if(m_IsModified == true)
  {
  	this->Update();
  }
  return m_mean;
}                                                    

template< class TInputHistogram, class TOutput >
typename HistogramStatisticsFunction< TInputHistogram, TOutput>::OutputType
HistogramStatisticsFunction< TInputHistogram, TOutput>
::GetCovariance()
{
  if(m_IsModified == true)
  {
  	this->Update();
  }
  return m_covariance;
}                                                    

template< class TInputHistogram, class TOutput >
void
HistogramStatisticsFunction< TInputHistogram, TOutput>
::CalculateEntropy()
{
  typename TInputHistogram::ConstPointer histogram = m_InputHistogram;


  typename TInputHistogram::ConstIterator iter = histogram->Begin() ;
  typename TInputHistogram::ConstIterator end = histogram->End() ;

  RealType entropy = itk::NumericTraits<RealType>::Zero;
  FrequencyType globalFrequency = histogram->GetTotalFrequency();
  if(globalFrequency == 0)
    {
    itkExceptionMacro(<<"Histogram must contain at least 1 element.");
    }
  while (iter != end)
    {
    RealType Proba = static_cast<RealType>(iter.GetFrequency());
    Proba /= static_cast<RealType>(globalFrequency); 
    if(Proba !=0.0)   
      {
      entropy -=  Proba * log(Proba);
      } 
    ++iter ;
    }
  m_entropy.resize(1);
  m_entropy[0] = static_cast<TOutput>(entropy); 
}


template< class TInputHistogram, class TOutput >
void
HistogramStatisticsFunction< TInputHistogram, TOutput>
::CalculateMean()
{
  typename TInputHistogram::ConstPointer histogram = m_InputHistogram;

  unsigned int NumberOfDimension =  histogram->GetSize().GetSizeDimension();
  std::vector<TOutput> GlobalMean(NumberOfDimension);
  m_mean.resize(NumberOfDimension);

  for( unsigned int noDim = 0; noDim < NumberOfDimension; noDim++ )
    {
    MeasurementType mean = itk::NumericTraits<MeasurementType>::Zero;
    for (unsigned int i = 0; i < histogram->GetSize()[noDim]; i++)
      {
      MeasurementType val  = histogram->GetMeasurement(i, noDim);
      FrequencyType freq = histogram->GetFrequency(i, noDim);
      mean += val*freq;
      }
    mean /=  histogram->GetTotalFrequency();
    m_mean[noDim] = static_cast<TOutput>(mean);
    }  
}

template< class TInputHistogram, class TOutput >
void
HistogramStatisticsFunction< TInputHistogram, TOutput>
::CalculateCovariance()
{
#if 0
  typename TInputHistogram::ConstPointer histogram = m_InputHistogram;

  // TODO: as an improvement, the class could accept multi-dimensional histograms
  // and the user could specify the dimension to apply the algorithm to.
  if (histogram->GetSize().GetSizeDimension() != 1)
    {
    itkExceptionMacro(<<"Histogram must be 1-dimensional.");
    }

  // compute global mean
  typename TInputHistogram::ConstIterator iter = histogram->Begin() ;
  typename TInputHistogram::ConstIterator end = histogram->End() ;

  RealType covariance = itk::NumericTraits<RealType>::Zero;
  FrequencyType globalFrequency = histogram->GetTotalFrequency();
  if(globalFrequency == 0)
    {
    itkExceptionMacro(<<"Histogram must contain at least 1 element.");
    }
  while (iter != end)
    {
    covariance +=    static_cast<RealType>(iter.GetFrequency()) 
                 * ( static_cast<RealType>(iter.GetMeasurementVector()[0])
	            -static_cast<RealType>(m_mean)              );
    ++iter ;
    }
  covariance /= static_cast<RealType>(globalFrequency);
  
  m_covariance = static_cast<OutputType>(covariance); 
#endif
}

template< class TInputHistogram, class TOutput >
void
HistogramStatisticsFunction< TInputHistogram, TOutput>
::GenerateData()
{

  	CalculateEntropy();
	CalculateMean();
	CalculateCovariance();
	m_IsModified = false;
}


template< class TInputHistogram, class TOutput >
void
HistogramStatisticsFunction< TInputHistogram, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
