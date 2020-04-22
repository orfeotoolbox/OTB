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

#ifndef otbHarrisImageToPointSetFilter_h
#define otbHarrisImageToPointSetFilter_h

#include "otbThresholdImageToPointSetFilter.h"
#include "otbHarrisImageFilter.h"

namespace otb
{

/** \class HarrisImageToPointSetFilter
 * \brief TODO
 *
 * \ingroup OTBCorner
 */

template <class TInputImage, class TOutputPointSet = itk::PointSet<typename TInputImage::PixelType, 2>>
class ITK_EXPORT HarrisImageToPointSetFilter : public ImageToPointSetFilter<TInputImage, TOutputPointSet>
{
public:
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  typedef TInputImage     InputImageType;
  typedef TOutputPointSet OutputPointSetType;

  typedef HarrisImageToPointSetFilter Self;
  typedef ImageToPointSetFilter<InputImageType, OutputPointSetType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(HarrisImageToPointSetFilter, ImageToPointSetFilter);

  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename InputImageType::SizeType  SizeType;

  typedef typename otb::HarrisImageFilter<InputImageType, InputImageType>                  HarrisImageFilterType;
  typedef typename otb::ThresholdImageToPointSetFilter<InputImageType, OutputPointSetType> ThresholdImageToPointSetType;

  typedef typename OutputPointSetType::PixelType OutputPixelType;

  itkSetMacro(SigmaD, double);
  itkGetConstReferenceMacro(SigmaD, double);
  itkSetMacro(SigmaI, double);
  itkGetConstReferenceMacro(SigmaI, double);
  itkSetMacro(Alpha, double);
  itkGetConstReferenceMacro(Alpha, double);
  itkSetMacro(LowerThreshold, InputPixelType);
  itkGetConstReferenceMacro(LowerThreshold, InputPixelType);
  itkSetMacro(UpperThreshold, InputPixelType);
  itkGetConstReferenceMacro(UpperThreshold, InputPixelType);

protected:
  HarrisImageToPointSetFilter();
  ~HarrisImageToPointSetFilter() override
  {
  }

  void GenerateData() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  HarrisImageToPointSetFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  InputPixelType m_LowerThreshold;
  InputPixelType m_UpperThreshold;
  double         m_SigmaD;
  double         m_SigmaI;
  double         m_Alpha;

  typename HarrisImageFilterType::Pointer        m_HarrisFilter;
  typename ThresholdImageToPointSetType::Pointer m_ThresholdFilter;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHarrisImageToPointSetFilter.hxx"
#endif

#endif
