/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSquaredDifferenceErrorFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:04:59 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSquaredDifferenceErrorFunction_h
#define __itkSquaredDifferenceErrorFunction_h

#include "itkErrorFunctionBase.h"
#include "vnl/vnl_matrix.h"
#include "vnl/vnl_vector.h"

namespace itk
{
namespace Statistics
{

template<class TMeasurementVector, class ScalarType>
class SquaredDifferenceErrorFunction : public ErrorFunctionBase<TMeasurementVector, ScalarType>
{
public:

  /** Standard class typedefs. */
  typedef SquaredDifferenceErrorFunction                    Self;
  typedef ErrorFunctionBase<TMeasurementVector, ScalarType> Superclass;
  typedef SmartPointer<Self>                                Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;

  typedef typename Superclass::ErrorVectorType    ErrorVectorType;
  typedef typename Superclass::InternalVectorType InternalVectorType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SquaredDifferenceErrorFunction, FunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Evaluate at the specified Error position */
  virtual ScalarType Evaluate(const TMeasurementVector& Errors) const;

  virtual InternalVectorType EvaluateDerivative(const TMeasurementVector& Errors) const;

protected:

  SquaredDifferenceErrorFunction();
  virtual ~SquaredDifferenceErrorFunction();

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkSquaredDifferenceErrorFunction.txx"
#endif


#endif
