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

#ifndef otbWatershedSegmentationFilter_h
#define otbWatershedSegmentationFilter_h

#include "otbMacro.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWatershedImageFilter.h"

namespace otb
{

/** \class WatershedSegmentationFilter
*   \brief Watershed composite filter allowing to set output type
*
*   This filter is a composite filter that encapsulates the
*   itk::WatershedImageFilter and allows setting the type of the output
*   image (which is not the case in the itk::WatershedImageFilter).
*
*   The composite filter internally casts the output from the
*   itk::WatershedImageFilter into TOutputLabelImage.
*
*   For more information, please refer to the documentation of the
*   original itk::WatershedImageFilter .
*
 *
 * \ingroup OTBWatersheds
*/
template <class TInputImage, class TOutputLabelImage>
class WatershedSegmentationFilter : public itk::ImageToImageFilter<TInputImage, TOutputLabelImage>
{
public:
  /** Standard Self typedef */
  typedef WatershedSegmentationFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputLabelImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage       InputImageType;
  typedef TOutputLabelImage OutputLabelImageType;

  typedef itk::WatershedImageFilter<TInputImage>        WatershedFilterType;
  typedef typename WatershedFilterType::OutputImageType InternalOutputImageType;

  typedef itk::CastImageFilter<InternalOutputImageType, OutputLabelImageType> CastImageFilterType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(WatershedSegmentationFilter, ImageToImageFilter);

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  otbSetObjectMemberMacro(WatershedFilter, Level, float);
  otbGetObjectMemberMacro(WatershedFilter, Level, float);

  otbSetObjectMemberMacro(WatershedFilter, Threshold, float);
  otbGetObjectMemberMacro(WatershedFilter, Threshold, float);


protected:
  WatershedSegmentationFilter();

  ~WatershedSegmentationFilter() override;

  void GenerateData() override;

private:
  typename CastImageFilterType::Pointer m_CastFilter;
  typename WatershedFilterType::Pointer m_WatershedFilter;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWatershedSegmentationFilter.hxx"
#endif

#endif
