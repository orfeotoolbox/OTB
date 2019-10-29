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


#ifndef otbWaveletImageFilter_h
#define otbWaveletImageFilter_h

#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbWaveletsBandsListToWaveletsSynopsisImageFilter.h"

namespace otb
{

/** \class WaveletImageFilter
 * \brief
 * This filter performs wavelet forward transform.
 * It takes three template arguments: Input, Output and WaveletOperator
 * This filter is not streamed.
 *
 * \ingroup OTBWavelet
 * \sa WaveletInverseImageFilter
 * \sa WaveletsBandsListToWaveletsSynopsisImageFilter
 * \sa WaveletGenerator
 */
template <class TInputImage, class TOutputImage, Wavelet::Wavelet TMotherWaveletOperator>
class WaveletImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);
  itkStaticConstMacro(MotherWaveletOperator, short, TMotherWaveletOperator);
  itkStaticConstMacro(DirectionOfTransformation, short, otb::Wavelet::FORWARD);

  typedef WaveletImageFilter<InputImageType, OutputImageType, TMotherWaveletOperator> Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename InputImageType::PixelType InputPixelType;

  typedef otb::WaveletOperator<TMotherWaveletOperator, otb::Wavelet::FORWARD, InputPixelType, ImageDimension> WaveletOperatorType;
  typedef otb::WaveletFilterBank<InputImageType, InputImageType, WaveletOperatorType, otb::Wavelet::FORWARD>  FilterBankType;
  typedef otb::WaveletTransform<InputImageType, InputImageType, FilterBankType, otb::Wavelet::FORWARD>        WaveletTransformFilterType;
  typedef typename WaveletTransformFilterType::Pointer WaveletTransformFilterPointerType;

  typedef typename WaveletTransformFilterType::OutputImageListType WaveletTransformFilterOutputImageListType;
  typedef otb::WaveletsBandsListToWaveletsSynopsisImageFilter<WaveletTransformFilterOutputImageListType, OutputImageType>
                                                                              WaveletBandsListToWaveletsSynopsisImageFilterType;
  typedef typename WaveletBandsListToWaveletsSynopsisImageFilterType::Pointer WaveletBandsListToWaveletsSynopsisImageFilterPointerType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(WaveletImageFilter, ImageToImageFilter);

  itkGetMacro(NumberOfDecompositions, unsigned int);
  itkSetMacro(NumberOfDecompositions, unsigned int);

protected:
  WaveletImageFilter();
  virtual ~WaveletImageFilter();

  virtual void GenerateInputRequestedRegion() override;

  virtual void GenerateData() override;

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  WaveletImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  WaveletTransformFilterPointerType                        m_WaveletTransform;
  WaveletBandsListToWaveletsSynopsisImageFilterPointerType m_WaveletBandsListToWaveletsSynopsis;
  unsigned int                                             m_NumberOfDecompositions;
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletImageFilter.hxx"
#endif

#endif
