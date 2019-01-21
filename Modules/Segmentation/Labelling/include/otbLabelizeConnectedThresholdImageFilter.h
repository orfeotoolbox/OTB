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

#ifndef otbLabelizeConnectedThresholdImageFilter_h
#define otbLabelizeConnectedThresholdImageFilter_h

#include "itkConnectedThresholdImageFilter.h"
#include "otbLabelizeImageFilterBase.h"

namespace otb
{

/** \class LabelizeConnectedThresholdImageFilter
 *  \brief TODO
 *
 *
 * \ingroup OTBLabelling
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelizeConnectedThresholdImageFilter
  : public otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::ConnectedThresholdImageFilter<TInputImage, TOutputImage> >
{
public:
  /** typedef for standard classes. */
  typedef LabelizeConnectedThresholdImageFilter
  Self;
  typedef otb::LabelizeImageFilterBase<TInputImage, TOutputImage,
      itk::ConnectedThresholdImageFilter<TInputImage, TOutputImage> > Superclass;
  typedef itk::SmartPointer<Self>
  Pointer;
  typedef itk::SmartPointer<const Self>
  ConstPointer;

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TInputImage::IndexType  IndexType;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(LabelizeConnectedThresholdImageFilter, LabelizeImageFilterBase);

  /** Get delta lower threshold */
  itkGetMacro(LowerThresholdDelta, InputPixelType);

  /** Set delta lower threshold */
  itkSetMacro(LowerThresholdDelta, InputPixelType);

  /** Get delta upper threshold */
  itkGetMacro(UpperThresholdDelta, InputPixelType);

  /** Set delta upper threshold */
  itkSetMacro(UpperThresholdDelta, InputPixelType);

  /** Get replace value */
  itkGetMacro(ReplaceValue, OutputPixelType);

  /** Set replace value */
  itkSetMacro(ReplaceValue, OutputPixelType);

protected:
  LabelizeConnectedThresholdImageFilter();
  ~LabelizeConnectedThresholdImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Region growing */
  void RegionGrowing(const IndexType indexSeed) override;

private:
  LabelizeConnectedThresholdImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Delta + threshold for growing region */
  InputPixelType m_UpperThresholdDelta;

  /** Delta - threshold for growing region */
  InputPixelType m_LowerThresholdDelta;

  /** Initial replace value*/
  OutputPixelType m_ReplaceValue;

}; // end class LabelizeconnectedThresholdImageFilter

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelizeConnectedThresholdImageFilter.hxx"
#endif

#endif
