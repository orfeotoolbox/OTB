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

#ifndef otbCloudDetectionFilter_h
#define otbCloudDetectionFilter_h

#include "otbCloudDetectionFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/** \class CloudDetectionFilter
 * \brief Applies cloud detection functor to an image.
 *
 * \ingroup OTBCloudDetection
 */
template <class TInputImage, class TOutputImage,
          class TFunction = Functor::CloudDetectionFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
class ITK_EXPORT CloudDetectionFilter : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef CloudDetectionFilter Self;
  typedef typename itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CloudDetectionFilter, UnaryFunctorImageFilter);

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
  void SetVariance(double var);
  void SetMinThreshold(double threshold);
  void SetMaxThreshold(double threshold);
  InputPixelType GetReferencePixel();
  double         GetMinThreshold();
  double         GetMaxThreshold();
  double         GetVariance();

protected:
  CloudDetectionFilter();

  ~CloudDetectionFilter() override
  {
  }

  void BeforeThreadedGenerateData() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  CloudDetectionFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCloudDetectionFilter.hxx"
#endif

#endif
