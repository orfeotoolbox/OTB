/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFiniteDifferenceFunction.txx,v $
  Language:  C++
  Date:      $Date: 2007/01/30 20:56:07 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFiniteDifferenceFunction_txx_
#define __itkFiniteDifferenceFunction_txx_

#include "itkFiniteDifferenceFunction.h"

namespace itk
{
template<class TImageType>
void
FiniteDifferenceFunction<TImageType>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
  os << indent << "ScaleCoefficients: " << m_ScaleCoefficients;
}
} // end namespace itk


#endif
