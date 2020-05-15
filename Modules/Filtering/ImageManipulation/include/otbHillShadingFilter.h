/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbHillShadingFilter_h
#define otbHillShadingFilter_h

#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "otbHillShadingFunctor.h"

namespace otb
{
/** \class HillShadingFilter
 *  \brief Filter to compute the lambertian of a surface given as an image
 *
 *  The light source is assumed to be at a given elevation and azimuth
 *  (by default \f$ \pi/4 \f$ and \f$ \pi/6 \f$ respectively). This is used
 *  to generate hill shading representation of relief. The output is
 *  a value between 0 and 1.
 *
 *  \example BasicFilters/HillShadingExample.cxx
 *
 * \ingroup OTBImageManipulation
*/
template <class TInputImage, class TOutputImage>
class ITK_EXPORT HillShadingFilter
    : public UnaryFunctorNeighborhoodImageFilter<
          TInputImage, TOutputImage, Functor::HillShadingFunctor<itk::ConstNeighborhoodIterator<TInputImage>, TInputImage, typename TOutputImage::PixelType>>
{

public:
  /** Standard class typedefs. */
  typedef HillShadingFilter Self;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(HillShadingFilter, UnaryFunctorNeighborhoodImageFilter);

  double GetAzimuthLight() const
  {
    return this->GetFunctor().GetAzimuthLight();
  }

  void SetAzimuthLight(double az)
  {
    this->GetFunctor().SetAzimuthLight(az);
  }

  double GetElevationLight() const
  {
    return this->GetFunctor().GetElevationLight();
  }

  void SetElevationLight(double el)
  {
    this->GetFunctor().SetElevationLight(el);
  }

protected:
  HillShadingFilter()
  {
  }
  ~HillShadingFilter() override
  {
  }

private:
  HillShadingFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
}

#endif
