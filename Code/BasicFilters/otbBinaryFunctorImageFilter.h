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
#ifndef __otbBinaryFunctorImageFilter_h
#define __otbBinaryFunctorImageFilter_h

#include "otbMacro.h"
#include "itkBinaryFunctorImageFilter.h"

namespace otb
{
/**
 * \class BinaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation on one image.
 *
 * Add the capability to change the number of channel when operation on
 * VectorImage compared to the itk::BinaryFunctorImageFilter
 *
 * The number of channel is provided by the functor: TFunction::OutputSize. If
 * this number is lower or equal to zero, the behavior of the itk::BinaryFunctorImageFilter
 * remains unchanged.
 *
 * \sa itk::BinaryFunctorImageFilter
 */
template <class TInputImage1, class TInputImage2, class TOutputImage, class TFunction>
class ITK_EXPORT BinaryFunctorImageFilter : public itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef BinaryFunctorImageFilter                                            Self;
  typedef itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryFunctorImageFilter, itk::BinaryFunctorImageFilter);

protected:
  BinaryFunctorImageFilter() {}
  virtual ~BinaryFunctorImageFilter() {}

private:
  BinaryFunctorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
