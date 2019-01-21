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

#ifndef otbAssociativeSymmetricalSumImageFilter_h
#define otbAssociativeSymmetricalSumImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace otb
{

/** \class AssociativeSymmetricalSumImageFilter
 * \brief Implements an asymmetric fusion of line detector image filter.
 *
 * This class implements a fusion of the output images of
 * otb::LineRatioDetector and otb::LineCorrelationDetector
 *
 * The associative symmetrical sum \f$ \sigma(x, y) \f$ is used to merge information from the
 * two detectors:
   \f[ \sigma(x, y)=\frac{xy}{1-x-y+2xy} \f] with \f$ x, y \in [0, 1] \f$.
 *
 *
 *
 * \ingroup OTBEdge
 */

namespace Functor
{
/** \class AssociativeSymmetricalSum
 * \brief Functor used with the AssociativeSymmetricalSumImageFilter.
 *
 * \ingroup OTBEdge
 */
template<class TInput1, class TInput2, class TOutput>
class ITK_EXPORT AssociativeSymmetricalSum
{
public:
  AssociativeSymmetricalSum() {};
  virtual ~AssociativeSymmetricalSum() {}

  inline TOutput operator ()(const TInput1 X,
                             const TInput2 Y)
  {

    TOutput SumXY = 0.;

    SumXY += static_cast<TOutput>((X * Y) / (1. - X - Y + (2 * X * Y)));

    return (SumXY);
  }
};
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT AssociativeSymmetricalSumImageFilter :
  public
  itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage,
      Functor::AssociativeSymmetricalSum<
          typename TInputImage1::PixelType,
          typename TInputImage2::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef AssociativeSymmetricalSumImageFilter Self;
  typedef itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage,
      Functor::AssociativeSymmetricalSum<
          typename TInputImage1::PixelType,
          typename TInputImage1::PixelType,
          typename TOutputImage::PixelType>
      > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  AssociativeSymmetricalSumImageFilter() {}
  ~AssociativeSymmetricalSumImageFilter() override {}

private:
  AssociativeSymmetricalSumImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
