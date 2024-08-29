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

#ifndef otbVectorData_h
#define otbVectorData_h

#include "itkDataObject.h"
#include "otbDataNode.h"
#include <boost/graph/graph_as_tree.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>

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
class VectorData : public itk::DataObject
{
public:
  /** Standard class usings */
  using Self = VectorData;
  using Superclass = itk::DataObject;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(VectorData, DataObject);
  itkStaticConstMacro(Dimension, unsigned int, VDimension);

  /** Template parameters using */
  using PrecisionType = TPrecision;
  using ValuePrecisionType = TValuePrecision;
  // define VDimension Dimension;
  using DataNodeType = otb::DataNode<TPrecision, VDimension, TValuePrecision>;
  using DataNodePointerType = typename DataNodeType::Pointer;
  using PointType = typename DataNodeType::PointType;
  using LineType = typename DataNodeType::LineType;
  using PolygonType = typename DataNodeType::PolygonType;
  using SpacingType = itk::Vector<double, 2>;
  using OriginType = itk::Point<double, 2>;
  using DataTreeType = boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, DataNodePointerType >;
  using TreeNodeType = typename boost::graph_traits< DataTreeType >::vertex_descriptor;
  using TreeEdgeType = typename boost::graph_traits< DataTreeType >::edge_descriptor;
  using ChildrenListType = std::vector<DataNodePointerType>;
  using VertexIterator = typename boost::graph_traits<DataTreeType>::vertex_iterator;
  
   void SetProjectionRef(const std::string& projectionRef);
   std::string GetProjectionRef() const;

  /** Set the origin of the vector data to put it in the corresponding
   * image coordinates
    * \sa GetOrigin() */
  itkSetMacro(Origin, OriginType);
   void SetOrigin(const double origin[2]);
   void SetOrigin(const float origin[2]);

  itkGetConstReferenceMacro(Origin, OriginType);
 
  /** Set the spacing of the vector data to put it in the corresponding
   * image coordinates
   * \sa GetSignedSpacing() */
   void SetSpacing(const SpacingType& spacing);
   void SetSpacing(const double spacing[2]);
   void SetSpacing(const float spacing[2]);


  itkGetConstReferenceMacro(Spacing, SpacingType);

  /** Clear the vector data  */
   void Clear();

  /** Return the number of element in the tree */
   int Size() const;

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
  void Graft(const itk::DataObject* data) override;

  void SetRoot(DataNodePointerType rootNode)
  {
    ResetRoot(*rootNode);
  }

  DataNodePointerType GetRoot() const
  {
    return m_root;
  };

  void Add(DataNodePointerType nodeToAdd,DataNodePointerType rootForNode)
  {
    // Add the vertex then create an edge between these vertex
    TreeNodeType nodevertex = boost::add_vertex(nodeToAdd,m_DataTree);
    // find the corresponding root vertex in the tree
    TreeNodeType rootvertex = ConvertToTreeNodeType(rootForNode);
    // add an edge between the node and its root
    boost::add_edge(rootvertex,nodevertex,m_DataTree);
  };

  std::vector<DataNodePointerType> GetChildrenList(DataNodePointerType parentNode) const
  {
    //typename boost::property_map<DataTreeType, boost::vertex_bundle_t>::type pmap = boost::get(boost::vertex_bundle, m_DataTree);
    std::vector<DataNodePointerType> childrenslist;
    VertexIterator it,it_end;
    boost::tie(it, it_end) = boost::vertices(m_DataTree);
    
    for (;it!=it_end;it++)
    {
      if(m_DataTree[*it] == parentNode)
      {
        typename boost::graph_traits<DataTreeType>::adjacency_iterator eit, eend;
        std::tie(eit, eend) = boost::adjacent_vertices(*it, m_DataTree);
        for(;eit != eend;eit++)
        {
            childrenslist.push_back(m_DataTree[*eit]);
        }
        break;
      }
    }
    return childrenslist;
  };

  std::pair<VertexIterator,VertexIterator> GetIteratorPair() const
  {
    VertexIterator it,it_end;
    boost::tie(it, it_end) = boost::vertices(m_DataTree);

    return std::make_pair(it,it_end);
  }

  DataNodePointerType Get(VertexIterator dataIt) const
  {
    return m_DataTree[*dataIt];
  }

protected:
  /** Constructor */
  VectorData();
  /** Destructor */
  ~VectorData() override
  {
  }
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  TreeNodeType ConvertToTreeNodeType(DataNodePointerType datanode)
  {
    typename boost::graph_traits<DataTreeType>::vertex_iterator it,it_end;
    boost::tie(it, it_end) = boost::vertices(m_DataTree);
    for (;it!=it_end;it++)
    {
      if(m_DataTree[*it] == datanode)
      {
        return *it;
      }
    }
    //Default case : return the vertex descriptor to the root of the graph
    return *boost::vertices(m_DataTree).first;
  };

  void Reset(const Self& inputVD)
  {
    boost::copy_graph(inputVD.m_DataTree,this->m_DataTree);
    this->m_root = inputVD.m_root;
    this->m_Spacing = inputVD.m_Spacing;
    this->m_Origin = inputVD.m_Origin;
  }

  void ResetRoot(const DataNodeType& rootNode)
  {
    this->m_root->Reset(rootNode);
  }
  
private:
  VectorData(const Self&) = delete;

  void CopyDataTree(const Self* inputVD)
  {
    boost::copy_graph(inputVD->m_DataTree,this->m_DataTree);
  }

  /** Data tree */
  DataTreeType m_DataTree;
  DataNodePointerType m_root;
  SpacingType m_Spacing;
  OriginType  m_Origin;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorData.hxx"
#endif

#endif
