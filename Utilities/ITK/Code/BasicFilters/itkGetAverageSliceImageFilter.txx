/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGetAverageSliceImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 16:45:08 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGetAverageSliceImageFilter_txx
#define __itkGetAverageSliceImageFilter_txx

#include "itkGetAverageSliceImageFilter.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage >
GetAverageSliceImageFilter<TInputImage,TOutputImage >
::GetAverageSliceImageFilter()
{
  m_AveragedOutDimension = this->GetAccumulateDimension();
  this->AverageOn();
}

template <class TInputImage, class TOutputImage >
void
GetAverageSliceImageFilter<TInputImage,TOutputImage>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "AveragedOutDimension: " << m_AveragedOutDimension << std::endl;
}

} // end namespace itk


#endif
