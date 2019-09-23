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

#ifndef otbImageToEnvelopeVectorDataFilter_h
#define otbImageToEnvelopeVectorDataFilter_h

#include "otbVectorDataSource.h"
#include "otbGenericRSTransform.h"
#include <string>

namespace otb
{

/** \class ImageToEnvelopeVectorDataFilter
  * \brief Build a vector data containing the polygon of the image envelope
  *
  * This filter uses the GenericRSTransform to project the four corners of the image into ground position.
  * If the sampling rate is not null, the image edges are also projected (using one point every
  * "m_SamplingRate" pixels).
  * In case of raw image geometry, a DEM directory or average elevation can be set for better accuracy.
  *
  * This filter supports user-specified output projection. If no projection is defined, the standard WGS84
  * projection will be used.
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  *
 *
 * \ingroup OTBProjection
  */
template <class TInputImage, class TOutputVectorData>
class ITK_EXPORT ImageToEnvelopeVectorDataFilter : public otb::VectorDataSource<TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef ImageToEnvelopeVectorDataFilter          Self;
  typedef otb::VectorDataSource<TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  typedef TInputImage                         InputImageType;
  typedef TOutputVectorData                   OutputVectorDataType;
  typedef typename TInputImage::ConstPointer  InputImagePointer;
  typedef typename TOutputVectorData::Pointer OutputVectorDataPointer;

  /** Some typedefs. */
  typedef otb::GenericRSTransform<double, 2, 2> InternalTransformType;
  typedef typename InternalTransformType::Pointer                   InternalTransformPointerType;
  typedef typename OutputVectorDataType::DataNodeType               OutputDataNodeType;
  typedef typename OutputVectorDataType::DataNodePointerType        OutputDataNodePointerType;
  typedef typename OutputVectorDataType::DataTreePointerType        OutputDataTreePointerType;
  typedef typename OutputVectorDataType::DataTreeType::TreeNodeType OutputInternalTreeNodeType;
  typedef typename OutputDataNodeType::PolygonType                  PolygonType;

  /** Set input image */
  using Superclass::SetInput;
  void SetInput(const InputImageType* input);

  /** Get input image */
  const InputImageType* GetInput();

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToEnvelopeVectorDataFilter, VectorDataSource);

  /** Set/Get output projection (default is WGS84)  */
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  itkSetMacro(SamplingRate, unsigned int);
  itkGetMacro(SamplingRate, unsigned int);

protected:
  ImageToEnvelopeVectorDataFilter();
  ~ImageToEnvelopeVectorDataFilter() override
  {
  }

  void GenerateOutputInformation(void) override;

  void GenerateInputRequestedRegion() override;

  void GenerateData(void) override;

  void InstantiateTransform();

private:
  ImageToEnvelopeVectorDataFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  InternalTransformPointerType m_Transform;
  std::string                  m_OutputProjectionRef;
  unsigned int                 m_SamplingRate; // Sampling rate for edges (in pixels)
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToEnvelopeVectorDataFilter.hxx"
#endif

#endif
