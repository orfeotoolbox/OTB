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
#ifndef otbBinaryFunctorImageFilter_h
#define otbBinaryFunctorImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace otb
{
/**
 * \class BinaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation on two images.
 *
 * Add the capability to change the number of channel when operation on
 * VectorImage compared to the itk::BinaryFunctorImageFilter
 *
 * The number of channel is provided by the functor: TFunction::GetOutputSize. If
 * this number is lower or equal to zero, the behavior of the itk::BinaryFunctorImageFilter
 * remains unchanged.
 *
 * \sa itk::BinaryFunctorImageFilter
 *
 * \ingroup OTBImageManipulation
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
  BinaryFunctorImageFilter() {};
  ~BinaryFunctorImageFilter() override {}

  /** BinaryFunctorImageFilter can produce an image which has a different number of bands
   * than its input image.  As such, BinaryFunctorImageFilter
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
  BinaryFunctorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
