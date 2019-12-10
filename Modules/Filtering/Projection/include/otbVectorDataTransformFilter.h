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

#ifndef otbVectorDataTransformFilter_h
#define otbVectorDataTransformFilter_h

#include "otbVectorDataToVectorDataFilter.h"
#include "itkTransform.h"
#include "otbImageKeywordlist.h"
#include "otbVectorData.h"

namespace otb
{

/** \class VectorDataTransformFilter
  * \brief Apply a Transform To a VectorData
  *
  * This class is used to reproject vector data using a itk::Transform
  * the input and output coordinate system are the same
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
 *
 * \ingroup OTBProjection
  */

template <class TInputVectorData, class TOutputVectorData>
class ITK_EXPORT VectorDataTransformFilter : public otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataTransformFilter Self;
  typedef otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputVectorData                        InputVectorDataType;
  typedef TOutputVectorData                       OutputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;
  typedef typename TOutputVectorData::Pointer     OutputVectorDataPointer;

  /** Some typedefs. */
  typedef itk::Transform<double, 2, 2> GenericTransformType;
  typedef typename GenericTransformType::Pointer GenericTransformPointerType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  typedef typename InputVectorDataType::DataNodePointerType  InputDataNodePointerType;
  typedef typename OutputVectorDataType::DataNodeType        OutputDataNodeType;
  typedef typename OutputVectorDataType::DataNodePointerType OutputDataNodePointerType;
  typedef typename OutputVectorDataType::DataTreePointerType OutputDataTreePointerType;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType  InputInternalTreeNodeType;
  typedef typename OutputVectorDataType::DataTreeType::TreeNodeType OutputInternalTreeNodeType;
  typedef typename InputInternalTreeNodeType::ChildrenListType      InputChildrenListType;


  typedef typename OutputDataNodeType::PointType            PointType;
  typedef typename OutputDataNodeType::LineType             LineType;
  typedef typename OutputDataNodeType::LineConstPointerType LineConstPointerType;
  typedef typename OutputDataNodeType::LinePointerType      LinePointerType;

  typedef typename OutputDataNodeType::PolygonType             PolygonType;
  typedef typename OutputDataNodeType::PolygonConstPointerType PolygonConstPointerType;
  typedef typename OutputDataNodeType::PolygonPointerType      PolygonPointerType;

  typedef typename OutputDataNodeType::PolygonListType             PolygonListType;
  typedef typename OutputDataNodeType::PolygonListConstPointerType PolygonListConstPointerType;
  typedef typename OutputDataNodeType::PolygonListPointerType      PolygonListPointerType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataTransformFilter, VectorDataToVectorDataFilter);

  /** Set The Transform applied to VectorData*/
  itkSetObjectMacro(Transform, GenericTransformType);
  itkGetObjectMacro(Transform, GenericTransformType);

protected:
  VectorDataTransformFilter();
  ~VectorDataTransformFilter() override{};

  PointType ProcessPoint(PointType point) const override;
  LinePointerType ProcessLine(LinePointerType line) const override;
  PolygonPointerType ProcessPolygon(PolygonPointerType polygon) const override;
  PolygonListPointerType ProcessPolygonList(PolygonListPointerType polygonList) const override;

  void GenerateData(void) override;

private:
  VectorDataTransformFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  GenericTransformPointerType m_Transform;
};

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbVectorDataTransformFilter.hxx"
#endif

#endif
