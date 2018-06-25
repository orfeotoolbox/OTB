/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorData_h
#define otbVectorData_h

#include "itkTreeContainer.h"
#include "itkDataObject.h"
#include "otbDataNode.h"

namespace otb
{
/** \class VectorData
 * \brief This class represents a hierarchy of vector data.
 *
 * This class contains an itk::TreeContainer templated with otb::DataNode.
 * It represents a tree structure of vector data. These data can be read from
 * shapefiles or kml files for instance.
 *
 * The internal tree can be walked with itk::TreeIteratorBase subclasses.
 *
 * The three templates indicate
 * - the precision of the points coordinates (by default: double)
 * - the number of dimensions of the space (by default: 2)
 * - the precision of the value associated with objects such as line or
 * polygons (by default: double)
 *
 * The elements in the VectorData have their coordinates in the projection
 * specified by the WKT string. This class also offer the possibility to store
 * the coordinates directly into an image coordinate system, using the origin and
 * spacing in a similar way as the corresponding image.
 *
 * \sa DataNode
 * \sa VectorDataFileReader
 * \sa VectorDataFileWriter
 *
 *
 * \ingroup OTBVectorDataBase
 */
template <class TPrecision = double, unsigned int VDimension = 2, class TValuePrecision = double>
class VectorData
  : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef VectorData                    Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(VectorData, DataObject);
  itkStaticConstMacro(Dimension, unsigned int, VDimension);

  /** Template parameters typedef */
  typedef TPrecision      PrecisionType;
  typedef TValuePrecision ValuePrecisionType;
  //define VDimension Dimension;
  typedef otb::DataNode<TPrecision, VDimension, TValuePrecision> DataNodeType;
  typedef typename DataNodeType::Pointer                         DataNodePointerType;
  typedef itk::TreeContainer<DataNodePointerType>                DataTreeType;
  typedef typename DataTreeType::Pointer                         DataTreePointerType;

  typedef typename DataNodeType::PointType   PointType;
  typedef typename DataNodeType::LineType    LineType;
  typedef typename DataNodeType::PolygonType PolygonType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  itkGetObjectMacro(DataTree, DataTreeType);
  itkGetConstObjectMacro(DataTree, DataTreeType);

  virtual void SetProjectionRef(const std::string& projectionRef);
  virtual std::string GetProjectionRef() const;

  /** Set the origin of the vector data to put it in the corresponding
   * image coordinates
    * \sa GetOrigin() */
  itkSetMacro(Origin, OriginType);
  virtual void SetOrigin(const double origin[2]);
  virtual void SetOrigin(const float origin[2]);

  itkGetConstReferenceMacro(Origin, OriginType);

  /** Set the spacing of the vector data to put it in the corresponding
   * image coordinates
   * \sa GetSignedSpacing() */
  virtual void SetSpacing(const SpacingType& spacing);
  virtual void SetSpacing(const double spacing[2]);
  virtual void SetSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(Spacing, SpacingType);

  /** Clear the vector data  */
  virtual bool Clear();

  /** Return the number of element in the tree */
  virtual int Size() const;

  void TransformPointToPhysicalPoint(const PointType& point, PointType& physicalPoint) const
  {
    physicalPoint[0] = point[0] * m_Spacing[0] + m_Origin[0];
    physicalPoint[1] = point[1] * m_Spacing[1] + m_Origin[1];
  }

  /** Graft the data and information from one VectorData to another. This
   * is a convenience method to setup a second VectorData with all the meta
   * information of another VectorData and use the same DataTree
   * Note that this method is different than just using two
   * SmartPointers to the same VectorData since separate DataObjects are
   * still maintained. This method is similar to
   * VectorDataSource::GraftOutput(). */
  void Graft(const itk::DataObject *data) override;

protected:
  /** Constructor */
  VectorData();
  /** Destructor */
  ~VectorData() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  VectorData(const Self&) = delete;
  void operator =(const Self&) = delete;

  /** Data tree */
  DataTreePointerType m_DataTree;

  SpacingType m_Spacing;
  OriginType  m_Origin;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorData.hxx"
#endif

#endif
