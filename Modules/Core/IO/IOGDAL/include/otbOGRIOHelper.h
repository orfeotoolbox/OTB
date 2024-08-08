/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
class OTBIOGDAL_EXPORT OGRIOHelper : public itk::Object
{
public:
  /** Standard class usings. */
  using Self = OGRIOHelper;
  using Superclass = itk::Object;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Template parameters usings */
  /** Data using */
  using VectorDataType = VectorData<double,2>;
  using VectorDataPointerType = VectorDataType::Pointer;
  using VectorDataConstPointerType = VectorDataType::ConstPointer;
  using DataTreeType = VectorDataType::DataTreeType;

  using DataNodeType = VectorDataType::DataNodeType;
  using DataNodePointerType = DataNodeType::Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRIOHelper, itk::Object);

  /** Conversion tools */
  void ConvertOGRLayerToDataTreeNode(VectorDataPointerType vecDataTree,OGRLayer* layer, DataNodePointerType document) const;


  unsigned int ProcessNodeWrite(VectorDataConstPointerType vdata, DataNodePointerType source, GDALDataset* m_DataSource, OGRGeometryCollection* ogrCollection, OGRLayer* ogrCurrentLayer,
                                OGRSpatialReference* oSRS);

  /** Return a list of OGRLayer * */
  std::vector<OGRLayer*> ConvertDataTreeNodeToOGRLayers(VectorDataConstPointerType vdata,DataNodePointerType source, GDALDataset* dummyDatasource, OGRLayer* ogrCurrentLayer,
                                                        OGRSpatialReference* oSRS);

  void ConvertGeometryToPointNode(const OGRGeometry* ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToLineNode(const OGRGeometry* ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToPolygonNode(const OGRGeometry* ogrGeometry, DataNodePointerType node) const;

protected:
  OGRIOHelper();
  ~OGRIOHelper() override;

private:
  OGRIOHelper(const Self&) = delete;
  void operator=(const Self&) = delete;

  using PointType = DataNodeType::PointType ;

  using LineType = DataNodeType::LineType;
  using LinePointerType = LineType::Pointer;
  using VertexListType = LineType::VertexListType;
  using VertexListConstPointerType = VertexListType::ConstPointer;

  using PolygonType = DataNodeType::PolygonType;
  using PolygonPointerType = PolygonType::Pointer;
  using PolygonListType = DataNodeType::PolygonListType;
  using PolygonListPointerType = PolygonListType::Pointer;

}; // end class OGRIOHelper

} // end namespace otb

#endif
