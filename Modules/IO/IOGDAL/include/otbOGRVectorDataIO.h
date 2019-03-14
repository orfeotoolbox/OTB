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

#ifndef otbOGRVectorDataIO_h
#define otbOGRVectorDataIO_h

#include <string>
#include <map>
#include <cassert>

#include "otbVectorDataIOBase.h"
#include "otbVectorData.h"

#include "OTBIOGDALExport.h"

namespace otb
{

/** \class OGRVectorDataIO
 *
 * \brief ImageIO object for reading and writing OGR format vector data
 *
 * ImageIO object for reading and writing OGR format vector data: it can
 * handle Esri Shapefile (shp), MapInfo (map) and Geography Markup Language (GML)
 * and possibly other OGR formats.
 *
 *
 * \ingroup OTBIOGDAL
 */
class OTBIOGDAL_EXPORT OGRVectorDataIO
  : public VectorDataIOBase
{
public:

  /** Standard class typedefs. */
  typedef OGRVectorDataIO               Self;
  typedef VectorDataIOBase              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRVectorDataIO, VectorDataIOBase);

  /** Byte order typedef */
  typedef Superclass::ByteOrder ByteOrder;

  /** Data typedef */
  typedef VectorData<double, 2>                  VectorDataType;
  typedef VectorDataType::DataTreeType           DataTreeType;
  typedef DataTreeType::TreeNodeType             InternalTreeNodeType;
  typedef InternalTreeNodeType::ChildrenListType ChildrenListType;
  typedef DataTreeType::Pointer                  DataTreePointerType;
  typedef DataTreeType::ConstPointer             DataTreeConstPointerType;
  typedef VectorDataType::DataNodeType           DataNodeType;
  typedef DataNodeType::Pointer                  DataNodePointerType;
  typedef DataNodeType::PointType                PointType;
  typedef DataNodeType::LineType                 LineType;
  typedef LineType::VertexListType               VertexListType;
  typedef VertexListType::ConstPointer           VertexListConstPointerType;
  typedef LineType::Pointer                      LinePointerType;
  typedef DataNodeType::PolygonType              PolygonType;
  typedef PolygonType::Pointer                   PolygonPointerType;
  typedef DataNodeType::PolygonListType          PolygonListType;
  typedef PolygonListType::Pointer               PolygonListPointerType;
  typedef VectorDataType::Pointer                VectorDataPointerType;
  typedef VectorDataType::ConstPointer           VectorDataConstPointerType;
  typedef Superclass::SpacingType                SpacingType;
  typedef Superclass::PointType                  OriginType;

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this VectorDataIO can read the
   * file specified. */
  bool CanReadFile(const char*) const override;

  /** Reads the data from disk into the memory buffer provided. */
  void Read(itk::DataObject* data) override;

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool CanWriteFile(const char*) const override;

  /** Writes the data to disk from the memory buffer provided */
  void Write(const itk::DataObject* data,  char ** papszOptions = nullptr) override;

protected:
  /** Constructor.*/
  OGRVectorDataIO();
  /** Destructor.*/
  ~OGRVectorDataIO() override;

  /*   virtual void InternalReadVectorDataInformation(){}; */

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  OGRVectorDataIO(const Self &) = delete;
  void operator =(const Self&) = delete;

  std::string GetOGRDriverName(std::string name) const;

  void CloseInternalDataSource();

  GDALDataset * m_DataSource;

  const std::map<std::string, std::string> 
    m_OGRExtensionsToDrivers = {
                                { ".SHP", "ESRI Shapefile"},
                                { ".TAB", "MapInfo File"},
                                { ".GML", "GML"},
                                { ".GPX", "GPX"},
                                { ".SQLITE", "SQLite"},
                                {".KML", "KML"},
                                { ".GMT", "OGR_GMT"},
                                { ".JSON", "GeoJSON"},
                                { ".GEOJSON", "GeoJSON"}
  };
};

} // end namespace otb

#endif // otbOGRVectorDataIO_h
