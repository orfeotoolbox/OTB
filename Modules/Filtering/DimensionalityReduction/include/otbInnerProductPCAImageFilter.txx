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
#ifndef __otbInnerProductPCAImageFilter_txx
#define __otbInnerProductPCAImageFilter_txx

#include "otbInnerProductPCAImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
InnerProductPCAImageFilter<TInputImage, TOutputImage>
::InnerProductPCAImageFilter()
{
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageType::New());
  m_EstimatePCAFilter  = EstimatePCAFilterType::New();
  m_NormalizePCAFilter  = NormalizePCAFilterType::New();
  m_CenterData = true;
  m_GenerateMeanComponent = false;
  m_MeanFilter = MeanFilterType::New();
  m_CastFilter = CastFilterType::New();
  m_ConcatenateFilter = ConcatenateFilterType::New();
}
/**
 * GenerateOutputInformation
 */
template<class TInputImage, class TOutputImage>
void
InnerProductPCAImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  if (m_GenerateMeanComponent == false)
    this->GetOutput()->SetNumberOfComponentsPerPixel(
      m_NumberOfPrincipalComponentsRequired);
  else this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfPrincipalComponentsRequired + 1);
}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
InnerProductPCAImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  m_EstimatePCAFilter->SetInput(this->GetInput());
  m_EstimatePCAFilter->SetNumberOfPrincipalComponentsRequired(m_NumberOfPrincipalComponentsRequired);
  m_EstimatePCAFilter->SetCenterData(m_CenterData);

  m_NormalizePCAFilter->SetInput(m_EstimatePCAFilter->GetOutput());

  if ((m_CenterData == false) || ((m_CenterData == true) && (m_GenerateMeanComponent == false)))
    {
    m_NormalizePCAFilter->GraftOutput(this->GetOutput());
    m_NormalizePCAFilter->Update();
    this->GraftOutput(m_NormalizePCAFilter->GetOutput());
    }
  else
    {
    m_MeanFilter->SetInput(this->GetInput());
    m_CastFilter->SetInput(m_MeanFilter->GetOutput());

    m_ConcatenateFilter->SetInput1(m_NormalizePCAFilter->GetOutput());
    m_ConcatenateFilter->SetInput2(m_CastFilter->GetOutput());

    m_ConcatenateFilter->GraftOutput(this->GetOutput());
    m_ConcatenateFilter->Update();
    this->GraftOutput(m_ConcatenateFilter->GetOutput());
    }
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
InnerProductPCAImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
