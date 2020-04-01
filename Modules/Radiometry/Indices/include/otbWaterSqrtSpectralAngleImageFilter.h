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

#include "otbSqrtSpectralAngleFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbRadiometricIndex.h"

namespace otb
{

namespace Functor
{
/** \class WaterSqrtSpectralAngleFunctor
 *  \brief This functor uses a spectral angle with a particular reference pixel.
 *
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class WaterSqrtSpectralAngleFunctor : public RadiometricIndex<TInput, TOutput>
{
public:
  using typename RadiometricIndex<TInput, TOutput>::PixelType;

  WaterSqrtSpectralAngleFunctor() 
        : RadiometricIndex<TInput, TOutput>({CommonBandNames::BLUE, CommonBandNames::GREEN, CommonBandNames::RED, CommonBandNames::NIR} )
  {
    // Set default reference water value
    m_ReferencePixel.SetSize(4);
    m_ReferencePixel[0] = 136.0;
    m_ReferencePixel[1] = 132.0;
    m_ReferencePixel[2] = 47.0;
    m_ReferencePixel[3] = 24.0;
    m_RefNorm = m_ReferencePixel.GetNorm();
  }
  
  virtual ~WaterSqrtSpectralAngleFunctor() = default;

  /** Set Reference Pixel 
   * \param ref : The new reference pixel, the band indices will be used.
   * */
  void SetReferenceWaterPixel(PixelType ref)
  {
    assert(m_ReferencePixel.Size == 4);
    m_ReferencePixel[0] = this->Value(CommonBandNames::BLUE, ref);
    m_ReferencePixel[1] = this->Value(CommonBandNames::GREEN, ref);
    m_ReferencePixel[2] = this->Value(CommonBandNames::RED, ref);
    m_ReferencePixel[3] = this->Value(CommonBandNames::NIR, ref);
    m_RefNorm = m_ReferencePixel.GetNorm();
  }

  // Binary operator
  inline TOutput operator()(PixelType const & inPix) const override
  {
    PixelType pix(4);
    pix[0] = this->Value(CommonBandNames::BLUE, inPix);
    pix[1] = this->Value(CommonBandNames::GREEN, inPix);
    pix[2] = this->Value(CommonBandNames::RED, inPix);
    pix[3] = this->Value(CommonBandNames::NIR, inPix);

    return std::sqrt(SpectralAngleDetails::ComputeSpectralAngle<PixelType, PixelType, TOutput>
                                                                  (pix,
                                                                    m_ReferencePixel, 
                                                                    m_RefNorm));
  }

  /**@{*/ 
  /** Legacy getters and setters (use SetBandIndex/GetBandIndex instead)
   * \deprecated
   * */
  void SetBlueChannel(unsigned int channel)
  {
    this->SetBandIndex(CommonBandNames::BLUE, channel + 1);
  }
  unsigned int GetBlueChannel() const
  {
    return this->GetBandIndex(CommonBandNames::BLUE) - 1;
  }
  void SetGreenChannel(unsigned int channel)
  {
    this->SetBandIndex(CommonBandNames::GREEN, channel + 1);
  }
  unsigned int GetGreenChannel() const
  {
    return this->GetBandIndex(CommonBandNames::GREEN) - 1;
  }
  void SetRedChannel(unsigned int channel)
  {
    this->SetBandIndex(CommonBandNames::RED, channel + 1);
  }
  unsigned int GetRedChannel() const
  {
     return this->GetBandIndex(CommonBandNames::RED) - 1;
  }
  void SetNIRChannel(unsigned int channel)
  {
    this->SetBandIndex(CommonBandNames::NIR, channel + 1);
  }
  unsigned int GetNIRChannel() const
  {
    return this->GetBandIndex(CommonBandNames::NIR) - 1;
  }
  /**@}*/
  

private:

  PixelType m_ReferencePixel;
  double m_RefNorm;
};
} // End namespace Functor


/** \class WaterSqrtSpectralAngleImageFilter
 *  \deprecated
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
          class TFunction = Functor::WaterSqrtSpectralAngleFunctor<double, typename TOutputImage::PixelType>>
class ITK_EXPORT WaterSqrtSpectralAngleImageFilter : public itk::UnaryFunctorImageFilter<TInputVectorImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef WaterSqrtSpectralAngleImageFilter Self;
  typedef itk::UnaryFunctorImageFilter<TInputVectorImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(WaterSqrtSpectralAngleImageFilter, itk::UnaryFunctorImageFilter);

  /** Some convenient typedef. */
  typedef TInputVectorImage                        InputVectorImageType;
  typedef typename InputVectorImageType::PixelType InputVectorPixelType;
  typedef TOutputImage                             OutputImageType;

protected:
  WaterSqrtSpectralAngleImageFilter()
  {
  }
  ~WaterSqrtSpectralAngleImageFilter() override
  {
  }

private:
  WaterSqrtSpectralAngleImageFilter(Self&) = delete;
  void operator=(const Self&) = delete;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
