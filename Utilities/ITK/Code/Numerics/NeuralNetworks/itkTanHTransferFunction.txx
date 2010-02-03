/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTanHTransferFunction.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-19 04:37:20 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTanHTransferFunction_txx
#define __itkTanHTransferFunction_txx

namespace itk
{
namespace Statistics
{

/** Constructor */
template<class ScalarType>
TanHTransferFunction<ScalarType>
::TanHTransferFunction()
{
}

/** Destructor */
template<class ScalarType>
TanHTransferFunction<ScalarType> 
::~TanHTransferFunction()
{
}

/** Evaluate */
template<class ScalarType>
ScalarType
TanHTransferFunction<ScalarType>
::Evaluate(const ScalarType& input)  const
{
  ScalarType x = vcl_exp(input);
  ScalarType y = vcl_exp(-input);
  return static_cast<ScalarType>((float) (x - y) / (x + y));
}

/** Evaluate derivatives */
template<class ScalarType>
ScalarType
TanHTransferFunction<ScalarType>
::EvaluateDerivative(const ScalarType& input)  const
{
  ScalarType f = Evaluate(input); 
  return 1 - vcl_pow(f, 2);
}

/** Print the object */
template<class ScalarType>
void  
TanHTransferFunction<ScalarType>
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "TanHTransferFunction(" << this << ")" << std::endl; 
  Superclass::PrintSelf( os, indent ); 
}

} // end namespace Statistics
} // end namespace itk

#endif
