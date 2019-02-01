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

#ifndef otbWaterSqrtSpectralAngleImageFilter_h
#define otbWaterSqrtSpectralAngleImageFilter_h

#include "otbWaterIndicesFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class WaterSqrtSpectralAngleImageFilter
 *  \brief Compute a radiometric water indice
 *
 *  This filter calculates a pixel wise water indice by calculating
 *  the spectral angle distance with a reference radiometry.
 *  This class is templated over the input multispectral image type,
 *  and the output image type.
 *  By default, the filter use the channel 0 as blue, the channel 1 as
 *  green, the channel 2 as red, and the channel 3 as near
 *  infrared. It can be changed with the SetBlue, SetGreen, SetRed and
 *  SetIR methods.
 *
 *  In the output image, value clse to zero denotes a highest
 *  probability of water.
 *
 *  \sa SpectralAngleDistanceImageFilter
 *
 * \ingroup OTBIndices
 */

template <class TInputVectorImage, class TOutputImage,
    class TFunction = Functor::WaterSqrtSpectralAngleFunctor <
        typename TInputVectorImage::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT WaterSqrtSpectralAngleImageFilter :
  public itk::UnaryFunctorImageFilter<TInputVectorImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef WaterSqrtSpectralAngleImageFilter                                        Self;
  typedef itk::UnaryFunctorImageFilter<TInputVectorImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                                  Pointer;
  typedef itk::SmartPointer<const Self>                                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(WaterSqrtSpectralAngleImageFilter, itk::UnaryFunctorImageFilter);

  /** Some convenient typedef. */
  typedef TInputVectorImage                        InputVectorImageType;
  typedef typename InputVectorImageType::PixelType InputVectorPixelType;
  typedef TOutputImage                             OutputImageType;

protected:
  WaterSqrtSpectralAngleImageFilter() {}
  ~WaterSqrtSpectralAngleImageFilter() override {}

private:
  WaterSqrtSpectralAngleImageFilter(Self &) = delete;
  void operator =(const Self&) = delete;

};
}  // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
