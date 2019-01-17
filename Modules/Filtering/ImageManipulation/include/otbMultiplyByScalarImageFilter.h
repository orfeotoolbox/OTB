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

#ifndef otbMultiplyByScalarImageFilter_h
#define otbMultiplyByScalarImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class MultiplyByScalarImageFilter
 * \brief TODO
 *
 * \ingroup OTBImageManipulation
 */

namespace Functor
{

template<class TInput, class TOutput>
class MultiplyByScalar
{
public:
  MultiplyByScalar()
  {
    m_Coef = 1.0;
  }
  virtual ~MultiplyByScalar() {}
  inline TOutput operator ()(const TInput& value)
  {
    TOutput result;

    result = static_cast<TOutput>(m_Coef * value);

    return result;
  }

  void SetCoef(double Coef)
  {
    m_Coef = Coef;
  }
  double GetCoef(void) const
  {
    return (m_Coef);
  }

private:
  double m_Coef;
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT MultiplyByScalarImageFilter :
  public itk::UnaryFunctorImageFilter<
      TInputImage, TOutputImage,
      Functor::MultiplyByScalar<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef MultiplyByScalarImageFilter Self;
  typedef typename itk::UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::MultiplyByScalar<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  void SetCoef(double Coef)
  {
    this->GetFunctor().SetCoef(Coef);
    this->Modified();
  }
  double GetCoef(void) const
  {
    return (this->GetFunctor().GetCoef());
  }
protected:
  MultiplyByScalarImageFilter() {}
  ~MultiplyByScalarImageFilter() override {}

private:
  MultiplyByScalarImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
