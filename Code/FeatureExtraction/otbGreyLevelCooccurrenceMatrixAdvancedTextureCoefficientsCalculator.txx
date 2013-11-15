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
#ifndef __otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator_txx
#define __otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator_txx

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace otb {

template<class THistogram>
void
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<THistogram>::
ComputeMean()
{
  // Initialize everything
  m_Mean = 0.;

  // Ok, now do the first pass through the histogram to get the marginal sums
  // and compute the pixel mean
  typename HistogramType::Iterator itr = m_Histogram->Begin();
  typename HistogramType::Iterator end = m_Histogram->End();
  while( itr != end )
    {
    RelativeFrequencyType frequency = static_cast<RelativeFrequencyType>(itr.GetFrequency()) /
      static_cast<RelativeFrequencyType>(m_Histogram->GetTotalFrequency());

    IndexType       index = m_Histogram->GetIndex(itr.GetInstanceIdentifier());
    m_Mean += static_cast<double>(index[0]) * static_cast<double>(frequency);
    ++itr;
    }
}

template<class THistogram>
void
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<THistogram>::
Compute()
{
  typedef typename HistogramType::Iterator HistogramIterator;

  // Now get the pixel mean.
  this->ComputeMean();

  m_SumAverage = 0;
  m_SumEntropy = 0;
  m_SumVariance = 0;
  double PSSquareCumul = 0;
  double log2 = vcl_log(2.);

  // First pass to compute SumAverage, SumVariance
  for (long unsigned int i = 0; i < m_Histogram->GetSize()[0] + m_Histogram->GetSize()[1]; ++i)
    {
    double psTmp = ComputePS (i);

    m_SumAverage += i * psTmp;
    m_SumEntropy -= (psTmp > 0.0001) ? psTmp * vcl_log(psTmp) / log2 : 0;
    PSSquareCumul += i * i * psTmp;
    }
  m_SumVariance = PSSquareCumul - m_SumAverage * m_SumAverage;

  double minSizeHist = std::min (m_Histogram->GetSize()[0], m_Histogram->GetSize()[1]);

  m_DifferenceEntropy = 0;
  m_DifferenceVariance = 0;

  double PDSquareCumul = 0;
  double PDCumul = 0;

  // Second pass to compute DifferenceVariance and DifferenceEntropy
  for (long unsigned int i = 0; i < minSizeHist; ++i)
    {
    double pdTmp = ComputePD (i);

    PDCumul += i * pdTmp;
    m_DifferenceEntropy -= (pdTmp > 0.0001) ? pdTmp * vcl_log(pdTmp) / log2 : 0;
    PDSquareCumul += i * i * pdTmp;
    }
  m_DifferenceVariance = PDSquareCumul - PDCumul * PDCumul;

  typedef typename HistogramType::Iterator HistogramIterator;

  double hx = 0;
  double hy = 0;

  // Compute hx and hy need to compute f12 and f13 texture coefficients
  for (long unsigned int i = 0; i < m_Histogram->GetSize()[0]; ++i)
    {
    double marginalfreq = static_cast<double>(m_Histogram->GetFrequency (i, 0))/static_cast<double>(m_Histogram->GetTotalFrequency());
    hx += (marginalfreq > 0.0001) ? vcl_log (marginalfreq) * marginalfreq : 0;
    }

  for (long unsigned int j = 0; j < m_Histogram->GetSize()[1]; ++j)
    {
    double marginalfreq = static_cast<double>(m_Histogram->GetFrequency (j, 1))/static_cast<double>(m_Histogram->GetTotalFrequency());
    hy += (marginalfreq > 0.0001) ? vcl_log (marginalfreq) * marginalfreq : 0;
    }

  double hxy1 = 0;
  double hxy2 = 0;

  m_Variance = 0;
  double Entropy = 0;

  // Third pass over the histogram to compute Sum of squares (variance), entropy (needed for f12)
  for (HistogramIterator hit = m_Histogram->Begin();
       hit != m_Histogram->End(); ++hit)
    {
    double frequency = static_cast<double>(hit.GetFrequency())
      / static_cast<double>(m_Histogram->GetTotalFrequency());

    IndexType index = m_Histogram->GetIndex(hit.GetInstanceIdentifier());

    m_Variance += ((static_cast<double>(index[0]) - m_Mean) * (static_cast<double>(index[0]) - m_Mean)) * frequency;
    Entropy -= (frequency > 0.0001) ? frequency * vcl_log(frequency) / log2 : 0.;

    double pipj =
      static_cast<double>(m_Histogram->GetFrequency (index[0], 0))/ static_cast<double>(m_Histogram->GetTotalFrequency())
      * static_cast<double>(m_Histogram->GetFrequency (index[1], 1))/  static_cast<double>(m_Histogram->GetTotalFrequency());

    hxy1 -= (pipj > 0.0001) ? frequency * vcl_log(pipj) : 0.;
    hxy2 -= (pipj > 0.0001) ? pipj * vcl_log(pipj) : 0.;
    }

  //Finally get f12 and f13
  m_IC1 = (vcl_abs(std::max (hx, hy)) > 0.0001) ? (Entropy - hxy1) / (std::max (hx, hy)) : 0;

  m_IC2 = 1 - vcl_exp (-2. * vcl_abs (hxy2 - Entropy));
  m_IC2 = (m_IC2 >= 0) ? vcl_sqrt (m_IC2) : 0;
}

//TODO check this!!
template<class THistogram>
double
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<THistogram>::
ComputePS(long unsigned int k)
{
  double            result = 0;
  IndexType         index(m_Histogram->GetMeasurementVectorSize());
  long unsigned int start = std::max (static_cast <long unsigned int> (0),  k - m_Histogram->GetSize()[1]);
  long unsigned int end = std::min (k,  m_Histogram->GetSize()[0]);
  for (long unsigned int i = start; i < end; ++i)
    {
    index[0] = i;
    index[1] = k - i;
    result += static_cast<double>(m_Histogram->GetFrequency (index))
      /static_cast<double>(m_Histogram->GetTotalFrequency());
    }
  return result;
}

//TODO check this!!
template<class THistogram>
double
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<THistogram>::
ComputePD(long unsigned int k)
{
  double            result = 0;
  IndexType         index(m_Histogram->GetMeasurementVectorSize());
  long unsigned int end = std::min (m_Histogram->GetSize()[0] - k, m_Histogram->GetSize()[1]);
  for (long unsigned int j = 0; j < end; ++j)
    {
    index[0] = j + k;
    index[1] = j;
    result += static_cast<double>(m_Histogram->GetFrequency (index))
      /static_cast<double>(m_Histogram->GetTotalFrequency());
    }
  return result;
}

template<class THistogram>
void
GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<THistogram>::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end of namespace otb

#endif
