/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDotProductImageFilter_h
#define otbDotProductImageFilter_h

#include "itkMacro.h"
#include "otbUnaryFunctorImageFilter.h"

namespace otb
{

namespace Functor {

/** \class DotProductFunctor
 *
 * \brief Computes the dot product against a specific vector
 *
 *
 * \ingroup OTBCommon
 */
template<class TInput, class TOutput>
class DotProductFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;

  DotProductFunctor() {}
  virtual ~DotProductFunctor() {}

  bool operator !=(const DotProductFunctor& itkNotUsed(other)) const
  {
    return false;
  }

  bool operator ==(const DotProductFunctor& other) const
  {
    return !(*this != other);
  }

  const InputType& GetVector()
  {
    return m_Vector;
  }

  void SetVector(const InputType& m)
  {
    m_Vector = m;
  }

  OutputType operator ()(const InputType& in)
  {
    OutputType result = 0;
    for(unsigned int i = 0; i < in.Size(); ++i)
      {
      result += in[i] * m_Vector[i];
      }
    return result;
  }

private:
  InputType m_Vector;
};
}

/** \class DotProductImageFilter
 *
 * \brief Applies pixel-wise dot product to a VectorImage
 *
 * Given a vector, this filter outputs the dot product of each pixel of a multiband image
 * with respect to the specified vector
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBCommon
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT DotProductImageFilter :
  public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::DotProductFunctor<typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef DotProductImageFilter Self;
  typedef itk::UnaryFunctorImageFilter
     <TInputImage,
      TOutputImage,
      Functor::DotProductFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType>
     >                                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::DotProductFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> FunctorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DotProductImageFilter, itk::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  const InputPixelType& GetVector()
  {
    return this->GetFunctor().GetVector();
  }

  void SetVector(const InputPixelType& p)
  {
    this->GetFunctor().SetVector(p);
    this->Modified();
  }

protected:
  DotProductImageFilter();

  ~DotProductImageFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  DotProductImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDotProductImageFilter.hxx"
#endif

#endif
