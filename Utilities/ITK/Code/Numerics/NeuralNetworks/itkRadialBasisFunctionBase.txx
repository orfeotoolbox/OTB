/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRadialBasisFunctionBase.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:04:59 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkRadialBasisFunctionBase_txx
#define __itkRadialBasisFunctionBase_txx

#include "itkFunctionBase.h"
#include "itkArray.h"

namespace itk
{
namespace Statistics
{

template<class ScalarType>
void
RadialBasisFunctionBase<ScalarType>
::SetCenter(ArrayType c)
{
  m_Center=c;
}
template<class ScalarType>
typename RadialBasisFunctionBase<ScalarType>::ArrayType
RadialBasisFunctionBase<ScalarType>
::GetCenter()
{
  return m_Center;
}
template<class ScalarType>
void
RadialBasisFunctionBase<ScalarType>
::SetRadius(ScalarType r)
{
  m_Radius=r;
}
template<class ScalarType>
ScalarType
RadialBasisFunctionBase<ScalarType>
::GetRadius()
{
  return m_Radius;
}

} // namespace itk
}//namespace Statistics
#endif
