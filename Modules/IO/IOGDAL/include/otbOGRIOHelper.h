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

#ifndef otbOGRIOHelper_h
#define otbOGRIOHelper_h

#include <vector>

#include "otbVectorData.h"

#include "OTBIOGDALExport.h"


class GDALDataset;
class OGRGeometryCollection;
class OGRLayer;
class OGRSpatialReference;
class OGRGeometry;


namespace otb
{

/** \class OGRIOHelper
 *  \brief This class IO OGR
 *
 *
 * \ingroup OTBIOGDAL
 */
class OTBIOGDAL_EXPORT OGRIOHelper: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef OGRIOHelper                   Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Template parameters typedefs */
  /** Data typedef */
  typedef VectorData<>                                    VectorDataType;
  typedef VectorDataType::DataTreeType           DataTreeType;
  typedef DataTreeType::TreeNodeType             InternalTreeNodeType;

  typedef VectorDataType::DataNodeType           DataNodeType;
  typedef DataNodeType::Pointer                  DataNodePointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRIOHelper, itk::Object);

  /** Conversion tools */
  void ConvertOGRLayerToDataTreeNode(OGRLayer * layer, InternalTreeNodeType * documentPtr) const;


  unsigned int ProcessNodeWrite(InternalTreeNodeType * source,
                                GDALDataset * m_DataSource,
                                OGRGeometryCollection * ogrCollection,
                                OGRLayer * ogrCurrentLayer,
                                OGRSpatialReference * oSRS);

  /** Return a list of OGRLayer * */
  std::vector<OGRLayer*> ConvertDataTreeNodeToOGRLayers(InternalTreeNodeType * source,
                                                        GDALDataset * dummyDatasource,
                                                        OGRLayer* ogrCurrentLayer,
                                                        OGRSpatialReference * oSRS);

  void ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

protected:
  OGRIOHelper();
  ~OGRIOHelper() override;

private:
  OGRIOHelper(const Self &) = delete;
  void operator =(const Self&) = delete;

  typedef DataNodeType::PointType                PointType;

  typedef DataNodeType::LineType                 LineType;
  typedef LineType::Pointer                      LinePointerType;
  typedef LineType::VertexListType               VertexListType;
  typedef VertexListType::ConstPointer           VertexListConstPointerType;

  typedef DataNodeType::PolygonType              PolygonType;
  typedef PolygonType::Pointer                   PolygonPointerType;
  typedef DataNodeType::PolygonListType          PolygonListType;
  typedef PolygonListType::Pointer               PolygonListPointerType;

}; // end class OGRIOHelper

} // end namespace otb

#endif
