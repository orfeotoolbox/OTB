/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbOGRIOHelper_h
#define otbOGRIOHelper_h

#include <vector>

#include "otbVectorData.h"
#include "otbOGRVersionProxy.h"

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
class OGRIOHelper: public itk::Object
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
                                ogr::version_proxy::GDALDatasetType * m_DataSource,
                                OGRGeometryCollection * ogrCollection,
                                OGRLayer * ogrCurrentLayer,
                                OGRSpatialReference * oSRS);

  /** Return a list of OGRLayer * */
  std::vector<OGRLayer*> ConvertDataTreeNodeToOGRLayers(InternalTreeNodeType * source,
                                                        ogr::version_proxy::GDALDatasetType * dummyDatasource,
                                                        OGRLayer* ogrCurrentLayer,
                                                        OGRSpatialReference * oSRS);

  void ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

protected:
  OGRIOHelper();
  ~OGRIOHelper() ITK_OVERRIDE;

private:
  OGRIOHelper(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

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
