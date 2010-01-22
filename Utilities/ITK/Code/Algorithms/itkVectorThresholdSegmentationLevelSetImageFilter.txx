/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorThresholdSegmentationLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-27 19:30:16 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorThresholdSegmentationLevelSetImageFilter_txx
#define __itkVectorThresholdSegmentationLevelSetImageFilter_txx

#include "itkVectorThresholdSegmentationLevelSetImageFilter.h"

namespace itk {


template <class TInputImage, class TFeatureImage, class TOutputType>
VectorThresholdSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::VectorThresholdSegmentationLevelSetImageFilter(void)
{
  m_ThresholdFunction = ThresholdFunctionType::New();
  m_ThresholdFunction->SetThreshold(0);

  this->SetSegmentationFunction(m_ThresholdFunction);
}
 
template <class TInputImage, class TFeatureImage, class TOutputType>
void
VectorThresholdSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "ThresholdFunction: " << m_ThresholdFunction;
}


}// end namespace itk

#endif
