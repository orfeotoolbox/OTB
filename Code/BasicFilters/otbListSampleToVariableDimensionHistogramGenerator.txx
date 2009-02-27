/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbListSampleToVariableDimensionHistogramGenerator_txx
#define __otbListSampleToVariableDimensionHistogramGenerator_txx

#include <exception>

namespace otb
{

template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer>
ListSampleToVariableDimensionHistogramGenerator< TListSample, 
                                THistogramMeasurement, 
                                TFrequencyContainer >
::ListSampleToVariableDimensionHistogramGenerator()
{
  m_Histogram = HistogramType::New() ;
  m_MarginalScale = 100 ;
  m_AutoMinMax = true;
}  


template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer >
void
ListSampleToVariableDimensionHistogramGenerator< TListSample, 
                                THistogramMeasurement, 
                                TFrequencyContainer >
::GenerateData()
{
  // TODO : Sanity checks
  if(m_Sizes.GetSize() != m_List->GetMeasurementVectorSize())
    {
    itkExceptionMacro("Sample list measurement vectors and histogram bin sizes vector have different dimensions !");
    }

  if(!m_AutoMinMax)
    {
    if(m_HistogramMin.GetSize() != m_List->GetMeasurementVectorSize())
    {
    itkExceptionMacro("Sample list measurement vectors and histogram min have different dimensions !");
    }
    if(m_HistogramMax.GetSize() != m_List->GetMeasurementVectorSize())
    {
    itkExceptionMacro("Sample list measurement vectors and histogram max have different dimensions !");
    }
    }

  typename TListSample::MeasurementVectorType lower(m_List->GetMeasurementVectorSize());
  typename TListSample::MeasurementVectorType upper(m_List->GetMeasurementVectorSize());

  typename HistogramType::MeasurementVectorType h_upper(m_List->GetMeasurementVectorSize());
  typename HistogramType::MeasurementVectorType h_lower(m_List->GetMeasurementVectorSize());

  // must test for the list size to avoid making FindSampleBound() segfault.
  // Also, the min and max can't be found automatically in that case. We can
  // only return an empty histogram
  if( m_AutoMinMax && m_List->Size() != 0 )
    {
    FindSampleBound(m_List, m_List->Begin(),
                    m_List->End(), lower, upper) ;
    
    float margin ;

    for ( unsigned int i = 0 ; i < m_List->GetMeasurementVectorSize() ; i++ )
      {
      if ( !itk::NumericTraits< THistogramMeasurement >::is_integer )
        {
        margin = 
	  ( (THistogramMeasurement)(upper[i] - lower[i]) / 
	    (THistogramMeasurement) m_Sizes[i] ) / 
	  (THistogramMeasurement) m_MarginalScale ;
        h_upper[i] = (THistogramMeasurement) (upper[i] + margin) ;
        if(h_upper[i] <= upper[i])
          { 
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          m_Histogram->SetClipBinsAtEnds(false);
          // The above function is okay since here we are within the autoMinMax 
          // computation and clearly the user intended to include min and max.
          }
        }
      else
        {
        h_upper[i] = ((THistogramMeasurement) upper[i]) + 
          itk::NumericTraits< THistogramMeasurement >::One ;
        if(h_upper[i] <= upper[i])
          { 
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          m_Histogram->SetClipBinsAtEnds(false);
          // The above function is okay since here we are within the autoMinMax 
          // computation and clearly the user intended to include min and max.
          }
        }
      h_lower[i] = ( THistogramMeasurement) lower[i] ;
      }
    }
  else
    {
    h_lower = m_HistogramMin;
    h_upper = m_HistogramMax;
    }

  // initialize the Histogram object using the sizes and
  // the upper and lower bound from the FindSampleBound function
  m_Histogram->Initialize(m_Sizes, h_lower, h_upper) ;

  typename TListSample::ConstIterator iter = m_List->Begin() ;
  typename TListSample::ConstIterator last = m_List->End() ;
  typename HistogramType::IndexType index(m_List->GetMeasurementVectorSize()) ;
  typename TListSample::MeasurementVectorType lvector(m_List->GetMeasurementVectorSize()) ;
  typename HistogramType::MeasurementVectorType hvector(m_List->GetMeasurementVectorSize()) ;
  unsigned int i;
  while (iter != last)
    {
    lvector = iter.GetMeasurementVector() ;
    for ( i = 0 ; i < m_List->GetMeasurementVectorSize() ; i++)
      {
      hvector[i] = (THistogramMeasurement) lvector[i] ;
      }

    m_Histogram->GetIndex(hvector,index);
    if (!m_Histogram->IsIndexOutOfBounds(index))
      {
      // if the measurement vector is out of bound then
      // the GetIndex method has returned an index set to the max size of
      // the invalid dimension - even if the hvector is less than the minimum
      // bin value.
      // If the index isn't valid, we don't increase the frequency.
      // See the comments in Histogram->GetIndex() for more info.
      m_Histogram->IncreaseFrequency(index, 1) ;
      }
    ++iter ;
    }
}

template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer >
void
ListSampleToVariableDimensionHistogramGenerator< TListSample, 
                                THistogramMeasurement, 
                                TFrequencyContainer >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "AutoMinMax: " << m_AutoMinMax << std::endl;
  os << indent << "Sizes: " << m_Sizes << std::endl;
  os << indent << "MarginalScale: "<< m_MarginalScale << std::endl;
  os << indent << "HistogramMin: "<< m_HistogramMin << std::endl;
  os << indent << "HistogramMax: "<< m_HistogramMax << std::endl;
}

} // end of namespace itk 

#endif


