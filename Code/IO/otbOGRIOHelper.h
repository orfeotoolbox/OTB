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
  /** Standard class typedefs. */
  typedef OGRIOHelper                   Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Template parameters typedefs */
  /** Data typedef */
  typedef TVectorData                                     VectorDataType;
  typedef typename VectorDataType::DataTreeType           DataTreeType;
  typedef typename DataTreeType::TreeNodeType             InternalTreeNodeType;

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

//  typedef  enum { UNKNOWN, FLOAT, DOUBLE } VectorDataComponentType; //unused (yet?)

//  itkSetEnumMacro(ComponentType, VectorDataComponentType); //unused (yet?)
//  itkGetEnumMacro(ComponentType, VectorDataComponentType); //unused (yet?)


protected:
  OGRIOHelper();
  ~OGRIOHelper();

private:
  OGRIOHelper(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typedef typename VectorDataType::DataNodeType           DataNodeType;
  typedef typename DataNodeType::Pointer                  DataNodePointerType;

  typedef typename DataNodeType::PointType                PointType;

  typedef typename DataNodeType::LineType                 LineType;
  typedef typename LineType::Pointer                      LinePointerType;
  typedef typename LineType::VertexListType               VertexListType;
  typedef typename VertexListType::ConstPointer           VertexListConstPointerType;

  typedef typename DataNodeType::PolygonType              PolygonType;
  typedef typename PolygonType::Pointer                   PolygonPointerType;
  typedef typename DataNodeType::PolygonListType          PolygonListType;
  typedef typename PolygonListType::Pointer               PolygonListPointerType;

  void ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  //VectorDataComponentType m_ComponentType; //unused (yet?)

}; // end class OGRIOHelper

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRIOHelper.txx"
#endif

#endif
