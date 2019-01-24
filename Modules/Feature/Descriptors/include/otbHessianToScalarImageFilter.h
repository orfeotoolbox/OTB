/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
  ~HessianToScalarImageFilter() override {}

private:
  HessianToScalarImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
