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

#ifndef otbImageToLabelMapWithAttributesFilter_h
#define otbImageToLabelMapWithAttributesFilter_h

#include "itkShapeLabelMapFilter.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "otbLabelImageToLabelMapWithAdjacencyFilter.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "otbLabelMapWithClassLabelToClassLabelImageFilter.h"


namespace otb
{

/** \class ImageToLabelMapWithAttributesFilter
 * \brief Convert a labeled image to a label map with adjacency information
 *        and computes feature attributes for each LabelObject
 *
 * Convert a labeled image and its associated vector image
 * to a label map with adjacency information, and computes shape and
 * radiometric attributes for each LabelObject.
 *
 * It is suitable to be used with otb::LabelMapWithAdjacency<otb::AttributesMapLabelObject>
 *
 * \ingroup OTBLabelMap
 */
template <class TInputImage, class TLabeledImage, class TLabel, class TLabelObject>
class ITK_EXPORT ImageToLabelMapWithAttributesFilter : public itk::ImageToImageFilter<TInputImage, LabelMapWithAdjacency<TLabelObject>>
{

public:
  /** Standard class typedefs */
  typedef ImageToLabelMapWithAttributesFilter Self;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  typedef itk::ImageToImageFilter<TInputImage, LabelMapWithAdjacency<TLabelObject>> Superclass;

  /** Standard type macro */
  itkTypeMacro(ImageToLabelMapWithAttributesFilter, itk::ImageToImageFilter);

  /** New macro*/
  itkNewMacro(Self);

  typedef TInputImage   InputImageType;
  typedef TLabeledImage LabeledImageType;
  typedef TLabelObject  LabelObjectType;

  typedef typename LabelObjectType::LabelType                LabelType;
  typedef LabelMapWithAdjacency<LabelObjectType>             LabelMapType;
  typedef typename LabelMapType::AdjacentLabelsContainerType AdjacentLabelsContainerType;

  typedef LabelImageToLabelMapWithAdjacencyFilter<LabeledImageType, LabelMapType> LabelMapFilterType;
  typedef ShapeAttributesLabelMapFilter<LabelMapType> ShapeLabelMapFilterType;
  typedef BandsStatisticsAttributesLabelMapFilter<LabelMapType, InputImageType> BandStatisticsLabelMapFilterType;

  using Superclass::SetInput;
  void SetInput(const InputImageType* image) override;
  virtual void SetLabeledImage(const LabeledImageType* image);
  const InputImageType*   GetInput(void);
  const LabeledImageType* GetLabeledImage();
  virtual LabelMapType*   GetOutput();

  void GenerateData() override;

protected:
  /** Constructor */
  ImageToLabelMapWithAttributesFilter();
  /** Destructor */
  ~ImageToLabelMapWithAttributesFilter() override{};

  void GenerateInputRequestedRegion() override;


private:
  ImageToLabelMapWithAttributesFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  typename LabelMapType::Pointer m_Output;
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToLabelMapWithAttributesFilter.hxx"
#endif
#endif
