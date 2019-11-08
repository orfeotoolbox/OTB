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

#ifndef otbCloudEstimatorFilter_h
#define otbCloudEstimatorFilter_h

#include "otbSpectralAngleFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/** \class CloudEstimatorFilter
 *  \brief Filter to help with the cloud detection
 *
 * Applies spectral angle functor to an image.
 * Multiplies by a Gaussian coefficient
 * Applies a color reversal.
 *
 * \example FeatureExtraction/CloudDetectionExample.cxx
 *
 * \ingroup OTBCloudDetection
 */
template <class TInputImage, class TOutputImage,
          class TFunction = Functor::SpectralAngleFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
class ITK_EXPORT CloudEstimatorFilter : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef CloudEstimatorFilter Self;
  typedef typename itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CloudEstimatorFilter, UnaryFunctorImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointer;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  /** Getters/Setters */
  void SetReferencePixel(InputPixelType ref);
  InputPixelType GetReferencePixel() const;
  void SetVariance(double var);
  double GetVariance() const;

protected:
  CloudEstimatorFilter();

  ~CloudEstimatorFilter() override
  {
  }

  void BeforeThreadedGenerateData() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  CloudEstimatorFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCloudEstimatorFilter.hxx"
#endif

#endif
