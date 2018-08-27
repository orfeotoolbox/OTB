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

#ifndef otbConnectedComponentStreamStitchingFilter_h
#define otbConnectedComponentStreamStitchingFilter_h

#include "otbOGRDataSourceWrapper.h"
#include "otbMacro.h"


#include "itkProgressReporter.h"

#include <algorithm>

namespace otb
{

/** \class ConnectedComponentStreamStitchingFilter
 *  \brief This filter fusion the geometries in a layer (\c OGRLayer) along streaming lines.
 *  It is a in-line filter which means that the result of the fusion overwrites the input layer.
 *  A polygon P1 is merge with a polygon P2 if:
 *  - P1 and P2 are on different side of the streaming line
 *  - P1 and P2 intersect each other.
 *  - P1 and P2 have the same labels.
 *  The \c SetStreamSize() method allows retrieving the number of streams in row and column,
 *  and their pixel coordinates.
 *  The input image is used to transform pixel coordinates of the streaming lines into
 *  coordinate system of the image, which must be the same as the one in the OGR input file.
 *  This filter is intended to be used after \c PersistentLabelImageVectorizationFilter.
 *
 *  \ingroup OBIA
 *
 *
 * \ingroup OTBOGRProcessing
 */
 
template <class TInputImage>
class ITK_EXPORT ConnectedComponentStreamStitchingFilter :
    public itk::ProcessObject
{
public:

   /** typedef for the classes standards. */
  typedef ConnectedComponentStreamStitchingFilter        Self;
  typedef itk::ProcessObject                   Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Definition of the input image */
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::IndexType   InputIndexType;
  typedef typename InputImageType::SizeType    SizeType;
  typedef typename InputImageType::RegionType  RegionType;
  typedef typename InputImageType::SpacingType SpacingType;
  typedef typename InputImageType::PointType   OriginType;
  typedef typename InputImageType::IndexType   IndexType;

  /** Definition of the vector data */
  typedef ogr::Layer                           OGRLayerType;
  typedef ogr::Feature                         OGRFeatureType;

  /** Set the input image of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType *input);
  /** Get the input image. */
  virtual const InputImageType * GetInput(void);

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(ConnectedComponentStreamStitchingFilter, ProcessObject);

  /** Set the input OGRLayer */
  void SetOGRLayer( const OGRLayerType & ogrLayer );
  /** Get the input OGRLayer */
  const OGRLayerType & GetOGRLayer( void ) const;

  /** Set the stream size.
   * As this filter is intended to be used right after the \c StreamingVectorizedSegmentation,
   * use the \c GetStreamSize() method on it to get the correct stream size.
   */
  itkSetMacro(StreamSize, SizeType);
  /** Get stream size*/
  itkGetMacro(StreamSize, SizeType);

  /** Generate Data method. This method must be called explicitly (not through the \c Update method). */
  void GenerateData() override;

  /** A class to find connected components on a graph, it is used to find all intersecting polygons from a set of intersecting pairs. */
  class IntersectionGraph
  {
  public:
  
    /** struct to store a node of the graph, it contains the list of the node's neighbors, 
     * and a flag to know if the node has been visited during the connected component algorithm
     */
    struct NodeType
    {
      bool isVisited;
      std::vector<int> adjacencyList;
    };

    /** this container contains all the information of the graph */
    typedef std::map<int, NodeType > GraphType;  
        
    IntersectionGraph() = default; // Constructor
    ~IntersectionGraph() = default; // Destructor
    
    /** Method to add two adjacent nodes idx1 and idx2 to the graph, if they don't exist already, 
     * and add the adjacency property for existing nodes */
    void registerEdge(int idx1, int idx2)
    {
      if (idx1 != idx2)
      {
        if (m_graph.find(idx1) == m_graph.end()) // The node (key in the map) doesn't exist
        {
          NodeType node;
          node.adjacencyList.push_back(idx2);
          node.isVisited = false;
          m_graph[idx1] = node; 
        }
        else
        {
          m_graph[idx1].adjacencyList.push_back(idx2);
        }
                  
        if (m_graph.find(idx2) == m_graph.end())
        {
          NodeType node;
          node.adjacencyList.push_back(idx1);
          node.isVisited = false;
          m_graph[idx2] = node; 
        }
        else
        {
          m_graph[idx2].adjacencyList.push_back(idx1);
        }
      }
    }	
    
    /** Graph search to find the connected component in the graph (depth-first search using recursivity) */ 
    std::vector< std::vector<int> > findConnectedComponents()
    {
      std::vector< std::vector<int> > fusionList;
      for (typename GraphType::iterator it = m_graph.begin(); it != m_graph.end(); it++)
      {
        std::vector<int> fusionIndexes;
        VisitNode( (*it).first, fusionIndexes);
        if (fusionIndexes.empty() != true)
        {
          fusionList.push_back(fusionIndexes);
        }
      }
      return fusionList;
    }
        
    /** Visit a node, and recursively call the method on its neighbors */    
    void VisitNode(int idx, std::vector<int>& fusionIndexes)
    {
      if (m_graph[idx].isVisited == false) 
      {
        fusionIndexes.push_back(idx);
        m_graph.find(idx)->second.isVisited = true;
        for (std::vector<int>::iterator it = m_graph[idx].adjacencyList.begin(); it != m_graph[idx].adjacencyList.end(); it++)
        {
          VisitNode( *it,fusionIndexes);
        }
      }
    }	
  private:
    GraphType m_graph;
  };

protected:
  ConnectedComponentStreamStitchingFilter();
  ~ConnectedComponentStreamStitchingFilter() override = default;

  struct FusionStruct
  {
    unsigned int indStream1;
    unsigned int indStream2;
    double overlap;
  };
  struct FeatureStruct
  {
    FeatureStruct(OGRFeatureDefn & defn) : feat(defn), fusioned(false),fusionIndex(0)
    {
    }
    OGRFeatureType feat;
    bool fusioned;
    unsigned int fusionIndex;
  };
  struct SortFeatureStruct
  {
    bool operator() (FusionStruct f1, FusionStruct f2) { return (f1.overlap > f2.overlap); }
  } SortFeature;

  /**
   Main computation method. if line is true process row part, else process column part.
   */
  void ProcessStreamingLine(bool line, itk::ProgressReporter &progress);
  /** get length in case of  OGRGeometryCollection.
   * This function recodes the get_lenght method available since gdal 1.8.0
   * in the case of OGRGeometryCollection. The aim is to allow accessing polygon stiching
   * functionalities with gdal 1.6.
   */
  double GetLengthOGRGeometryCollection(OGRGeometryCollection * intersection);

private:
  ConnectedComponentStreamStitchingFilter(const Self &) = delete; 
  void operator =(const Self&) = delete;      

  SizeType m_StreamSize;
  unsigned int m_Radius;
  OGRLayerType m_OGRLayer;

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConnectedComponentStreamStitchingFilter.hxx"
#endif

#endif
