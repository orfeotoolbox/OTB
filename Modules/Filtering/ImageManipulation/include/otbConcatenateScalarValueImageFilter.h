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

#ifndef otbConcatenateScalarValueImageFilter_h
#define otbConcatenateScalarValueImageFilter_h

#include "itkMacro.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

namespace Functor
{

/** \class ConcatenateScalarValueFunctor
 *
 * \brief TODO
 *
 *
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TOutput>
class ConcatenateScalarValueFunctor
{
public:
  typedef TInput  InputType;
  typedef TOutput OutputType;

  typedef typename InputType::ComponentType ComponentType;

  ConcatenateScalarValueFunctor() : m_ScalarValue(0)
  {
  }
  virtual ~ConcatenateScalarValueFunctor()
  {
  }

  const ComponentType& GetScalarValue()
  {
    return m_ScalarValue;
  }

  void SetScalarValue(const ComponentType& p)
  {
    m_ScalarValue = p;
  }

  bool operator!=(const ConcatenateScalarValueFunctor& itkNotUsed(other)) const
  {
    return false;
  }

  bool operator==(const ConcatenateScalarValueFunctor& other) const
  {
    return !(*this != other);
  }

  OutputType operator()(const InputType& in)
  {
    OutputType out(in.Size() + 1);
    for (unsigned int j = 0; j < in.Size(); ++j)
    {
      out[j] = in[j];
    }
    out[in.Size()] = m_ScalarValue;
    return out;
  }

private:
  ComponentType m_ScalarValue;
};
}

/** \class ConcatenateScalarValueImageFilter
 *
 * \brief Applies a projective projection to each pixel of an image
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ConcatenateScalarValueImageFilter
    : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                                          Functor::ConcatenateScalarValueFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs. */
  typedef ConcatenateScalarValueImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                                       Functor::ConcatenateScalarValueFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::ConcatenateScalarValueFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateScalarValueImageFilter, otb::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType        InputPixelType;
  typedef typename InputPixelType::ComponentType InputComponentType;
  typedef typename TOutputImage::PixelType       OutputPixelType;

  const InputComponentType& GetScalarValue()
  {
    return this->GetFunctor().GetScalarValue();
  }

  void SetScalarValue(const InputComponentType& p)
  {
    this->GetFunctor().SetScalarValue(p);
    this->Modified();
  }

protected:
  ConcatenateScalarValueImageFilter();

  ~ConcatenateScalarValueImageFilter() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateOutputInformation() override;

private:
  ConcatenateScalarValueImageFilter(const Self&) = delete;

  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateScalarValueImageFilter.hxx"
#endif

#endif
