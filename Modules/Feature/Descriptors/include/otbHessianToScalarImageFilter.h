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
#ifndef otbHessianToScalarImageFilter_h
#define otbHessianToScalarImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class HessianToScalarImageFilter
 * \brief TODO
 *
 * \ingroup OTBDescriptors
 */

namespace Functor
{

template<class TInput, class TOutput>
class HessianToScalar
{
public:
  HessianToScalar()
  {
    m_Alpha = 1.0;
  }
  virtual ~HessianToScalar() {}
  inline TOutput operator ()(const TInput& Hessian)
  {

    TOutput det;
    TOutput trace;
    TOutput result;

    det   = static_cast<TOutput>(Hessian(0, 0) * Hessian(1, 1) - Hessian(0, 1) * Hessian(1, 0));
    trace = static_cast<TOutput>(Hessian(0, 0) + Hessian(1, 1));

    result = det - (static_cast<TOutput>(m_Alpha) * trace * trace);

    return result;
  }

  void SetAlpha(double Alpha)
  {
    m_Alpha = Alpha;
  }
  double GetAlpha(void) const
  {
    return (m_Alpha);
  }

private:
  double m_Alpha;
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT HessianToScalarImageFilter :
  public itk::UnaryFunctorImageFilter<
      TInputImage, TOutputImage,
      Functor::HessianToScalar<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef HessianToScalarImageFilter Self;
  typedef typename itk::UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::HessianToScalar<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  void SetAlpha(double Alpha)
  {
    this->GetFunctor().SetAlpha(Alpha);
    this->Modified();
  }
  double GetAlpha(void) const
  {
    return (this->GetFunctor().GetAlpha());
  }
protected:
  HessianToScalarImageFilter() {}
  ~HessianToScalarImageFilter() ITK_OVERRIDE {}

private:
  HessianToScalarImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
