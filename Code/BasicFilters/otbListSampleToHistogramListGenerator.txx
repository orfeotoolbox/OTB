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
#ifndef __otbListSampleToHistogramListGenerator_txx
#define __otbListSampleToHistogramListGenerator_txx

#include <exception>
#include "otbMacro.h"
#include "itkArray.h"

namespace otb
{

template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer>
ListSampleToHistogramListGenerator< TListSample, 
                                    THistogramMeasurement, 
                                    TFrequencyContainer >
::ListSampleToHistogramListGenerator() : m_List(), m_Size(), m_MarginalScale(100), m_HistogramMin(), m_HistogramMax(), m_AutoMinMax(true), m_HistogramList()
{
  m_HistogramList = HistogramListType::New() ;
  m_Size.Fill(255);
}  


template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer >
void
ListSampleToHistogramListGenerator< TListSample, 
                                    THistogramMeasurement, 
                                    TFrequencyContainer >
::GenerateData()
{
  otbMsgDebugMacro(<<"ListSampleToHistogramListGenerator::GenerateData(): Entering");

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

  typename TListSample::MeasurementVectorType h_upper(m_List->GetMeasurementVectorSize());
  typename TListSample::MeasurementVectorType h_lower(m_List->GetMeasurementVectorSize());

  bool clipBinsAtEnds = true;

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
            (THistogramMeasurement) m_Size[0] ) / 
          (THistogramMeasurement) m_MarginalScale ;
        h_upper[i] = (THistogramMeasurement) (upper[i] + margin) ;
        if(h_upper[i] <= upper[i])
          { 
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          clipBinsAtEnds = false;
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
          clipBinsAtEnds = false;
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

  // Clearing previous histograms
  m_HistogramList->Clear();

  // For each dimension 
  for(unsigned int comp = 0; comp<m_List->GetMeasurementVectorSize() ;++comp)
    {
    // initialize the Histogram object using the sizes and
    // the upper and lower bound from the FindSampleBound function
    typename HistogramType::MeasurementVectorType comp_lower, comp_upper;
    comp_lower[0] = h_lower[comp];
    comp_upper[0] = h_upper[comp];

    otbMsgDevMacro(<<"ListSampleToHistogramListGenerator::GenerateData(): Intializing histogram "<<comp<<" with (size= "<<m_Size<<", lower = "<<comp_lower<<", upper = "<<comp_upper<<")");

    // Create a new histogrma for this component
    m_HistogramList->PushBack(HistogramType::New());
    m_HistogramList->Back()->SetClipBinsAtEnds(clipBinsAtEnds);
    m_HistogramList->Back()->Initialize(m_Size, comp_lower, comp_upper) ;

    typename TListSample::ConstIterator iter = m_List->Begin() ;
    typename TListSample::ConstIterator last = m_List->End() ;
    typename HistogramType::IndexType index ;
    typename HistogramType::MeasurementVectorType hvector;
  
    while (iter != last)
      {   
      hvector[0] = static_cast<THistogramMeasurement>(iter.GetMeasurementVector()[comp]);
      m_HistogramList->Back()->GetIndex(hvector,index);

      if (!m_HistogramList->Back()->IsIndexOutOfBounds(index))
        {
        // if the measurement vector is out of bound then
        // the GetIndex method has returned an index set to the max size of
        // the invalid dimension - even if the hvector is less than the minimum
        // bin value.
        // If the index isn't valid, we don't increase the frequency.
        // See the comments in Histogram->GetIndex() for more info.
        m_HistogramList->Back()->IncreaseFrequency(index, 1) ;
        }
      ++iter ;
      }
    }
  otbMsgDebugMacro(<<"ListSampleToHistogramListGenerator::GenerateData(): Leaving");
}

template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer >
void
ListSampleToHistogramListGenerator< TListSample, 
                                    THistogramMeasurement, 
                                    TFrequencyContainer >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "AutoMinMax: " << m_AutoMinMax << std::endl;
  os << indent << "Size: " << m_Size << std::endl;
  os << indent << "MarginalScale: "<< m_MarginalScale << std::endl;
  os << indent << "HistogramMin: "<< m_HistogramMin << std::endl;
  os << indent << "HistogramMax: "<< m_HistogramMax << std::endl;
}

} // end of namespace itk 

#endif


