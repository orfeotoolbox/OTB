/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbImageToHessianDeterminantImageFilter_hxx
#define otbImageToHessianDeterminantImageFilter_hxx

#include "otbImageToHessianDeterminantImageFilter.h"

namespace otb
{
template <class TInputImage, class TOutputImage, class TPrecision>
ImageToHessianDeterminantImageFilter<TInputImage, TOutputImage, TPrecision>::ImageToHessianDeterminantImageFilter()
{
  // Filter initialization
  m_HessianFilter     = ImageToHessianImageFilterType::New();
  m_DeterminantFilter = DeterminantFilterType::New();

  // pipeline wiring
  m_DeterminantFilter->SetInput(m_HessianFilter->GetOutput());

  // default parameter value
  m_Sigma = 1.0;
}

template <class TInputImage, class TOutputImage, class TPrecision>
ImageToHessianDeterminantImageFilter<TInputImage, TOutputImage, TPrecision>::~ImageToHessianDeterminantImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
void ImageToHessianDeterminantImageFilter<TInputImage, TOutputImage, TPrecision>::GenerateData(void)
{
  // Execute minipipeline
  m_HessianFilter->SetInput(this->GetInput());
  m_HessianFilter->SetSigma(m_Sigma);
  m_DeterminantFilter->GraftOutput(this->GetOutput());
  m_DeterminantFilter->Update();
  this->GraftOutput(m_DeterminantFilter->GetOutput());
}

/*PrintSelf*/
template <class TInputImage, class TOutputImage, class TPrecision>
void ImageToHessianDeterminantImageFilter<TInputImage, TOutputImage, TPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Sigma: " << m_Sigma << std::endl;
}
}
#endif
