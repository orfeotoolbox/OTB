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
#include "ogrsf_frmts.h"
#include "otbVectorData.h"

namespace otb
{

/** \class OGRIOHelper
 *  \brief This class IO OGR
 *
 */
template <class TVectorData>
class OGRIOHelper: public itk::Object
{
public:
  /** Template parameters typedefs */
  /** Data typedef */
  typedef TVectorData                                     VectorDataType;
  typedef typename VectorDataType::DataTreeType           DataTreeType;
  typedef typename DataTreeType::TreeNodeType             InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType ChildrenListType;
  typedef typename DataTreeType::Pointer                  DataTreePointerType;
  typedef typename DataTreeType::ConstPointer             DataTreeConstPointerType;
  typedef typename VectorDataType::DataNodeType           DataNodeType;
  typedef typename DataNodeType::Pointer                  DataNodePointerType;
  typedef typename DataNodeType::PointType                PointType;
  typedef typename DataNodeType::LineType                 LineType;
  typedef typename LineType::VertexListType               VertexListType;
  typedef typename VertexListType::ConstPointer           VertexListConstPointerType;
  typedef typename LineType::Pointer                      LinePointerType;
  typedef typename DataNodeType::PolygonType              PolygonType;
  typedef typename PolygonType::Pointer                   PolygonPointerType;
  typedef typename DataNodeType::PolygonListType          PolygonListType;
  typedef typename PolygonListType::Pointer               PolygonListPointerType;
  typedef typename VectorDataType::Pointer                VectorDataPointerType;
  typedef typename VectorDataType::ConstPointer           VectorDataConstPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRIOHelper, itk::Object);

  /** Conversion tools */
  inline void ConvertOGRLayerToDataTreeNode(OGRLayer * layer, InternalTreeNodeType * documentPtr) const;


  unsigned int ProcessNodeWrite(InternalTreeNodeType * source,
                                OGRDataSource * m_DataSource,
                                OGRGeometryCollection * ogrCollection,
                                OGRLayer * ogrCurrentLayer,
                                OGRSpatialReference * oSRS);

  OGRIOHelper() : m_Kept(-1)
  {}
  virtual ~OGRIOHelper() {}

private:

  inline void ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  inline void ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  inline void ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;


  /** Number of layer in the Tree*/
  unsigned int m_Kept;

}; // end class OGRIOHelper

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRIOHelper.txx"
#endif

#endif
