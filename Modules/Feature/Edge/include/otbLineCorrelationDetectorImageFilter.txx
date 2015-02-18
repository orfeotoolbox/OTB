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
#ifndef __otbLineCorrelationDetectorImageFilter_txx
#define __otbLineCorrelationDetectorImageFilter_txx

#include "otbLineCorrelationDetectorImageFilter.h"

#include "itkDataObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
LineCorrelationDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
::LineCorrelationDetectorImageFilter()
{
  this->m_Radius.Fill(1);
  this->m_LengthLine = 1;
  this->m_WidthLine = 0;
  this->m_FaceList.Fill(0);
}

template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
double
LineCorrelationDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
::ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3)
{

  double M1 = 0.0;
  double M2 = 0.0;
  double M3 = 0.0;

  std::vector<double>::iterator m1It = m1->begin();
  std::vector<double>::iterator m1End = m1->end();

  std::vector<double>::iterator m2It = m2->begin();
  std::vector<double>::iterator m2End = m2->end();

  std::vector<double>::iterator m3It = m3->begin();
  std::vector<double>::iterator m3End = m3->end();

  while (m1It != m1End && m2It != m2End && m3It != m3End)
    {

    M1 += (*m1It);
    ++m1It;

    M2 += (*m2It);
    ++m2It;

    M3 += (*m3It);
    ++m3It;

    }

  M1 /= m1->size();
  M2 /= m2->size();
  M3 /= m3->size();

  double sigma1 = 0.0;
  double sigma2 = 0.0;
  double sigma3 = 0.0;

  m1It = m1->begin();
  m2It = m2->begin();
  m3It = m3->begin();

  while (m1It != m1End && m2It != m2End && m3It != m3End)
    {

    sigma1 += vcl_pow((*m1It) - M1, 2);
    ++m1It;

    sigma2 += vcl_pow((*m2It) - M2, 2);
    ++m2It;

    sigma3 += vcl_pow((*m3It) - M3, 2);
    ++m3It;

    }

  sigma1 /= m1->size();
  sigma2 /= m2->size();
  sigma3 /= m3->size();

  // Actually, we use the variance
  /*  sigma1 = vcl_sqrt(sigma1);
    sigma2 = vcl_sqrt(sigma2);
    sigma3 = vcl_sqrt(sigma3);
    */

  // Calculation of the cross correlation coefficient

  double d1 = 0.;
  double d2 = 0.;
  double d3 = 0.;

  double rho12 = 0.;
  double rho13 = 0.;

  // rho12
  if (M2 != 0.)
    {
    d1 = sigma1 / vcl_pow(M2, 2) * m1->size();
    d2 = sigma2 / vcl_pow(M2, 2) * m2->size();

    d3 = vcl_pow(((M1 / M2) - 1.), 2) * (m1->size() * m2->size());

    if ((d3 != 0.)) rho12 = static_cast<double>(1. / (1. + ((m1->size() + m2->size()) * (d1 + d2) / d3)));
    else rho12 = 0.;

    }
  if (M3 != 0.)
    {
    d1 = sigma1 / vcl_pow(M3, 2) * m1->size();
    d2 = sigma3 / vcl_pow(M3, 2) * m2->size();

    d3 = vcl_pow(((M1 / M3) - 1.), 2) * (m1->size() * m2->size());

    if ((d3 != 0.)) rho13 = static_cast<double>(1. / (1. + ((m1->size() + m2->size()) * (d1 + d2) / d3)));
    else rho13 = 0.;

    }

  rho12 = vcl_sqrt(rho12);
  rho13 = vcl_sqrt(rho13);

  // Determination of the minimum intensity of detection between R12 et R13
  return static_cast<double>(std::min(rho12, rho13));

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
void
LineCorrelationDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
