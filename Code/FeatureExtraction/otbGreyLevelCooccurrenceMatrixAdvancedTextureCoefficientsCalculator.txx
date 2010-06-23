/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-04 15:23:50 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator_txx
#define __otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator_txx

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace otb {
    
       
 
template< class THistogram >
void
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator< THistogram >::
NormalizeHistogram( void )
{
  typename HistogramType::Iterator hit;
  typename HistogramType::FrequencyType totalFrequency = 
    m_Histogram->GetTotalFrequency();
        
  for (hit = m_Histogram->Begin(); hit != m_Histogram->End(); ++hit)
    {
    hit.SetFrequency(hit.GetFrequency() / totalFrequency);
    }
}
      
template< class THistogram >
void
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator< THistogram >::
ComputeMean( double &pixelMean )
{
  // This function takes two passes through the histogram and two passes through
  // an array of the same length as a histogram axis. This could probably be
  // cleverly compressed to one pass, but it's not clear that that's necessary.
  typedef typename HistogramType::Iterator HistogramIterator;
      
  // Initialize everything
  typename HistogramType::SizeValueType binsPerAxis = m_Histogram->GetSize(0);
  pixelMean = 0;
      
  // Ok, now do the first pass through the histogram to get the marginal sums
  // and compute the pixel mean
  HistogramIterator hit;
  for (hit = m_Histogram->Begin(); hit != m_Histogram->End(); ++hit)
    {
    MeasurementType frequency = hit.GetFrequency();
    IndexType index = m_Histogram->GetIndex(hit.GetInstanceIdentifier());
    pixelMean += index[0] * frequency;
    }
} 

 
template< class THistogram >
void
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator< THistogram >::
Compute ()
{
  typedef typename HistogramType::Iterator HistogramIterator;
      
  // First, normalize the histogram if it doesn't look normalized.
  // This is one pass through the histogram.
  FrequencyType totalFrequency = m_Histogram->GetTotalFrequency();
  if ( (totalFrequency - itk::NumericTraits<MeasurementType>::One) > 0.0001 )
    {
    // Doesn't look normalized:
    this->NormalizeHistogram();
    }
      
  // Now get the various means and variances. This is takes two passes
  // through the histogram.
  double pixelMean;
  this->ComputeMean( pixelMean );
                                                                
  m_SumAverage = 0;
  m_SumEntropy = 0;
  m_SumVariance = 0;
  double PSSquareCumul = 0;
  double log2 = vcl_log(2.);
  
  for (long unsigned int i = 0 ; i < m_Histogram->GetSize()[0] + m_Histogram->GetSize()[1]; ++i)
    {
    double psTmp = ComputePS (i);
    
    m_SumAverage += i *psTmp;
    m_SumEntropy -= (psTmp > 0.0001) ? psTmp * vcl_log(psTmp) / log2 : 0;
    PSSquareCumul += i * i * psTmp;
    }                          
  m_SumVariance = PSSquareCumul - m_SumAverage * m_SumAverage;
  
  double minSizeHist = std::min (m_Histogram->GetSize()[0] , m_Histogram->GetSize()[1]);
  
  m_DifferenceEntropy = 0;
  m_DifferenceVariance = 0;
  double PDSquareCumul = 0;
  double PDCumul = 0;
  
  for (long unsigned int i = 0 ; i < minSizeHist ; ++i)
    {
    double pdTmp = ComputePD (i);
    
    PDCumul += i *pdTmp;
    m_DifferenceEntropy -= (pdTmp > 0.0001) ? pdTmp * vcl_log(pdTmp) / log2 : 0;
    PDSquareCumul += i * i * pdTmp;
    }
  m_DifferenceVariance = PDSquareCumul - PDCumul * PDCumul;
  
  typedef typename HistogramType::Iterator HistogramIterator;
  
  double hx = 0;
  double hy = 0;
  
  for (long unsigned int i = 0 ; i < m_Histogram->GetSize()[0] ; ++i)
    {
    double marginalfreq = m_Histogram->GetFrequency ( i , 0 );
    hx += vcl_log ( marginalfreq ) * marginalfreq ;
    }
  
  for (long unsigned int j = 0 ; j < m_Histogram->GetSize()[1] ; ++j)
    {
    double marginalfreq = m_Histogram->GetFrequency ( j , 1 );
    hy += vcl_log ( marginalfreq ) * marginalfreq ;
    }
    
  double hxy1 = 0;
  double hxy2 = 0;
  
  m_Variance = 0;
  double Entropy = 0;
  
  for (HistogramIterator hit = m_Histogram->Begin();
       hit != m_Histogram->End(); ++hit)
    {
    MeasurementType frequency = hit.GetFrequency();
//       if (frequency == 0)
//         {
//         continue; // no use doing these calculations if we're just multiplying by zero.
//         }
    IndexType index = m_Histogram->GetIndex(hit.GetInstanceIdentifier());
    
    m_Variance += ( (index[0] - pixelMean) * (index[0] - pixelMean) ) * frequency;
    Entropy -= (frequency > 0.0001) ? frequency * vcl_log(frequency) / log2 : 0;
    
    double pipj = m_Histogram->GetFrequency (index[0] , 0) * m_Histogram->GetFrequency (index[1] , 1);
    
    hxy1 += (pipj > 0.0001) ? frequency * vcl_log ( pipj ) : 0;
    hxy2 += (pipj > 0.0001) ? pipj * vcl_log ( pipj ) : 0;
    }
    
    m_IC1 = (vcl_abs( std::max ( hx, hy ) ) > 0.0001) ? ( Entropy - hxy1 ) / (std::max ( hx, hy ) ) : 0;
    m_IC2 = vcl_sqrt ( 1 - vcl_exp ( -2. * vcl_abs ( hxy2 - Entropy ) ) ); 
}   



//TODO check this!!
template< class THistogram >
double
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator< THistogram >::
ComputePS ( long unsigned int k )
{               
  double result = 0;
  IndexType index;
  long unsigned int start = std::max (static_cast <long unsigned int> (0),  k - m_Histogram->GetSize()[1]);
  long unsigned int end = std::min (k,  m_Histogram->GetSize()[0]);
  for (long unsigned int i = start; i < end ; ++i)
    {
      index[0] = i;
      index[1] = k - i;
      result += m_Histogram->GetFrequency ( index ); 
    } 
  return result;
}

//TODO check this!!
template< class THistogram >
double
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator< THistogram >::
ComputePD ( long unsigned int k )
{               
  double result = 0;
  IndexType index;
  long unsigned int end = std::min (m_Histogram->GetSize()[0] - k, m_Histogram->GetSize()[1]);
  for (long unsigned int j = 0; j < end ; ++j)
    {
      index[0] = j + k;
      index[1] = j;
      result += m_Histogram->GetFrequency ( index ); 
    } 
  return result;
}


template< class THistogram >
void
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator< THistogram >::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end of namespace otb


#endif
