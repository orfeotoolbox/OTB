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

#ifndef otbConcatenateVectorDataFilter_h
#define otbConcatenateVectorDataFilter_h

#include "otbVectorDataToVectorDataFilter.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"

namespace otb
{
/** \class ConcatenateVectorDataFilter
 *  \brief Produces a VectorData from the concatenation
 *  of several input vectordatas.
 *
 *  This generator produces a unique vector by concatening several
 *  inputs set via the method AddInput(VectorData *).
 *
 * Note that the input vectordatas must have the same node type,
 * this is due that vectordata creation does not support multiple geometries
 * in a single vectordata.
 * Warning : this filter does not create a deep copy of each node.
 *
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TVectorData>
class ITK_EXPORT ConcatenateVectorDataFilter : public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs */
  typedef ConcatenateVectorDataFilter   Self;
  typedef VectorDataSource<TVectorData> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateVectorDataFilter, VectorDataSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TVectorData                                    VectorDataType;
  typedef typename VectorDataType::Pointer               VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType          DataNodeType;
  typedef typename DataNodeType::Pointer                 DataNodePointerType;
  typedef typename DataNodeType::PolygonType::RegionType RegionType;
  typedef typename DataNodeType::PointType               PointType;
  typedef typename DataNodeType::PolygonType::VertexType VertexType;
  typedef typename DataNodeType::LineType                LineType;
  typedef typename VectorDataType::DataTreeType          DataTreeType;
  typedef typename DataTreeType::TreeNodeType            TreeNodeType;
  typedef typename TreeNodeType::ChildrenListType        ChildrenListType;

  typedef typename std::vector<PointType> PointVectorType;

  /** Connects the VectorData from which the localizations are going to be extracted. */
  void AddInput(const VectorDataType*);
  using Superclass::AddInput;

  const VectorDataType* GetInput(unsigned int idx) const;

protected:
  ConcatenateVectorDataFilter();
  ~ConcatenateVectorDataFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Triggers the Computation of the sample list */
  void GenerateData(void) override;

  /** Recursive method to visit efficiently the vectordata*/
  void ProcessNode(TreeNodeType* source, DataNodeType* outputDocument);

private:
  ConcatenateVectorDataFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  DataNodePointerType m_Folder;
  DataNodePointerType m_Document;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateVectorDataFilter.hxx"
#endif

#endif
