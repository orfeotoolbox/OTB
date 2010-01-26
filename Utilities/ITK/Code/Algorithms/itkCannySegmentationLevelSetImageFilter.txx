/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCannySegmentationLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-23 16:15:22 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCannySegmentationLevelSetImageFilter_txx
#define __itkCannySegmentationLevelSetImageFilter_txx

#include "itkCannySegmentationLevelSetImageFilter.h"

namespace itk {

template <class TInputImage, class TFeatureImage, class TOutputPixelType>
CannySegmentationLevelSetImageFilter<TInputImage, TFeatureImage,
                                     TOutputPixelType>
::CannySegmentationLevelSetImageFilter()
{
  m_CannyFunction = CannyFunctionType::New();

  this->SetSegmentationFunction(m_CannyFunction.GetPointer());
}
  
}// end namespace itk

#endif
