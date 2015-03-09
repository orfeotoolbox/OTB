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
#ifndef __otbUnaryFunctorImageFilter_h
#define __otbUnaryFunctorImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/**
 * \class UnaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation on one image.
 *
 * Add the capability to change the number of channel when operation on
 * VectorImage compared to the itk::UnaryFunctorImageFilter
 *
 * The number of channel is provided by the functor: TFunction::OutputSize. If
 * this number is lower or equal to zero, the behavior of the itk::UnaryFunctorImageFilter
 * remains unchanged.
 *
 * \sa itk::UnaryFunctorImageFilter
 *
 * \ingroup OTBCommon
 */
template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT UnaryFunctorImageFilter : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef UnaryFunctorImageFilter                                            Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorImageFilter, itk::UnaryFunctorImageFilter);

protected:
  UnaryFunctorImageFilter() {};
  virtual ~UnaryFunctorImageFilter() {}

  /** UnaryFunctorImageFilter can produce an image which has a different number of bands
   * than its input image.  As such, UnaryFunctorImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    outputPtr->SetNumberOfComponentsPerPixel( // propagate vector length info
      this->GetFunctor().GetOutputSize());
  }

private:
  UnaryFunctorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
