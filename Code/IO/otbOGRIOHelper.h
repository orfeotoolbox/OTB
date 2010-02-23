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
#ifndef __otbOGRIOHelper_h
#define __otbOGRIOHelper_h

#include <vector>

//TODO change this include  have to define from what inherate this class
//#include "otbPolyLineParametricPathWithValue.h" //for vcl_abs
#include "ogrsf_frmts.h"
#include "otbVectorData.h"

namespace otb
{


/** \class OGRIOHelper
 *  \brief This class IO OGR
 *
 */
template <class TVectorData >
class OGRIOHelper
{
public:
  /** Template parameters typedefs */
  /** Data typedef */
  typedef TVectorData VectorDataType;
  typedef typename VectorDataType::DataTreeType  DataTreeType;
  typedef typename DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType        ChildrenListType;
  typedef typename DataTreeType::Pointer         DataTreePointerType;
  typedef typename DataTreeType::ConstPointer    DataTreeConstPointerType;
  typedef typename VectorDataType::DataNodeType  DataNodeType;
  typedef typename DataNodeType::Pointer         DataNodePointerType;
  typedef typename DataNodeType::PointType       PointType;
  typedef typename DataNodeType::LineType        LineType;
  typedef typename LineType::VertexListType      VertexListType;
  typedef typename VertexListType::ConstPointer  VertexListConstPointerType;
  typedef typename LineType::Pointer             LinePointerType;
  typedef typename DataNodeType::PolygonType     PolygonType;
  typedef typename PolygonType::Pointer          PolygonPointerType;
  typedef typename DataNodeType::PolygonListType PolygonListType;
  typedef typename PolygonListType::Pointer      PolygonListPointerType;
  typedef typename VectorDataType::Pointer       VectorDataPointerType;
  typedef typename VectorDataType::ConstPointer  VectorDataConstPointerType;
  //typedef typename Superclass::SpacingType       SpacingType;
  //typedef typename Superclass::PointType         OriginType;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRIOHelper, 0);
  
  /**
   * \param labelObject the label object to vectorize
   * \return The vectorized label object as a polygon.
   */
  //inline PolygonPointerType operator()(const LabelObjectType * labelObject);
  /** Conversion tools */

  inline DataNodePointerType ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry) const;

  inline DataNodePointerType ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry) const;

  inline DataNodePointerType ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry) const;

  inline void ConvertOGRLayerToDataTreeNode (OGRLayer * layer, InternalTreeNodeType * documentPtr) const;
  /** end conversion tools */
  
  unsigned int ProcessNodeWrite(InternalTreeNodeType * source, OGRDataSource * m_DataSource, OGRGeometryCollection * ogrCollection, OGRLayer * ogrCurrentLayer, OGRSpatialReference * oSRS);
  
  OGRIOHelper():m_Kept(-1)
  {};
  virtual ~OGRIOHelper() {};
  
  private:
  /** Number of layer in the Tree*/
  unsigned int m_Kept;
}; // end class OGRIOHelper


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRIOHelper.txx"
#endif

#endif


