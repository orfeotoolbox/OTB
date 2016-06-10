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
#ifndef __otbPointSetDensityGaussianFunction_h
#define __otbPointSetDensityGaussianFunction_h

#include "otbPointSetFunction.h"
#include "itkPoint.h"

namespace otb
{

/**
 * \class PointSetDensityGaussianFunction
 * \brief Calculate the density in the neighborhood of a pixel using a gaussian function
 *
 *
 * \ingroup PointSetFunctions
 *
 * \ingroup OTBDensity
 */
template <class TPointSet, class TOutput>
class ITK_EXPORT PointSetDensityGaussianFunction : public PointSetFunction<TPointSet, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef PointSetDensityGaussianFunction      Self;
  typedef PointSetFunction<TPointSet, TOutput> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetDensityGaussianFunction, PointSetFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** PointSet Type typedef Support*/
  typedef TPointSet                       PointSetType;
  typedef typename Superclass::InputType  InputType;
  typedef typename  PointSetType::Pointer PointSetPointerType;

  /** TOutput typedef support*/
  typedef TOutput OutputType;

  /** Set/Get the number of scales*/
  itkSetMacro(Radius, unsigned int);
  itkGetMacro(Radius, unsigned int);

  /** Evaluate Method */
  OutputType Evaluate(const InputType& input) const ITK_OVERRIDE;

protected:
  PointSetDensityGaussianFunction() : m_Radius(1) {};
  ~PointSetDensityGaussianFunction() ITK_OVERRIDE {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  PointSetDensityGaussianFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  unsigned int m_Radius;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetDensityGaussianFunction.txx"
#endif

#endif
