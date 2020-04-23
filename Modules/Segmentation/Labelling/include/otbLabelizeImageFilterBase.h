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

#ifndef otbLabelizeImageFilterBase_h
#define otbLabelizeImageFilterBase_h

#include "itkImageToImageFilter.h"
#include "itkAddImageFilter.h"

#include "otbThresholdImageToPointSetFilter.h"
#include "otbMultiplyByScalarImageFilter.h"

namespace otb
{
/** \class LabelizeImageFilterBase
 *  \brief Base class for filter labelizing image region based on segmentation.
 *
 *
 * \ingroup OTBLabelling
 */
template <class TInputImage, class TOutputImage, class TFilter>
class ITK_EXPORT LabelizeImageFilterBase : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** typedef for standard classes. */
  typedef LabelizeImageFilterBase Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** typedef to simplify variables definition and declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  typedef typename TInputImage::IndexType IndexType;

  typedef itk::PointSet<InputPixelType, 2> PointSetType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  typedef TFilter                   RegionGrowingFilterType;
  typedef typename TFilter::Pointer RegionGrowingFilterPointerType;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(LabelizeImageFilterBase, ImageToImageFilter);

  /** Get lower threshold */
  itkGetMacro(LowerThreshold, InputPixelType);

  /** Set lower threshold */
  itkSetMacro(LowerThreshold, InputPixelType);

  /** Get upper threshold */
  itkGetMacro(UpperThreshold, InputPixelType);

  /** Set upper threshold */
  itkSetMacro(UpperThreshold, InputPixelType);

  /** Get Object count*/
  // only set after completion
  //       itkGetConstReferenceMacro(ObjectCount, unsigned long);
  itkGetMacro(ObjectCount, unsigned long);

  /** Internal */
  typedef otb::ThresholdImageToPointSetFilter<InputImageType, PointSetType> ThresholdFilterType;
  typedef typename ThresholdFilterType::Pointer ThresholdFilterPointerType;

  typedef otb::MultiplyByScalarImageFilter<InputImageType, OutputImageType> MultiplyFilterType;
  typedef typename MultiplyFilterType::Pointer MultiplyFilterPointerType;

  typedef itk::AddImageFilter<OutputImageType, OutputImageType, OutputImageType> AddImageFilterType;
  typedef typename AddImageFilterType::Pointer AddImageFilterPointerType;

protected:
  LabelizeImageFilterBase();
  ~LabelizeImageFilterBase() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Main computation method */
  void GenerateData() override;

  /** Region growing */
  virtual void RegionGrowing(const IndexType itkNotUsed(indexSeed))
  {
  }

  /** Instance region growing filter*/
  RegionGrowingFilterPointerType m_RegionGrowingFilter;

  /** Lower threshold */
  InputPixelType m_LowerThreshold;

  /** Upper threshold */
  InputPixelType m_UpperThreshold;

  /** Point set to store thresold results */
  PointSetPointerType m_PointSet;

  /** Multiply filter*/
  MultiplyFilterPointerType m_MultiplyFilter;

  /** Threshold point set filter */
  ThresholdFilterPointerType m_ThresholdPointSetFilter;

  /** Object counting */
  unsigned long m_ObjectCount;

private:
  LabelizeImageFilterBase(const Self&) = delete;
  void operator=(const Self&) = delete;

}; // end class LabelizeImageFilterBase

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelizeImageFilterBase.hxx"
#endif

#endif
