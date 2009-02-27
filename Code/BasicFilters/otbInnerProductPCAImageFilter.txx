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
InnerProductPCAImageFilter<TInputImage,TOutputImage>
::InnerProductPCAImageFilter()
{
  this->SetNumberOfOutputs(1);
  this->SetNthOutput(0,OutputImageType::New());
  m_EstimatePCAFilter  = EstimatePCAFilterType::New();
  m_NormalizePCAFilter  = NormalizePCAFilterType::New();
}
/**
 * GenerateOutputInformation
 */
template<class TInputImage, class TOutputImage>
void
InnerProductPCAImageFilter<TInputImage,TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfPrincipalComponentsRequired);
}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
InnerProductPCAImageFilter<TInputImage,TOutputImage>
::GenerateData()
{
  m_EstimatePCAFilter->SetInput(this->GetInput());
  m_EstimatePCAFilter->SetNumberOfPrincipalComponentsRequired(m_NumberOfPrincipalComponentsRequired);
  m_NormalizePCAFilter->SetInput(m_EstimatePCAFilter->GetOutput());

  m_NormalizePCAFilter->GraftOutput(this->GetOutput());
  m_NormalizePCAFilter->Update();
  this->GraftOutput(m_NormalizePCAFilter->GetOutput());
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
InnerProductPCAImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
