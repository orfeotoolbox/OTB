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

#ifndef otbVectorImageToAmplitudeImageFilter_h
#define otbVectorImageToAmplitudeImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "otbMath.h"
#include "itkConceptChecking.h"

namespace otb
{
namespace Functor
{
template <class TInput, class TOutput>
class VectorToAmplitudeFunctor
{
public:
  VectorToAmplitudeFunctor() {}
  virtual ~VectorToAmplitudeFunctor() {}

  inline TOutput operator ()(const TInput& A)
  {
    return static_cast<TOutput>(A.GetNorm());
  }

  itkConceptMacro(OutputShouldNotBeVectorImageCheck,
                  (itk::Concept::Convertible<TOutput, double>));

}; // end namespace Functor
}

/** \class VectorImageToAmplitudeImageFilter
 *  \brief This filter returns the the amplitude of the pixel vector.
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBImageManipulation
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT VectorImageToAmplitudeImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::VectorToAmplitudeFunctor<
          typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
  /** Standard typedefs */
  typedef VectorImageToAmplitudeImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::VectorToAmplitudeFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToAmplitudeImageFilter, UnaryFunctorImageFilter);

protected:
  /** Constructor */
  VectorImageToAmplitudeImageFilter() {};
  /** Destructor */
  ~VectorImageToAmplitudeImageFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  VectorImageToAmplitudeImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // End namespace otb
#endif
