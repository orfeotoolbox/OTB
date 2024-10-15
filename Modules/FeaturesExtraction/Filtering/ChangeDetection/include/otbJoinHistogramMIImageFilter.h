/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbJoinHistogramMIImageFilter_h
#define otbJoinHistogramMIImageFilter_h

#include "otbBinaryFunctorNeighborhoodJoinHistogramImageFilter.h"
#include "otbJoinHistogramMI.h"
#include "otbMacro.h" //for 

namespace otb
{

/** \class JoinHistogramMIImageFilter
 * \brief Implements neighborhood-wise the computation of mutual
 * information using a join histogram.
 *
 * This filter is parametrized over the types of the two
 * input images and the type of the output image.
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following:
 *
 * - cast the input 1 pixel value to \c double
 * - cast the input 2 pixel value to \c double
 * - compute the difference of the two pixel values
 * - compute the value of the mutual information
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *
 * The filter expect all images to have the same dimension
 * (e.g. all 2D, or all 3D, or all ND)
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBChangeDetection
 */

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT JoinHistogramMIImageFilter
    : public BinaryFunctorNeighborhoodJoinHistogramImageFilter<
          TInputImage1, TInputImage2, TOutputImage,
          Functor::JoinHistogramMI<typename itk::ConstNeighborhoodIterator<TInputImage1>, typename itk::ConstNeighborhoodIterator<TInputImage2>,
                                   typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs. */
  typedef JoinHistogramMIImageFilter Self;
  typedef BinaryFunctorNeighborhoodJoinHistogramImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      Functor::JoinHistogramMI<typename itk::ConstNeighborhoodIterator<TInputImage1>, typename itk::ConstNeighborhoodIterator<TInputImage2>,
                               typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro defining the type*/
  itkTypeMacro(JoinHistogramMIImageFilter, SuperClass);

protected:
  JoinHistogramMIImageFilter() = default;
  ~JoinHistogramMIImageFilter() = default;

private:
  JoinHistogramMIImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
