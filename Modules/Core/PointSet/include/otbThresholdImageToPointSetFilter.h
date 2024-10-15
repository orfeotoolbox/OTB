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

#ifndef otbThresholdImageToPointSetFilter_h
#define otbThresholdImageToPointSetFilter_h

#include "otbImageToPointSetFilter.h"
#include "itkPointSet.h"
#include "itkConceptChecking.h"

namespace otb
{

/** \class ThresholdImageToPointSetFilter
 * \brief Produce a PointSet according to filtering conditions
 *
 *
 * \ingroup OTBPointSet
 */

template <class TInputImage, class TOutputPointSet = itk::PointSet<typename TInputImage::PixelType, 2>>
class ITK_EXPORT ThresholdImageToPointSetFilter : public ImageToPointSetFilter<TInputImage, TOutputPointSet>
{
public:
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  typedef TInputImage InputImageType;

  typedef ThresholdImageToPointSetFilter Self;
  typedef ImageToPointSetFilter<InputImageType, TOutputPointSet> Superclass;
  typedef typename Superclass::OutputPointSetType OutputPointSetType;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ThresholdImageToPointSetFilter, ImageToPointSetFilter);

  typedef typename Superclass::InputImagePixelType       InputPixelType;
  typedef typename Superclass::InputImagePointer         InputImagePointer;
  typedef typename Superclass::InputImageRegionType      InputImageRegionType;
  typedef typename Superclass::InputImageConstPointer    InputImageConstPointer;
  typedef typename Superclass::InputImageType::SizeType  SizeType;
  typedef typename Superclass::InputImageType::IndexType IndexType;

  typedef typename Superclass::OutputPointSetPointer         OutputPointSetPointer;
  typedef typename Superclass::OutputPointSetType::PixelType OutputPointSetPixelType;
  typedef typename Superclass::PointsContainerType           PointsContainerType;
  typedef typename Superclass::PointDataContainerType        PointDataContainerType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(PixelConvertibleToPointSetDataType, (itk::Concept::Convertible<InputPixelType, typename PointDataContainerType::Element>));
#endif


  itkSetMacro(LowerThreshold, InputPixelType);
  itkGetConstReferenceMacro(LowerThreshold, InputPixelType);
  itkSetMacro(UpperThreshold, InputPixelType);
  itkGetConstReferenceMacro(UpperThreshold, InputPixelType);

protected:
  ThresholdImageToPointSetFilter();
  ~ThresholdImageToPointSetFilter() override
  {
  }

  void ThreadedGenerateData(const InputImageRegionType& inputRegionForThread, itk::ThreadIdType threadId) override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ThresholdImageToPointSetFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  InputPixelType m_LowerThreshold;
  InputPixelType m_UpperThreshold;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbThresholdImageToPointSetFilter.hxx"
#endif

#endif
