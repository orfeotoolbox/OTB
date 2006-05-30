/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIsotropicFourthOrderLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/21 22:47:26 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

     =========================================================================*/
#ifndef _itkIsotropicFourthOrderLevelSetImageFilter_txx_
#define _itkIsotropicFourthOrderLevelSetImageFilter_txx_

#include "itkIsotropicFourthOrderLevelSetImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage>
IsotropicFourthOrderLevelSetImageFilter<TInputImage, TOutputImage>
::IsotropicFourthOrderLevelSetImageFilter()
{
  RadiusType radius;
  for (unsigned int j=0; j<Self::ImageDimension;j++)
    {
    radius[j] = 1;
    }
  
  m_Function=FunctionType::New();
  this->SetLevelSetFunction(m_Function);
  this->SetNumberOfLayers(this->GetMinimumNumberOfLayers());
  
  this->SetNormalProcessType (0);  // isotropic diffusion 
  this->SetMaxNormalIteration(25);
  this->SetMaxRefitIteration(100);
  m_MaxFilterIteration = 1000;
  m_Function->Initialize(radius);
}

template <class TInputImage, class TOutputImage>
void
IsotropicFourthOrderLevelSetImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);     
  os << indent << "MaxFilterIteration: " << m_MaxFilterIteration << std::endl;      
}

} // end namespace itk

#endif
