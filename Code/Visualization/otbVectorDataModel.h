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
#ifndef __otbVectorDataModel_h
#define __otbVectorDataModel_h

#include "itkObject.h"

#include "otbMVCModel.h"
#include "otbListenerBase.h"

#include "otbVectorData.h"

namespace otb
{
/** \class VectorDataModel
 *  \brief Model to handle vector data (point, line, polygons) in an OTB application
 *  \todo all cases are not handled currently, this class need some additions.
 *
 *  \sa VectorDataActionHandler
 *
 */
class ITK_EXPORT VectorDataModel
  : public MVCModel<ListenerBase>, public itk::Object
{
public:
  /** Standard class typedefs */
  typedef VectorDataModel               Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::VectorData<double, 2>   VectorDataType;
  typedef VectorDataType::Pointer      VectorDataPointer;
  typedef VectorDataType::DataNodeType DataNodeType;
  typedef VectorDataType::DataTreeType DataTreeType;
  typedef DataTreeType::TreeNodeType   TreeNodeType;
  typedef TreeNodeType::ChildrenListType ChildrenListType;
  typedef VectorDataType::PointType    PointType;
  typedef VectorDataType::SpacingType  SpacingType;
  typedef VectorDataType::PolygonType  PolygonType;
  typedef VectorDataType::LineType     LineType;
  typedef PolygonType::VertexType      VertexType;

  /** Runtime information */
  itkTypeMacro(VectorDataModel, Object);

  /** New macro */
  itkNewMacro(Self);

  /** Update will notify all listeners. */
  void Update(void);

  /** Set/Get the node type used */
  itkSetMacro(CurrentNodeType, NodeType);
  itkGetMacro(CurrentNodeType, NodeType);

  /** Return a pointer to the vector data */
  itkGetObjectMacro(VectorData, VectorDataType);
  
  /** Load a vector data. */
  void AddVectorData( VectorDataPointer vData );
  void AddNode( TreeNodeType * node );
  
  /** Load a vector data using image reprojection. */
  template <typename TImage> void AddVectorData( VectorDataPointer vData, TImage * image );
  template <typename TImage> void AddNode( TreeNodeType * node, TImage * image );

  void AddPointToGeometry(VertexType& vertex, bool callUpdate = true);
  void EndGeometry(void);
  void DeleteGeometry(void);

  /** return the Nth data node without counting the Root/Document/Folder*/
  DataNodeType::Pointer GetNthDataNode(int n);
  void SetSelectedGeometry(int n);
  itkGetObjectMacro(SelectedGeometry, DataNodeType);

  itkSetMacro(Origin, PointType);
  itkGetConstReferenceMacro(Origin, PointType);

  itkSetMacro(Spacing, SpacingType);
  itkGetConstReferenceMacro(Spacing, SpacingType);

protected:
  /** Constructor */
  VectorDataModel();

  /** Destructor */
  virtual ~VectorDataModel(){}

private:
  VectorDataModel(const Self &);
  void operator =(const Self&);

  VectorDataPointer m_VectorData;
  NodeType          m_CurrentNodeType;

  /** Node where the next geometry should be attached */
  DataNodeType::Pointer m_CurrentRootNode;

  /** Current geometry being processed (for adding node, etc) */
  DataNodeType::Pointer m_CurrentGeometry;

  /** Currently selected node */
  DataNodeType::Pointer m_SelectedGeometry;

  /** Origin of image used for vectorization */
  PointType m_Origin;

  /** Spacing of image used for vectorization */
  SpacingType m_Spacing;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataModel.txx"
#endif

#endif
