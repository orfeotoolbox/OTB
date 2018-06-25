/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRCC8Graph_h
#define otbRCC8Graph_h

#include "otbRCC8Edge.h"

//fix compilation in VS7.1 for boost 1.42
//similar to the fix in Monteverdi
#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
#define BOOST_NO_0X_HDR_INITIALIZER_LIST
#endif

#include "otb_boost_graph_header.h"

#include "otbImage.h"
#include "otbImageList.h"

namespace otb
{
/**
 * \class RCC8Graph
 * \brief This class is a data structure designed to store RCC8 Graph
 * computed from a pyramidal segmentation.
 *
 * A batch of boost operation has been embedded in order to provide
 * basic functionality such as adding a new edge, or retrieving the
 * number of vertices. A method is also provided to retrieve the boost
 * graph object, in case more advanced processing is required. One
 * should remember that if a peculiar operation is needed for some
 * applications, one should provide a patch for this class including
 * the new functionnalities, for consistency reason.
 *
 * \ingroup OTBRCC8
 */
template <class TVertex>
class ITK_EXPORT RCC8Graph  :
  public itk::DataObject
{
public:
  /** Standard class typedefs. */
  typedef RCC8Graph                     Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8Graph, itk::DataObject);
  /** Vertex typedef */
  typedef TVertex                      VertexType;
  typedef typename VertexType::Pointer VertexPointerType;
  /** Edge typedef */
  typedef RCC8Edge                         EdgeType;
  typedef typename EdgeType::Pointer       EdgePointerType;
  typedef typename EdgeType::RCC8ValueType RCC8ValueType;
  /** Typedef for the boost graph representation */
  typedef boost::adjacency_list
  <boost::vecS, boost::vecS, boost::bidirectionalS,
      VertexPointerType, EdgePointerType> InternalGraphType;
  /** Edges and vertices descriptors typedefs (boost objects)*/
  typedef typename InternalGraphType::vertex_descriptor VertexDescriptorType;
  typedef typename InternalGraphType::edge_descriptor   EdgeDescriptorType;

  /** Getters and Setters for the number of vertices */
  itkSetMacro(NumberOfVertices, unsigned int);
  itkGetConstReferenceMacro(NumberOfVertices, unsigned int);

  /**
   *  Return the internal boost graph object.
   *  \return The internal boost graph object
   */
  InternalGraphType * GetGraph(void)
  {
    return &m_Graph;
  }
  /**
   * Since the number of vertices is mandatory to instantiate the
   * internal boost representation, the build method has to be called
   * once this parameter is set.
   */
  void Build(void);
  /**
   * Set a vertex.
   * \param index The index of the vertex in the graph.
   * \param vertex The vertex to set.
   */
  void SetVertex(unsigned int index, VertexPointerType vertex);
  /**
   * Get a vertex.
   * \param index The index of the vertex in the graph
   * \return The vertex.
   */
  VertexPointerType GetVertex(unsigned int index);
  /**
   * Add an edge in the graph.
   * \param index1 The index of the source vertex.
   * \param index2 The index of the target vertex.
   * \param r The RCC8 value.
   */
  void AddEdge(unsigned int index1, unsigned int index2, RCC8ValueType r);
  /**
   * Get number of edges
   * /return The number of edges.
   */
  unsigned int GetNumberOfEdges(void);

protected:
  /** Constructor */
  RCC8Graph();
  /** Destructor */
  ~RCC8Graph() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * Initialize a range of vertex.
   * \param num The index of the last vertices to initialize.
   */
  void InitializeGraph(unsigned int num);

private:

  /** Defines the number of vertices (ie total number of segmentation regions)*/
  unsigned int m_NumberOfVertices;
  /** Internal representation using the boost graph library */
  InternalGraphType m_Graph;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8Graph.hxx"
#endif

#endif
