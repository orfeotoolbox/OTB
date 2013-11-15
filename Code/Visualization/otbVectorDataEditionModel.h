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
#ifndef __otbVectorDataEditionModel_h
#define __otbVectorDataEditionModel_h

#include "itkObject.h"

#include "otbMVCModel.h"
#include "otbListenerBase.h"

#include "otbVectorData.h"
#include "otbVectorDataModel.h"

#include "otbRemoteSensingRegion.h"
#include "itkImageRegion.h"

namespace otb
{
/** \class VectorDataEditionModel
 *  \brief Model to handle vector data (point, line, polygons) vertex
 *  edition and geometry position
 *
 *  \sa VectorDataEditVertexActionHandler, VectorDataTranslateGeometryActionHandler
 *
 */
class ITK_EXPORT VectorDataEditionModel
  : public VectorDataModel
{
public:
  /** Standard class typedefs */
  typedef VectorDataEditionModel         Self;
  typedef VectorDataModel                Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  typedef otb::VectorData<double, 2>     VectorDataType;
  typedef VectorDataType::Pointer        VectorDataPointer;
  typedef VectorDataType::DataNodeType   DataNodeType;
  typedef VectorDataType::DataTreeType   DataTreeType;
  typedef DataTreeType::TreeNodeType     TreeNodeType;
  typedef TreeNodeType::ChildrenListType ChildrenListType;
  typedef VectorDataType::PointType      PointType;
  typedef VectorDataType::SpacingType  SpacingType;
  typedef VectorDataType::PolygonType  PolygonType;
  typedef VectorDataType::LineType     LineType;
  typedef PolygonType::VertexType      VertexType;
  typedef PolygonType::VertexListType      VertexListType;
  typedef PolygonType::VertexListConstIteratorType
                                       VertexListConstIteratorType;
  /** Runtime information */
  itkTypeMacro(VectorDataEditionModel, Object);

  /** New macro */
  itkNewMacro(Self);

  itkSetMacro(Threshold, double);
  itkGetMacro(Threshold, double);

  void SearchDataNodeIndexFromPoint( PointType srcPoint);
  void UpdatePoint( PointType srcPoint,  PointType dstPoint);
  void UpdateGeometryPosition(double tx, double ty);
  void DeleteSelectedGeometry();
  void DeleteSelectedGeometriesInROI(itk::ImageRegion<2> rsRegion);


protected:
  /** Constructor */
  VectorDataEditionModel();

  /** Destructor */
  virtual ~VectorDataEditionModel(){}

  void ProcessVertexListTranslation(VertexListType* vertexList,
                                    double tx,
                                    double ty);

private:
  VectorDataEditionModel(const Self &);
  void operator =(const Self&);

  int m_SearchIndex;
  double m_Threshold;

}; // end class
} // end namespace otb

#endif
