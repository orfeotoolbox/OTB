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


#ifndef otbVectorDataProperties_h
#define otbVectorDataProperties_h

#include "itkObjectFactory.h"

// #include "itkNumericTraits.h"

#include "otbVectorData.h"
namespace otb
{
/** \class VectorDataProperties
 *   \brief Compute public properties of the vectordata
 *
 * \sa VectorData
 *
 * \ingroup OTBVectorDataBase
 */

template <class TVectorData>
class ITK_EXPORT VectorDataProperties : public itk::DataObject
{
public:
  /** Standard class typedefs. */
  typedef VectorDataProperties          Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Convenient typedefs */
  typedef TVectorData                           VectorDataType;
  typedef typename VectorDataType::Pointer      VectorDataPointerType;
  typedef typename VectorDataType::ConstPointer InputVectorDataConstPointer;
  typedef typename VectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer        DataNodePointerType;
  typedef typename DataNodeType::PolygonType    PolygonType;

  typedef typename VectorDataType::DataTreeType::TreeNodeType InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType     ChildrenListType;

  typedef typename PolygonType::RegionType RegionType;
  typedef typename RegionType::Pointer     RegionPointerType;
  typedef typename RegionType::IndexType   IndexType;
  typedef typename RegionType::SizeType    SizeType;
  typedef typename RegionType::Type        Type;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataProperties, LightObject);

  /** Number of dimensions. */
  itkStaticConstMacro(VectorDataDimension, unsigned int, TVectorData::Dimension);

  /** Get/Set the VectorData. */
  //      itkGetObjectMacro(VectorDataObject, VectorDataType);
  void SetVectorDataObject(const VectorDataType* v)
  {
    m_VectorDataObject = const_cast<VectorDataType*>(v);
  }
  //
  RegionType GetBoundingRegion()
  {
    return m_BoundingRegion;
  }
  void SetBoundingRegion(RegionType& region)
  {
    m_BoundingRegion = region;
  }

  void AddRegion(const RegionType& region);
  void ComputeBoundingRegion();

protected:
  /** Constructor */
  VectorDataProperties() : m_VectorDataObject(nullptr){};
  /** Destructor */
  ~VectorDataProperties() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void ProcessNode(InternalTreeNodeType* source);
  bool IsBoundingRegionNull();

private:
  VectorDataProperties(const Self&) = delete;
  void operator=(const Self&) = delete;

  // Pointer to the VectorData
  VectorDataType* m_VectorDataObject;
  RegionType      m_BoundingRegion;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataProperties.hxx"
#endif

#endif
