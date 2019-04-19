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
template <class TInputVectorPixel, class TOutputPixel>
class WaterSqrtSpectralAngleFunctor : public SqrtSpectralAngleFunctor<TInputVectorPixel, TOutputPixel>
{
public:
  typedef WaterSqrtSpectralAngleFunctor                             Self;
  typedef SqrtSpectralAngleFunctor<TInputVectorPixel, TOutputPixel> Superclass;
  typedef TInputVectorPixel                                         InputVectorPixelType;
  WaterSqrtSpectralAngleFunctor()
  {

    // Set the channels indices
    m_BlueIndex  = 0;
    m_GreenIndex = 1;
    m_RedIndex   = 2;
    m_NIRIndex   = 3;

    // Set reference water value
    InputVectorPixelType reference;
    reference.SetSize(4);
    reference[0] = 136.0;
    reference[1] = 132.0;
    reference[2] = 47.0;
    reference[3] = 24.0;
    this->SetReferenceWaterPixel(reference);
  }
  ~WaterSqrtSpectralAngleFunctor() override
  {
  }

  /** Set Reference Pixel */
  void SetReferenceWaterPixel(InputVectorPixelType ref)
  {
    if (ref.GetSize() != 4)
    {
    }
    InputVectorPixelType reference;
    reference.SetSize(4);
    reference[m_BlueIndex]  = ref[0];
    reference[m_GreenIndex] = ref[1];
    reference[m_RedIndex]   = ref[2];
    reference[m_NIRIndex]   = ref[3];
    this->SetReferencePixel(reference);
  }

  /** Getters and setters */
  void SetBlueChannel(unsigned int channel)
  {
    m_BlueIndex = channel;
  }
  unsigned int GetBlueChannel() const
  {
    return m_BlueIndex;
  }
  void SetGreenChannel(unsigned int channel)
  {
    m_GreenIndex = channel;
  }
  unsigned int GetGreenChannel() const
  {
    return m_GreenIndex;
  }
  void SetRedChannel(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  unsigned int GetRedChannel() const
  {
    return m_RedIndex;
  }
  void SetNIRChannel(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  unsigned int GetNIRChannel() const
  {
    return m_NIRIndex;
  }

protected:
  inline TOutputPixel Evaluate(const TInputVectorPixel& inPix) const override
  {
    return static_cast<TOutputPixel>(Superclass::Evaluate(inPix));
  }

  /** Channels */
  int m_BlueIndex;
  int m_GreenIndex;
  int m_RedIndex;
  int m_NIRIndex;
};
} // End namespace Functor


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
