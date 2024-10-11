/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbKullbackLeiblerSupervizedDistanceImageFilter_hxx
#define otbKullbackLeiblerSupervizedDistanceImageFilter_hxx

#include "otbKullbackLeiblerSupervizedDistanceImageFilter.h"

namespace otb
{

namespace Functor
{

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>::KullbackLeiblerSupervizedDistance()
{
  m_CumROI1 = nullptr;
  m_CumROI2 = nullptr;
}

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>::~KullbackLeiblerSupervizedDistance()
{
  delete m_CumROI1;
  m_CumROI1 = nullptr;

  delete m_CumROI2;
  m_CumROI2 = nullptr;
}

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
void KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>::Evaluate(const typename TInput1::ImageType* img1,
                                                                                            const typename TInput2::ImageType* img2,
                                                                                            const TInputROIImage*              imgROI)
{
  typename ROIConversionType1::Pointer conversion1 = ROIConversionType1::New();
  conversion1->SetInputImage(img1);
  conversion1->SetROIImage(imgROI);
  conversion1->Update();

  if (m_CumROI1 != nullptr)
    delete m_CumROI1;

  m_CumROI1 = new CumulantsForEdgeworth<ROIInputType1>(conversion1->GetOutput());

  if (!m_CumROI1->IsDataAvailable())
  {
    throw itk::ExceptionObject(__FILE__, __LINE__, "Cumulants estimated from ROI in image 1 are not usable", ITK_LOCATION);
  }

  typename ROIConversionType2::Pointer conversion2 = ROIConversionType2::New();
  conversion2->SetInputImage(img2);
  conversion2->SetROIImage(imgROI);
  conversion2->Update();

  if (m_CumROI2 != nullptr)
    delete m_CumROI2;

  m_CumROI2 = new CumulantsForEdgeworth<ROIInputType2>(conversion2->GetOutput());

  if (!m_CumROI2->IsDataAvailable())
  {
    throw itk::ExceptionObject(__FILE__, __LINE__, "Cumulants estimated from ROI in image 2 are not usable", ITK_LOCATION);
  }
}

template <class TInput1, class TInput2, class TInputROIImage, class TOutput>
TOutput KullbackLeiblerSupervizedDistance<TInput1, TInput2, TInputROIImage, TOutput>::operator()(const TInput1& it1, const TInput2& it2)
{
  CumulantsForEdgeworth<TInput1> cum1(it1);

  if (!cum1.IsDataAvailable())
  {
    return static_cast<TOutput>(0.);
  }

  CumulantsForEdgeworth<TInput2> cum2(it2);

  if (!cum2.IsDataAvailable())
  {
    return static_cast<TOutput>(0.);
  }

  return static_cast<TOutput>(m_CumROI1->Divergence(cum1) + m_CumROI2->Divergence(cum2));
}

} // end of namespace Functor

/**
 *  Connect the training area to build the reference pdfs,
 *  with parameters to be hold by
 *  Functor::KullbackLeiblerSupervizedDistance.
 *
 *  Images 1 & 2 are supposed to be already connected.
 */
template <class TInputImage1, class TInputImage2, class TInputROIImage, class TOutputImage>
void KullbackLeiblerSupervizedDistanceImageFilter<TInputImage1, TInputImage2, TInputROIImage, TOutputImage>::SetTrainingArea(
    const TInputROIImage* trainingImage)
{
  this->itk::ProcessObject::SetNthInput(2, const_cast<TInputROIImage*>(trainingImage));
}

template <class TInputImage1, class TInputImage2, class TInputROIImage, class TOutputImage>
void KullbackLeiblerSupervizedDistanceImageFilter<TInputImage1, TInputImage2, TInputROIImage, TOutputImage>::BeforeThreadedGenerateData(void)
{
  typename TInputImage1::ConstPointer   inputPtr1     = dynamic_cast<const TInputImage1*>(this->GetInput(0));
  typename TInputImage2::ConstPointer   inputPtr2     = dynamic_cast<const TInputImage2*>(this->GetInput(1));
  typename TInputROIImage::ConstPointer trainingImage = static_cast<const TInputROIImage*>(this->itk::ProcessObject::GetInput(2));

  this->GetFunctor().Evaluate(inputPtr1, inputPtr2, trainingImage);
}

} // end of namespace otb

#endif
