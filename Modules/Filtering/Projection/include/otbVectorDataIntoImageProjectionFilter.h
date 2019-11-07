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

#ifndef otbVectorDataIntoImageProjectionFilter_h
#define otbVectorDataIntoImageProjectionFilter_h


#include "otbImage.h"

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataExtractROI.h"

namespace otb
{

/** \class VectorDataIntoImageProjectionFilter
  * \brief Reproject vector data into the coordinate system of an image
  *
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  *
  *
 *
 * \ingroup OTBProjection
  */

template <class TInputVectorData, class TInputImage>
class ITK_EXPORT VectorDataIntoImageProjectionFilter : public otb::VectorDataToVectorDataFilter<TInputVectorData, TInputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataIntoImageProjectionFilter Self;
  typedef otb::VectorDataToVectorDataFilter<TInputVectorData, TInputVectorData> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** VectorData typedefs. */
  typedef TInputVectorData                        InputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;

  /** Image typedefs. */
  typedef TInputImage                   ImageType;
  typedef typename TInputImage::Pointer ImagePointerType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  // use to display vectordata
  typedef itk::PreOrderTreeIterator<typename InputVectorDataType::DataTreeType> TreeIteratorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataIntoImageProjectionFilter, VectorDataToVectorDataFilter);

  void SetInputVectorData(InputVectorDataPointer input)
  {
    this->SetInput(input);
  }

  void SetInputImage(ImagePointerType input)
  {
    m_InputImage = input;
  }

  /** Set the origin of the output vector data.
   * \sa GetOrigin() */
  itkSetMacro(OutputOrigin, OriginType);
  virtual void SetOutputOrigin(const double origin[2]);
  virtual void SetOutputOrigin(const float origin[2]);

  itkGetConstReferenceMacro(OutputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the output vector data.
   * \sa GetSignedSpacing() */
  virtual void SetOutputSpacing(const SpacingType& spacing);
  virtual void SetOutputSpacing(const double spacing[2]);
  virtual void SetOutputSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  virtual void SetUseOutputSpacingAndOriginFromImage(bool flag);

protected:
  VectorDataIntoImageProjectionFilter();

  ~VectorDataIntoImageProjectionFilter() override
  {
  }

  void GenerateData(void) override;

  // Projection filter
  typedef VectorDataProjectionFilter<InputVectorDataType, InputVectorDataType> VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<InputVectorDataType>     VectorDataExtractROIType;
  typedef typename VectorDataExtractROIType::RegionType RemoteSensingRegionType;

private:
  VectorDataIntoImageProjectionFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  ImagePointerType m_InputImage;

  typename VectorDataProjectionFilterType::Pointer m_VdProjFilter;
  typename VectorDataExtractROIType::Pointer       m_VdExtractFilter;

  SpacingType m_OutputSpacing;
  OriginType  m_OutputOrigin;

  bool m_UseOutputSpacingAndOriginFromImage;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataIntoImageProjectionFilter.hxx"
#endif

#endif
