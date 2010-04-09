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
#ifndef __otbHarrisImageToPointSetFilter_txx
#define __otbHarrisImageToPointSetFilter_txx

#include "otbHarrisImageToPointSetFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
HarrisImageToPointSetFilter<TInputImage, TOutputPointSet>
::HarrisImageToPointSetFilter()
{
  m_SigmaD = 1.0;
  m_SigmaI = 1.0;
  m_Alpha  = 1.0;

  m_LowerThreshold = itk::NumericTraits<InputPixelType>::NonpositiveMin();
  m_UpperThreshold = itk::NumericTraits<InputPixelType>::max();

  m_HarrisFilter     = HarrisImageFilterType::New();
  m_ThresholdFilter  = ThresholdImageToPointSetType::New();
}

template <class TInputImage, class TOutputPointSet>
void
HarrisImageToPointSetFilter<TInputImage, TOutputPointSet>
::GenerateData()
{

  typename OutputPointSetType::Pointer pointList = this->GetOutput();

  m_HarrisFilter->SetInput(0, this->GetInput(0));
  m_HarrisFilter->SetSigmaD(m_SigmaD);
  m_HarrisFilter->SetSigmaI(m_SigmaI);
  m_HarrisFilter->SetAlpha(m_Alpha);

  m_ThresholdFilter->SetInput(0, m_HarrisFilter->GetOutput());
  m_ThresholdFilter->SetLowerThreshold(m_LowerThreshold);
  m_ThresholdFilter->SetUpperThreshold(m_UpperThreshold);

  m_ThresholdFilter->SetOutput(pointList);
  m_ThresholdFilter->Update();

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputPointSet>
void
HarrisImageToPointSetFilter<TInputImage, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Sigma_D    : " << this->m_SigmaD    << std::endl;
  os << indent << "Sigma_I    : " << this->m_SigmaI    << std::endl;
  os << indent << "Alpha      : " << this->m_Alpha     << std::endl;
  os << indent << "LowerThreshold  : " << this->m_LowerThreshold << std::endl;
  os << indent << "UpperThreshold  : " << this->m_UpperThreshold << std::endl;
}

} // end namespace otb

#endif
