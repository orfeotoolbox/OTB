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
#ifndef otbTernaryFunctorImageFilter_h
#define otbTernaryFunctorImageFilter_h

#include "itkTernaryFunctorImageFilter.h"

namespace otb
{
/**
 * \class TernaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation on three images.
 *
 * Add the capability to change the number of channel when operation on
 * VectorImage compared to the itk::TernaryFunctorImageFilter
 *
 * The number of channel is provided by the functor: TFunction::GetOutputSize. If
 * this number is lower or equal to zero, the behavior of the itk::TernaryFunctorImageFilter
 * remains unchanged.
 *
 * \sa itk::TernaryFunctorImageFilter
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage1, class TInputImage2, class TInputImage3, class TOutputImage, class TFunction>
class ITK_TEMPLATE_EXPORT TernaryFunctorImageFilter : public itk::TernaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef TernaryFunctorImageFilter                                            Self;
  typedef itk::TernaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                            Pointer;
  typedef itk::SmartPointer<const Self>                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TernaryFunctorImageFilter, itk::TernaryFunctorImageFilter);

protected:
  TernaryFunctorImageFilter() {};
  ~TernaryFunctorImageFilter() override {}

  /** TernaryFunctorImageFilter can produce an image which has a different number of bands
   * than its input image.  As such, TernaryFunctorImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  void GenerateOutputInformation() override
  {
    Superclass::GenerateOutputInformation();
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    outputPtr->SetNumberOfComponentsPerPixel( // propagate vector length info
      this->GetFunctor().GetOutputSize());
  }

private:
  TernaryFunctorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
